//go:build darwin || linux

package uimd

import (
	"os"
	"syscall"
	"unsafe"
)

type terminalWindowSize struct {
	rows    uint16
	cols    uint16
	xPixels uint16
	yPixels uint16
}

func terminalCellPixelSize() Size {
	window := terminalWindowSize{}
	_, _, errno := syscall.Syscall(
		syscall.SYS_IOCTL,
		os.Stdout.Fd(),
		uintptr(syscall.TIOCGWINSZ),
		uintptr(unsafe.Pointer(&window)),
	)
	if errno == 0 && window.rows > 0 && window.cols > 0 && window.xPixels > 0 && window.yPixels > 0 {
		return Size{
			Width:  maxInt(minimumRenderableSize, int(window.xPixels)/int(window.cols)),
			Height: maxInt(minimumRenderableSize, int(window.yPixels)/int(window.rows)),
		}
	}
	return Size{Width: imageCellPixelWidth, Height: imageCellPixelHeight}
}
