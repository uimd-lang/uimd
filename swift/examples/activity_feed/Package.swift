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
    name: "activity_feed",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "activity_feed", targets: ["activity_feed"]),
    ],
    dependencies: [
        .package(path: uimdPackagePath),
    ],
    targets: [
        .executableTarget(
            name: "activity_feed",
            dependencies: [.product(name: "Uimd", package: "Uimd")],
            path: ".",
            exclude: ["activity_feed.uimd", "activity_feed_panel", "activity_item", "settings"],
            sources: ["activity_feed.swift", "activity_feed_panel_ui.swift", "activity_feed_ui.swift", "activity_item_ui.swift", "settings_ui.swift"]
        ),
    ]
)
