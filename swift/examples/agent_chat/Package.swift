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
    name: "agent_chat",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "agent_chat", targets: ["agent_chat"]),
    ],
    dependencies: [
        .package(path: uimdPackagePath),
    ],
    targets: [
        .executableTarget(
            name: "agent_chat",
            dependencies: [.product(name: "Uimd", package: "Uimd")],
            path: ".",
            exclude: ["agent_chat.uimd", "chat_entry", "transcript"],
            sources: ["agent_chat.swift", "agent_chat_ui.swift", "chat_entry_ui.swift", "transcript_ui.swift"]
        ),
    ]
)
