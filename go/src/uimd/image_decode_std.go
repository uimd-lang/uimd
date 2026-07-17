//go:build !cgo

package uimd

func loadDecodedImageRaster(path string) (imageRaster, bool) {
	return imageRaster{}, false
}
