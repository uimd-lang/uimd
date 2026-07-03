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
    name: "text_editor",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "text_editor", targets: ["text_editor"]),
    ],
    dependencies: [
        .package(path: uimdPackagePath),
    ],
    targets: [
        .executableTarget(
            name: "text_editor",
            dependencies: [.product(name: "Uimd", package: "Uimd")],
            path: ".",
            exclude: ["notes", "text_editor.uimd"],
            sources: ["text_editor.swift", "file_browser_ui.swift", "message_box_yes_no_cancel_ui.swift", "message_box_yes_no_ui.swift", "text_editor_ui.swift"]
        ),
    ]
)
