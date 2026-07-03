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
    name: "contacts_manager",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "contacts_manager", targets: ["contacts_manager"]),
    ],
    dependencies: [
        .package(path: uimdPackagePath),
    ],
    targets: [
        .executableTarget(
            name: "contacts_manager",
            dependencies: [.product(name: "Uimd", package: "Uimd")],
            path: ".",
            exclude: ["contact_form", "contacts_manager.uimd", "data", "message_box_yes_no.uimd"],
            sources: ["contacts_manager.swift", "contact_form_ui.swift", "contacts_manager_ui.swift", "message_box_yes_no_ui.swift"]
        ),
    ]
)
