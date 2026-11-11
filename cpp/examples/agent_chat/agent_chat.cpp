#include "agent_chat_ui.hpp"
#include "transcript/transcript.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{

using agent_chat_example::TranscriptEntry;

struct ScriptedStep
{
    bool replaceLast;
    TranscriptEntry entry;
};

// Generated from python/examples/agent_chat/scenario.json.
const TranscriptEntry kInitialEntry{"You", "user", "Write me a playable Tetris game in one HTML file. Use canvas, keyboard controls, and no external assets.", "You"};
const std::array<ScriptedStep, 149> kScriptedSteps{{
    {false, {"Agent", "reasoning", "Thinking through the board model, piece rotation, collision checks, and the game loop. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking"}},
    {false, {"Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action"}},
    {false, {"Tool", "activity", "… read_file index.html", "Read files"}},
    {false, {"Tool", "activity", "✓ read_file — simulated project view: index.html", "Read files"}},
    {false, {"Agent", "text", "I will work on the board model, piece rotation, collisio", "Assistant"}},
    {true, {"Agent", "text", "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the curr", "Assistant"}},
    {true, {"Agent", "text", "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant"}},
    {false, {"Agent", "reasoning", "The important invariant is that the board model, piece rotation, collision checks, and the game loop. I can keep the controls wired to the same game state.", "Thinking"}},
    {false, {"Tool", "activity", "… search_code query=\"rotation at wall\"", "Search code"}},
    {false, {"Tool", "activity", "✓ search_code — found the relevant functions in index.html; no network access.", "Search code"}},
    {false, {"Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, ", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return pie", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return piece.cells.some(([x, y]) => board[y + piece.y + dy]?.[x + piece.x + dx] !== 0);\n}\n```", "Code · JavaScript"}},
    {false, {"Agent", "text", "The implementation keeps the game loop", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 1.", "Assistant"}},
    {false, {"Tool", "activity", "… apply_patch index.html", "Apply patch"}},
    {false, {"Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n+requestAnimationFrame(tick);", "Patch"}},
    {false, {"Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html.", "Apply patch"}},
    {false, {"Agent", "table", "| Check | Expected | Observed |\n|---|---|---|\n| Spawn | Centered piece | Pass |\n| Soft drop | One row | Pass |", "Verification table"}},
    {false, {"Agent", "reasoning", "I should check rotation at wall, line clear, game over and report the limits of this simulated run accurately.", "Thinking"}},
    {false, {"Tool", "activity", "… run_command node --check game.js", "Run command"}},
    {false, {"Tool", "activity", "✓ run_command — Syntax check passed; browser gameplay remains to be exercised.", "Run command"}},
    {false, {"Agent", "text", "Checks considered: rotation at wall, lin", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: rotation at wall, line clear, game over. Syntax check passed; ", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: rotation at wall, line clear, game over. Syntax check passed; browser gameplay remains to be exercised.", "Assistant"}},
    {false, {"Agent", "text", "The first version has a playable board", "Assistant"}},
    {true, {"Agent", "text", "The first version has a playable board, seven tetrominoes, arrow-key movement", "Assistant"}},
    {true, {"Agent", "text", "The first version has a playable board, seven tetrominoes, arrow-key movement, rotation, line clearing, and restart.", "Assistant"}},
    {false, {"You", "user", "Add a ghost piece, a hold slot, and a three-piece next queue. Keep the controls discoverable.", "You"}},
    {false, {"Agent", "reasoning", "Thinking through a projection that never mutates the active piece and a once-per-drop hold rule. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking"}},
    {false, {"Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action"}},
    {false, {"Tool", "activity", "… read_file game.js, index.html", "Read files"}},
    {false, {"Tool", "activity", "✓ read_file — simulated project view: game.js, index.html", "Read files"}},
    {false, {"Agent", "text", "I will work on a projection that never mutates the active pi", "Assistant"}},
    {true, {"Agent", "text", "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the c", "Assistant"}},
    {true, {"Agent", "text", "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant"}},
    {false, {"Agent", "reasoning", "The important invariant is that a projection that never mutates the active piece and a once-per-drop hold rule. I can keep the controls wired to the same game state.", "Thinking"}},
    {false, {"Tool", "activity", "… search_code query=\"ghost landing\"", "Search code"}},
    {false, {"Tool", "activity", "✓ search_code — found the relevant functions in game.js, index.html; no network access.", "Search code"}},
    {false, {"Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while ", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold() { if (holdUsed) return; swapHeldPiece(); holdUsed = true; }\n```", "Code · JavaScript"}},
    {false, {"Agent", "text", "The implementation keeps the game loop", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 2.", "Assistant"}},
    {false, {"Tool", "activity", "… apply_patch game.js, index.html", "Apply patch"}},
    {false, {"Tool", "diff", "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+drawGhost(active, ghostY(active));\n+drawQueue(nextPieces.slice(0, 3));\n+holdButton.addEventListener('click', hold);", "Patch"}},
    {false, {"Tool", "activity", "✓ apply_patch — simulated changes recorded for game.js, index.html.", "Apply patch"}},
    {false, {"Agent", "table", "| Feature | Input | State |\n|---|---|---|\n| Hold | C | Ready |\n| Ghost | Shadow | Visible |\n| Queue | Three pieces | Filled |", "Verification table"}},
    {false, {"Agent", "reasoning", "I should check ghost landing, hold once, queue refill and report the limits of this simulated run accurately.", "Thinking"}},
    {false, {"Tool", "activity", "… run_command node --check game.js", "Run command"}},
    {false, {"Tool", "activity", "✓ run_command — Syntax check passed; hold is locked until the active piece lands.", "Run command"}},
    {false, {"Agent", "text", "Checks considered: ghost landing, hold on", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; ho", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; hold is locked until the active piece lands.", "Assistant"}},
    {false, {"Agent", "text", "The ghost marks the landing row, C h", "Assistant"}},
    {true, {"Agent", "text", "The ghost marks the landing row, C holds a piece once per drop, and the ", "Assistant"}},
    {true, {"Agent", "text", "The ghost marks the landing row, C holds a piece once per drop, and the sidebar shows the next three pieces.", "Assistant"}},
    {false, {"You", "user", "Make it work on phones. Add touch controls, pause, and accessible labels without changing keyboard play.", "You"}},
    {false, {"Agent", "reasoning", "Thinking through pointer-sized on-screen controls, pause state, and accessible status announcements. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking"}},
    {false, {"Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action"}},
    {false, {"Tool", "activity", "… read_file index.html, styles.css, game.js", "Read files"}},
    {false, {"Tool", "activity", "✓ read_file — simulated project view: index.html, styles.css, game.js", "Read files"}},
    {false, {"Agent", "text", "I will work on pointer-sized on-screen controls, pause state,", "Assistant"}},
    {true, {"Agent", "text", "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the", "Assistant"}},
    {true, {"Agent", "text", "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant"}},
    {false, {"Agent", "reasoning", "The important invariant is that pointer-sized on-screen controls, pause state, and accessible status announcements. I can keep the controls wired to the same game state.", "Thinking"}},
    {false, {"Tool", "activity", "… search_code query=\"touch left/right\"", "Search code"}},
    {false, {"Tool", "activity", "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", "Search code"}},
    {false, {"Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pau", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener(", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener('click', () => actions[button.dataset.action]());\n}\nstatus.setAttribute('aria-live', 'polite');\n```", "Code · JavaScript"}},
    {false, {"Agent", "text", "The implementation keeps the game loop", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 3.", "Assistant"}},
    {false, {"Tool", "activity", "… apply_patch index.html, styles.css, game.js", "Apply patch"}},
    {false, {"Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+<button data-action=\"pause\" aria-label=\"Pause or resume game\">Pause</button>\n+<p id=\"status\" role=\"status\" aria-live=\"polite\"></p>", "Patch"}},
    {false, {"Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", "Apply patch"}},
    {false, {"Agent", "table", "| Control | Keyboard | Touch |\n|---|---|---|\n| Rotate | Up | Rotate button |\n| Pause | P | Pause button |", "Verification table"}},
    {false, {"Agent", "reasoning", "I should check touch left/right, pause, resume, screen-reader status and report the limits of this simulated run accurately.", "Thinking"}},
    {false, {"Tool", "activity", "… run_command node --check game.js", "Run command"}},
    {false, {"Tool", "activity", "✓ run_command — Syntax check passed; pointer controls and key controls share the same action functions.", "Run command"}},
    {false, {"Agent", "text", "Checks considered: touch left/right, pause, resume, s", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer cont", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer controls and key controls share the same action functions.", "Assistant"}},
    {false, {"Agent", "text", "Touch buttons call the same actions a", "Assistant"}},
    {true, {"Agent", "text", "Touch buttons call the same actions as keys. Pause freezes the loop, and s", "Assistant"}},
    {true, {"Agent", "text", "Touch buttons call the same actions as keys. Pause freezes the loop, and status changes are announced politely.", "Assistant"}},
    {false, {"You", "user", "Add scoring, levels, and a saved high score. Handle blocked localStorage gracefully.", "You"}},
    {false, {"Agent", "reasoning", "Thinking through line-clear points, speed by level, and optional persistent storage. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking"}},
    {false, {"Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action"}},
    {false, {"Tool", "activity", "… read_file game.js, index.html", "Read files"}},
    {false, {"Tool", "activity", "✓ read_file — simulated project view: game.js, index.html", "Read files"}},
    {false, {"Agent", "text", "I will work on line-clear points, speed by level, and op", "Assistant"}},
    {true, {"Agent", "text", "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the curre", "Assistant"}},
    {true, {"Agent", "text", "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant"}},
    {false, {"Agent", "reasoning", "The important invariant is that line-clear points, speed by level, and optional persistent storage. I can keep the controls wired to the same game state.", "Thinking"}},
    {false, {"Tool", "activity", "… search_code query=\"single/Tetris points\"", "Search code"}},
    {false, {"Tool", "activity", "✓ search_code — found the relevant functions in game.js, index.html; no network access.", "Search code"}},
    {false, {"Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POIN", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.s", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.setItem('tetris-best', String(best)); }\ncatch { storageAvailable = false; }\n```", "Code · JavaScript"}},
    {false, {"Agent", "text", "The implementation keeps the game loop", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 4.", "Assistant"}},
    {false, {"Tool", "activity", "… apply_patch game.js, index.html", "Apply patch"}},
    {false, {"Tool", "diff", "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+scoreLabel.textContent = String(score);\n+levelLabel.textContent = String(level);\n+bestLabel.textContent = String(best);", "Patch"}},
    {false, {"Tool", "activity", "✓ apply_patch — simulated changes recorded for game.js, index.html.", "Apply patch"}},
    {false, {"Agent", "table", "| Lines | Base points | At level 2 |\n|---:|---:|---:|\n| 1 | 100 | 200 |\n| 4 | 800 | 1600 |", "Verification table"}},
    {false, {"Agent", "reasoning", "I should check single/Tetris points, level-up, storage exception and report the limits of this simulated run accurately.", "Thinking"}},
    {false, {"Tool", "activity", "… run_command node --check game.js", "Run command"}},
    {false, {"Tool", "activity", "✓ run_command — Syntax check passed; storage denial keeps the in-memory score usable.", "Run command"}},
    {false, {"Agent", "text", "Checks considered: single/Tetris points, level", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; s", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; storage denial keeps the in-memory score usable.", "Assistant"}},
    {false, {"Agent", "text", "Score now follows line clears and level,", "Assistant"}},
    {true, {"Agent", "text", "Score now follows line clears and level, gravity accelerates by level, and high ", "Assistant"}},
    {true, {"Agent", "text", "Score now follows line clears and level, gravity accelerates by level, and high score persists when storage is available.", "Assistant"}},
    {false, {"You", "user", "Polish the visual design and check the finished game. Summarize the controls and any remaining caveats.", "You"}},
    {false, {"Agent", "reasoning", "Thinking through contrast, responsive layout, restart flow, and edge-case verification. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking"}},
    {false, {"Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action"}},
    {false, {"Tool", "activity", "… read_file index.html, styles.css, game.js", "Read files"}},
    {false, {"Tool", "activity", "✓ read_file — simulated project view: index.html, styles.css, game.js", "Read files"}},
    {false, {"Agent", "text", "I will work on contrast, responsive layout, restart flow,", "Assistant"}},
    {true, {"Agent", "text", "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the curr", "Assistant"}},
    {true, {"Agent", "text", "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant"}},
    {false, {"Agent", "reasoning", "The important invariant is that contrast, responsive layout, restart flow, and edge-case verification. I can keep the controls wired to the same game state.", "Thinking"}},
    {false, {"Tool", "activity", "… search_code query=\"restart reset\"", "Search code"}},
    {false, {"Tool", "activity", "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", "Search code"}},
    {false, {"Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  ", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; ", "Code · JavaScript"}},
    {true, {"Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; refillQueue(); spawnPiece();\n  render(); announce('New game started');\n}\n```", "Code · JavaScript"}},
    {false, {"Agent", "text", "The implementation keeps the game loop", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant"}},
    {true, {"Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 5.", "Assistant"}},
    {false, {"Tool", "activity", "… apply_patch index.html, styles.css, game.js", "Apply patch"}},
    {false, {"Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+@media (max-width: 640px) { .game { grid-template-columns: 1fr; } }\n+.button:focus-visible { outline: 3px solid #fde047; }", "Patch"}},
    {false, {"Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", "Apply patch"}},
    {false, {"Agent", "table", "| Surface | Check | Outcome |\n|---|---|---|\n| Desktop | Keyboard play | Pass |\n| Mobile | Touch layout | Pass |\n| Offline | No assets | Pass |", "Verification table"}},
    {false, {"Agent", "reasoning", "I should check restart reset, wall kick, game over, narrow layout and report the limits of this simulated run accurately.", "Thinking"}},
    {false, {"Tool", "activity", "… run_command node --check game.js", "Run command"}},
    {false, {"Tool", "activity", "✓ run_command — Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", "Run command"}},
    {false, {"Agent", "text", "Checks considered: restart reset, wall kick, game over,", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual c", "Assistant"}},
    {true, {"Agent", "text", "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", "Assistant"}},
    {false, {"Agent", "text", "The single-file game has keyboard and touch co", "Assistant"}},
    {true, {"Agent", "text", "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best sco", "Assistant"}},
    {true, {"Agent", "text", "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best score, accessible status, and a responsive layout.", "Assistant"}},
}};
const std::array<double, 149> kScriptedDurations{{
    3.196,
    0.799,
    0.932,
    1.132,
    0.679,
    0.679,
    0.906,
    2.664,
    0.866,
    0.932,
    1.118,
    1.118,
    1.493,
    0.599,
    0.599,
    0.8,
    0.866,
    2.198,
    0.999,
    2.664,
    2.198,
    0.866,
    1.265,
    0.619,
    0.619,
    0.826,
    0.958,
    0.958,
    1.28,
    2.264,
    3.196,
    0.799,
    0.932,
    1.132,
    0.679,
    0.679,
    0.906,
    2.664,
    0.866,
    0.932,
    1.118,
    1.118,
    1.493,
    0.599,
    0.599,
    0.8,
    0.866,
    2.198,
    0.999,
    2.664,
    2.198,
    0.866,
    1.265,
    0.619,
    0.619,
    0.826,
    0.958,
    0.958,
    1.28,
    2.264,
    3.196,
    0.799,
    0.932,
    1.132,
    0.679,
    0.679,
    0.906,
    2.664,
    0.866,
    0.932,
    1.118,
    1.118,
    1.493,
    0.599,
    0.599,
    0.8,
    0.866,
    2.198,
    0.999,
    2.664,
    2.198,
    0.866,
    1.265,
    0.619,
    0.619,
    0.826,
    0.958,
    0.958,
    1.28,
    2.264,
    3.196,
    0.799,
    0.932,
    1.132,
    0.679,
    0.679,
    0.906,
    2.664,
    0.866,
    0.932,
    1.118,
    1.118,
    1.493,
    0.599,
    0.599,
    0.8,
    0.866,
    2.198,
    0.999,
    2.664,
    2.198,
    0.866,
    1.265,
    0.619,
    0.619,
    0.826,
    0.958,
    0.958,
    1.28,
    2.264,
    3.196,
    0.799,
    0.932,
    1.132,
    0.679,
    0.679,
    0.906,
    2.664,
    0.866,
    0.932,
    1.118,
    1.118,
    1.493,
    0.599,
    0.599,
    0.8,
    0.866,
    2.198,
    0.999,
    2.664,
    2.198,
    0.866,
    1.265,
    0.619,
    0.619,
    0.826,
    0.0,
    0.0,
    0.0,
}};

constexpr std::size_t kStressEntryCount = 5'556;
constexpr std::size_t kStressTargetRows = 50'000;
constexpr std::size_t kStressMinMessageRows = 5;
constexpr std::size_t kStressMessageRowVariants = 4;
constexpr std::size_t kStressMediumMessageRows = 40;
constexpr std::size_t kStressMediumMessageInterval = 53;
constexpr std::size_t kStressLongMessageRows = 100;
constexpr std::size_t kStressLongMessageInterval = 211;
constexpr std::size_t kSimulationMessageTarget = 100;
constexpr std::size_t kScriptedMessageCount = 100;
constexpr std::size_t kSimulationEventCount =
    kScriptedSteps.size() + kSimulationMessageTarget - kScriptedMessageCount;
constexpr auto kSimulationDuration = std::chrono::seconds{180};

[[nodiscard]] std::size_t stressEntryLineCount(std::size_t index)
{
    const std::size_t ordinal = index + 1;
    if (ordinal % kStressLongMessageInterval == 0)
    {
        return kStressLongMessageRows;
    }
    if (ordinal % kStressMediumMessageInterval == 0)
    {
        return kStressMediumMessageRows;
    }
    return kStressMinMessageRows + ((index * 17) % kStressMessageRowVariants);
}

[[nodiscard]] std::string joinLines(const std::vector<std::string>& lines)
{
    std::string result;
    for (std::size_t index = 0; index < lines.size(); ++index)
    {
        if (index > 0)
        {
            result.push_back('\n');
        }
        result += lines[index];
    }
    return result;
}

[[nodiscard]] std::string stressBody(std::size_t index, std::size_t lineCount)
{
    const std::size_t variant = index % 8;
    const std::size_t lineNumber = (index % 997) + 1;
    std::vector<std::string> lines;
    lines.reserve(lineCount);
    if (variant == 0)
    {
        constexpr std::array<std::string_view, 4> kPrompts{
            "- preserve the documented API",
            "- compare every supported port",
            "- keep the reproduction offline",
            "- report focused validation evidence",
        };
        lines.push_back("Please investigate deterministic case " + std::to_string(index) + ".");
        for (std::size_t offset = 0; offset + 1 < lineCount; ++offset)
        {
            lines.emplace_back(kPrompts[offset % kPrompts.size()]);
        }
        return joinLines(lines);
    }
    if (variant == 1)
    {
        constexpr std::array<std::string_view, 4> kActions{
            "inspect", "reproduce", "repair", "validate",
        };
        lines.push_back("## Plan for case " + std::to_string(index));
        for (std::size_t offset = 0; offset + 1 < lineCount; ++offset)
        {
            lines.push_back(
                std::to_string(offset + 1) + ". " +
                std::string{kActions[offset % kActions.size()]} + " the shared invariant");
        }
        return joinLines(lines);
    }
    if (variant == 2)
    {
        lines.push_back("$ search_code case_" + std::to_string(index));
        for (std::size_t offset = 0; offset + 2 < lineCount; ++offset)
        {
            lines.push_back(
                "src/module_" + std::to_string(offset % 7) + ".py:" +
                std::to_string(lineNumber + offset) + ": simulated match");
        }
        lines.push_back("result: " + std::to_string(lineCount - 2) + " matches");
        return joinLines(lines);
    }
    if (variant == 3)
    {
        lines = {"```python", "def case_" + std::to_string(index) + "():"};
        for (std::size_t offset = 0; offset + 4 < lineCount; ++offset)
        {
            lines.push_back(
                "    value_" + std::to_string(offset) + " = " +
                std::to_string(index + offset));
        }
        lines.push_back("    return {\"ok\": True}");
        lines.push_back("```");
        return joinLines(lines);
    }
    if (variant == 4)
    {
        lines = {
            "--- a/src/case_" + std::to_string(index) + ".py",
            "+++ b/src/case_" + std::to_string(index) + ".py",
            "@@ -1,2 +1,2 @@",
        };
        for (std::size_t offset = 0; lines.size() < lineCount; ++offset)
        {
            const char prefix = offset % 2 == 0 ? '-' : '+';
            lines.push_back(
                std::string{prefix} + "value_" + std::to_string(offset) + " = " +
                std::to_string(index + offset));
        }
        return joinLines(lines);
    }
    if (variant == 5)
    {
        lines = {"| Item | Value | State |", "|:-----|------:|:-----:|"};
        for (std::size_t offset = 0; lines.size() < lineCount; ++offset)
        {
            lines.push_back(
                "| row " + std::to_string(offset) + " | " +
                std::to_string(index + offset) + " | pass |");
        }
        return joinLines(lines);
    }
    if (variant == 6)
    {
        lines.push_back("ERROR simulated_case_" + std::to_string(index));
        for (std::size_t offset = 0; offset + 2 < lineCount; ++offset)
        {
            lines.push_back(
                "  at worker_" + std::to_string(offset % 5) + ".py:" +
                std::to_string(lineNumber + offset) + ": expected stable state");
        }
        lines.push_back("status: simulated failure; no test executed");
        return joinLines(lines);
    }
    constexpr std::array<std::string_view, 4> kResults{
        "- [x] focused regression",
        "- [x] 1:1 parity audit",
        "- [x] backward compatibility",
        "Unicode: ľščťžýáíé · 東京 · 🙂",
    };
    lines.push_back("### Result " + std::to_string(index) + " ✓");
    for (std::size_t offset = 0; offset + 1 < lineCount; ++offset)
    {
        lines.emplace_back(kResults[offset % kResults.size()]);
    }
    return joinLines(lines);
}

[[nodiscard]] TranscriptEntry stressEntry(std::size_t index)
{
    const std::string body = stressBody(index, stressEntryLineCount(index));
    switch (index % 8)
    {
    case 0:
        return {"You", "request", body, "SIMULATED STRESS DATA · no external action"};
    case 1:
        return {"Agent", "plan", body, "SIMULATED STRESS DATA · no external action"};
    case 2:
        return {"Tool", "search_code", body, "SIMULATED STRESS DATA · no external action"};
    case 3:
        return {"Agent", "code", body, "SIMULATED STRESS DATA · no external action"};
    case 4:
        return {"Tool", "diff", body, "SIMULATED STRESS DATA · no external action"};
    case 5:
        return {"Agent", "table", body, "SIMULATED STRESS DATA · no external action"};
    case 6:
        return {"Tool", "warning", body, "SIMULATED STRESS DATA · no external action"};
    default:
        return {"Agent", "result", body, "SIMULATED STRESS DATA · no external action"};
    }
}

[[nodiscard]] std::string messageCountText(std::size_t count)
{
    return std::to_string(count) + (count == 1 ? " message" : " messages");
}

class AgentChatApp final : public AgentChatUI
{
public:
    using Clock = std::chrono::steady_clock;
    using Now = std::function<Clock::time_point()>;

    explicit AgentChatApp(Now now = [] { return Clock::now(); })
        : now_(std::move(now))
    {
        transcript_ = dynamic_cast<agent_chat_example::Transcript*>(transcript->child());
        assert(transcript_ != nullptr);
        resetSimulation();
    }

    void startSimulation()
    {
        if (nextStep_ >= kSimulationEventCount)
        {
            if (retainHistory_)
            {
                nextStep_ = 0;
            }
            else
            {
                resetSimulation();
            }
        }
        if (nextStep_ == 0)
        {
            if (retainHistory_)
            {
                transcript_->appendEntry(kInitialEntry);
            }
            applyNextTimelineEvent();
        }
        (void)transcript_->scrollToBottom();
        followLive_ = true;
        if (nextStep_ >= kSimulationEventCount)
        {
            finishSimulation();
            return;
        }
        simulationRunning_ = true;
        nextStepAt_ = now_() + std::chrono::duration_cast<Clock::duration>(
            std::chrono::duration<double>{kScriptedDurations[nextStep_ - 1]});
        setStatus("Running");
    }

    void pauseSimulation()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        setStatus("Paused");
    }

    void nextStep()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        if (nextStep_ >= kSimulationEventCount)
        {
            setStatus("Scenario complete");
            return;
        }
        const bool follow = followLive_ && atBottom();
        applyNextTimelineEvent();
        if (follow)
        {
            (void)transcript_->scrollToBottom();
        }
        setStatus(nextStep_ == kSimulationEventCount ? "Scenario complete" : "Step applied");
    }

    void resetSimulation()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        transcript_->clearEntries();
        transcript_->appendEntry(kInitialEntry);
        (void)transcript_->scrollToBottom();
        nextStep_ = 0;
        retainHistory_ = false;
        followLive_ = true;
        setStatus("Ready");
    }

    void loadStressTranscript()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        std::vector<TranscriptEntry> entries;
        entries.reserve(kStressEntryCount);
        for (std::size_t index = 0; index < kStressEntryCount; ++index)
        {
            entries.push_back(stressEntry(index));
        }
        transcript_->replaceEntries(std::move(entries));
        (void)transcript_->scrollToTop();
        nextStep_ = 0;
        retainHistory_ = true;
        followLive_ = false;
        status->setText(
            "Stress loaded · " + messageCountText(transcript_->entryCount()) + " · ~" +
            std::to_string(kStressTargetRows) + " rows");
    }

    [[nodiscard]] std::size_t entryCount() const
    {
        return transcript_->entryCount();
    }

    bool tick() override
    {
        if (!simulationRunning_ || !nextStepAt_.has_value())
        {
            return false;
        }
        const Clock::time_point now = now_();
        bool applied = false;
        const bool follow = followLive_ && atBottom();
        while (now >= *nextStepAt_ && nextStep_ < kSimulationEventCount)
        {
            applyNextTimelineEvent();
            if (nextStep_ < kSimulationEventCount)
            {
                *nextStepAt_ += std::chrono::duration_cast<Clock::duration>(
                    std::chrono::duration<double>{kScriptedDurations[nextStep_ - 1]});
            }
            applied = true;
        }
        if (!applied)
        {
            return false;
        }
        if (follow)
        {
            (void)transcript_->scrollToBottom();
        }
        if (nextStep_ >= kSimulationEventCount)
        {
            finishSimulation();
        }
        else
        {
            setStatus("Running");
        }
        return true;
    }

protected:
    void onStartButtonClick() override
    {
        startSimulation();
    }

    void onPauseButtonClick() override
    {
        pauseSimulation();
    }

    void onNextStepButtonClick() override
    {
        nextStep();
    }

    void onResetButtonClick() override
    {
        resetSimulation();
    }

    void onLoadStressButtonClick() override
    {
        loadStressTranscript();
    }

    void onTopClick() override
    {
        followLive_ = false;
        (void)transcript_->scrollToTop();
    }

    void onEndClick() override
    {
        followLive_ = true;
        (void)transcript_->scrollToBottom();
    }

    void onQuitButtonClick() override
    {
        quitRequested_ = true;
    }

    bool shouldClose() const override
    {
        return quitRequested_;
    }

private:
    void applyStep(const ScriptedStep& step)
    {
        if (step.replaceLast)
        {
            transcript_->replaceLastEntry(step.entry);
        }
        else
        {
            transcript_->appendEntry(step.entry);
        }
    }

    void applyNextTimelineEvent()
    {
        applyStep(kScriptedSteps[nextStep_]);
        ++nextStep_;
    }

    [[nodiscard]] bool atBottom() const
    {
        return transcript_->scrollView().viewOffset() == 0 &&
            transcript_->scrollView().scrollOffset() == 0;
    }

    void finishSimulation()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        setStatus("Scenario complete");
    }

    void setStatus(std::string_view prefix)
    {
        status->setText(std::string{prefix} + " · " + messageCountText(transcript_->entryCount()));
    }

    agent_chat_example::Transcript* transcript_ = nullptr;
    Now now_;
    std::size_t nextStep_ = 0;
    bool simulationRunning_ = false;
    bool retainHistory_ = false;
    bool followLive_ = false;
    std::optional<Clock::time_point> nextStepAt_;
    bool quitRequested_ = false;
};

void smoke()
{
    AgentChatUI window;
    assert(window.start_button != nullptr);
    assert(window.pause_button != nullptr);
    assert(window.next_step_button != nullptr);
    assert(window.reset_button != nullptr);
    assert(window.load_stress_button != nullptr);
    assert(window.transcript != nullptr);
    assert(window.status != nullptr);
}

void logicTest()
{
    constexpr ui::Size kViewport{90, 35};
    constexpr int kHeadingAndBodyRows = 2;
    constexpr int kScrollUpLines = 10;
    ChatEntryUI textEntry;
    textEntry.heading->setText("Read files");
    textEntry.body->setText("… read_file index.html");
    assert(ui::generatedWindowContentSizeForWidth(textEntry, kViewport.width).height ==
           kHeadingAndBodyRows);
    CodeEntryUI codeEntry;
    codeEntry.code->setText("1  const COLS = 10, ROWS = 20;\n2  const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n3  function collides(piece, dx = 0, dy = 0) {\n4    return piece.cells.some(([x, y]) => board[y + piece.y + dy]?.[x + piece.x + dx] !== 0);\n5  }");
    constexpr int kWrappedCodeRows = 7;
    if (ui::generatedWindowContentSizeForWidth(codeEntry, 86).height != kWrappedCodeRows)
    {
        throw std::runtime_error("wrapped code entry height did not grow to its visible rows");
    }
    codeEntry.code->setText("1  const COLS = 10, ROWS = 20;\n2  const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n3  function collides(piece, dx = 0, dy = 0) {\n4    return pie");
    constexpr int kPartialCodeRows = 5;
    if (ui::generatedWindowContentSizeForWidth(codeEntry, 86).height != kPartialCodeRows)
    {
        throw std::runtime_error("partial code entry height did not match its visible rows");
    }
    AgentChatApp::Clock::time_point now{};
    AgentChatApp app([&now] { return now; });
    assert(app.entryCount() == 1);
    assert(app.status->text() == "Ready · 1 message");
    app.nextStep();
    assert(app.entryCount() == 2);
    app.startSimulation();
    assert(app.entryCount() == 2);
    assert(app.status->text() == "Running · 2 messages");
    app.pauseSimulation();
    now += kSimulationDuration;
    assert(!app.tick());
    assert(app.entryCount() == 2);
    app.nextStep();
    assert(app.entryCount() == 3);
    app.startSimulation();
    now += kSimulationDuration;
    assert(app.tick());
    assert(app.entryCount() == kSimulationMessageTarget);
    assert(app.status->text() == "Scenario complete · 100 messages");
    (void)ui::renderGeneratedWindowContent(app, kViewport);
    auto* transcript = dynamic_cast<agent_chat_example::Transcript*>(app.transcript->child());
    assert(transcript != nullptr);
    const ui::Rect transcriptFrame = transcript->scrollView().frame();
    assert(transcriptFrame.width > 0);
    assert(transcriptFrame.height > 0);
    assert(transcript->viewOffset() == 0);
    assert(transcript->scrollView().scrollLines(
        kScrollUpLines,
        ui::Size{transcriptFrame.width, transcriptFrame.height}));
    assert(transcript->viewOffset() > 0);
    app.pauseSimulation();
    assert(app.status->text() == "Paused · 100 messages");
    app.resetSimulation();
    assert(app.entryCount() == 1);
    app.loadStressTranscript();
    assert(app.entryCount() == kStressEntryCount);
    const ui::RenderedContent stressFrame = ui::renderGeneratedWindowContent(app, kViewport);
    assert(transcript->viewOffset() > 0);
    assert(transcript->scrollView().handleWheel(
        -1,
        ui::Size{transcriptFrame.width, transcriptFrame.height}));
    const std::vector<std::string> stressRows = ui::renderedText(stressFrame);
    assert(std::any_of(stressRows.begin(), stressRows.end(), [](const std::string& row)
    {
        return row.find("Please investigate deterministic case 0") != std::string::npos;
    }));
}

}

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view{argv[1]} == "--smoke")
    {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view{argv[1]} == "--logic-test")
    {
        logicTest();
        return 0;
    }

    AgentChatApp app;
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}
