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
    name: "task_board",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "task_board", targets: ["task_board"]),
    ],
    dependencies: [
        .package(path: uimdPackagePath),
    ],
    targets: [
        .executableTarget(
            name: "task_board",
            dependencies: [.product(name: "Uimd", package: "Uimd")],
            path: ".",
            exclude: ["task_board.uimd", "message_box_yes_no.uimd", "task_dialog", "task_filters", "task_list", "task_row"],
            sources: ["task_board.swift", "task_board_ui.swift", "message_box_yes_no_ui.swift", "task_dialog_ui.swift", "task_filters_ui.swift", "task_list_ui.swift", "task_row_ui.swift"]
        ),
    ]
)
