#ifndef UIMD_IMAGE_DECODER_H
#define UIMD_IMAGE_DECODER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UimdDecodedImage
{
    int width;
    int height;
    uint8_t* rgba;
} UimdDecodedImage;

int uimd_decode_image_rgba(const char* path, UimdDecodedImage* output);
void uimd_free_decoded_image(UimdDecodedImage image);

#ifdef __cplusplus
}
#endif

#endif
