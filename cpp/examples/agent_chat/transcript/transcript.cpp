#include "transcript.hpp"

#include "ui/elements/ReusableElement.hpp"

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace agent_chat_example
{

namespace
{

constexpr std::array<std::string_view, 9> kCodePhases{
    "code",
    "code · python",
    "diff",
    "patch",
    "result",
    "search_code",
    "structured data",
    "test failure",
    "warning",
};

[[nodiscard]] bool isCodePhase(std::string_view phase)
{
    return std::find(kCodePhases.begin(), kCodePhases.end(), phase) != kCodePhases.end();
}

[[nodiscard]] std::string withoutCodeFence(std::string text)
{
    if (text.rfind("```", 0) == 0)
    {
        const std::size_t firstNewline = text.find('\n');
        text = firstNewline == std::string::npos ? std::string{} : text.substr(firstNewline + 1);
    }
    const std::size_t lastNewline = text.rfind('\n');
    if (lastNewline != std::string::npos && text.compare(lastNewline + 1, 3, "```") == 0)
    {
        text.resize(lastNewline);
    }
    return text;
}

[[nodiscard]] std::string numberedCode(const std::string& text)
{
    const std::string raw = withoutCodeFence(text);
    std::string result;
    std::size_t line = 1;
    std::size_t start = 0;
    while (start < raw.size())
    {
        const std::size_t end = raw.find('\n', start);
        result += std::to_string(line++) + "  " + raw.substr(start, end - start);
        if (end == std::string::npos)
        {
            break;
        }
        result.push_back('\n');
        start = end + 1;
    }
    return result;
}

}

Transcript::Transcript()
{
    setAutoScroll(false);
    scrollView().setVirtualDataSource(ui::VirtualScrollDataSource{
        .itemCount = [this]()
        {
            return static_cast<int>(entries_.size());
        },
        .itemKey = [](int index)
        {
            return std::to_string(index);
        },
        .materialize = [this](int index, std::unique_ptr<ui::Element> recycled)
        {
            auto* reusable = dynamic_cast<ui::ReusableElement*>(recycled.get());
            if (reusable == nullptr)
            {
                recycled = std::make_unique<ui::ReusableElement>("entry");
                reusable = static_cast<ui::ReusableElement*>(recycled.get());
            }
            setEntry(*reusable, entries_.at(static_cast<std::size_t>(index)));
            return recycled;
        },
        .estimatedHeight = {},
    });
}

void Transcript::appendEntry(TranscriptEntry entry)
{
    const int index = static_cast<int>(entries_.size());
    entries_.push_back(std::move(entry));
    scrollView().virtualItemsInserted(index);
}

void Transcript::replaceLastEntry(TranscriptEntry entry)
{
    if (entries_.empty())
    {
        appendEntry(std::move(entry));
        return;
    }
    entries_.back() = std::move(entry);
    scrollView().virtualItemsReplaced(static_cast<int>(entries_.size() - 1));
}

void Transcript::replaceEntries(std::vector<TranscriptEntry> entries)
{
    clearEntries();
    entries_ = std::move(entries);
    if (!entries_.empty())
    {
        scrollView().virtualItemsInserted(0, static_cast<int>(entries_.size()));
    }
}

void Transcript::clearEntries()
{
    const int previousCount = static_cast<int>(entries_.size());
    entries_.clear();
    if (previousCount > 0)
    {
        scrollView().virtualItemsRemoved(0, previousCount);
    }
    (void)scrollToTop();
}

std::size_t Transcript::entryCount() const
{
    return entries_.size();
}

void Transcript::setEntry(ui::ReusableElement& reusable, const TranscriptEntry& entry)
{
    if (entry.role == "You")
    {
        auto* view = dynamic_cast<UserEntryUI*>(reusable.child());
        if (view == nullptr)
        {
            reusable.setChild(std::make_unique<UserEntryUI>());
            view = static_cast<UserEntryUI*>(reusable.child());
        }
        view->body->setText(entry.body);
    }
    else if (entry.phase == "reasoning" || entry.phase == "status")
    {
        auto* view = dynamic_cast<ReasoningEntryUI*>(reusable.child());
        if (view == nullptr)
        {
            reusable.setChild(std::make_unique<ReasoningEntryUI>());
            view = static_cast<ReasoningEntryUI*>(reusable.child());
        }
        view->heading->setText(entry.detail);
        view->body->setText(entry.body);
    }
    else if (entry.phase == "table")
    {
        auto* view = dynamic_cast<TableEntryUI*>(reusable.child());
        if (view == nullptr)
        {
            reusable.setChild(std::make_unique<TableEntryUI>());
            view = static_cast<TableEntryUI*>(reusable.child());
        }
        view->heading->setText(entry.detail);
        view->table->setText(entry.body);
    }
    else if (isCodePhase(entry.phase))
    {
        auto* view = dynamic_cast<CodeEntryUI*>(reusable.child());
        if (view == nullptr)
        {
            reusable.setChild(std::make_unique<CodeEntryUI>());
            view = static_cast<CodeEntryUI*>(reusable.child());
        }
        view->heading->setText(entry.detail);
        view->code->setText(entry.phase == "code" || entry.phase == "code · python"
            ? numberedCode(entry.body) : withoutCodeFence(entry.body));
    }
    else
    {
        auto* view = dynamic_cast<ChatEntryUI*>(reusable.child());
        if (view == nullptr)
        {
            reusable.setChild(std::make_unique<ChatEntryUI>());
            view = static_cast<ChatEntryUI*>(reusable.child());
        }
        view->heading->setText(entry.role == "Tool" ? entry.detail :
            entry.role == "Agent" ? "Assistant" : entry.role);
        view->body->setText(entry.body);
    }
}

}
