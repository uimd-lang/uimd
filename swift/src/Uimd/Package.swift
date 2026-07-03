// swift-tools-version: 6.0
import PackageDescription

let package = Package(
    name: "Uimd",
    platforms: [.macOS(.v13)],
    products: [
        .library(name: "Uimd", targets: ["Uimd"]),
    ],
    targets: [
        .target(name: "CUimdImageDecoder"),
        .target(name: "Uimd", dependencies: ["CUimdImageDecoder"]),
        .testTarget(name: "UimdTests", dependencies: ["Uimd"]),
    ]
)
