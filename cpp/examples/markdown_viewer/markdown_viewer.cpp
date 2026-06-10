#include "markdown_viewer_ui.hpp"

#include "document_view/document_view.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{

constexpr std::size_t kExpectedLayoutEntries = 8;
const std::vector<std::string> kPreferredDocuments = {
    "overview.markdown",
    "styles.markdown",
    "tables.markdown",
    "code.markdown",
};

std::filesystem::path docsDir()
{
    return std::filesystem::path(__FILE__).parent_path() / "docs";
}

std::string readTextFile(const std::filesystem::path& path)
{
    std::ifstream input(path);
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

std::vector<std::string> splitLines(std::string_view text)
{
    std::vector<std::string> lines;
    std::size_t offset = 0;
    while (offset <= text.size())
    {
        const std::size_t newline = text.find('\n', offset);
        if (newline == std::string_view::npos)
        {
            lines.emplace_back(text.substr(offset));
            break;
        }
        lines.emplace_back(text.substr(offset, newline - offset));
        offset = newline + 1;
    }
    return lines;
}

std::string trim(std::string_view text)
{
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin])))
    {
        ++begin;
    }
    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])))
    {
        --end;
    }
    return std::string(text.substr(begin, end - begin));
}

bool isTableSeparator(std::string_view line)
{
    const std::string value = trim(line);
    if (value.size() < 3 || value.front() != '|' || value.back() != '|')
    {
        return false;
    }
    for (char ch : value)
    {
        if (ch != '|' && ch != ' ' && ch != '-' && ch != ':')
        {
            return false;
        }
    }
    return true;
}

std::vector<markdown_viewer_example::MarkdownBlock> parseMarkdown(std::string_view text)
{
    const std::vector<std::string> lines = splitLines(text);
    std::vector<markdown_viewer_example::MarkdownBlock> blocks;
    std::vector<std::string> paragraph;
    std::size_t index = 0;

    const auto flushParagraph = [&]
    {
        std::ostringstream joined;
        for (std::size_t line = 0; line < paragraph.size(); ++line)
        {
            if (line > 0)
            {
                joined << '\n';
            }
            joined << paragraph[line];
        }
        const std::string value = trim(joined.str());
        if (!value.empty())
        {
            blocks.push_back(markdown_viewer_example::MarkdownBlock{.kind = "paragraph", .text = value});
        }
        paragraph.clear();
    };

    while (index < lines.size())
    {
        const std::string& line = lines[index];
        if (trim(line).empty())
        {
            flushParagraph();
            ++index;
        }
        else if (line.rfind("```", 0) == 0)
        {
            flushParagraph();
            std::string language = trim(std::string_view(line).substr(3));
            if (language.empty())
            {
                language = "text";
            }
            ++index;
            std::ostringstream code;
            bool first = true;
            while (index < lines.size() && lines[index].rfind("```", 0) != 0)
            {
                if (!first)
                {
                    code << '\n';
                }
                code << lines[index];
                first = false;
                ++index;
            }
            if (index < lines.size())
            {
                ++index;
            }
            blocks.push_back(markdown_viewer_example::MarkdownBlock{.kind = "code", .text = code.str(), .language = language});
        }
        else if (!line.empty() && line.front() == '#')
        {
            flushParagraph();
            const std::size_t firstText = line.find_first_not_of('#');
            const int level = static_cast<int>(firstText == std::string::npos ? line.size() : firstText);
            blocks.push_back(markdown_viewer_example::MarkdownBlock{
                .kind = "heading",
                .level = level,
                .text = trim(firstText == std::string::npos ? std::string_view{} : std::string_view(line).substr(firstText)),
            });
            ++index;
        }
        else if (line.rfind("|", 0) == 0 && index + 1 < lines.size() && isTableSeparator(lines[index + 1]))
        {
            flushParagraph();
            std::ostringstream table;
            table << line << '\n' << lines[index + 1];
            index += 2;
            while (index < lines.size() && lines[index].rfind("|", 0) == 0)
            {
                table << '\n' << lines[index];
                ++index;
            }
            blocks.push_back(markdown_viewer_example::MarkdownBlock{.kind = "table", .text = table.str()});
        }
        else
        {
            paragraph.push_back(line);
            ++index;
        }
    }
    flushParagraph();
    return blocks;
}

std::map<std::string, std::string> loadDocuments()
{
    std::map<std::string, std::string> docs;
    for (const std::string& name : kPreferredDocuments)
    {
        const std::filesystem::path path = docsDir() / name;
        if (std::filesystem::exists(path))
        {
            docs[name] = readTextFile(path);
        }
    }
    for (const auto& entry : std::filesystem::directory_iterator(docsDir()))
    {
        const std::string name = entry.path().filename().string();
        if (entry.path().extension() == ".markdown" && !docs.contains(name))
        {
            docs[name] = readTextFile(entry.path());
        }
    }
    return docs;
}

class MarkdownViewerApp : public MarkdownViewerUI
{
public:
    MarkdownViewerApp()
    {
        documents_ = loadDocuments();
        std::vector<std::string> names;
        names.reserve(documents_.size());
        for (const std::string& preferred : kPreferredDocuments)
        {
            if (documents_.contains(preferred))
            {
                names.push_back(preferred);
            }
        }
        for (const auto& [name, _] : documents_)
        {
            if (std::find(names.begin(), names.end(), name) == names.end())
            {
                names.push_back(name);
            }
        }
        docs->setOptions(std::move(names));
        docs->setSelectedIndex(0);
        showSelected();
    }

    [[nodiscard]] MarkdownViewerUI& window()
    {
        return *this;
    }

    void showSelected()
    {
        const auto& options = docs->options();
        if (options.empty())
        {
            return;
        }
        const std::string& name = options[static_cast<std::size_t>(docs->selectedIndex())];
        const auto it = documents_.find(name);
        const std::vector<markdown_viewer_example::MarkdownBlock> blocks = parseMarkdown(it == documents_.end() ? "" : it->second);
        documentView().setBlocks(blocks);
        status->setText(name + " - " + std::to_string(blocks.size()) + " blocks - arrows preview");
    }

    void scroll(int direction)
    {
        const std::string_view key = direction > 0 ? "ArrowUp" : "ArrowDown";
        (void)documentView().handleKey(key);
    }

    void focus(std::string_view name)
    {
        if (name == "viewer")
        {
            status->setText("viewer focus - Enter scroll mode, Esc navigation, arrows/PageUp/PageDown/Home/End scroll");
        }
        else if (name == "up_btn" || name == "down_btn")
        {
            status->setText("buttons scroll the viewer; Right focuses viewer");
        }
    }

    [[nodiscard]] ui::GeneratedWindowRuntimeOptions runtimeOptions() override
    {
        ui::GeneratedWindowRuntimeOptions options = MarkdownViewerUI::runtimeOptions();
        options.initialFocusName = "docs";
        options.startInEditMode = true;
        options.onFocusChanged = [this](std::string_view name, bool focused)
        {
            if (focused)
            {
                this->focus(name);
            }
        };
        return options;
    }

protected:
    void onUpBtnClick() override
    {
        scroll(1);
    }
    void onDownBtnClick() override
    {
        scroll(-1);
    }
    void onCloseBtnClick() override
    {
        finished_ = true;
    }
    void onDocsSelectionChange(const std::vector<std::string>&) override
    {
        showSelected();
    }

    bool shouldClose() const override
    {
        return finished_;
    }

private:
    [[nodiscard]] markdown_viewer_example::DocumentView& documentView()
    {
        auto* view = dynamic_cast<markdown_viewer_example::DocumentView*>(viewer->child());
        if (view == nullptr)
        {
            throw std::runtime_error("Document view child is not initialized.");
        }
        return *view;
    }

    [[nodiscard]] const markdown_viewer_example::DocumentView& documentView() const
    {
        auto* view = dynamic_cast<const markdown_viewer_example::DocumentView*>(viewer->child());
        if (view == nullptr)
        {
            throw std::runtime_error("Document view child is not initialized.");
        }
        return *view;
    }

    bool finished_ = false;
    std::map<std::string, std::string> documents_;
};

void smoke()
{
    MarkdownViewerApp app;
    MarkdownViewerUI& window = app.window();
    assert(window.title() == "Markdown Viewer");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(window.header != nullptr);
    assert(window.docs != nullptr);
    assert(window.viewer != nullptr);
    assert(window.close_btn != nullptr);
    assert(window.docs->options().size() == 4);
    assert(window.status->text().find("blocks") != std::string::npos);
}

void logicTest()
{
    MarkdownViewerApp app;
    MarkdownViewerUI& window = app.window();
    auto* documentView = dynamic_cast<markdown_viewer_example::DocumentView*>(window.viewer->child());
    assert(documentView != nullptr);
    assert(documentView->contentHeight() > 0);
    assert(window.status->text().find("overview.markdown") == 0);
    assert(!ui::renderGeneratedWindowContent(window, ui::Size{80, 24}, 0, false).empty());
    assert(!ui::renderGeneratedWindowContent(window, ui::Size{80, 24}, 0, true).empty());
    assert(documentView->scrollView().selectedStyle().has_value());
    assert(documentView->scrollView().selectedStyle()->background.has_value());
    assert(documentView->scrollView().selectedStyle()->background->rgba().has_value());
    assert(documentView->scrollView().selectedStyle()->background->rgba()->alpha == 0x26);
    window.docs->setSelectedIndex(2);
    app.showSelected();
    assert(window.status->text().find("tables.markdown") == 0);
    assert(documentView->contentHeight() > 0);
    assert(parseMarkdown("# H\n\ntext\n\n```python\nprint(1)\n```\n\n| A |\n| - |\n| B |").size() == 4);
}

}  // namespace

int main(int argc, char** argv)
{
    MarkdownViewerApp app;
    if (argc > 1 && std::string_view(argv[1]) == "--smoke")
    {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--logic-test")
    {
        logicTest();
        return 0;
    }

    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}
