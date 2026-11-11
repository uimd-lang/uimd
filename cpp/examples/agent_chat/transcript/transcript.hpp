#pragma once

#include "../chat_entry/chat_entry_ui.hpp"
#include "../code_entry/code_entry_ui.hpp"
#include "../table_entry/table_entry_ui.hpp"
#include "../user_entry/user_entry_ui.hpp"
#include "../reasoning_entry/reasoning_entry_ui.hpp"
#include "transcript_ui.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace ui
{
class ReusableElement;
}

namespace agent_chat_example
{

struct TranscriptEntry
{
    std::string role;
    std::string phase;
    std::string body;
    std::string detail;
};

class Transcript final : public TranscriptUI
{
public:
    Transcript();

    void appendEntry(TranscriptEntry entry);
    void replaceLastEntry(TranscriptEntry entry);
    void replaceEntries(std::vector<TranscriptEntry> entries);
    void clearEntries();
    [[nodiscard]] std::size_t entryCount() const;

private:
    static void setEntry(ui::ReusableElement& reusable, const TranscriptEntry& entry);

    std::vector<TranscriptEntry> entries_;
};

}
