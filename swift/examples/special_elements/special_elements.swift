import Foundation
import Uimd

public final class SpecialElementsApp: SpecialElementsUI
{
    // Override generated event hooks here.
}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            let app = SpecialElementsApp()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}
