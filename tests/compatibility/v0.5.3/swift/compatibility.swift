import Foundation
import Uimd

public final class CompatibilityApp: CompatibilityUI
{
    public override init()
    {
        super.init()
        scroller.setChild(CompatScrollUI())
    }
}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            let app = CompatibilityApp()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}
