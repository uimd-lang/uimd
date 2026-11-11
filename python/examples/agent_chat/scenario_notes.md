# Agent Chat replay

This offline example replays a single fictional English coding conversation.
The canonical `scenario.json` records each visible message, its role and Heron
block kind, its text, and the time spent before the next message. No tool call
in the transcript is executed. The generated language-specific constants are
copies of this fixture; regenerate them with
`python3 tools/generate_agent_chat_replay.py` after editing the scenario.

The conversation has five user requests: build HTML Tetris, add hold and a
ghost piece, add touch and accessibility controls, add scoring and saved high
scores, and polish and verify the game. Within each exchange, reasoning is a
visible replay block; commands and outputs are simulated activity blocks.
Long agent prose and code entries are revealed in three timed fragments.
Every port must append these messages after the `Load 50k` corpus and follow
the bottom during replay.

Run the Python example with `./uimd run python/examples/agent_chat/agent_chat.uimd`.
`Start` plays the timed 180-second conversation, while `Next step` advances one
visible text fragment or message. `Pause` stops the clock. `Load 50k` fills a
virtual transcript with roughly 50,000 rows; `Start` then appends the same
conversation after that history and jumps to its live bottom. `Top` and
`Bottom` jump to either end. Scrolling away from the live bottom pauses
automatic following until `Bottom` is selected. `Reset` restores the first
user prompt.

The row components render user prompts, animated reasoning/status headings
with visible detail, assistant prose, simulated tool activity, line-numbered
code and diffs that wrap to the viewport, and structured tables. This is an
offline replay of a fictional exchange; displayed commands are transcript
data only.
