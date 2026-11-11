mod agent_chat_ui;
#[path = "chat_entry/chat_entry_ui.rs"]
mod chat_entry_ui;
#[path = "code_entry/code_entry_ui.rs"]
mod code_entry_ui;
#[path = "table_entry/table_entry_ui.rs"]
mod table_entry_ui;
#[path = "user_entry/user_entry_ui.rs"]
mod user_entry_ui;
#[path = "reasoning_entry/reasoning_entry_ui.rs"]
mod reasoning_entry_ui;

use agent_chat_ui::*;
use chat_entry_ui::ChatEntryUI;
use code_entry_ui::CodeEntryUI;
use std::cell::RefCell;
use std::rc::Rc;
use std::time::Instant;

#[derive(Clone, Copy)]
struct TranscriptEntry {
    role: &'static str,
    phase: &'static str,
    body: &'static str,
    detail: &'static str,
}

#[derive(Clone, Copy)]
struct ScriptedStep {
    replace_last: bool,
    entry: TranscriptEntry,
}

// Generated from python/examples/agent_chat/scenario.json.
const INITIAL_ENTRY: TranscriptEntry = TranscriptEntry { role: "You", phase: "user", body: "Write me a playable Tetris game in one HTML file. Use canvas, keyboard controls, and no external assets.", detail: "You" };
const SCRIPTED_STEPS: [ScriptedStep; 149] = [
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "Thinking through the board model, piece rotation, collision checks, and the game loop. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… read_file index.html", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: index.html", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on the board model, piece rotation, collisio", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the curr", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "The important invariant is that the board model, piece rotation, collision checks, and the game loop. I can keep the controls wired to the same game state.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… search_code query=\"rotation at wall\"", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in index.html; no network access.", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, ", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return pie", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return piece.cells.some(([x, y]) => board[y + piece.y + dy]?.[x + piece.x + dx] !== 0);\n}\n```", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 1.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… apply_patch index.html", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "diff", body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n+requestAnimationFrame(tick);", detail: "Patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for index.html.", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "table", body: "| Check | Expected | Observed |\n|---|---|---|\n| Spawn | Centered piece | Pass |\n| Soft drop | One row | Pass |", detail: "Verification table" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "I should check rotation at wall, line clear, game over and report the limits of this simulated run accurately.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; browser gameplay remains to be exercised.", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: rotation at wall, lin", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: rotation at wall, line clear, game over. Syntax check passed; ", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: rotation at wall, line clear, game over. Syntax check passed; browser gameplay remains to be exercised.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The first version has a playable board", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The first version has a playable board, seven tetrominoes, arrow-key movement", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The first version has a playable board, seven tetrominoes, arrow-key movement, rotation, line clearing, and restart.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "You", phase: "user", body: "Add a ghost piece, a hold slot, and a three-piece next queue. Keep the controls discoverable.", detail: "You" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "Thinking through a projection that never mutates the active piece and a once-per-drop hold rule. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… read_file game.js, index.html", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: game.js, index.html", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on a projection that never mutates the active pi", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the c", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "The important invariant is that a projection that never mutates the active piece and a once-per-drop hold rule. I can keep the controls wired to the same game state.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… search_code query=\"ghost landing\"", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in game.js, index.html; no network access.", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while ", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold() { if (holdUsed) return; swapHeldPiece(); holdUsed = true; }\n```", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 2.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… apply_patch game.js, index.html", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "diff", body: "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+drawGhost(active, ghostY(active));\n+drawQueue(nextPieces.slice(0, 3));\n+holdButton.addEventListener('click', hold);", detail: "Patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for game.js, index.html.", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "table", body: "| Feature | Input | State |\n|---|---|---|\n| Hold | C | Ready |\n| Ghost | Shadow | Visible |\n| Queue | Three pieces | Filled |", detail: "Verification table" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "I should check ghost landing, hold once, queue refill and report the limits of this simulated run accurately.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; hold is locked until the active piece lands.", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: ghost landing, hold on", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; ho", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; hold is locked until the active piece lands.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The ghost marks the landing row, C h", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The ghost marks the landing row, C holds a piece once per drop, and the ", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The ghost marks the landing row, C holds a piece once per drop, and the sidebar shows the next three pieces.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "You", phase: "user", body: "Make it work on phones. Add touch controls, pause, and accessible labels without changing keyboard play.", detail: "You" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "Thinking through pointer-sized on-screen controls, pause state, and accessible status announcements. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… read_file index.html, styles.css, game.js", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: index.html, styles.css, game.js", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on pointer-sized on-screen controls, pause state,", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "The important invariant is that pointer-sized on-screen controls, pause state, and accessible status announcements. I can keep the controls wired to the same game state.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… search_code query=\"touch left/right\"", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pau", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener(", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener('click', () => actions[button.dataset.action]());\n}\nstatus.setAttribute('aria-live', 'polite');\n```", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 3.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… apply_patch index.html, styles.css, game.js", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "diff", body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+<button data-action=\"pause\" aria-label=\"Pause or resume game\">Pause</button>\n+<p id=\"status\" role=\"status\" aria-live=\"polite\"></p>", detail: "Patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "table", body: "| Control | Keyboard | Touch |\n|---|---|---|\n| Rotate | Up | Rotate button |\n| Pause | P | Pause button |", detail: "Verification table" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "I should check touch left/right, pause, resume, screen-reader status and report the limits of this simulated run accurately.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; pointer controls and key controls share the same action functions.", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: touch left/right, pause, resume, s", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer cont", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer controls and key controls share the same action functions.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Touch buttons call the same actions a", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Touch buttons call the same actions as keys. Pause freezes the loop, and s", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Touch buttons call the same actions as keys. Pause freezes the loop, and status changes are announced politely.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "You", phase: "user", body: "Add scoring, levels, and a saved high score. Handle blocked localStorage gracefully.", detail: "You" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "Thinking through line-clear points, speed by level, and optional persistent storage. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… read_file game.js, index.html", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: game.js, index.html", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on line-clear points, speed by level, and op", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the curre", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "The important invariant is that line-clear points, speed by level, and optional persistent storage. I can keep the controls wired to the same game state.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… search_code query=\"single/Tetris points\"", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in game.js, index.html; no network access.", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POIN", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.s", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.setItem('tetris-best', String(best)); }\ncatch { storageAvailable = false; }\n```", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 4.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… apply_patch game.js, index.html", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "diff", body: "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+scoreLabel.textContent = String(score);\n+levelLabel.textContent = String(level);\n+bestLabel.textContent = String(best);", detail: "Patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for game.js, index.html.", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "table", body: "| Lines | Base points | At level 2 |\n|---:|---:|---:|\n| 1 | 100 | 200 |\n| 4 | 800 | 1600 |", detail: "Verification table" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "I should check single/Tetris points, level-up, storage exception and report the limits of this simulated run accurately.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; storage denial keeps the in-memory score usable.", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: single/Tetris points, level", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; s", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; storage denial keeps the in-memory score usable.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Score now follows line clears and level,", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Score now follows line clears and level, gravity accelerates by level, and high ", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Score now follows line clears and level, gravity accelerates by level, and high score persists when storage is available.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "You", phase: "user", body: "Polish the visual design and check the finished game. Summarize the controls and any remaining caveats.", detail: "You" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "Thinking through contrast, responsive layout, restart flow, and edge-case verification. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… read_file index.html, styles.css, game.js", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: index.html, styles.css, game.js", detail: "Read files" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on contrast, responsive layout, restart flow,", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the curr", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "The important invariant is that contrast, responsive layout, restart flow, and edge-case verification. I can keep the controls wired to the same game state.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… search_code query=\"restart reset\"", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", detail: "Search code" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  ", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; ", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "code", body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; refillQueue(); spawnPiece();\n  render(); announce('New game started');\n}\n```", detail: "Code · JavaScript" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 5.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… apply_patch index.html, styles.css, game.js", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "diff", body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+@media (max-width: 640px) { .game { grid-template-columns: 1fr; } }\n+.button:focus-visible { outline: 3px solid #fde047; }", detail: "Patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", detail: "Apply patch" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "table", body: "| Surface | Check | Outcome |\n|---|---|---|\n| Desktop | Keyboard play | Pass |\n| Mobile | Touch layout | Pass |\n| Offline | No assets | Pass |", detail: "Verification table" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "reasoning", body: "I should check restart reset, wall kick, game over, narrow layout and report the limits of this simulated run accurately.", detail: "Thinking" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", detail: "Run command" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: restart reset, wall kick, game over,", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual c", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", detail: "Assistant" } },
    ScriptedStep { replace_last: false, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The single-file game has keyboard and touch co", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best sco", detail: "Assistant" } },
    ScriptedStep { replace_last: true, entry: TranscriptEntry { role: "Agent", phase: "text", body: "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best score, accessible status, and a responsive layout.", detail: "Assistant" } },
];
const SCRIPTED_DURATIONS: [f64; 149] = [3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.0, 0.0, 0.0];

const STRESS_ENTRY_COUNT: usize = 5_556;
const STRESS_TARGET_ROWS: usize = 50_000;
const STRESS_MIN_MESSAGE_ROWS: usize = 5;
const STRESS_MESSAGE_ROW_VARIANTS: usize = 4;
const STRESS_MEDIUM_MESSAGE_ROWS: usize = 40;
const STRESS_MEDIUM_MESSAGE_INTERVAL: usize = 53;
const STRESS_LONG_MESSAGE_ROWS: usize = 100;
const STRESS_LONG_MESSAGE_INTERVAL: usize = 211;
const SIMULATION_MESSAGE_TARGET: usize = 100;
const SCRIPTED_MESSAGE_COUNT: usize = 100;
const SIMULATION_EVENT_COUNT: usize =
    SCRIPTED_STEPS.len() + SIMULATION_MESSAGE_TARGET - SCRIPTED_MESSAGE_COUNT;
const SIMULATION_DURATION_SECONDS: f64 = 180.0;

struct AgentChatApp {
    next_step: usize,
    simulation_running: bool,
    retain_history: bool,
    follow_live: bool,
    next_step_at: Option<f64>,
    clock_origin: Instant,
    entries: Rc<RefCell<Vec<OwnedTranscriptEntry>>>,
}

impl AgentChatApp {
    fn new(ui: &mut AgentChatUI) -> Self {
        let mut app = Self {
            next_step: 0,
            simulation_running: false,
            retain_history: false,
            follow_live: false,
            next_step_at: None,
            clock_origin: Instant::now(),
            entries: Rc::new(RefCell::new(Vec::new())),
        };
        app.configure_virtual_transcript(ui);
        app.reset_simulation(ui);
        app
    }

    fn configure_virtual_transcript(&self, ui: &AgentChatUI) {
        let count_entries = self.entries.clone();
        let materialize_entries = self.entries.clone();
        Self::with_transcript(ui, |transcript| {
            transcript
                .scroll_view()
                .set_virtual_data_source(uimd::VirtualScrollDataSource::new(
                    move || count_entries.borrow().len(),
                    move |index| index.to_string(),
                    move |index, recycled| {
                        let row = recycled.unwrap_or_else(|| {
                            uimd::new_reusable_element("entry", "ChatEntryUI").into_element()
                        });
                        {
                            let mut reusable = row.borrow_mut();
                            let entry = materialize_entries.borrow()[index].clone();
                            let expected_class = if entry.role == "You" {
                                "UserEntry"
                            } else if matches!(entry.phase.as_str(), "reasoning" | "status") {
                                "ReasoningEntry"
                            } else if entry.phase == "table" {
                                "TableEntry"
                            } else if is_code_phase(&entry.phase) {
                                "CodeEntry"
                            } else {
                                "ChatEntry"
                            };
                            let needs_child = reusable.child_window().is_none_or(|window| {
                                window.mcp_class_name() != expected_class
                            });
                            if needs_child {
                                let child = if entry.role == "You" {
                                    user_entry_ui::UserEntryUI::new().base
                                } else if matches!(entry.phase.as_str(), "reasoning" | "status") {
                                    reasoning_entry_ui::ReasoningEntryUI::new().base
                                } else if entry.phase == "table" {
                                    table_entry_ui::TableEntryUI::new().base
                                } else if is_code_phase(&entry.phase) {
                                    CodeEntryUI::new().base
                                } else {
                                    ChatEntryUI::new().base
                                };
                                reusable.set_child_window(child);
                            }
                            let view = reusable
                                .child_window_mut()
                                .expect("virtual transcript row child must be initialized");
                            set_owned_entry_window(view, &entry);
                        }
                        row
                    },
                ));
        });
    }

    fn with_transcript<T>(ui: &AgentChatUI, operation: impl FnOnce(&mut uimd::GeneratedWindow) -> T) -> T {
        let mut reusable = ui.transcript.borrow_mut();
        let transcript = reusable
            .child_window_mut()
            .expect("generated transcript child must be initialized");
        operation(transcript)
    }

    fn append_entry(&self, ui: &AgentChatUI, entry: &TranscriptEntry) {
        self.append_owned_entry(ui, OwnedTranscriptEntry::from(entry));
    }

    fn append_owned_entry(&self, ui: &AgentChatUI, entry: OwnedTranscriptEntry) {
        let index = self.entries.borrow().len();
        self.entries.borrow_mut().push(entry);
        Self::with_transcript(ui, |transcript| {
            transcript.scroll_view().virtual_items_inserted(index, 1);
        });
    }

    fn replace_last_entry(&self, ui: &AgentChatUI, entry: &TranscriptEntry) {
        if self.entries.borrow().is_empty() {
            self.append_entry(ui, entry);
            return;
        }
        let index = self.entries.borrow().len() - 1;
        self.entries.borrow_mut()[index] = OwnedTranscriptEntry::from(entry);
        Self::with_transcript(ui, |transcript| {
            transcript.scroll_view().virtual_items_replaced(index, 1);
        });
    }

    fn clear_entries(&self, ui: &AgentChatUI) {
        let previous_count = self.entries.borrow().len();
        self.entries.borrow_mut().clear();
        if previous_count == 0 {
            return;
        }
        Self::with_transcript(ui, |transcript| {
            transcript
                .scroll_view()
                .virtual_items_removed(0, previous_count);
        });
    }

    fn replace_entries(&self, ui: &AgentChatUI, entries: Vec<OwnedTranscriptEntry>) {
        self.clear_entries(ui);
        let count = entries.len();
        *self.entries.borrow_mut() = entries;
        if count == 0 {
            return;
        }
        Self::with_transcript(ui, |transcript| {
            transcript.scroll_view().virtual_items_inserted(0, count);
        });
    }

    fn entry_count(&self) -> usize {
        self.entries.borrow().len()
    }

    fn message_count_text(count: usize) -> String {
        format!(
            "{count} {}",
            if count == 1 { "message" } else { "messages" }
        )
    }

    fn set_status(&self, ui: &AgentChatUI, prefix: &str) {
        ui.status.borrow_mut().set_text(format!(
            "{prefix} · {}",
            Self::message_count_text(self.entry_count()),
        ));
    }

    fn apply_step(&self, ui: &AgentChatUI, step: &ScriptedStep) {
        if step.replace_last {
            self.replace_last_entry(ui, &step.entry);
        } else {
            self.append_entry(ui, &step.entry);
        }
    }

    fn start_simulation(&mut self, ui: &mut AgentChatUI) {
        self.start_simulation_at(ui, self.clock_origin.elapsed().as_secs_f64());
    }

    fn start_simulation_at(&mut self, ui: &mut AgentChatUI, now: f64) {
        if self.next_step >= SIMULATION_EVENT_COUNT {
            if self.retain_history {
                self.next_step = 0;
            } else {
                self.reset_simulation(ui);
            }
        }
        if self.next_step == 0 {
            if self.retain_history {
                self.append_entry(ui, &INITIAL_ENTRY);
            }
            self.apply_next_timeline_event(ui);
        }
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_bottom();
        });
        self.follow_live = true;
        if self.next_step >= SIMULATION_EVENT_COUNT {
            self.finish_simulation(ui);
            return;
        }
        self.simulation_running = true;
        self.next_step_at = Some(now + SCRIPTED_DURATIONS[self.next_step - 1]);
        self.set_status(ui, "Running");
    }

    fn pause_simulation(&mut self, ui: &AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        self.set_status(ui, "Paused");
    }

    fn next_step(&mut self, ui: &mut AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        if self.next_step >= SIMULATION_EVENT_COUNT {
            self.set_status(ui, "Scenario complete");
            return;
        }
        let follow = self.follow_live && Self::at_bottom(ui);
        self.apply_next_timeline_event(ui);
        if follow {
            Self::with_transcript(ui, |transcript| {
                let _ = transcript.scroll_to_bottom();
            });
        }
        let prefix = if self.next_step == SIMULATION_EVENT_COUNT {
            "Scenario complete"
        } else {
            "Step applied"
        };
        self.set_status(ui, prefix);
    }

    fn reset_simulation(&mut self, ui: &mut AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        self.clear_entries(ui);
        self.append_entry(ui, &INITIAL_ENTRY);
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_bottom();
        });
        self.next_step = 0;
        self.retain_history = false;
        self.follow_live = true;
        self.set_status(ui, "Ready");
    }

    fn load_stress_transcript(&mut self, ui: &mut AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        self.replace_entries(ui, (0..STRESS_ENTRY_COUNT).map(stress_entry).collect());
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_top();
        });
        self.next_step = 0;
        self.retain_history = true;
        self.follow_live = false;
        ui.status.borrow_mut().set_text(format!(
            "Stress loaded · {} · ~{STRESS_TARGET_ROWS} rows",
            Self::message_count_text(self.entry_count()),
        ));
    }

    fn apply_next_timeline_event(&mut self, ui: &AgentChatUI) {
        self.apply_step(ui, &SCRIPTED_STEPS[self.next_step]);
        self.next_step += 1;
    }

    fn at_bottom(ui: &AgentChatUI) -> bool {
        Self::with_transcript(ui, |transcript| {
            let position = transcript.scroll_view().scroll_position();
            position.view_offset == 0 && position.scroll_offset == 0
        })
    }

    fn finish_simulation(&mut self, ui: &AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        self.set_status(ui, "Scenario complete");
    }

    fn tick_at(&mut self, ui: &AgentChatUI, now: f64) -> bool {
        if !self.simulation_running {
            return false;
        }
        let mut applied = false;
        let follow = self.follow_live && Self::at_bottom(ui);
        while self.next_step_at.is_some_and(|deadline| now >= deadline)
            && self.next_step < SIMULATION_EVENT_COUNT
        {
            self.apply_next_timeline_event(ui);
            if self.next_step < SIMULATION_EVENT_COUNT {
                self.next_step_at = self.next_step_at.map(|deadline| {
                    deadline + SCRIPTED_DURATIONS[self.next_step - 1]
                });
            }
            applied = true;
        }
        if !applied {
            return false;
        }
        if follow {
            Self::with_transcript(ui, |transcript| {
                let _ = transcript.scroll_to_bottom();
            });
        }
        if self.next_step >= SIMULATION_EVENT_COUNT {
            self.finish_simulation(ui);
        } else {
            self.set_status(ui, "Running");
        }
        true
    }
}

impl AgentChatUIEvents for AgentChatApp {
    fn on_top_click(&mut self, ui: &mut AgentChatUI) {
        self.follow_live = false;
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_top();
        });
    }

    fn on_end_click(&mut self, ui: &mut AgentChatUI) {
        self.follow_live = true;
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_bottom();
        });
    }

    fn on_quit_button_click(&mut self, ui: &mut AgentChatUI) {
        ui.base.request_close();
    }

    fn on_start_button_click(&mut self, ui: &mut AgentChatUI) {
        self.start_simulation(ui);
    }

    fn on_pause_button_click(&mut self, ui: &mut AgentChatUI) {
        self.pause_simulation(ui);
    }

    fn on_next_step_button_click(&mut self, ui: &mut AgentChatUI) {
        self.next_step(ui);
    }

    fn on_reset_button_click(&mut self, ui: &mut AgentChatUI) {
        self.reset_simulation(ui);
    }

    fn on_load_stress_button_click(&mut self, ui: &mut AgentChatUI) {
        self.load_stress_transcript(ui);
    }

    fn tick(&mut self, ui: &mut AgentChatUI) -> bool {
        self.tick_at(ui, self.clock_origin.elapsed().as_secs_f64())
    }
}

#[derive(Clone)]
struct OwnedTranscriptEntry {
    role: String,
    phase: String,
    body: String,
    detail: String,
}

impl From<&TranscriptEntry> for OwnedTranscriptEntry {
    fn from(entry: &TranscriptEntry) -> Self {
        Self {
            role: entry.role.to_string(),
            phase: entry.phase.to_string(),
            body: entry.body.to_string(),
            detail: entry.detail.to_string(),
        }
    }
}

fn stress_entry(index: usize) -> OwnedTranscriptEntry {
    let detail = "SIMULATED STRESS DATA · no external action";
    let body = stress_body(index, stress_entry_line_count(index));
    match index % 8 {
        0 => OwnedTranscriptEntry {
            role: "You".to_string(),
            phase: "request".to_string(),
            body,
            detail: detail.to_string(),
        },
        1 => OwnedTranscriptEntry {
            role: "Agent".to_string(),
            phase: "plan".to_string(),
            body,
            detail: detail.to_string(),
        },
        2 => OwnedTranscriptEntry {
            role: "Tool".to_string(),
            phase: "search_code".to_string(),
            body,
            detail: detail.to_string(),
        },
        3 => OwnedTranscriptEntry {
            role: "Agent".to_string(),
            phase: "code".to_string(),
            body,
            detail: detail.to_string(),
        },
        4 => OwnedTranscriptEntry {
            role: "Tool".to_string(),
            phase: "diff".to_string(),
            body,
            detail: detail.to_string(),
        },
        5 => OwnedTranscriptEntry {
            role: "Agent".to_string(),
            phase: "table".to_string(),
            body,
            detail: detail.to_string(),
        },
        6 => OwnedTranscriptEntry {
            role: "Tool".to_string(),
            phase: "warning".to_string(),
            body,
            detail: detail.to_string(),
        },
        _ => OwnedTranscriptEntry {
            role: "Agent".to_string(),
            phase: "result".to_string(),
            body,
            detail: detail.to_string(),
        },
    }
}

fn stress_entry_line_count(index: usize) -> usize {
    let ordinal = index + 1;
    if ordinal % STRESS_LONG_MESSAGE_INTERVAL == 0 {
        return STRESS_LONG_MESSAGE_ROWS;
    }
    if ordinal % STRESS_MEDIUM_MESSAGE_INTERVAL == 0 {
        return STRESS_MEDIUM_MESSAGE_ROWS;
    }
    STRESS_MIN_MESSAGE_ROWS + ((index * 17) % STRESS_MESSAGE_ROW_VARIANTS)
}

fn stress_body(index: usize, line_count: usize) -> String {
    let variant = index % 8;
    let line_number = (index % 997) + 1;
    let mut lines = Vec::with_capacity(line_count);
    match variant {
        0 => {
            let prompts = [
                "- preserve the documented API",
                "- compare every supported port",
                "- keep the reproduction offline",
                "- report focused validation evidence",
            ];
            lines.push(format!("Please investigate deterministic case {index}."));
            for offset in 0..line_count - 1 {
                lines.push(prompts[offset % prompts.len()].to_string());
            }
        }
        1 => {
            let actions = ["inspect", "reproduce", "repair", "validate"];
            lines.push(format!("## Plan for case {index}"));
            for offset in 0..line_count - 1 {
                lines.push(format!(
                    "{}. {} the shared invariant",
                    offset + 1,
                    actions[offset % actions.len()]
                ));
            }
        }
        2 => {
            lines.push(format!("$ search_code case_{index}"));
            for offset in 0..line_count - 2 {
                lines.push(format!(
                    "src/module_{}.py:{}: simulated match",
                    offset % 7,
                    line_number + offset
                ));
            }
            lines.push(format!("result: {} matches", line_count - 2));
        }
        3 => {
            lines.push("```python".to_string());
            lines.push(format!("def case_{index}():"));
            for offset in 0..line_count - 4 {
                lines.push(format!("    value_{offset} = {}", index + offset));
            }
            lines.push("    return {\"ok\": True}".to_string());
            lines.push("```".to_string());
        }
        4 => {
            lines.push(format!("--- a/src/case_{index}.py"));
            lines.push(format!("+++ b/src/case_{index}.py"));
            lines.push("@@ -1,2 +1,2 @@".to_string());
            let mut offset = 0;
            while lines.len() < line_count {
                let prefix = if offset % 2 == 0 { '-' } else { '+' };
                lines.push(format!("{prefix}value_{offset} = {}", index + offset));
                offset += 1;
            }
        }
        5 => {
            lines.push("| Item | Value | State |".to_string());
            lines.push("|:-----|------:|:-----:|".to_string());
            let mut offset = 0;
            while lines.len() < line_count {
                lines.push(format!("| row {offset} | {} | pass |", index + offset));
                offset += 1;
            }
        }
        6 => {
            lines.push(format!("ERROR simulated_case_{index}"));
            for offset in 0..line_count - 2 {
                lines.push(format!(
                    "  at worker_{}.py:{}: expected stable state",
                    offset % 5,
                    line_number + offset
                ));
            }
            lines.push("status: simulated failure; no test executed".to_string());
        }
        _ => {
            let results = [
                "- [x] focused regression",
                "- [x] 1:1 parity audit",
                "- [x] backward compatibility",
                "Unicode: ľščťžýáíé · 東京 · 🙂",
            ];
            lines.push(format!("### Result {index} ✓"));
            for offset in 0..line_count - 1 {
                lines.push(results[offset % results.len()].to_string());
            }
        }
    }
    lines.join("\n")
}

fn is_code_phase(phase: &str) -> bool {
    matches!(
        phase,
        "code"
            | "code · python"
            | "diff"
            | "patch"
            | "result"
            | "search_code"
            | "structured data"
            | "test failure"
            | "warning"
    )
}

fn without_code_fence(text: &str) -> String {
    let mut lines: Vec<&str> = text.lines().collect();
    if lines.first().is_some_and(|line| line.starts_with("```")) {
        lines.remove(0);
    }
    if lines.last().is_some_and(|line| line.starts_with("```")) {
        lines.pop();
    }
    lines.join("\n")
}

fn numbered_code(text: &str) -> String {
    without_code_fence(text)
        .lines()
        .enumerate()
        .map(|(index, line)| format!("{}  {line}", index + 1))
        .collect::<Vec<_>>()
        .join("\n")
}

fn set_owned_entry_window(window: &mut uimd::GeneratedWindow, entry: &OwnedTranscriptEntry) {
    if matches!(entry.phase.as_str(), "reasoning" | "status") {
        let heading = entry.detail.clone();
        window.find_direct_element("heading")
            .expect("reasoning heading must exist")
            .borrow_mut().set_text(heading);
        window.find_direct_element("body")
            .expect("reasoning body must exist")
            .borrow_mut().set_text(entry.body.clone());
        return;
    }
    let (name, value) = if entry.phase == "table" {
        ("table", entry.body.clone())
    } else if is_code_phase(&entry.phase) {
        ("code", if matches!(entry.phase.as_str(), "code" | "code · python") {
            numbered_code(&entry.body)
        } else {
            without_code_fence(&entry.body)
        })
    } else {
        ("body", entry.body.clone())
    };
    if entry.role != "You" {
        let heading = if matches!(entry.phase.as_str(), "reasoning" | "status" | "table")
            || is_code_phase(&entry.phase) {
            entry.detail.as_str()
        } else if entry.role == "Tool" {
            entry.detail.as_str()
        } else if entry.role == "Agent" {
            "Assistant"
        } else {
            entry.role.as_str()
        };
        window
            .find_direct_element("heading")
            .expect("generated chat entry heading member must exist")
            .borrow_mut()
            .set_text(heading);
    }
    window
        .find_direct_element(name)
        .expect("generated chat entry content member must exist")
        .borrow_mut()
        .set_text(value);
}

fn logic_test() {
    let mut ui = AgentChatUI::new();
    let mut app = AgentChatApp::new(&mut ui);
    assert_eq!(app.entry_count(), 1);
    assert_eq!(ui.status.borrow().text(), "Ready · 1 message");
    app.next_step(&mut ui);
    app.start_simulation_at(&mut ui, 0.0);
    assert_eq!(app.entry_count(), 2);
    assert_eq!(ui.status.borrow().text(), "Running · 2 messages");
    app.pause_simulation(&ui);
    assert!(!app.tick_at(&ui, SIMULATION_DURATION_SECONDS));
    assert_eq!(app.entry_count(), 2);
    app.next_step(&mut ui);
    app.start_simulation_at(&mut ui, SIMULATION_DURATION_SECONDS);
    assert!(app.tick_at(&ui, SIMULATION_DURATION_SECONDS * 2.0));
    assert_eq!(app.entry_count(), SIMULATION_MESSAGE_TARGET);
    assert_eq!(
        ui.status.borrow().text(),
        "Scenario complete · 100 messages"
    );
}

fn main() {
    if std::env::args().nth(1).as_deref() == Some("--logic-test") {
        logic_test();
        return;
    }
    let code = uimd::run_generated_app_main(|| {
        let mut ui = AgentChatUI::new();
        let mut app = AgentChatApp::new(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
