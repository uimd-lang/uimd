#include <cassert>
#include <memory>
#include <string_view>
#include <vector>

#include "ui/app/Application.hpp"
#include "ui/app/Control.hpp"
#include "ui/app/Window.hpp"
#include "ui/core/Color.hpp"
#include "ui/core/Event.hpp"
#include "ui/core/Layout.hpp"
#include "ui/core/Rect.hpp"
#include "ui/core/Style.hpp"
#include "ui/core/Version.hpp"
#include "ui/elements/Button.hpp"
#include "ui/elements/CheckBox.hpp"
#include "ui/elements/ComboBox.hpp"
#include "ui/elements/FrameBufferView.hpp"
#include "ui/elements/InfoLabel.hpp"
#include "ui/elements/Label.hpp"
#include "ui/elements/ListBox.hpp"
#include "ui/elements/NumberInput.hpp"
#include "ui/elements/ReusableElement.hpp"
#include "ui/elements/ScrollView.hpp"
#include "ui/elements/TextInput.hpp"
#include "ui/elements/TextArea.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"
#include "ui/terminal/Input.hpp"
#include "ui/terminal/TerminalBuffer.hpp"

#ifndef UIMD_EXPECTED_VERSION
#error "UIMD_EXPECTED_VERSION must be defined by the build system"
#endif

void run_dialog_tests();

int main() {
    constexpr int kResizeWidth = 80;
    constexpr int kResizeHeight = 24;
    constexpr int kLayoutWidth = 30;
    constexpr int kLayoutHeight = 10;
    constexpr int kWideCellWidth = 20;
    constexpr int kNarrowCellWidth = 5;
    constexpr int kPadding = 2;
    constexpr int kGap = 3;
    constexpr int kCellsBorderCol = 2;
    constexpr int kCellsBorderRow = 1;
    constexpr int kCellsGap = 2;
    constexpr int kCellsCell1Width = 17;
    constexpr int kCellsCell2Width = 9;
    constexpr int kCellsCell3Width = 4;
    constexpr int kCellsCell5Width = 21;
    constexpr int kCellsCell6Width = 5;
    constexpr int kCellsTopHeight = 1;
    constexpr int kCellsMiddleHeight = 2;
    constexpr int kCellsRootWidth = kCellsCell1Width + kCellsGap + kCellsCell2Width + kCellsGap + kCellsCell3Width;
    constexpr int kCellsRootHeight = 14;
    constexpr int kTextAreaTestWidth = 5;
    constexpr int kTextAreaTestHeight = 3;
    constexpr int kEmptyListWidth = 8;
    constexpr int kEmptyListHeight = 2;
    constexpr int kReusableScrollChildWidth = 10;
    constexpr int kReusableScrollChildHeight = 2;
    constexpr int kReusableScrollWrapperStaleHeight = 20;
    constexpr int kReusableScrollViewportHeight = 5;
    constexpr int kVerticalTrailingSourceWidth = 10;
    constexpr int kVerticalTrailingSourceHeight = 6;
    constexpr int kVerticalTrailingRenderWidth = 20;
    constexpr int kVerticalTrailingRenderHeight = 12;
    constexpr int kVerticalTrailingButtonWidth = 5;
    constexpr int kVerticalTrailingButtonRow = 5;
    constexpr int kVerticalTrailingButtonCol = 3;
    constexpr int kVerticalTrailingPhotoSourceHeight = 4;
    constexpr int kVerticalTrailingPhotoMarginBottom = 2;
    constexpr int kVerticalTrailingGap = 1;
    constexpr std::string_view kAnsiSyncUpdateBegin = "\x1b[?2026h";
    constexpr std::string_view kAnsiSyncUpdateEnd = "\x1b[?2026l";

    assert(ui::runtimeVersion() == std::string_view(UIMD_EXPECTED_VERSION));

    const ui::Rect rect{1, 2, 3, 4};
    assert(rect.contains(ui::Point{1, 2}));
    assert(rect.contains(ui::Point{4, 4}));
    assert(!rect.contains(ui::Point{5, 4}));

    const ui::TerminalBuffer buffer{10, 5};
    assert(buffer.width() == 10);
    assert(buffer.height() == 5);
    ui::InputParser terminalPixelResponseParser;
    assert(terminalPixelResponseParser.feed("\x1b[6;10;8t").empty());
    assert(terminalPixelResponseParser.feed("\x1b[4;350;720t").empty());

    const ui::Color shortHex{"#abc"};
    assert(shortHex.kind() == ui::Color::Kind::Rgb);
    assert(shortHex.rgba().has_value());
    assert(shortHex.rgba()->red == 0xaa);
    assert(shortHex.rgba()->green == 0xbb);
    assert(shortHex.rgba()->blue == 0xcc);

    const ui::Color fullHex{"#112233"};
    assert(fullHex.toString() == "#112233");
    assert(ui::Color{"transparent"}.isTransparent());
    assert(ui::Color{"blue"}.kind() == ui::Color::Kind::Named);

    ui::Style base;
    base.color = ui::Color{"#111111"};
    base.background = ui::Color{"#222222"};
    base.padding = 1;

    ui::Style override;
    override.background = ui::Color{"#333333"};
    override.borderWidthHorizontal = 1;
    override.borderWidthVertical = 2;
    override.gap = 2;
    override.textAlign = "center";

    const ui::Style merged = base.merged(override);
    assert(merged.color == base.color);
    assert(merged.background == override.background);
    assert(merged.borderWidthHorizontal == override.borderWidthHorizontal);
    assert(merged.borderWidthVertical == override.borderWidthVertical);
    assert(merged.padding == base.padding);
    assert(merged.gap == override.gap);
    assert(merged.textAlign == override.textAlign);

    ui::Window window{"Core"};
    ui::Element& root = window.addElement(std::make_unique<ui::Control>("root"));
    ui::Element& child = root.addChild<ui::Control>("child");
    assert(window.elements().size() == 1);
    assert(root.parent() == nullptr);
    assert(child.parent() == &root);
    assert(root.children().size() == 1);

    auto appWindow = std::make_unique<ui::Window>("App");
    ui::Window* rawWindow = appWindow.get();
    ui::Application app;
    ui::Window& opened = app.open(std::move(appWindow));
    assert(&opened == rawWindow);
    assert(app.activeWindow() == rawWindow);

    ui::Style widgetStyle;
    widgetStyle.color = ui::Color{"#111111"};
    widgetStyle.background = ui::Color{"#222222"};
    ui::Style focusStyle;
    focusStyle.background = ui::Color{"#333333"};
    ui::Style editStyle;
    editStyle.background = ui::Color{"#444444"};
    ui::Style cursorStyle;
    cursorStyle.color = ui::Color{"#000000"};
    cursorStyle.background = ui::Color{"#ffffff"};
    ui::Style selectedStyle;
    selectedStyle.background = ui::Color{"#555555"};
    ui::Style checkedStyle;
    checkedStyle.color = ui::Color{"#00ff00"};

    ui::Label label{"label", "Hello"};
    label.setStyle(widgetStyle);
    assert(ui::renderedText(label.render(ui::Size{8, 1}))[0] == "Hello   ");
    assert(label.render(ui::Size{8, 1})[0][0].foreground == widgetStyle.color);
    ui::Style centeredLabelStyle = widgetStyle;
    centeredLabelStyle.textAlign = "center";
    label.setStyle(centeredLabelStyle);
    assert(ui::renderedText(label.render(ui::Size{9, 1}))[0] == "  Hello  ");
    label.setText("Alpha\r");
    label.setStyle(widgetStyle);
    assert(ui::renderedText(label.render(ui::Size{7, 1}))[0] == "Alpha  ");

    ui::InfoLabel infoLabel{"info", "Glow\nNext"};
    infoLabel.setStyle(widgetStyle);
    const ui::RenderedContent infoRendered = infoLabel.render(ui::Size{8, 2});
    assert(ui::renderedText(infoRendered)[0] == "Glow    ");
    assert(ui::renderedText(infoRendered)[1] == "Next    ");
    assert(infoRendered[0][0].foreground.has_value());

    ui::Button button{"button", "Run"};
    button.setStyle(widgetStyle);
    button.setFocusStyle(focusStyle);
    assert(ui::renderedText(button.render(ui::Size{7, 1}))[0] == "  Run  ");
    const ui::RenderedContent focusedButton = button.render(ui::Size{7, 1}, ui::ElementRenderState{.focused = true});
    assert(ui::renderedText(focusedButton)[0] == " [Run] ");
    assert(focusedButton[0][0].background == focusStyle.background);
    const ui::Button wideButton{"wide", "Save"};
    assert(ui::renderedText(wideButton.render(ui::Size{14, 1}, ui::ElementRenderState{.focused = true}))[0] == " [   Save   ] ");
    const ui::Button narrowButton{"narrow", "DEL"};
    assert(ui::renderedText(narrowButton.render(ui::Size{3, 1}))[0] == "DEL");

    class TestGeneratedWindow : public ui::GeneratedWindowBase {
    public:
        TestGeneratedWindow() : ui::GeneratedWindowBase("Generated") {
            ui::Style style;
            style.background = ui::Color{"#111111"};
            style.borderColor = ui::Color{"#222222"};
            style.borderWidthHorizontal = 1;
            style.borderWidthVertical = 1;
            setGeneratedWindowStyle(style);
            label = &addElement<ui::Label>("hello", "World");
            setGeneratedLayout({
                ui::GeneratedLayoutEntry{
                    .name = "hello",
                    .type = "label",
                    .relative = ui::Rect{0, 0, 5, 1},
                    .sourceCell = ui::Rect{0, 0, 5, 1},
                    .width = ui::AxisDimension::fixed(5),
                    .height = ui::AxisDimension::fixed(1),
                    .charsSize = ui::Size{5, 1},
                    .cellCharsSize = ui::Size{5, 1},
                },
            });
        }

        ui::Label* label = nullptr;
    };

    TestGeneratedWindow generatedWindow;
    ui::TerminalBuffer generatedBuffer{20, 6};
    ui::renderGeneratedWindow(generatedWindow, generatedBuffer);
    assert(generatedBuffer.cell(2, 7).text == "W");
    assert(generatedBuffer.cell(2, 11).text == "d");
    assert(generatedBuffer.cell(1, 6).background == ui::Color{"#111111"});
    assert(generatedBuffer.cell(1, 6).foreground == ui::Color{"#222222"});
    assert(generatedBuffer.cell(3, 12).background == ui::Color{"#111111"});

    ui::ViewHost viewHost{"main"};
    viewHost.setView(std::make_unique<TestGeneratedWindow>());
    assert(viewHost.currentView() != nullptr);
    bool viewHostRenderedChild = false;
    for (const std::string& line : ui::renderedText(viewHost.render(ui::Size{8, 3}))) {
        viewHostRenderedChild = viewHostRenderedChild || line.find("World") != std::string::npos;
    }
    assert(viewHostRenderedChild);
    viewHost.clearView();
    assert(viewHost.currentView() == nullptr);

    class TexturedGeneratedWindow : public ui::GeneratedWindowBase {
    public:
        TexturedGeneratedWindow() : ui::GeneratedWindowBase("Textured") {
            ui::Style style;
            style.background = ui::Color{"#111111"};
            setGeneratedWindowStyle(style);

            ui::Style cellStyle;
            cellStyle.background = ui::Color{"#222222"};
            cellStyle.backgroundTexture = "ab";
            cellStyle.backgroundTextureColor = ui::Color{"#abcdef"};

            setGeneratedLayout({
                ui::GeneratedLayoutEntry{
                    .name = "",
                    .type = "",
                    .cellName = "accent",
                    .relative = ui::Rect{0, 0, 4, 2},
                    .sourceCell = ui::Rect{0, 0, 4, 2},
                    .width = ui::AxisDimension::fixed(4),
                    .height = ui::AxisDimension::fixed(2),
                    .cellWidth = ui::AxisDimension::fixed(4),
                    .cellHeight = ui::AxisDimension::fixed(2),
                    .charsSize = ui::Size{4, 2},
                    .cellCharsSize = ui::Size{4, 2},
                    .cellStyle = cellStyle,
                },
            });
        }
    };

    TexturedGeneratedWindow texturedWindow;
    const ui::RenderedContent textured = ui::renderGeneratedWindowContent(texturedWindow, ui::Size{4, 2});
    assert(textured[0][0].text == "a");
    assert(textured[0][1].text == "b");
    assert(textured[1][0].text == "a");
    assert(textured[0][0].foreground == ui::Color{"#abcdef"});
    assert(textured[0][0].background == ui::Color{"#222222"});

    class ReusableChildWindow : public ui::GeneratedWindowBase {
    public:
        ReusableChildWindow() : ui::GeneratedWindowBase("Reusable Child") {
            label = &addElement<ui::Label>("nested", "Nested");
            setGeneratedLayout({
                ui::GeneratedLayoutEntry{
                    .name = "nested",
                    .type = "label",
                    .relative = ui::Rect{0, 0, 6, 1},
                    .sourceCell = ui::Rect{0, 0, 6, 1},
                    .width = ui::AxisDimension::fixed(6),
                    .height = ui::AxisDimension::fixed(1),
                    .cellWidth = ui::AxisDimension::fixed(6),
                    .cellHeight = ui::AxisDimension::fixed(1),
                    .charsSize = ui::Size{6, 1},
                    .cellCharsSize = ui::Size{6, 1},
                },
            });
        }

        ui::Label* label = nullptr;
    };

    ui::ReusableElement reusable{"reusable", std::make_unique<ReusableChildWindow>()};
    const ui::RenderedContent reusableRendered = reusable.render(ui::Size{8, 2});
    assert(ui::renderedText(reusableRendered)[0].find("Nested") != std::string::npos);

    class ScrollReusableChildWindow : public ui::GeneratedWindowBase {
    public:
        ScrollReusableChildWindow() : ui::GeneratedWindowBase("Scroll Reusable Child") {
            label = &addElement<ui::Label>("label", "Hi");
            setGeneratedLayout({
                ui::GeneratedLayoutEntry{
                    .name = "label",
                    .type = "label",
                    .relative = ui::Rect{0, 0, kReusableScrollChildWidth, kReusableScrollChildHeight},
                    .sourceCell = ui::Rect{0, 0, kReusableScrollChildWidth, kReusableScrollChildHeight},
                    .width = ui::AxisDimension::fixed(kReusableScrollChildWidth),
                    .height = ui::AxisDimension::fixed(kReusableScrollChildHeight),
                    .cellWidth = ui::AxisDimension::fixed(kReusableScrollChildWidth),
                    .cellHeight = ui::AxisDimension::fixed(kReusableScrollChildHeight),
                    .charsSize = ui::Size{kReusableScrollChildWidth, kReusableScrollChildHeight},
                    .cellCharsSize = ui::Size{kReusableScrollChildWidth, kReusableScrollChildHeight},
                },
            });
        }

        ui::Label* label = nullptr;
    };

    ui::ScrollView reusableScroll{"reusable-scroll"};
    auto staleReusable = std::make_unique<ui::ReusableElement>(
        "stale",
        std::make_unique<ScrollReusableChildWindow>()
    );
    staleReusable->setFrame(ui::Rect{0, 0, kReusableScrollChildWidth, kReusableScrollWrapperStaleHeight});
    reusableScroll.addChild(std::move(staleReusable));
    assert(reusableScroll.contentHeight(kReusableScrollChildWidth) == kReusableScrollChildHeight);
    const std::vector<ui::ScrollViewChildView> reusableChildViews =
        reusableScroll.childViews(ui::Size{kReusableScrollChildWidth, kReusableScrollViewportHeight});
    assert(reusableChildViews.size() == 1);
    assert(reusableChildViews.front().frame.height == kReusableScrollChildHeight);

    class PaddedCellGeneratedWindow : public ui::GeneratedWindowBase {
    public:
        PaddedCellGeneratedWindow() : ui::GeneratedWindowBase("Padded") {
            label = &addElement<ui::Label>("label", "Pad");
            ui::Style cellStyle;
            cellStyle.background = ui::Color{"#202020"};
            cellStyle.paddingTop = 1;
            cellStyle.paddingRight = 1;
            cellStyle.paddingBottom = 0;
            cellStyle.paddingLeft = 2;
            setGeneratedLayout({
                ui::GeneratedLayoutEntry{
                    .name = "label",
                    .type = "label",
                    .relative = ui::Rect{0, 0, 3, 1},
                    .sourceCell = ui::Rect{0, 0, 8, 3},
                    .width = ui::AxisDimension::expanded(),
                    .height = ui::AxisDimension::fixed(1),
                    .cellWidth = ui::AxisDimension::fixed(8),
                    .cellHeight = ui::AxisDimension::fixed(3),
                    .charsSize = ui::Size{3, 1},
                    .cellCharsSize = ui::Size{8, 3},
                    .cellStyle = cellStyle,
                },
            });
        }

        ui::Label* label = nullptr;
    };

    PaddedCellGeneratedWindow paddedWindow;
    const ui::RenderedContent padded = ui::renderGeneratedWindowContent(paddedWindow, ui::Size{8, 3});
    assert(paddedWindow.label->frame().row == 1);
    assert(paddedWindow.label->frame().col == 2);
    assert(ui::renderedText(padded)[1].substr(2, 3) == "Pad");

    class SeparatedGeneratedWindow : public ui::GeneratedWindowBase {
    public:
        SeparatedGeneratedWindow() : ui::GeneratedWindowBase("Separated") {
            ui::Style style;
            style.background = ui::Color{"#111111"};
            style.borderWidthHorizontal = 1;
            style.borderWidthVertical = 1;
            setGeneratedWindowStyle(style);
            first = &addElement<ui::Label>("first", "A");
            second = &addElement<ui::Label>("second", "B");
            setGeneratedLayout({
                ui::GeneratedLayoutEntry{
                    .name = "first",
                    .type = "label",
                    .relative = ui::Rect{0, 0, 5, 1},
                    .sourceCell = ui::Rect{0, 0, 5, 1},
                    .width = ui::AxisDimension::fixed(5),
                    .height = ui::AxisDimension::fixed(1),
                    .cellWidth = ui::AxisDimension::fixed(5),
                    .cellHeight = ui::AxisDimension::fixed(1),
                    .charsSize = ui::Size{5, 1},
                    .cellCharsSize = ui::Size{5, 1},
                },
                ui::GeneratedLayoutEntry{
                    .name = "second",
                    .type = "label",
                    .relative = ui::Rect{0, 0, 5, 1},
                    .sourceCell = ui::Rect{2, 0, 5, 1},
                    .width = ui::AxisDimension::fixed(5),
                    .height = ui::AxisDimension::fixed(1),
                    .cellWidth = ui::AxisDimension::fixed(5),
                    .cellHeight = ui::AxisDimension::fixed(1),
                    .charsSize = ui::Size{5, 1},
                    .cellCharsSize = ui::Size{5, 1},
                },
            });
        }

        ui::Label* first = nullptr;
        ui::Label* second = nullptr;
    };

    SeparatedGeneratedWindow separatedWindow;
    const ui::Size separatedContentSize = ui::generatedWindowContentSize(separatedWindow);
    assert(separatedContentSize.width == 5);
    assert(separatedContentSize.height == 3);
    ui::TerminalBuffer separatedBuffer{20, 8};
    ui::renderGeneratedWindow(separatedWindow, separatedBuffer);
    assert(separatedWindow.second->frame().row - separatedWindow.first->frame().row == 2);

    class ExpandedGeneratedWindow : public ui::GeneratedWindowBase {
    public:
        ExpandedGeneratedWindow() : ui::GeneratedWindowBase("Expanded") {
            ui::Style style;
            style.background = ui::Color{"#111111"};
            style.borderWidthHorizontal = 0;
            style.borderWidthVertical = 0;
            setGeneratedWindowStyle(style);
            title = &addElement<ui::Label>("title", "HERON");
            chat = &addElement<ui::Label>("chat", "chat");
            prompt = &addElement<ui::TextArea>("prompt", "");
            tokens = &addElement<ui::Label>("tokens", "tokens");
            setGeneratedLayout({
                ui::GeneratedLayoutEntry{
                    .name = "title",
                    .type = "label",
                    .relative = ui::Rect{0, 0, 0, 1},
                    .sourceCell = ui::Rect{0, 0, 19, 1},
                    .width = ui::AxisDimension::expanded(),
                    .height = ui::AxisDimension::fixed(1),
                    .cellWidth = ui::AxisDimension::expanded(),
                    .cellHeight = ui::AxisDimension::fixed(1),
                    .charsSize = ui::Size{19, 1},
                    .cellCharsSize = ui::Size{19, 1},
                },
                ui::GeneratedLayoutEntry{
                    .name = "chat",
                    .type = "label",
                    .relative = ui::Rect{1, 1, 0, 0},
                    .sourceCell = ui::Rect{2, 0, 19, 8},
                    .width = ui::AxisDimension::expanded(),
                    .height = ui::AxisDimension::expanded(),
                    .cellWidth = ui::AxisDimension::fixed(19),
                    .cellHeight = ui::AxisDimension::expanded(),
                    .charsSize = ui::Size{17, 6},
                    .cellCharsSize = ui::Size{19, 8},
                    .marginRight = 1,
                    .marginBottom = 1,
                },
                ui::GeneratedLayoutEntry{
                    .name = "prompt",
                    .type = "textarea",
                    .relative = ui::Rect{0, 1, 0, 3},
                    .sourceCell = ui::Rect{11, 0, 19, 3},
                    .width = ui::AxisDimension::expanded(),
                    .height = ui::AxisDimension::fixed(3),
                    .cellWidth = ui::AxisDimension::expanded(),
                    .cellHeight = ui::AxisDimension::fixed(3),
                    .charsSize = ui::Size{17, 3},
                    .cellCharsSize = ui::Size{19, 3},
                    .marginRight = 1,
                },
                ui::GeneratedLayoutEntry{
                    .name = "tokens",
                    .type = "label",
                    .relative = ui::Rect{0, 1, 17, 1},
                    .sourceCell = ui::Rect{15, 0, 19, 1},
                    .width = ui::AxisDimension::fixed(17),
                    .height = ui::AxisDimension::fixed(1),
                    .cellWidth = ui::AxisDimension::fixed(19),
                    .cellHeight = ui::AxisDimension::fixed(1),
                    .charsSize = ui::Size{17, 1},
                    .cellCharsSize = ui::Size{19, 1},
                    .marginRight = 1,
                },
            });
        }

        ui::Label* title = nullptr;
        ui::Label* chat = nullptr;
        ui::TextArea* prompt = nullptr;
        ui::Label* tokens = nullptr;
    };

    ExpandedGeneratedWindow expandedWindow;
    ui::TerminalBuffer expandedBuffer{80, 24};
    ui::renderGeneratedWindow(expandedWindow, expandedBuffer);
    assert(expandedWindow.title->frame().row == 0);
    assert(expandedWindow.title->frame().col == 0);
    assert(expandedWindow.title->frame().width == 80);
    assert(expandedWindow.title->frame().height == 1);
    assert(expandedWindow.chat->frame().row == 2);
    assert(expandedWindow.chat->frame().col == 1);
    assert(expandedWindow.chat->frame().width == 78);
    assert(expandedWindow.chat->frame().height == 17);
    assert(expandedWindow.prompt->frame().row == 20);
    assert(expandedWindow.prompt->frame().col == 1);
    assert(expandedWindow.prompt->frame().width == 78);
    assert(expandedWindow.prompt->frame().height == 3);
    assert(expandedWindow.tokens->frame().row == 23);
    assert(expandedWindow.tokens->frame().col == 1);
    assert(expandedWindow.tokens->frame().width == 17);
    assert(expandedWindow.tokens->frame().height == 1);

    class VerticalTrailingGeneratedWindow : public ui::GeneratedWindowBase {
    public:
        VerticalTrailingGeneratedWindow() : ui::GeneratedWindowBase("VerticalTrailing") {
            ui::Style style;
            style.borderWidthHorizontal = 0;
            style.borderWidthVertical = 0;
            setGeneratedWindowStyle(style);
            photo = &addElement<ui::Label>("photo", "photo");
            close = &addElement<ui::Button>("close_btn", "Close");
            setGeneratedLayout({
                ui::GeneratedLayoutEntry{
                    .name = "photo",
                    .type = "label",
                    .relative = ui::Rect{0, 0, kVerticalTrailingSourceWidth, kVerticalTrailingPhotoSourceHeight},
                    .sourceCell = ui::Rect{0, 0, kVerticalTrailingSourceWidth, kVerticalTrailingSourceHeight},
                    .width = ui::AxisDimension::expanded(),
                    .height = ui::AxisDimension::expanded(),
                    .cellWidth = ui::AxisDimension::expanded(),
                    .cellHeight = ui::AxisDimension::expanded(),
                    .charsSize = ui::Size{kVerticalTrailingSourceWidth, kVerticalTrailingPhotoSourceHeight},
                    .cellCharsSize = ui::Size{kVerticalTrailingSourceWidth, kVerticalTrailingSourceHeight},
                    .marginBottom = kVerticalTrailingPhotoMarginBottom,
                },
                ui::GeneratedLayoutEntry{
                    .name = "close_btn",
                    .type = "button",
                    .relative = ui::Rect{
                        kVerticalTrailingButtonRow,
                        kVerticalTrailingButtonCol,
                        kVerticalTrailingButtonWidth,
                        1,
                    },
                    .sourceCell = ui::Rect{0, 0, kVerticalTrailingSourceWidth, kVerticalTrailingSourceHeight},
                    .width = ui::AxisDimension::fixed(kVerticalTrailingButtonWidth),
                    .height = ui::AxisDimension::fixed(1),
                    .cellWidth = ui::AxisDimension::expanded(),
                    .cellHeight = ui::AxisDimension::expanded(),
                    .charsSize = ui::Size{kVerticalTrailingButtonWidth, 1},
                    .cellCharsSize = ui::Size{kVerticalTrailingSourceWidth, kVerticalTrailingSourceHeight},
                    .marginRight = 2,
                    .marginBottom = 0,
                },
            });
        }

        ui::Label* photo = nullptr;
        ui::Button* close = nullptr;
    };

    VerticalTrailingGeneratedWindow verticalTrailingWindow;
    ui::TerminalBuffer verticalTrailingBuffer{kVerticalTrailingRenderWidth, kVerticalTrailingRenderHeight};
    ui::renderGeneratedWindow(verticalTrailingWindow, verticalTrailingBuffer);
    assert(verticalTrailingWindow.photo->frame().height == kVerticalTrailingRenderHeight - kVerticalTrailingPhotoMarginBottom);
    assert(verticalTrailingWindow.close->frame().row == kVerticalTrailingRenderHeight - 1);
    assert(verticalTrailingWindow.close->frame().row ==
           verticalTrailingWindow.photo->frame().row + verticalTrailingWindow.photo->frame().height + kVerticalTrailingGap);

    ui::CheckBox checkbox{"checkbox", "Debug", false};
    checkbox.setStyle(widgetStyle);
    checkbox.setCheckedStyle(checkedStyle);
    assert(ui::renderedText(checkbox.render(ui::Size{10, 1}))[0] == "[ ] Debug ");
    assert(checkbox.handleKey("Enter"));
    const ui::RenderedContent checkedBox = checkbox.render(ui::Size{10, 1});
    assert(ui::renderedText(checkedBox)[0] == "[x] Debug ");
    assert(checkedBox[0][1].foreground == checkedStyle.color);

    ui::TextInput input{"input", "abc", 10};
    input.setStyle(widgetStyle);
    input.setEditStyle(editStyle);
    input.setCursorStyle(cursorStyle);
    input.setCursor(1);
    const ui::RenderedContent editInput = input.render(ui::Size{5, 1}, ui::ElementRenderState{.editMode = true});
    assert(ui::renderedText(editInput)[0] == "abc  ");
    assert(editInput[0][1].background == cursorStyle.background);
    assert(input.handleKey("X"));
    assert(input.value() == "aXbc");
    input.selectRange(1, 3);
    const ui::RenderedContent selectedInput = input.render(ui::Size{5, 1}, ui::ElementRenderState{.editMode = true});
    assert(selectedInput[0][1].background == cursorStyle.background);
    assert(selectedInput[0][2].background == cursorStyle.background);
    assert(input.handleKey("Y"));
    assert(input.value() == "aYc");
    assert(!input.handleKey("Alt+Enter"));
    ui::TextArea textArea{"area", input.value(), 10};
    textArea.setStyle(widgetStyle);
    textArea.setCursorStyle(cursorStyle);
    textArea.setCursor(input.cursor());
    assert(textArea.handleKey("Enter"));
    assert(textArea.handleKey("z"));
    const ui::RenderedContent multilineInput = textArea.render(ui::Size{5, 2}, ui::ElementRenderState{.editMode = true});
    assert(ui::renderedText(multilineInput)[0] == "aY   ");
    assert(ui::renderedText(multilineInput)[1] == "zc   ");
    textArea.setCursor(0);
    assert(textArea.handleKey("Shift+Down"));
    assert(textArea.selectedText() == "aY\n");
    const ui::RenderedContent shiftedDownInput = textArea.render(ui::Size{5, 2}, ui::ElementRenderState{.editMode = true});
    assert(shiftedDownInput[0][0].background == cursorStyle.background);
    assert(shiftedDownInput[0][1].background == cursorStyle.background);
    assert(shiftedDownInput[0][2].background != cursorStyle.background);
    assert(textArea.cursorForPoint(1, 1, ui::Size{5, 2}) == 4);
    ui::TextArea emptyTextArea{"empty_area", "", 0};
    emptyTextArea.setStyle(widgetStyle);
    emptyTextArea.setCursorStyle(cursorStyle);
    const ui::RenderedContent emptyRendered = emptyTextArea.render(
        ui::Size{kTextAreaTestWidth, kTextAreaTestHeight},
        ui::ElementRenderState{.editMode = true});
    assert(emptyRendered[0][0].background == cursorStyle.background);
    assert(emptyRendered[1][0].background != cursorStyle.background);
    assert(emptyRendered[2][0].background != cursorStyle.background);
    const std::string rawTextAreaValue = std::string("Plain line\nTabbed\tline\nControl\x1bline\nWide") +
        "\xE4\xB8\xAD" + "line";
    ui::TextArea rawTextArea{"raw_area", rawTextAreaValue, 0};
    rawTextArea.setStyle(widgetStyle);
    rawTextArea.setCursorStyle(cursorStyle);
    auto highlightedCount = [&](const ui::RenderedContent& content) {
        int count = 0;
        for (const ui::RenderedRow& row : content) {
            for (const ui::TerminalCell& cell : row) {
                if (cell.background == cursorStyle.background) {
                    ++count;
                }
            }
        }
        return count;
    };
    for (const int cursor : {
             0,
             1,
             static_cast<int>(rawTextAreaValue.find('\t')),
             static_cast<int>(rawTextAreaValue.find('\x1b')),
             static_cast<int>(rawTextAreaValue.find("\xE4\xB8\xAD")),
             static_cast<int>(rawTextAreaValue.size()),
         }) {
        rawTextArea.setCursor(cursor);
        rawTextArea.selectRange(cursor, cursor);
        assert(highlightedCount(rawTextArea.render(ui::Size{20, 4}, ui::ElementRenderState{.editMode = true})) == 1);
    }
    const int rawTabIndex = static_cast<int>(rawTextAreaValue.find('\t'));
    rawTextArea.selectRange(rawTabIndex, rawTabIndex + 1);
    const ui::RenderedContent rawTabSelection = rawTextArea.render(ui::Size{20, 4}, ui::ElementRenderState{.editMode = true});
    assert(rawTabSelection[1][6].background == cursorStyle.background);
    assert(rawTabSelection[1][7].background == cursorStyle.background);
    assert(rawTabSelection[1][8].background != cursorStyle.background);
    ui::TextInput clickedInput{"clicked", "", 0};
    clickedInput.selectRange(0, 0);
    assert(clickedInput.handleKey("a"));
    assert(clickedInput.handleKey("b"));
    assert(clickedInput.value() == "ab");
    ui::TextArea pastedInput{"pasted", "", 0};
    pastedInput.insertText("one\rtwo\nthree\r\nfour");
    assert(pastedInput.value() == "one\ntwo\nthree\nfour");
    pastedInput.setCursor(0);
    assert(pastedInput.scrollByRows(1, 2));
    const ui::RenderedContent scrolledTextArea = pastedInput.render(ui::Size{8, 2}, ui::ElementRenderState{.editMode = true});
    assert(ui::renderedText(scrolledTextArea)[0] == "two    ^");
    assert(ui::renderedText(scrolledTextArea)[1] == "three  v");
    ui::TextArea viewOnlyTextArea{"view_only", "one\ntwo\nthree\nfour", 0};
    viewOnlyTextArea.setCursor(static_cast<int>(viewOnlyTextArea.value().size()));
    const ui::RenderedContent viewOnlyRendered = viewOnlyTextArea.render(ui::Size{8, 2});
    assert(ui::renderedText(viewOnlyRendered)[0] == "one     ");
    assert(ui::renderedText(viewOnlyRendered)[1] == "two    v");
    ui::TextArea wrappedArea{"wrapped", "alpha beta gamma", 0};
    const ui::RenderedContent wrappedTextArea = wrappedArea.render(ui::Size{10, 3});
    assert(ui::renderedText(wrappedTextArea)[0] == "alpha     ");
    assert(ui::renderedText(wrappedTextArea)[1] == "beta      ");
    assert(ui::renderedText(wrappedTextArea)[2] == "gamma     ");
    ui::TextArea hardWrappedArea{"hard_wrapped", "abcdefghijX", 0};
    const ui::RenderedContent hardWrappedTextArea = hardWrappedArea.render(ui::Size{5, 3});
    assert(ui::renderedText(hardWrappedTextArea)[0] == "abcde");
    assert(ui::renderedText(hardWrappedTextArea)[1] == "fghij");
    assert(ui::renderedText(hardWrappedTextArea)[2] == "X    ");
    assert(wrappedArea.cursorForPoint(1, 2, ui::Size{10, 3}) == 8);
    ui::TextInput fastInput{"fast", "0123456789", 0};
    fastInput.setCursor(0);
    assert(fastInput.handleKey("Alt+Right"));
    assert(fastInput.cursor() == 5);
    assert(fastInput.handleKey("Alt+Left"));
    assert(fastInput.cursor() == 0);
    ui::TextArea fastArea{"fast_area", "a\nb\nc\nd\ne", 0};
    fastArea.setCursor(0);
    assert(fastArea.handleKey("Alt+Down"));
    assert(fastArea.cursor() == 6);

    ui::NumberInput number{"number", 2.0, 0.5};
    number.setCursorStyle(cursorStyle);
    assert(ui::renderedText(number.render(ui::Size{4, 1}))[0] == "2   ");
    assert(number.handleKey("Up"));
    assert(number.value() == 2.5);
    assert(number.handleKey("Backspace"));
    assert(number.handleKey("4"));
    assert(number.handleKey("Enter"));
    assert(number.value() == 2.4);
    number.beginEdit();
    number.setEditCursor(0);
    const ui::RenderedContent editNumber = number.render(ui::Size{4, 1}, ui::ElementRenderState{.editMode = true});
    assert(editNumber[0][0].background == cursorStyle.background);
    number.setEditCursor(3);
    assert(number.handleKey("Backspace"));
    assert(number.handleKey("Backspace"));
    assert(number.handleKey("Backspace"));
    assert(ui::renderedText(number.render(ui::Size{4, 1}, ui::ElementRenderState{.editMode = true}))[0] == "    ");
    number.cancelEdit();
    assert(number.value() == 2.4);
    number.setEditText("");
    assert(ui::renderedText(number.render(ui::Size{4, 1}, ui::ElementRenderState{.editMode = true}))[0] == "    ");
    assert(number.handleKey("3"));
    assert(number.handleKey("4"));
    number.commitEdit();
    assert(number.value() == 34.0);
    ui::NumberInput zeroNumber{"zero", 0.0};
    zeroNumber.beginEdit();
    zeroNumber.setEditCursor(1);
    assert(zeroNumber.handleKey("5"));
    assert(zeroNumber.handleKey("Enter"));
    assert(zeroNumber.value() == 5.0);
    ui::NumberInput insertedNumber{"inserted", 500.0};
    insertedNumber.beginEdit();
    insertedNumber.setEditCursor(0);
    assert(insertedNumber.handleKey("1"));
    assert(insertedNumber.handleKey("Enter"));
    assert(insertedNumber.value() == 1500.0);
    ui::NumberInput homeNumber{"home", 500.0};
    homeNumber.beginEdit();
    assert(homeNumber.handleKey("Home"));
    assert(homeNumber.handleKey("1"));
    assert(homeNumber.handleKey("Enter"));
    assert(homeNumber.value() == 1500.0);

    ui::ComboBox combo{"combo", {"one", "two", "three"}};
    assert(ui::renderedText(combo.render(ui::Size{8, 1}, ui::ElementRenderState{.focused = true}))[0] == "one    >");
    assert(ui::renderedText(combo.render(ui::Size{8, 1}, ui::ElementRenderState{.focused = true, .passiveFocus = true}))[0] == "one    >");
    assert(combo.handleKey("Down"));
    assert(combo.selectedIndex() == 1);
    assert(ui::renderedText(combo.render(ui::Size{8, 1}, ui::ElementRenderState{.focused = true}))[0] == "two    >");
    const ui::RenderedContent openCombo = combo.render(ui::Size{8, 3}, ui::ElementRenderState{.editMode = true});
    assert(ui::renderedText(openCombo)[1] == "  one   ");
    assert(ui::renderedText(openCombo)[2] == "  two   ");

    ui::ListBox list{"list", {"one", "two", "three"}};
    list.setStyle(widgetStyle);
    list.setSelectedStyle(selectedStyle);
    assert(ui::renderedText(list.render(ui::Size{8, 2}))[0] == "one     ");
    assert(list.handleKey("Down"));
    const ui::RenderedContent listRendered = list.render(ui::Size{8, 2});
    assert(ui::renderedText(listRendered)[1] == "two    v");
    assert(listRendered[1][0].background == selectedStyle.background);
    assert(list.handleKey("Down"));
    const ui::RenderedContent scrolledList = list.render(ui::Size{8, 2});
    assert(ui::renderedText(scrolledList)[0] == "two    ^");
    assert(ui::renderedText(scrolledList)[1] == "three   ");
    ui::ListBox emptyList{"empty-list", {}};
    emptyList.setStyle(widgetStyle);
    emptyList.setSelectedStyle(selectedStyle);
    const ui::RenderedContent emptyListRendered = emptyList.render(ui::Size{kEmptyListWidth, kEmptyListHeight});
    assert(ui::renderedText(emptyListRendered)[0] == "        ");
    assert(ui::renderedText(emptyListRendered)[1] == "        ");
    assert(emptyListRendered[0][0].background != selectedStyle.background);
    ui::ListBox transparentEditList{"transparent-list", {"one"}};
    ui::Style translucentBase;
    translucentBase.background = ui::Color{"#ffffff11"};
    ui::Style transparentEdit;
    transparentEdit.background = ui::Color{"#00000000"};
    transparentEditList.setStyle(translucentBase);
    transparentEditList.setEditStyle(transparentEdit);
    const ui::RenderedContent editList = transparentEditList.render(ui::Size{8, 1}, ui::ElementRenderState{.editMode = true});
    assert(editList[0][0].background == translucentBase.background);
    list.scrollBy(-1, 2);
    const ui::RenderedContent wheelList = list.render(ui::Size{8, 2});
    assert(ui::renderedText(wheelList)[0] == "one     ");
    assert(ui::renderedText(wheelList)[1] == "two    v");

    ui::ScrollView tallScroll{"tall"};
    tallScroll.addChild(ui::renderPlainText("01234", 1, 5, widgetStyle));
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[0] == "0");
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[1] == "1");
    assert(!tallScroll.handleWheel(1, ui::Size{1, 3}));
    assert(tallScroll.handleWheel(-1, ui::Size{1, 3}));
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[0] == "^");
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[1] == "2");
    assert(tallScroll.handleWheel(-1, ui::Size{1, 3}));
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[0] == "^");
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[1] == "3");
    assert(!tallScroll.handleWheel(-1, ui::Size{1, 3}));
    assert(tallScroll.handleWheel(1, ui::Size{1, 3}));
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[1] == "2");
    assert(tallScroll.handleKey("Down", ui::Size{1, 3}));
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[1] == "3");
    assert(tallScroll.handleKey("Up", ui::Size{1, 3}));
    assert(ui::renderedText(tallScroll.render(ui::Size{1, 3}))[1] == "2");

    ui::ScrollView fastScroll{"fast"};
    fastScroll.addChild(ui::renderPlainText("0123456789abcdefghijklmnopqrst", 1, 30, widgetStyle));
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[0] == "0");
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[1] == "1");
    assert(!fastScroll.handleKey("Alt+Up", ui::Size{1, 12}));
    assert(fastScroll.handleKey("Alt+Down", ui::Size{1, 12}));
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[0] == "^");
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[1] == "5");
    assert(fastScroll.handleKey("Alt+Up", ui::Size{1, 12}));
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[0] == "0");
    assert(fastScroll.handleWheel(-3, ui::Size{1, 12}));
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[0] == "^");
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[1] == "d");
    assert(fastScroll.handleWheel(3, ui::Size{1, 12}));
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[0] == "0");
    assert(fastScroll.handleKey("Ctrl+Down", ui::Size{1, 12}));
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[0] == "^");
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[1] == "d");
    assert(fastScroll.handleKey("Ctrl+Up", ui::Size{1, 12}));
    assert(ui::renderedText(fastScroll.render(ui::Size{1, 12}))[0] == "0");

    ui::ScrollView manyScroll{"many", 1};
    manyScroll.addChild(ui::renderPlainText("A", 1, 1, widgetStyle));
    manyScroll.addChild(ui::renderPlainText("B", 1, 1, widgetStyle));
    manyScroll.addChild(ui::renderPlainText("C", 1, 1, widgetStyle));
    manyScroll.addChild(ui::renderPlainText("D", 1, 1, widgetStyle));
    manyScroll.scrollToBottom(ui::Size{1, 3});
    assert(ui::renderedText(manyScroll.render(ui::Size{1, 3}))[0] == "^");
    assert(ui::renderedText(manyScroll.render(ui::Size{1, 3}))[1] == " ");
    assert(ui::renderedText(manyScroll.render(ui::Size{1, 3}))[2] == "D");
    assert(!manyScroll.handleWheel(-1, ui::Size{1, 3}));

    manyScroll.invalidateHeightCache();
    const std::vector<int>& cachedHeights = manyScroll.cachedChildHeights();
    assert(cachedHeights.size() == 4);
    assert(cachedHeights[0] == 1);
    assert(cachedHeights[3] == 1);

    ui::ScrollView autoScroll{"auto"};
    autoScroll.setAutoScroll(true);
    autoScroll.addChild(ui::renderPlainText("A", 1, 1, widgetStyle));
    autoScroll.addChild(ui::renderPlainText("B", 1, 1, widgetStyle));
    autoScroll.addChild(ui::renderPlainText("C", 1, 1, widgetStyle));
    autoScroll.addChild(ui::renderPlainText("D", 1, 1, widgetStyle));
    assert(ui::renderedText(autoScroll.render(ui::Size{1, 2}))[0] == "^");
    assert(ui::renderedText(autoScroll.render(ui::Size{1, 2}))[1] == "D");

    ui::FrameBufferView frameView{"frame"};
    frameView.setFrames(
        {{"app", {{ui::FrameBufferCell{.text = "\xE2\x96\x90", .foreground = "#38bdf8", .background = "#0b1018"}}}}},
        {"app"}
    );
    const ui::RenderedContent frameRendered = frameView.render(ui::Size{1, 1});
    assert(frameRendered[0][0].text == std::string("\xE2\x96\x90"));

    ui::Button disabledButton{"disabled", "Disabled"};
    ui::Style disabledButtonStyle;
    disabledButtonStyle.color = ui::Color{"#64748b"};
    disabledButton.setDisabledStyle(disabledButtonStyle);
    disabledButton.setEnabled(false);
    assert(!disabledButton.enabled());
    assert(disabledButton.effectiveStyle(true, false).color == ui::Color{"#64748b"});

    const ui::Event resize{.type = ui::EventType::Resize, .size = ui::Size{kResizeWidth, kResizeHeight}};
    assert(resize.type == ui::EventType::Resize);
    assert(resize.size.width == kResizeWidth);
    assert(resize.size.height == kResizeHeight);

    ui::TerminalBuffer diffBuffer{4, 2};
    assert(diffBuffer.renderDiff().empty());

    diffBuffer.setCell(0, 0, ui::TerminalCell{.text = "A"});
    const std::string oneCell = diffBuffer.renderDiff();
    assert(oneCell.find("\x1b[1;1H") != std::string::npos);
    assert(oneCell.find("A") != std::string::npos);
    assert(diffBuffer.renderDiff().empty());

    diffBuffer.setCell(0, 1, ui::TerminalCell{.text = "B", .foreground = ui::Color{"#ff0000"}});
    diffBuffer.setCell(0, 2, ui::TerminalCell{.text = "C", .foreground = ui::Color{"#00ff00"}});
    const std::string colorSplit = diffBuffer.renderDiff();
    assert(colorSplit.find("38;2;255;0;0") != std::string::npos);
    assert(colorSplit.find("38;2;0;255;0") != std::string::npos);

    diffBuffer.clear();
    const std::string cleared = diffBuffer.renderDiff();
    assert(cleared.find(" ") != std::string::npos);

    diffBuffer.resize(4, 2);
    assert(diffBuffer.renderDiff().empty());

    diffBuffer.resize(2, 1);
    const std::string resized = diffBuffer.renderDiff();
    assert(resized.find("\x1b[1;1H") != std::string::npos);

    ui::TerminalBuffer rawDiffBuffer{2, 1};
    rawDiffBuffer.setCell(0, 0, ui::TerminalCell{.text = " ", .raw = "RAW", .rawWidth = 2, .rawHeight = 1});
    rawDiffBuffer.setCell(0, 1, ui::TerminalCell{.text = " ", .rawSkip = true});
    const std::string rawDiff = rawDiffBuffer.renderDiff();
    const std::size_t rawSyncBegin = rawDiff.find(kAnsiSyncUpdateBegin);
    const std::size_t rawPayload = rawDiff.find("RAW");
    const std::size_t rawSyncEnd = rawDiff.find(kAnsiSyncUpdateEnd);
    assert(rawSyncBegin == 0);
    assert(rawPayload != std::string::npos);
    assert(rawSyncEnd != std::string::npos);
    assert(rawSyncBegin < rawPayload);
    assert(rawPayload < rawSyncEnd);

    ui::InputParser parser;
    std::vector<ui::Event> events = parser.feed("a\t\r\x7f\x03");
    assert(events.size() == 5);
    assert(events[0].type == ui::EventType::Key);
    assert(events[0].key == "a");
    assert(events[1].key == "Tab");
    assert(events[2].key == "Enter");
    assert(events[3].key == "Backspace");
    assert(events[4].key == "Ctrl+C");

    events = parser.feed("\x1b[A\x1b[B\x1b[C\x1b[D\x1b[Z");
    assert(events.size() == 5);
    assert(events[0].key == "Up");
    assert(events[1].key == "Down");
    assert(events[2].key == "Right");
    assert(events[3].key == "Left");
    assert(events[4].key == "Shift+Tab");

    events = parser.feed("\x1b[1;2D\x1b[1;2C\x1b[1;2A\x1b[1;2B");
    assert(events.size() == 4);
    assert(events[0].key == "Shift+Left");
    assert(events[1].key == "Shift+Right");
    assert(events[2].key == "Shift+Up");
    assert(events[3].key == "Shift+Down");
    events = parser.feed("\x1b[1;3D\x1b[1;3C\x1b[1;3A\x1b[1;3B");
    assert(events.size() == 4);
    assert(events[0].key == "Alt+Left");
    assert(events[1].key == "Alt+Right");
    assert(events[2].key == "Alt+Up");
    assert(events[3].key == "Alt+Down");
    events = parser.feed("\x1b[1;5D\x1b[1;5C\x1b[1;5A\x1b[1;5B");
    assert(events.size() == 4);
    assert(events[0].key == "Ctrl+Left");
    assert(events[1].key == "Ctrl+Right");
    assert(events[2].key == "Ctrl+Up");
    assert(events[3].key == "Ctrl+Down");

    ui::InputParser altEnterParser;
    events = altEnterParser.feed("\x1b\r\x1b[13;3u");
    assert(events.size() == 2);
    assert(events[0].key == "Alt+Enter");
    assert(events[1].key == "Alt+Enter");
    events = altEnterParser.feed("\x1b[13;9u");
    assert(events.size() == 1);
    assert(events[0].key == "Alt+Enter");
    events = altEnterParser.feed("\x1b[99;5u");
    assert(events.size() == 1);
    assert(events[0].key == "Ctrl+C");
    events = altEnterParser.feed("\x1b[99;9u\x1b[118;9u");
    assert(events.size() == 2);
    assert(events[0].key == "cmd_c");
    assert(events[1].key == "cmd_v");
    events = altEnterParser.feed("\x1b[3;5u\x1b[27;5;99~");
    assert(events.size() == 2);
    assert(events[0].key == "Ctrl+C");
    assert(events[1].key == "Ctrl+C");
    ui::InputParser partialAltEnterParser;
    events = partialAltEnterParser.feed("\x1b[13;");
    assert(events.empty());
    events = partialAltEnterParser.feed("9u");
    assert(events.size() == 1);
    assert(events[0].key == "Alt+Enter");
    events = partialAltEnterParser.feed("\x1b");
    assert(events.size() == 1);
    assert(events[0].key == "Escape");

    events = parser.feed("\x1b[200~hello\nworld\x1b[201~");
    assert(events.size() == 1);
    assert(events[0].type == ui::EventType::Paste);
    assert(events[0].text == "hello\nworld");

    events = parser.feed("\x1b[<64;10;20M\x1b[<65;10;20M");
    assert(events.size() == 2);
    assert(events[0].type == ui::EventType::MouseWheel);
    assert(events[0].wheelDelta == 1);
    assert(events[0].position.row == 19);
    assert(events[0].position.col == 9);
    assert(events[1].wheelDelta == -1);

    events = parser.feed("\x1b[<0;3;4M\x1b[<32;5;6M\x1b[<0;3;4m");
    assert(events.size() == 3);
    assert(events[0].type == ui::EventType::MousePress);
    assert(events[0].position.row == 3);
    assert(events[0].position.col == 2);
    assert(events[1].type == ui::EventType::MouseDrag);
    assert(events[2].type == ui::EventType::MouseRelease);

    events = parser.feed("\x1b");
    assert(events.size() == 1);
    assert(events[0].key == "Escape");
    events = parser.feed("[A");
    assert(events.size() == 2);
    assert(events[0].key == "[");
    assert(events[1].key == "A");

    ui::LayoutResolver layoutResolver;

    ui::LayoutCell rowRoot{
        .name = "root",
        .width = ui::AxisDimension::fixed(kLayoutWidth),
        .height = ui::AxisDimension::fixed(kLayoutHeight),
        .gap = kGap,
        .direction = ui::LayoutDirection::Row,
        .children = {
            ui::LayoutCell{.name = "fixed", .width = ui::AxisDimension::fixed(kNarrowCellWidth), .height = ui::AxisDimension::expanded()},
            ui::LayoutCell{.name = "fill", .width = ui::AxisDimension::expanded(), .height = ui::AxisDimension::expanded()},
        },
    };
    ui::ResolvedCell resolvedRow = layoutResolver.resolve(rowRoot, ui::Rect{});
    assert(resolvedRow.rect.width == kLayoutWidth);
    assert(resolvedRow.rect.height == kLayoutHeight);
    assert(resolvedRow.children.size() == 2);
    assert(resolvedRow.children[0].rect.width == kNarrowCellWidth);
    assert(resolvedRow.children[0].rect.height == kLayoutHeight);
    assert(resolvedRow.children[1].rect.col == kNarrowCellWidth + kGap);
    assert(resolvedRow.children[1].rect.width == kLayoutWidth - kNarrowCellWidth - kGap);

    ui::LayoutCell fitRoot{
        .name = "fit",
        .width = ui::AxisDimension::fitContent(),
        .height = ui::AxisDimension::fitContent(),
        .fitContentSize = ui::Size{7, 2},
        .padding = kPadding,
    };
    ui::ResolvedCell resolvedFit = layoutResolver.resolve(fitRoot, ui::Rect{});
    assert(resolvedFit.rect.width == 7 + (kPadding * 2));
    assert(resolvedFit.rect.height == 2 + (kPadding * 2));

    ui::LayoutCell elementRoot{
        .name = "element-root",
        .width = ui::AxisDimension::fixed(kWideCellWidth),
        .height = ui::AxisDimension::fixed(kLayoutHeight),
        .padding = kPadding,
        .elements = {
            ui::LayoutElement{
                .name = "prompt",
                .relative = ui::Rect{1, 2, 4, 1},
            },
            ui::LayoutElement{
                .name = "label",
                .relative = ui::Rect{3, 1, 0, 0},
                .width = ui::AxisDimension::fitContent(),
                .height = ui::AxisDimension::fitContent(),
                .fitContentSize = ui::Size{6, 1},
            },
        },
    };
    ui::ResolvedCell resolvedElements = layoutResolver.resolve(elementRoot, ui::Rect{5, 6, kWideCellWidth, kLayoutHeight});
    assert(resolvedElements.elements.size() == 2);
    assert(resolvedElements.elements[0].rect.row == 5 + kPadding + 1);
    assert(resolvedElements.elements[0].rect.col == 6 + kPadding + 2);
    assert(resolvedElements.elements[0].rect.width == 4);
    assert(resolvedElements.elements[1].rect.width == 6);

    ui::LayoutCell columnRoot{
        .name = "column",
        .width = ui::AxisDimension::fixed(kWideCellWidth),
        .height = ui::AxisDimension::fixed(kLayoutHeight),
        .gap = kGap,
        .direction = ui::LayoutDirection::Column,
        .children = {
            ui::LayoutCell{.name = "top", .width = ui::AxisDimension::expanded(), .height = ui::AxisDimension::fixed(2)},
            ui::LayoutCell{.name = "bottom", .width = ui::AxisDimension::expanded(), .height = ui::AxisDimension::expanded()},
        },
    };
    ui::ResolvedCell resolvedColumn = layoutResolver.resolve(columnRoot, ui::Rect{});
    assert(resolvedColumn.children[0].rect.height == 2);
    assert(resolvedColumn.children[1].rect.row == 2 + kGap);
    assert(resolvedColumn.children[1].rect.height == kLayoutHeight - 2 - kGap);
    assert(resolvedColumn.children[1].rect.width == kWideCellWidth);

    ui::LayoutCell lookupRoot{
        .name = "lookup-root",
        .width = ui::AxisDimension::fixed(kWideCellWidth),
        .height = ui::AxisDimension::fixed(kLayoutHeight),
        .children = {
            ui::LayoutCell{
                .name = "child-a",
                .width = ui::AxisDimension::fixed(kNarrowCellWidth),
                .height = ui::AxisDimension::fixed(2),
                .elements = {
                    ui::LayoutElement{.name = "nested-label", .relative = ui::Rect{0, 1, 3, 1}},
                },
            },
            ui::LayoutCell{.name = "child-b", .width = ui::AxisDimension::fixed(kNarrowCellWidth), .height = ui::AxisDimension::fixed(2)},
        },
    };
    const ui::ResolvedCell lookupResolved = layoutResolver.resolve(lookupRoot, ui::Rect{});
    const ui::ResolvedCell* childA = ui::findCellByName(lookupResolved, "child-a");
    assert(childA != nullptr);
    assert(childA->rect.row == 0);
    assert(ui::findCellByName(lookupResolved, "missing") == nullptr);

    const ui::ResolvedElement* nestedLabel = ui::findElementByName(lookupResolved, "nested-label");
    assert(nestedLabel != nullptr);
    assert(nestedLabel->rect.col == 1);
    assert(ui::findElementByName(lookupResolved, "missing") == nullptr);

    const std::vector<ui::FlatResolvedCell> flatCells = ui::flattenCells(lookupResolved);
    assert(flatCells.size() == 3);
    assert(flatCells[0].name == "lookup-root");
    assert(flatCells[1].name == "child-a");
    assert(flatCells[2].name == "child-b");

    const std::vector<ui::FlatResolvedElement> flatElements = ui::flattenElements(lookupResolved);
    assert(flatElements.size() == 1);
    assert(flatElements[0].cellName == "child-a");
    assert(flatElements[0].name == "nested-label");

    ui::LayoutCell cellsRoot{
        .name = "cells-root",
        .width = ui::AxisDimension::fixed(kCellsRootWidth),
        .height = ui::AxisDimension::fixed(kCellsRootHeight),
        .gap = 1,
        .direction = ui::LayoutDirection::Column,
        .children = {
            ui::LayoutCell{
                .name = "top-row",
                .width = ui::AxisDimension::expanded(),
                .height = ui::AxisDimension::fixed(kCellsTopHeight),
                .gap = kCellsGap,
                .direction = ui::LayoutDirection::Row,
                .children = {
                    ui::LayoutCell{
                        .name = "cell1",
                        .width = ui::AxisDimension::fixed(kCellsCell1Width),
                        .height = ui::AxisDimension::expanded(),
                        .elements = {ui::LayoutElement{.name = "a1", .relative = ui::Rect{0, 1, 2, 1}}},
                    },
                    ui::LayoutCell{
                        .name = "cell2",
                        .width = ui::AxisDimension::fixed(kCellsCell2Width),
                        .height = ui::AxisDimension::expanded(),
                        .elements = {ui::LayoutElement{.name = "b1", .relative = ui::Rect{0, 0, 3, 1}}},
                    },
                    ui::LayoutCell{
                        .name = "cl3",
                        .width = ui::AxisDimension::fixed(kCellsCell3Width),
                        .height = ui::AxisDimension::expanded(),
                        .elements = {ui::LayoutElement{.name = "d1", .relative = ui::Rect{0, 0, 2, 1}}},
                    },
                },
            },
            ui::LayoutCell{
                .name = "cell4",
                .width = ui::AxisDimension::expanded(),
                .height = ui::AxisDimension::fixed(kCellsMiddleHeight),
            },
            ui::LayoutCell{
                .name = "bottom-row",
                .width = ui::AxisDimension::expanded(),
                .height = ui::AxisDimension::expanded(),
                .gap = kCellsGap,
                .direction = ui::LayoutDirection::Row,
                .children = {
                    ui::LayoutCell{.name = "cell5", .width = ui::AxisDimension::fixed(kCellsCell5Width), .height = ui::AxisDimension::expanded()},
                    ui::LayoutCell{
                        .name = "cl6",
                        .width = ui::AxisDimension::fixed(kCellsCell6Width),
                        .height = ui::AxisDimension::expanded(),
                        .elements = {ui::LayoutElement{.name = "f1", .relative = ui::Rect{0, 0, 2, 1}}},
                    },
                    ui::LayoutCell{.name = "cl7", .width = ui::AxisDimension::expanded(), .height = ui::AxisDimension::expanded()},
                },
            },
        },
    };

    const ui::ResolvedCell cellsResolved = layoutResolver.resolve(
        cellsRoot,
        ui::Rect{kCellsBorderRow, kCellsBorderCol, kCellsRootWidth, kCellsRootHeight}
    );
    const ui::ResolvedCell* cell1 = ui::findCellByName(cellsResolved, "cell1");
    const ui::ResolvedCell* cell2 = ui::findCellByName(cellsResolved, "cell2");
    const ui::ResolvedCell* cl3 = ui::findCellByName(cellsResolved, "cl3");
    const ui::ResolvedCell* cl6 = ui::findCellByName(cellsResolved, "cl6");
    assert(cell1 != nullptr);
    assert(cell2 != nullptr);
    assert(cl3 != nullptr);
    assert(cl6 != nullptr);
    assert(cell1->rect.col == 2);
    assert(cell2->rect.col == 21);
    assert(cl3->rect.col == 32);
    assert(cl6->rect.col == 25);

    const ui::ResolvedElement* a1 = ui::findElementByName(cellsResolved, "a1");
    const ui::ResolvedElement* b1 = ui::findElementByName(cellsResolved, "b1");
    const ui::ResolvedElement* d1 = ui::findElementByName(cellsResolved, "d1");
    const ui::ResolvedElement* f1 = ui::findElementByName(cellsResolved, "f1");
    assert(a1 != nullptr);
    assert(b1 != nullptr);
    assert(d1 != nullptr);
    assert(f1 != nullptr);
    assert(a1->rect.row == 1);
    assert(a1->rect.col == 3);
    assert(b1->rect.col == 21);
    assert(d1->rect.col == 32);
    assert(f1->rect.col == 25);

    run_dialog_tests();

    return 0;
}
