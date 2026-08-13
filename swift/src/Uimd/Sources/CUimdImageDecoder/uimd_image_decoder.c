#include "uimd_image_decoder.h"

#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <dlfcn.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define UIMD_SIXEL_MAX_COLORS 64
#define UIMD_SIXEL_COLOR_LEVELS 4
#define UIMD_SIXEL_FALSE_STATUS_MASK 0x1000
#define UIMD_SIXEL_PIXEL_FORMAT_RGB888 0x03
#define UIMD_SIXEL_DIFFUSE_NONE 0x1
#define UIMD_SIXEL_OPTIMIZE_PALETTE 0x1
#define UIMD_PATH_BUFFER_SIZE 4096

typedef struct sixel_output sixel_output_t;
typedef struct sixel_dither sixel_dither_t;
typedef int SixelStatus;
typedef int (*SixelWriteFunction)(char*, int, void*);
typedef SixelStatus (*SixelOutputNewFunction)(sixel_output_t**, SixelWriteFunction, void*, void*);
typedef SixelStatus (*SixelDitherNewFunction)(sixel_dither_t**, int, void*);
typedef void (*SixelDitherSetPaletteFunction)(sixel_dither_t*, unsigned char*);
typedef void (*SixelDitherSetPixelFormatFunction)(sixel_dither_t*, int);
typedef void (*SixelDitherSetOptimizePaletteFunction)(sixel_dither_t*, int);
typedef void (*SixelDitherSetDiffusionTypeFunction)(sixel_dither_t*, int);
typedef SixelStatus (*SixelEncodeFunction)(unsigned char*, int, int, int, sixel_dither_t*, sixel_output_t*);
typedef void (*SixelOutputUnrefFunction)(sixel_output_t*);
typedef void (*SixelDitherUnrefFunction)(sixel_dither_t*);

typedef struct UimdSixelApi
{
    SixelOutputNewFunction output_new;
    SixelDitherNewFunction dither_new;
    SixelDitherSetPaletteFunction dither_set_palette;
    SixelDitherSetPixelFormatFunction dither_set_pixel_format;
    SixelDitherSetOptimizePaletteFunction dither_set_optimize_palette;
    SixelDitherSetDiffusionTypeFunction dither_set_diffusion_type;
    SixelEncodeFunction encode;
    SixelOutputUnrefFunction output_unref;
    SixelDitherUnrefFunction dither_unref;
} UimdSixelApi;

typedef struct UimdDynamicBuffer
{
    char* data;
    int size;
    int capacity;
} UimdDynamicBuffer;

#if defined(_WIN32)
typedef HMODULE UimdDynamicLibraryHandle;
#else
typedef void* UimdDynamicLibraryHandle;
#endif

static int uimd_sixel_status_succeeded(SixelStatus status)
{
    return (status & UIMD_SIXEL_FALSE_STATUS_MASK) == 0;
}

static const char** uimd_sixel_library_names(void)
{
#if defined(_WIN32)
    static const char* names[] = {"libsixel.dll", "sixel.dll", "libsixel-1.dll", 0};
#elif defined(__APPLE__)
    static const char* names[] = {"libsixel.1.dylib", "libsixel.dylib", 0};
#else
    static const char* names[] = {"libsixel.so.1", "libsixel.so", 0};
#endif
    return names;
}

static char uimd_library_path_delimiter(void)
{
#if defined(_WIN32)
    return ';';
#else
    return ':';
#endif
}

static UimdDynamicLibraryHandle uimd_open_dynamic_library(const char* path)
{
    if (path == 0 || path[0] == '\0')
    {
        return 0;
    }
#if defined(_WIN32)
    return LoadLibraryA(path);
#elif defined(__unix__) || defined(__APPLE__)
    return dlopen(path, RTLD_LAZY | RTLD_LOCAL);
#else
    (void)path;
    return 0;
#endif
}

static void* uimd_load_symbol(UimdDynamicLibraryHandle handle, const char* name)
{
    if (handle == 0 || name == 0)
    {
        return 0;
    }
#if defined(_WIN32)
    return (void*)GetProcAddress(handle, name);
#elif defined(__unix__) || defined(__APPLE__)
    return dlsym(handle, name);
#else
    (void)handle;
    (void)name;
    return 0;
#endif
}

static UimdDynamicLibraryHandle uimd_try_open_named_library_in_directory(const char* directory, const char* name)
{
    if (directory == 0 || directory[0] == '\0' || name == 0 || name[0] == '\0')
    {
        return 0;
    }

    char path[UIMD_PATH_BUFFER_SIZE];
    const size_t length = strlen(directory);
    const char separator =
#if defined(_WIN32)
        '\\';
#else
        '/';
#endif
    const int needs_separator = length > 0 && directory[length - 1] != '/' && directory[length - 1] != '\\';
    const int written = needs_separator
        ? snprintf(path, sizeof(path), "%s%c%s", directory, separator, name)
        : snprintf(path, sizeof(path), "%s%s", directory, name);
    if (written <= 0 || written >= (int)sizeof(path))
    {
        return 0;
    }
    return uimd_open_dynamic_library(path);
}

static UimdDynamicLibraryHandle uimd_open_named_library_from_delimited_dirs(
    const char* directories,
    char delimiter,
    const char** names
)
{
    if (directories == 0 || directories[0] == '\0')
    {
        return 0;
    }

    const char* start = directories;
    while (*start != '\0')
    {
        const char* end = strchr(start, delimiter);
        const size_t length = end == 0 ? strlen(start) : (size_t)(end - start);
        if (length > 0 && length < UIMD_PATH_BUFFER_SIZE)
        {
            char directory[UIMD_PATH_BUFFER_SIZE];
            memcpy(directory, start, length);
            directory[length] = '\0';
            for (int index = 0; names[index] != 0; ++index)
            {
                UimdDynamicLibraryHandle handle = uimd_try_open_named_library_in_directory(directory, names[index]);
                if (handle != 0)
                {
                    return handle;
                }
            }
        }
        if (end == 0)
        {
            break;
        }
        start = end + 1;
    }
    return 0;
}

static UimdDynamicLibraryHandle uimd_open_sixel_library(void)
{
    const char** names = uimd_sixel_library_names();
    for (int index = 0; names[index] != 0; ++index)
    {
        UimdDynamicLibraryHandle handle = uimd_open_dynamic_library(names[index]);
        if (handle != 0)
        {
            return handle;
        }
    }

    const char* configured_path = getenv("UIMD_LIBSIXEL_PATH");
    UimdDynamicLibraryHandle handle = uimd_open_dynamic_library(configured_path);
    if (handle != 0)
    {
        return handle;
    }

    handle = uimd_open_named_library_from_delimited_dirs(getenv("UIMD_LIBSIXEL_DIR"), uimd_library_path_delimiter(), names);
    if (handle != 0)
    {
        return handle;
    }

#if defined(_WIN32)
    handle = uimd_open_named_library_from_delimited_dirs(getenv("PATH"), ';', names);
    if (handle != 0) { return handle; }
    handle = uimd_open_named_library_from_delimited_dirs(getenv("LIB"), ';', names);
    if (handle != 0) { return handle; }
#else
    handle = uimd_open_named_library_from_delimited_dirs(getenv("LD_LIBRARY_PATH"), ':', names);
    if (handle != 0) { return handle; }
#if defined(__APPLE__)
    handle = uimd_open_named_library_from_delimited_dirs(getenv("DYLD_LIBRARY_PATH"), ':', names);
    if (handle != 0) { return handle; }
    handle = uimd_open_named_library_from_delimited_dirs(getenv("DYLD_FALLBACK_LIBRARY_PATH"), ':', names);
    if (handle != 0) { return handle; }
#endif
#endif

    const char* homebrew = getenv("HOMEBREW_PREFIX");
    if (homebrew != 0 && homebrew[0] != '\0')
    {
        char path[UIMD_PATH_BUFFER_SIZE];
        if (snprintf(path, sizeof(path), "%s/lib", homebrew) > 0)
        {
            handle = uimd_open_named_library_from_delimited_dirs(path, uimd_library_path_delimiter(), names);
            if (handle != 0) { return handle; }
        }
        if (snprintf(path, sizeof(path), "%s/opt/libsixel/lib", homebrew) > 0)
        {
            handle = uimd_open_named_library_from_delimited_dirs(path, uimd_library_path_delimiter(), names);
            if (handle != 0) { return handle; }
        }
    }

#if defined(__APPLE__)
    static const char* directories[] = {
        "/opt/homebrew/opt/libsixel/lib",
        "/opt/homebrew/lib",
        "/usr/local/opt/libsixel/lib",
        "/usr/local/lib",
        "/opt/local/lib",
        0,
    };
#elif defined(_WIN32)
    static const char* directories[] = {
        "C:/Program Files/libsixel/bin",
        "C:/Program Files/libsixel/lib",
        "C:/Program Files (x86)/libsixel/bin",
        "C:/Program Files (x86)/libsixel/lib",
        0,
    };
#else
    static const char* directories[] = {
        "/usr/local/lib",
        "/usr/lib",
        "/usr/lib64",
        "/lib",
        "/lib64",
        "/usr/lib/x86_64-linux-gnu",
        "/usr/lib/aarch64-linux-gnu",
        "/usr/lib/arm-linux-gnueabihf",
        0,
    };
#endif
    for (int directory_index = 0; directories[directory_index] != 0; ++directory_index)
    {
        for (int name_index = 0; names[name_index] != 0; ++name_index)
        {
            handle = uimd_try_open_named_library_in_directory(directories[directory_index], names[name_index]);
            if (handle != 0)
            {
                return handle;
            }
        }
    }

    return 0;
}

static UimdSixelApi* uimd_load_sixel_api(void)
{
    static int attempted = 0;
    static UimdSixelApi api;
    static UimdSixelApi* cached = 0;
    if (attempted)
    {
        return cached;
    }
    attempted = 1;

    UimdDynamicLibraryHandle handle = uimd_open_sixel_library();
    if (handle == 0)
    {
        return 0;
    }

    api.output_new = (SixelOutputNewFunction)uimd_load_symbol(handle, "sixel_output_new");
    api.dither_new = (SixelDitherNewFunction)uimd_load_symbol(handle, "sixel_dither_new");
    api.dither_set_palette = (SixelDitherSetPaletteFunction)uimd_load_symbol(handle, "sixel_dither_set_palette");
    api.dither_set_pixel_format = (SixelDitherSetPixelFormatFunction)uimd_load_symbol(handle, "sixel_dither_set_pixelformat");
    api.dither_set_optimize_palette = (SixelDitherSetOptimizePaletteFunction)uimd_load_symbol(handle, "sixel_dither_set_optimize_palette");
    api.dither_set_diffusion_type = (SixelDitherSetDiffusionTypeFunction)uimd_load_symbol(handle, "sixel_dither_set_diffusion_type");
    api.encode = (SixelEncodeFunction)uimd_load_symbol(handle, "sixel_encode");
    api.output_unref = (SixelOutputUnrefFunction)uimd_load_symbol(handle, "sixel_output_unref");
    api.dither_unref = (SixelDitherUnrefFunction)uimd_load_symbol(handle, "sixel_dither_unref");
    if (api.output_new == 0 || api.dither_new == 0 || api.dither_set_palette == 0 ||
        api.dither_set_pixel_format == 0 || api.dither_set_optimize_palette == 0 ||
        api.dither_set_diffusion_type == 0 ||
        api.encode == 0 || api.output_unref == 0 || api.dither_unref == 0)
    {
        return 0;
    }
    cached = &api;
    return cached;
}

static int uimd_sixel_write(char* data, int size, void* priv)
{
    if (data == 0 || size <= 0 || priv == 0)
    {
        return 0;
    }
    UimdDynamicBuffer* buffer = (UimdDynamicBuffer*)priv;
    const int required = buffer->size + size;
    if (required > buffer->capacity)
    {
        int next_capacity = buffer->capacity > 0 ? buffer->capacity : 4096;
        while (next_capacity < required)
        {
            next_capacity *= 2;
        }
        char* next = (char*)realloc(buffer->data, (size_t)next_capacity);
        if (next == 0)
        {
            return 0;
        }
        buffer->data = next;
        buffer->capacity = next_capacity;
    }
    memcpy(buffer->data + buffer->size, data, (size_t)size);
    buffer->size += size;
    return size;
}

int uimd_decode_image_rgba(const char* path, UimdDecodedImage* output)
{
    if (path == 0 || output == 0)
    {
        return 0;
    }

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* decoded = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
    if (decoded == 0 || width <= 0 || height <= 0)
    {
        if (decoded != 0)
        {
            stbi_image_free(decoded);
        }
        output->width = 0;
        output->height = 0;
        output->rgba = 0;
        return 0;
    }

    output->width = width;
    output->height = height;
    output->rgba = decoded;
    return 1;
}

void uimd_free_decoded_image(UimdDecodedImage image)
{
    if (image.rgba != 0)
    {
        stbi_image_free(image.rgba);
    }
}

int uimd_encode_sixel_rgb(const uint8_t* rgb, int width, int height, UimdSixelPayload* output)
{
    if (output == 0)
    {
        return 0;
    }
    output->data = 0;
    output->size = 0;
    if (rgb == 0 || width <= 0 || height <= 0)
    {
        return 0;
    }

    UimdSixelApi* api = uimd_load_sixel_api();
    if (api == 0)
    {
        return 0;
    }

    UimdDynamicBuffer buffer;
    buffer.data = 0;
    buffer.size = 0;
    buffer.capacity = 0;

    sixel_output_t* sixel_output = 0;
    sixel_dither_t* dither = 0;
    if (!uimd_sixel_status_succeeded(api->output_new(&sixel_output, uimd_sixel_write, &buffer, 0)))
    {
        return 0;
    }
    if (!uimd_sixel_status_succeeded(api->dither_new(&dither, UIMD_SIXEL_MAX_COLORS, 0)))
    {
        api->output_unref(sixel_output);
        free(buffer.data);
        return 0;
    }

    unsigned char palette[UIMD_SIXEL_MAX_COLORS * 3];
    int palette_offset = 0;
    for (int red = 0; red < UIMD_SIXEL_COLOR_LEVELS; ++red)
    {
        for (int green = 0; green < UIMD_SIXEL_COLOR_LEVELS; ++green)
        {
            for (int blue = 0; blue < UIMD_SIXEL_COLOR_LEVELS; ++blue)
            {
                palette[palette_offset++] = (unsigned char)(red * 255 / (UIMD_SIXEL_COLOR_LEVELS - 1));
                palette[palette_offset++] = (unsigned char)(green * 255 / (UIMD_SIXEL_COLOR_LEVELS - 1));
                palette[palette_offset++] = (unsigned char)(blue * 255 / (UIMD_SIXEL_COLOR_LEVELS - 1));
            }
        }
    }
    unsigned char* mutable_rgb = (unsigned char*)rgb;
    api->dither_set_palette(dither, palette);
    api->dither_set_pixel_format(dither, UIMD_SIXEL_PIXEL_FORMAT_RGB888);
    api->dither_set_optimize_palette(dither, UIMD_SIXEL_OPTIMIZE_PALETTE);
    api->dither_set_diffusion_type(dither, UIMD_SIXEL_DIFFUSE_NONE);
    api->encode(mutable_rgb, width, height, 3, dither, sixel_output);
    api->dither_unref(dither);
    api->output_unref(sixel_output);

    if (buffer.size <= 0 || buffer.data == 0)
    {
        free(buffer.data);
        return 0;
    }
    output->data = buffer.data;
    output->size = buffer.size;
    return 1;
}

void uimd_free_sixel_payload(UimdSixelPayload payload)
{
    free(payload.data);
}
