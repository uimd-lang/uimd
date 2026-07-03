// swift-tools-version: 6.0
import Foundation
import PackageDescription

let localUimdPackagePath = "../../src/Uimd"

func installedUimdPackagePath() -> String?
{
    let environment = ProcessInfo.processInfo.environment
    let home = environment["UIMD_HOME"] ?? environment["HOME"].map { $0 + "/.uimd" }
    guard let sdkHome = home else { return nil }
    let currentFile = sdkHome + "/current"
    guard let version = try? String(contentsOfFile: currentFile, encoding: .utf8).trimmingCharacters(in: .whitespacesAndNewlines), !version.isEmpty else { return nil }
    let packagePath = sdkHome + "/sdk/" + version + "/targets/swift"
    return FileManager.default.fileExists(atPath: packagePath + "/Package.swift") ? packagePath : nil
}

let uimdPackagePath = FileManager.default.fileExists(atPath: localUimdPackagePath + "/Package.swift")
    ? localUimdPackagePath
    : (installedUimdPackagePath() ?? localUimdPackagePath)

let package = Package(
    name: "image_browser",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "image_browser", targets: ["image_browser"]),
    ],
    dependencies: [
        .package(path: uimdPackagePath),
    ],
    targets: [
        .executableTarget(
            name: "image_browser",
            dependencies: [.product(name: "Uimd", package: "Uimd")],
            path: ".",
            exclude: ["gallery_item.uimd", "gallery_mosaic.uimd", "gallery_scroll.uimd", "gallery_view.uimd", "image_browser.uimd", "image_button.uimd", "image_list_button.uimd", "image_list_item.uimd", "image_list_scroll.uimd", "image_list_view.uimd", "image_show_dialog.uimd", "image_view.uimd"],
            sources: ["image_browser.swift", "gallery_item_ui.swift", "gallery_mosaic_ui.swift", "gallery_scroll_ui.swift", "gallery_view_ui.swift", "image_browser_ui.swift", "image_button_ui.swift", "image_list_button_ui.swift", "image_list_item_ui.swift", "image_list_scroll_ui.swift", "image_list_view_ui.swift", "image_show_dialog_ui.swift", "image_view_ui.swift"]
        ),
    ]
)
