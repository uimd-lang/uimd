#pragma once

#include "../chat_entry/chat_entry_ui.hpp"
#include "transcript_ui.hpp"
#include <cstddef>
#include <string>

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
    void clearEntries();
    [[nodiscard]] std::size_t entryCount() const;

private:
    static void setEntry(ChatEntryUI& view, const TranscriptEntry& entry);
};

}
