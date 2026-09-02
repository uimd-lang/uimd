#include "transcript.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"
#include "ui/elements/ReusableElement.hpp"

#include <algorithm>
#include <memory>
#include <utility>

namespace agent_chat_example
{

Transcript::Transcript()
{
    setAutoScroll(false);
}

void Transcript::appendEntry(TranscriptEntry entry)
{
    auto view = std::make_unique<ChatEntryUI>();
    setEntry(*view, entry);
    const ui::Size natural = ui::generatedWindowContentSize(*view);
    auto reusable = std::make_unique<ui::ReusableElement>("entry", std::move(view));
    reusable->setFrame(ui::Rect{0, 0, 0, std::max(1, natural.height)});
    addChild(std::move(reusable));
}

void Transcript::replaceLastEntry(TranscriptEntry entry)
{
    const auto& children = scrollView().children();
    if (children.empty())
    {
        appendEntry(std::move(entry));
        return;
    }
    auto* reusable = dynamic_cast<ui::ReusableElement*>(children.back().get());
    auto* view = reusable == nullptr ? nullptr : dynamic_cast<ChatEntryUI*>(reusable->child());
    if (view == nullptr)
    {
        return;
    }
    setEntry(*view, entry);
    scrollView().invalidateHeightCache();
}

void Transcript::clearEntries()
{
    clearChildren();
    (void)scrollToTop();
}

std::size_t Transcript::entryCount() const
{
    return scrollView().children().size();
}

void Transcript::setEntry(ChatEntryUI& view, const TranscriptEntry& entry)
{
    view.role->setText(entry.role);
    view.phase->setText(entry.phase);
    view.body->setText(entry.body);
    view.detail->setText(entry.detail);
}

}
