//go:build cgo

package uimd

/*
#cgo CFLAGS: -I../../../cpp/third_party/stb -Ithird_party/stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdlib.h>
*/
import "C"

import "unsafe"

const stbRgbaChannels = 4

func loadDecodedImageRaster(path string) (imageRaster, bool) {
	cPath := C.CString(path)
	defer C.free(unsafe.Pointer(cPath))

	var width C.int
	var height C.int
	var channels C.int
	decoded := C.stbi_load(cPath, &width, &height, &channels, C.int(stbRgbaChannels))
	if decoded == nil || width <= 0 || height <= 0 {
		if decoded != nil {
			C.stbi_image_free(unsafe.Pointer(decoded))
		}
		return imageRaster{}, false
	}
	defer C.stbi_image_free(unsafe.Pointer(decoded))

	rasterWidth := int(width)
	rasterHeight := int(height)
	pixelCount := rasterWidth * rasterHeight
	data := unsafe.Slice((*byte)(unsafe.Pointer(decoded)), pixelCount*stbRgbaChannels)
	raster := imageRaster{
		width:  rasterWidth,
		height: rasterHeight,
		pixels: make([]imageRgb, 0, pixelCount),
		alpha:  make([]int, 0, pixelCount),
	}
	for offset := 0; offset+3 < len(data); offset += stbRgbaChannels {
		raster.pixels = append(raster.pixels, imageRgb{
			red:   int(data[offset]),
			green: int(data[offset+1]),
			blue:  int(data[offset+2]),
		})
		raster.alpha = append(raster.alpha, int(data[offset+3]))
	}
	return raster, true
}
