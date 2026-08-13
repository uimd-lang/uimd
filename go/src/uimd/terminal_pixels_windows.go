//go:build !darwin && !linux

package uimd

func terminalCellPixelSize() Size {
	return Size{Width: imageCellPixelWidth, Height: imageCellPixelHeight}
}
