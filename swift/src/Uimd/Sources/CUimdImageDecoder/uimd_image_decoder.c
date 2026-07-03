#include "uimd_image_decoder.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
