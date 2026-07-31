#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* uimd_stbi_load_rgba_from_memory(
    const unsigned char* data,
    int length,
    int* width,
    int* height,
    int* source_channels)
{
    return stbi_load_from_memory(
        data,
        length,
        width,
        height,
        source_channels,
        STBI_rgb_alpha);
}

void uimd_stbi_image_free(void* data)
{
    stbi_image_free(data);
}
