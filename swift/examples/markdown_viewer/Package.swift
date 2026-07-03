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
    name: "markdown_viewer",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "markdown_viewer", targets: ["markdown_viewer"]),
    ],
    dependencies: [
        .package(path: uimdPackagePath),
    ],
    targets: [
        .executableTarget(
            name: "markdown_viewer",
            dependencies: [.product(name: "Uimd", package: "Uimd")],
            path: ".",
            exclude: ["blocks", "docs", "document_view", "markdown_viewer.uimd"],
            sources: ["markdown_viewer.swift", "code_block_ui.swift", "document_view_ui.swift", "heading_ui.swift", "markdown_viewer_ui.swift", "paragraph_ui.swift", "table_block_ui.swift"]
        ),
    ]
)
