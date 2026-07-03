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
    name: "expense_tracker",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "expense_tracker", targets: ["expense_tracker"]),
    ],
    dependencies: [
        .package(path: uimdPackagePath),
    ],
    targets: [
        .executableTarget(
            name: "expense_tracker",
            dependencies: [.product(name: "Uimd", package: "Uimd")],
            path: ".",
            exclude: ["categories_view.uimd", "expense_row", "expense_tracker.uimd", "expenses_list", "expenses_view.uimd", "settings_view.uimd"],
            sources: ["expense_tracker.swift", "categories_view_ui.swift", "expense_row_ui.swift", "expense_tracker_ui.swift", "expenses_list_ui.swift", "expenses_view_ui.swift", "settings_view_ui.swift"]
        ),
    ]
)
