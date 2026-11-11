package main

import (
	"fmt"
	"os"
	"strings"
	"time"

	codeentry "uimd_examples/agent_chat/code_entry"
	transcriptview "uimd_examples/agent_chat/transcript"

	uimd "uimd"
)

type scriptedStep struct {
	replaceLast bool
	entry       transcriptview.Entry
}

// Generated from python/examples/agent_chat/scenario.json.
var initialEntry = transcriptview.Entry{Role: "You", Phase: "user", Body: "Write me a playable Tetris game in one HTML file. Use canvas, keyboard controls, and no external assets.", Detail: "You"}
var scriptedSteps = []scriptedStep{
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "Thinking through the board model, piece rotation, collision checks, and the game loop. I should keep the implementation in one offline HTML document and check interactions before summarizing.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "status", Body: "Checking the available files and preparing the next tool action.", Detail: "Preparing action"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… read_file index.html", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ read_file — simulated project view: index.html", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on the board model, piece rotation, collisio", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the curr", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the current state, then I will make the smallest cohesive update.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "The important invariant is that the board model, piece rotation, collision checks, and the game loop. I can keep the controls wired to the same game state.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… search_code query=\"rotation at wall\"", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ search_code — found the relevant functions in index.html; no network access.", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, ", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return pie", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return piece.cells.some(([x, y]) => board[y + piece.y + dy]?.[x + piece.x + dx] !== 0);\n}\n```", Detail: "Code · JavaScript"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the ", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 1.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… apply_patch index.html", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "diff", Body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n+requestAnimationFrame(tick);", Detail: "Patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ apply_patch — simulated changes recorded for index.html.", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "table", Body: "| Check | Expected | Observed |\n|---|---|---|\n| Spawn | Centered piece | Pass |\n| Soft drop | One row | Pass |", Detail: "Verification table"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "I should check rotation at wall, line clear, game over and report the limits of this simulated run accurately.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… run_command node --check game.js", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ run_command — Syntax check passed; browser gameplay remains to be exercised.", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: rotation at wall, lin", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: rotation at wall, line clear, game over. Syntax check passed; ", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: rotation at wall, line clear, game over. Syntax check passed; browser gameplay remains to be exercised.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The first version has a playable board", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The first version has a playable board, seven tetrominoes, arrow-key movement", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The first version has a playable board, seven tetrominoes, arrow-key movement, rotation, line clearing, and restart.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "You", Phase: "user", Body: "Add a ghost piece, a hold slot, and a three-piece next queue. Keep the controls discoverable.", Detail: "You"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "Thinking through a projection that never mutates the active piece and a once-per-drop hold rule. I should keep the implementation in one offline HTML document and check interactions before summarizing.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "status", Body: "Checking the available files and preparing the next tool action.", Detail: "Preparing action"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… read_file game.js, index.html", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ read_file — simulated project view: game.js, index.html", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on a projection that never mutates the active pi", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the c", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the current state, then I will make the smallest cohesive update.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "The important invariant is that a projection that never mutates the active piece and a once-per-drop hold rule. I can keep the controls wired to the same game state.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… search_code query=\"ghost landing\"", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ search_code — found the relevant functions in game.js, index.html; no network access.", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while ", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold() { if (holdUsed) return; swapHeldPiece(); holdUsed = true; }\n```", Detail: "Code · JavaScript"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the ", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 2.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… apply_patch game.js, index.html", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "diff", Body: "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+drawGhost(active, ghostY(active));\n+drawQueue(nextPieces.slice(0, 3));\n+holdButton.addEventListener('click', hold);", Detail: "Patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ apply_patch — simulated changes recorded for game.js, index.html.", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "table", Body: "| Feature | Input | State |\n|---|---|---|\n| Hold | C | Ready |\n| Ghost | Shadow | Visible |\n| Queue | Three pieces | Filled |", Detail: "Verification table"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "I should check ghost landing, hold once, queue refill and report the limits of this simulated run accurately.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… run_command node --check game.js", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ run_command — Syntax check passed; hold is locked until the active piece lands.", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: ghost landing, hold on", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; ho", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; hold is locked until the active piece lands.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The ghost marks the landing row, C h", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The ghost marks the landing row, C holds a piece once per drop, and the ", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The ghost marks the landing row, C holds a piece once per drop, and the sidebar shows the next three pieces.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "You", Phase: "user", Body: "Make it work on phones. Add touch controls, pause, and accessible labels without changing keyboard play.", Detail: "You"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "Thinking through pointer-sized on-screen controls, pause state, and accessible status announcements. I should keep the implementation in one offline HTML document and check interactions before summarizing.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "status", Body: "Checking the available files and preparing the next tool action.", Detail: "Preparing action"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… read_file index.html, styles.css, game.js", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ read_file — simulated project view: index.html, styles.css, game.js", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on pointer-sized on-screen controls, pause state,", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the current state, then I will make the smallest cohesive update.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "The important invariant is that pointer-sized on-screen controls, pause state, and accessible status announcements. I can keep the controls wired to the same game state.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… search_code query=\"touch left/right\"", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pau", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener(", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener('click', () => actions[button.dataset.action]());\n}\nstatus.setAttribute('aria-live', 'polite');\n```", Detail: "Code · JavaScript"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the ", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 3.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… apply_patch index.html, styles.css, game.js", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "diff", Body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+<button data-action=\"pause\" aria-label=\"Pause or resume game\">Pause</button>\n+<p id=\"status\" role=\"status\" aria-live=\"polite\"></p>", Detail: "Patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "table", Body: "| Control | Keyboard | Touch |\n|---|---|---|\n| Rotate | Up | Rotate button |\n| Pause | P | Pause button |", Detail: "Verification table"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "I should check touch left/right, pause, resume, screen-reader status and report the limits of this simulated run accurately.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… run_command node --check game.js", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ run_command — Syntax check passed; pointer controls and key controls share the same action functions.", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: touch left/right, pause, resume, s", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer cont", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer controls and key controls share the same action functions.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Touch buttons call the same actions a", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Touch buttons call the same actions as keys. Pause freezes the loop, and s", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Touch buttons call the same actions as keys. Pause freezes the loop, and status changes are announced politely.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "You", Phase: "user", Body: "Add scoring, levels, and a saved high score. Handle blocked localStorage gracefully.", Detail: "You"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "Thinking through line-clear points, speed by level, and optional persistent storage. I should keep the implementation in one offline HTML document and check interactions before summarizing.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "status", Body: "Checking the available files and preparing the next tool action.", Detail: "Preparing action"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… read_file game.js, index.html", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ read_file — simulated project view: game.js, index.html", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on line-clear points, speed by level, and op", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the curre", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the current state, then I will make the smallest cohesive update.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "The important invariant is that line-clear points, speed by level, and optional persistent storage. I can keep the controls wired to the same game state.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… search_code query=\"single/Tetris points\"", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ search_code — found the relevant functions in game.js, index.html; no network access.", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POIN", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.s", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.setItem('tetris-best', String(best)); }\ncatch { storageAvailable = false; }\n```", Detail: "Code · JavaScript"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the ", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 4.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… apply_patch game.js, index.html", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "diff", Body: "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+scoreLabel.textContent = String(score);\n+levelLabel.textContent = String(level);\n+bestLabel.textContent = String(best);", Detail: "Patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ apply_patch — simulated changes recorded for game.js, index.html.", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "table", Body: "| Lines | Base points | At level 2 |\n|---:|---:|---:|\n| 1 | 100 | 200 |\n| 4 | 800 | 1600 |", Detail: "Verification table"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "I should check single/Tetris points, level-up, storage exception and report the limits of this simulated run accurately.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… run_command node --check game.js", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ run_command — Syntax check passed; storage denial keeps the in-memory score usable.", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: single/Tetris points, level", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; s", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; storage denial keeps the in-memory score usable.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Score now follows line clears and level,", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Score now follows line clears and level, gravity accelerates by level, and high ", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Score now follows line clears and level, gravity accelerates by level, and high score persists when storage is available.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "You", Phase: "user", Body: "Polish the visual design and check the finished game. Summarize the controls and any remaining caveats.", Detail: "You"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "Thinking through contrast, responsive layout, restart flow, and edge-case verification. I should keep the implementation in one offline HTML document and check interactions before summarizing.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "status", Body: "Checking the available files and preparing the next tool action.", Detail: "Preparing action"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… read_file index.html, styles.css, game.js", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ read_file — simulated project view: index.html, styles.css, game.js", Detail: "Read files"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on contrast, responsive layout, restart flow,", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the curr", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the current state, then I will make the smallest cohesive update.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "The important invariant is that contrast, responsive layout, restart flow, and edge-case verification. I can keep the controls wired to the same game state.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… search_code query=\"restart reset\"", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", Detail: "Search code"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  ", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; ", Detail: "Code · JavaScript"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; refillQueue(); spawnPiece();\n  render(); announce('New game started');\n}\n```", Detail: "Code · JavaScript"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the ", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 5.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… apply_patch index.html, styles.css, game.js", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "diff", Body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+@media (max-width: 640px) { .game { grid-template-columns: 1fr; } }\n+.button:focus-visible { outline: 3px solid #fde047; }", Detail: "Patch"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", Detail: "Apply patch"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "table", Body: "| Surface | Check | Outcome |\n|---|---|---|\n| Desktop | Keyboard play | Pass |\n| Mobile | Touch layout | Pass |\n| Offline | No assets | Pass |", Detail: "Verification table"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "reasoning", Body: "I should check restart reset, wall kick, game over, narrow layout and report the limits of this simulated run accurately.", Detail: "Thinking"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "… run_command node --check game.js", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "activity", Body: "✓ run_command — Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", Detail: "Run command"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: restart reset, wall kick, game over,", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual c", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", Detail: "Assistant"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The single-file game has keyboard and touch co", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best sco", Detail: "Assistant"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "text", Body: "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best score, accessible status, and a responsive layout.", Detail: "Assistant"}},
}
var scriptedDurations = []time.Duration{3196 * time.Millisecond, 799 * time.Millisecond, 932 * time.Millisecond, 1132 * time.Millisecond, 679 * time.Millisecond, 679 * time.Millisecond, 906 * time.Millisecond, 2664 * time.Millisecond, 866 * time.Millisecond, 932 * time.Millisecond, 1118 * time.Millisecond, 1118 * time.Millisecond, 1493 * time.Millisecond, 599 * time.Millisecond, 599 * time.Millisecond, 800 * time.Millisecond, 866 * time.Millisecond, 2198 * time.Millisecond, 999 * time.Millisecond, 2664 * time.Millisecond, 2198 * time.Millisecond, 866 * time.Millisecond, 1265 * time.Millisecond, 619 * time.Millisecond, 619 * time.Millisecond, 826 * time.Millisecond, 958 * time.Millisecond, 958 * time.Millisecond, 1280 * time.Millisecond, 2264 * time.Millisecond, 3196 * time.Millisecond, 799 * time.Millisecond, 932 * time.Millisecond, 1132 * time.Millisecond, 679 * time.Millisecond, 679 * time.Millisecond, 906 * time.Millisecond, 2664 * time.Millisecond, 866 * time.Millisecond, 932 * time.Millisecond, 1118 * time.Millisecond, 1118 * time.Millisecond, 1493 * time.Millisecond, 599 * time.Millisecond, 599 * time.Millisecond, 800 * time.Millisecond, 866 * time.Millisecond, 2198 * time.Millisecond, 999 * time.Millisecond, 2664 * time.Millisecond, 2198 * time.Millisecond, 866 * time.Millisecond, 1265 * time.Millisecond, 619 * time.Millisecond, 619 * time.Millisecond, 826 * time.Millisecond, 958 * time.Millisecond, 958 * time.Millisecond, 1280 * time.Millisecond, 2264 * time.Millisecond, 3196 * time.Millisecond, 799 * time.Millisecond, 932 * time.Millisecond, 1132 * time.Millisecond, 679 * time.Millisecond, 679 * time.Millisecond, 906 * time.Millisecond, 2664 * time.Millisecond, 866 * time.Millisecond, 932 * time.Millisecond, 1118 * time.Millisecond, 1118 * time.Millisecond, 1493 * time.Millisecond, 599 * time.Millisecond, 599 * time.Millisecond, 800 * time.Millisecond, 866 * time.Millisecond, 2198 * time.Millisecond, 999 * time.Millisecond, 2664 * time.Millisecond, 2198 * time.Millisecond, 866 * time.Millisecond, 1265 * time.Millisecond, 619 * time.Millisecond, 619 * time.Millisecond, 826 * time.Millisecond, 958 * time.Millisecond, 958 * time.Millisecond, 1280 * time.Millisecond, 2264 * time.Millisecond, 3196 * time.Millisecond, 799 * time.Millisecond, 932 * time.Millisecond, 1132 * time.Millisecond, 679 * time.Millisecond, 679 * time.Millisecond, 906 * time.Millisecond, 2664 * time.Millisecond, 866 * time.Millisecond, 932 * time.Millisecond, 1118 * time.Millisecond, 1118 * time.Millisecond, 1493 * time.Millisecond, 599 * time.Millisecond, 599 * time.Millisecond, 800 * time.Millisecond, 866 * time.Millisecond, 2198 * time.Millisecond, 999 * time.Millisecond, 2664 * time.Millisecond, 2198 * time.Millisecond, 866 * time.Millisecond, 1265 * time.Millisecond, 619 * time.Millisecond, 619 * time.Millisecond, 826 * time.Millisecond, 958 * time.Millisecond, 958 * time.Millisecond, 1280 * time.Millisecond, 2264 * time.Millisecond, 3196 * time.Millisecond, 799 * time.Millisecond, 932 * time.Millisecond, 1132 * time.Millisecond, 679 * time.Millisecond, 679 * time.Millisecond, 906 * time.Millisecond, 2664 * time.Millisecond, 866 * time.Millisecond, 932 * time.Millisecond, 1118 * time.Millisecond, 1118 * time.Millisecond, 1493 * time.Millisecond, 599 * time.Millisecond, 599 * time.Millisecond, 800 * time.Millisecond, 866 * time.Millisecond, 2198 * time.Millisecond, 999 * time.Millisecond, 2664 * time.Millisecond, 2198 * time.Millisecond, 866 * time.Millisecond, 1265 * time.Millisecond, 619 * time.Millisecond, 619 * time.Millisecond, 826 * time.Millisecond, 0 * time.Millisecond, 0 * time.Millisecond, 0 * time.Millisecond}

const (
	stressEntryCount            = 5_556
	stressTargetRows            = 50_000
	stressMinMessageRows        = 5
	stressMessageRowVariants    = 4
	stressMediumMessageRows     = 40
	stressMediumMessageInterval = 53
	stressLongMessageRows       = 100
	stressLongMessageInterval   = 211
	simulationMessageTarget     = 100
	scriptedMessageCount        = 100
	simulationDuration          = 180 * time.Second
)

var simulationEventCount = len(scriptedSteps) + simulationMessageTarget - scriptedMessageCount

type AgentChatApp struct {
	*AgentChatUI
	transcript    *transcriptview.Transcript
	nextStep      int
	clock         func() time.Duration
	running       bool
	retainHistory bool
	followLive    bool
	nextStepAt    time.Duration
}

func NewAgentChatApp() *AgentChatApp {
	origin := time.Now()
	return newAgentChatApp(func() time.Duration { return time.Since(origin) })
}

func newAgentChatApp(clock func() time.Duration) *AgentChatApp {
	panel := transcriptview.NewTranscript()
	app := &AgentChatApp{AgentChatUI: NewAgentChatUI(), transcript: panel, clock: clock}
	app.SetEventHandler(app)
	app.Transcript.SetChild(panel)
	app.ResetSimulation()
	return app
}

func (app *AgentChatApp) OnStartButtonClick() {
	app.StartSimulation()
}

func (app *AgentChatApp) OnPauseButtonClick() {
	app.PauseSimulation()
}

func (app *AgentChatApp) OnNextStepButtonClick() {
	app.NextStep()
}

func (app *AgentChatApp) OnResetButtonClick() {
	app.ResetSimulation()
}

func (app *AgentChatApp) OnLoadStressButtonClick() {
	app.LoadStressTranscript()
}

func (app *AgentChatApp) OnTopClick() {
	app.followLive = false
	app.transcript.Panel.ScrollToTop()
}

func (app *AgentChatApp) OnEndClick() {
	app.followLive = true
	app.transcript.Panel.ScrollToBottom()
}

func (app *AgentChatApp) OnQuitButtonClick() {
	app.RequestClose()
}

func (app *AgentChatApp) StartSimulation() {
	if app.nextStep >= simulationEventCount {
		if app.retainHistory {
			app.nextStep = 0
		} else {
			app.ResetSimulation()
		}
	}
	if app.nextStep == 0 {
		if app.retainHistory {
			app.transcript.AppendEntry(initialEntry)
		}
		app.applyNextTimelineEvent()
	}
	app.transcript.Panel.ScrollToBottom()
	app.followLive = true
	if app.nextStep >= simulationEventCount {
		app.finishSimulation()
		return
	}
	app.running = true
	app.nextStepAt = app.clock() + scriptedDurations[app.nextStep-1]
	app.setStatus("Running")
}

func (app *AgentChatApp) PauseSimulation() {
	app.running = false
	app.nextStepAt = 0
	app.setStatus("Paused")
}

func (app *AgentChatApp) NextStep() {
	app.running = false
	app.nextStepAt = 0
	if app.nextStep >= simulationEventCount {
		app.setStatus("Scenario complete")
		return
	}
	follow := app.followLive && app.transcript.Panel.AutoScroll
	app.applyNextTimelineEvent()
	if follow {
		app.transcript.Panel.ScrollToBottom()
	}
	prefix := "Step applied"
	if app.nextStep == simulationEventCount {
		prefix = "Scenario complete"
	}
	app.setStatus(prefix)
}

func (app *AgentChatApp) ResetSimulation() {
	app.running = false
	app.nextStepAt = 0
	app.transcript.ClearEntries()
	app.transcript.AppendEntry(initialEntry)
	app.transcript.Panel.ScrollToBottom()
	app.nextStep = 0
	app.retainHistory = false
	app.followLive = true
	app.setStatus("Ready")
}

func (app *AgentChatApp) LoadStressTranscript() {
	app.running = false
	app.nextStepAt = 0
	entries := make([]transcriptview.Entry, 0, stressEntryCount)
	for index := 0; index < stressEntryCount; index++ {
		entries = append(entries, stressEntry(index))
	}
	app.transcript.ReplaceEntries(entries)
	app.transcript.Panel.ScrollToTop()
	app.nextStep = 0
	app.retainHistory = true
	app.followLive = false
	app.Status.SetText(fmt.Sprintf("Stress loaded · %s · ~%d rows", messageCountText(app.transcript.EntryCount()), stressTargetRows))
}

func (app *AgentChatApp) applyStep(step scriptedStep) {
	if step.replaceLast {
		app.transcript.ReplaceLastEntry(step.entry)
	} else {
		app.transcript.AppendEntry(step.entry)
	}
}

func (app *AgentChatApp) applyNextTimelineEvent() {
	app.applyStep(scriptedSteps[app.nextStep])
	app.nextStep++
}

func (app *AgentChatApp) finishSimulation() {
	app.running = false
	app.nextStepAt = 0
	app.setStatus("Scenario complete")
}

func (app *AgentChatApp) Tick() bool {
	if !app.running {
		return false
	}
	now := app.clock()
	applied := false
	follow := app.followLive && app.transcript.Panel.AutoScroll
	for now >= app.nextStepAt && app.nextStep < simulationEventCount {
		app.applyNextTimelineEvent()
		if app.nextStep < simulationEventCount {
			app.nextStepAt += scriptedDurations[app.nextStep-1]
		}
		applied = true
	}
	if !applied {
		return false
	}
	if follow {
		app.transcript.Panel.ScrollToBottom()
	}
	if app.nextStep >= simulationEventCount {
		app.finishSimulation()
	} else {
		app.setStatus("Running")
	}
	return true
}

func (app *AgentChatApp) setStatus(prefix string) {
	app.Status.SetText(prefix + " · " + messageCountText(app.transcript.EntryCount()))
}

func messageCountText(count int) string {
	noun := "messages"
	if count == 1 {
		noun = "message"
	}
	return fmt.Sprintf("%d %s", count, noun)
}

func stressEntry(index int) transcriptview.Entry {
	detail := "SIMULATED STRESS DATA · no external action"
	body := stressBody(index, stressEntryLineCount(index))
	switch index % 8 {
	case 0:
		return transcriptview.Entry{Role: "You", Phase: "request", Body: body, Detail: detail}
	case 1:
		return transcriptview.Entry{Role: "Agent", Phase: "plan", Body: body, Detail: detail}
	case 2:
		return transcriptview.Entry{Role: "Tool", Phase: "search_code", Body: body, Detail: detail}
	case 3:
		return transcriptview.Entry{Role: "Agent", Phase: "code", Body: body, Detail: detail}
	case 4:
		return transcriptview.Entry{Role: "Tool", Phase: "diff", Body: body, Detail: detail}
	case 5:
		return transcriptview.Entry{Role: "Agent", Phase: "table", Body: body, Detail: detail}
	case 6:
		return transcriptview.Entry{Role: "Tool", Phase: "warning", Body: body, Detail: detail}
	default:
		return transcriptview.Entry{Role: "Agent", Phase: "result", Body: body, Detail: detail}
	}
}

func stressEntryLineCount(index int) int {
	ordinal := index + 1
	if ordinal%stressLongMessageInterval == 0 {
		return stressLongMessageRows
	}
	if ordinal%stressMediumMessageInterval == 0 {
		return stressMediumMessageRows
	}
	return stressMinMessageRows + ((index * 17) % stressMessageRowVariants)
}

func stressBody(index int, lineCount int) string {
	variant := index % 8
	lineNumber := (index % 997) + 1
	lines := make([]string, 0, lineCount)
	switch variant {
	case 0:
		prompts := []string{
			"- preserve the documented API",
			"- compare every supported port",
			"- keep the reproduction offline",
			"- report focused validation evidence",
		}
		lines = append(lines, fmt.Sprintf("Please investigate deterministic case %d.", index))
		for offset := 0; offset < lineCount-1; offset++ {
			lines = append(lines, prompts[offset%len(prompts)])
		}
	case 1:
		actions := []string{"inspect", "reproduce", "repair", "validate"}
		lines = append(lines, fmt.Sprintf("## Plan for case %d", index))
		for offset := 0; offset < lineCount-1; offset++ {
			lines = append(lines, fmt.Sprintf("%d. %s the shared invariant", offset+1, actions[offset%len(actions)]))
		}
	case 2:
		lines = append(lines, fmt.Sprintf("$ search_code case_%d", index))
		for offset := 0; offset < lineCount-2; offset++ {
			lines = append(lines, fmt.Sprintf("src/module_%d.py:%d: simulated match", offset%7, lineNumber+offset))
		}
		lines = append(lines, fmt.Sprintf("result: %d matches", lineCount-2))
	case 3:
		lines = append(lines, "```python", fmt.Sprintf("def case_%d():", index))
		for offset := 0; offset < lineCount-4; offset++ {
			lines = append(lines, fmt.Sprintf("    value_%d = %d", offset, index+offset))
		}
		lines = append(lines, "    return {\"ok\": True}", "```")
	case 4:
		lines = append(lines,
			fmt.Sprintf("--- a/src/case_%d.py", index),
			fmt.Sprintf("+++ b/src/case_%d.py", index),
			"@@ -1,2 +1,2 @@",
		)
		for offset := 0; len(lines) < lineCount; offset++ {
			prefix := "-"
			if offset%2 != 0 {
				prefix = "+"
			}
			lines = append(lines, fmt.Sprintf("%svalue_%d = %d", prefix, offset, index+offset))
		}
	case 5:
		lines = append(lines, "| Item | Value | State |", "|:-----|------:|:-----:|")
		for offset := 0; len(lines) < lineCount; offset++ {
			lines = append(lines, fmt.Sprintf("| row %d | %d | pass |", offset, index+offset))
		}
	case 6:
		lines = append(lines, fmt.Sprintf("ERROR simulated_case_%d", index))
		for offset := 0; offset < lineCount-2; offset++ {
			lines = append(lines, fmt.Sprintf("  at worker_%d.py:%d: expected stable state", offset%5, lineNumber+offset))
		}
		lines = append(lines, "status: simulated failure; no test executed")
	default:
		results := []string{
			"- [x] focused regression",
			"- [x] 1:1 parity audit",
			"- [x] backward compatibility",
			"Unicode: ľščťžýáíé · 東京 · 🙂",
		}
		lines = append(lines, fmt.Sprintf("### Result %d ✓", index))
		for offset := 0; offset < lineCount-1; offset++ {
			lines = append(lines, results[offset%len(results)])
		}
	}
	return strings.Join(lines, "\n")
}

func logicTest() {
	const partialCodeRows = 5
	const codeViewportWidth = 86
	codeView := codeentry.NewCodeEntryUI()
	codeView.Code.SetText("1  const COLS = 10, ROWS = 20;\n2  const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n3  function collides(piece, dx = 0, dy = 0) {\n4    return pie")
	if uimd.GeneratedWindowContentSizeForWidth(codeView, codeViewportWidth).Height != partialCodeRows {
		panic("partial code entry height did not match its visible rows")
	}
	now := time.Duration(0)
	app := newAgentChatApp(func() time.Duration { return now })
	if app.transcript.EntryCount() != 1 || app.Status.Text != "Ready · 1 message" {
		panic("agent chat initial state failed")
	}
	app.NextStep()
	app.StartSimulation()
	if app.transcript.EntryCount() != 2 || app.Status.Text != "Running · 2 messages" {
		panic("agent chat timed start failed")
	}
	app.PauseSimulation()
	now += simulationDuration
	if app.Tick() || app.transcript.EntryCount() != 2 {
		panic("agent chat pause failed")
	}
	app.NextStep()
	app.StartSimulation()
	now += simulationDuration
	if !app.Tick() || app.transcript.EntryCount() != simulationMessageTarget || app.Status.Text != "Scenario complete · 100 messages" {
		panic("agent chat scenario failed")
	}
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		if len(os.Args) > 1 && os.Args[1] == "--logic-test" {
			logicTest()
			return 0
		}
		app := NewAgentChatApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
