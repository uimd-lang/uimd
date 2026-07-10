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

typedef struct UimdSixelPayload
{
    char* data;
    int size;
} UimdSixelPayload;

int uimd_decode_image_rgba(const char* path, UimdDecodedImage* output);
void uimd_free_decoded_image(UimdDecodedImage image);
int uimd_encode_sixel_rgb(const uint8_t* rgb, int width, int height, UimdSixelPayload* output);
void uimd_free_sixel_payload(UimdSixelPayload payload);

#ifdef __cplusplus
}
#endif

#endif
