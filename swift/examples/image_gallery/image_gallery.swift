import Foundation
import Uimd

public final class ImageGallery: ImageGalleryUI
{
    private var finished = false

    override public func onCloseBtnClick()
    {
        finished = true
    }

    override public func shouldClose() -> Bool
    {
        finished
    }
}

@main
enum Program
{
    static func main()
    {
        let exitCode = GeneratedWindowRuntime.runGeneratedAppMain
        {
            let app = ImageGallery()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(exitCode))
    }
}
