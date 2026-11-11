import Foundation
import Uimd

private struct TranscriptEntry: Hashable
{
    let role: String
    let phase: String
    let body: String
    let detail: String
}

private struct ScriptedStep
{
    let replaceLast: Bool
    let entry: TranscriptEntry
}

// Generated from python/examples/agent_chat/scenario.json.
private let kInitialEntry = TranscriptEntry(role: "You", phase: "user", body: "Write me a playable Tetris game in one HTML file. Use canvas, keyboard controls, and no external assets.", detail: "You")
private let kScriptedSteps = [
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "Thinking through the board model, piece rotation, collision checks, and the game loop. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… read_file index.html", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: index.html", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on the board model, piece rotation, collisio", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the curr", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "The important invariant is that the board model, piece rotation, collision checks, and the game loop. I can keep the controls wired to the same game state.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… search_code query=\"rotation at wall\"", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in index.html; no network access.", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, ", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return pie", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return piece.cells.some(([x, y]) => board[y + piece.y + dy]?.[x + piece.x + dx] !== 0);\n}\n```", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 1.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… apply_patch index.html", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "diff", body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n+requestAnimationFrame(tick);", detail: "Patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for index.html.", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "table", body: "| Check | Expected | Observed |\n|---|---|---|\n| Spawn | Centered piece | Pass |\n| Soft drop | One row | Pass |", detail: "Verification table")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "I should check rotation at wall, line clear, game over and report the limits of this simulated run accurately.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; browser gameplay remains to be exercised.", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: rotation at wall, lin", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: rotation at wall, line clear, game over. Syntax check passed; ", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: rotation at wall, line clear, game over. Syntax check passed; browser gameplay remains to be exercised.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The first version has a playable board", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The first version has a playable board, seven tetrominoes, arrow-key movement", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The first version has a playable board, seven tetrominoes, arrow-key movement, rotation, line clearing, and restart.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "You", phase: "user", body: "Add a ghost piece, a hold slot, and a three-piece next queue. Keep the controls discoverable.", detail: "You")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "Thinking through a projection that never mutates the active piece and a once-per-drop hold rule. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… read_file game.js, index.html", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: game.js, index.html", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on a projection that never mutates the active pi", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the c", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "The important invariant is that a projection that never mutates the active piece and a once-per-drop hold rule. I can keep the controls wired to the same game state.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… search_code query=\"ghost landing\"", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in game.js, index.html; no network access.", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while ", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold() { if (holdUsed) return; swapHeldPiece(); holdUsed = true; }\n```", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 2.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… apply_patch game.js, index.html", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "diff", body: "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+drawGhost(active, ghostY(active));\n+drawQueue(nextPieces.slice(0, 3));\n+holdButton.addEventListener('click', hold);", detail: "Patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for game.js, index.html.", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "table", body: "| Feature | Input | State |\n|---|---|---|\n| Hold | C | Ready |\n| Ghost | Shadow | Visible |\n| Queue | Three pieces | Filled |", detail: "Verification table")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "I should check ghost landing, hold once, queue refill and report the limits of this simulated run accurately.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; hold is locked until the active piece lands.", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: ghost landing, hold on", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; ho", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; hold is locked until the active piece lands.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The ghost marks the landing row, C h", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The ghost marks the landing row, C holds a piece once per drop, and the ", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The ghost marks the landing row, C holds a piece once per drop, and the sidebar shows the next three pieces.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "You", phase: "user", body: "Make it work on phones. Add touch controls, pause, and accessible labels without changing keyboard play.", detail: "You")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "Thinking through pointer-sized on-screen controls, pause state, and accessible status announcements. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… read_file index.html, styles.css, game.js", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: index.html, styles.css, game.js", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on pointer-sized on-screen controls, pause state,", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "The important invariant is that pointer-sized on-screen controls, pause state, and accessible status announcements. I can keep the controls wired to the same game state.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… search_code query=\"touch left/right\"", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pau", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener(", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener('click', () => actions[button.dataset.action]());\n}\nstatus.setAttribute('aria-live', 'polite');\n```", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 3.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… apply_patch index.html, styles.css, game.js", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "diff", body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+<button data-action=\"pause\" aria-label=\"Pause or resume game\">Pause</button>\n+<p id=\"status\" role=\"status\" aria-live=\"polite\"></p>", detail: "Patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "table", body: "| Control | Keyboard | Touch |\n|---|---|---|\n| Rotate | Up | Rotate button |\n| Pause | P | Pause button |", detail: "Verification table")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "I should check touch left/right, pause, resume, screen-reader status and report the limits of this simulated run accurately.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; pointer controls and key controls share the same action functions.", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: touch left/right, pause, resume, s", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer cont", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer controls and key controls share the same action functions.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Touch buttons call the same actions a", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Touch buttons call the same actions as keys. Pause freezes the loop, and s", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Touch buttons call the same actions as keys. Pause freezes the loop, and status changes are announced politely.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "You", phase: "user", body: "Add scoring, levels, and a saved high score. Handle blocked localStorage gracefully.", detail: "You")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "Thinking through line-clear points, speed by level, and optional persistent storage. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… read_file game.js, index.html", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: game.js, index.html", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on line-clear points, speed by level, and op", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the curre", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "The important invariant is that line-clear points, speed by level, and optional persistent storage. I can keep the controls wired to the same game state.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… search_code query=\"single/Tetris points\"", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in game.js, index.html; no network access.", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POIN", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.s", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.setItem('tetris-best', String(best)); }\ncatch { storageAvailable = false; }\n```", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 4.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… apply_patch game.js, index.html", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "diff", body: "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+scoreLabel.textContent = String(score);\n+levelLabel.textContent = String(level);\n+bestLabel.textContent = String(best);", detail: "Patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for game.js, index.html.", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "table", body: "| Lines | Base points | At level 2 |\n|---:|---:|---:|\n| 1 | 100 | 200 |\n| 4 | 800 | 1600 |", detail: "Verification table")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "I should check single/Tetris points, level-up, storage exception and report the limits of this simulated run accurately.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; storage denial keeps the in-memory score usable.", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: single/Tetris points, level", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; s", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; storage denial keeps the in-memory score usable.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Score now follows line clears and level,", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Score now follows line clears and level, gravity accelerates by level, and high ", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Score now follows line clears and level, gravity accelerates by level, and high score persists when storage is available.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "You", phase: "user", body: "Polish the visual design and check the finished game. Summarize the controls and any remaining caveats.", detail: "You")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "Thinking through contrast, responsive layout, restart flow, and edge-case verification. I should keep the implementation in one offline HTML document and check interactions before summarizing.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "status", body: "Checking the available files and preparing the next tool action.", detail: "Preparing action")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… read_file index.html, styles.css, game.js", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ read_file — simulated project view: index.html, styles.css, game.js", detail: "Read files")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on contrast, responsive layout, restart flow,", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the curr", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the current state, then I will make the smallest cohesive update.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "The important invariant is that contrast, responsive layout, restart flow, and edge-case verification. I can keep the controls wired to the same game state.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… search_code query=\"restart reset\"", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", detail: "Search code")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  ", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; ", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "code", body: "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; refillQueue(); spawnPiece();\n  render(); announce('New game started');\n}\n```", detail: "Code · JavaScript")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the ", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 5.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… apply_patch index.html, styles.css, game.js", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "diff", body: "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+@media (max-width: 640px) { .game { grid-template-columns: 1fr; } }\n+.button:focus-visible { outline: 3px solid #fde047; }", detail: "Patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", detail: "Apply patch")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "table", body: "| Surface | Check | Outcome |\n|---|---|---|\n| Desktop | Keyboard play | Pass |\n| Mobile | Touch layout | Pass |\n| Offline | No assets | Pass |", detail: "Verification table")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "reasoning", body: "I should check restart reset, wall kick, game over, narrow layout and report the limits of this simulated run accurately.", detail: "Thinking")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "… run_command node --check game.js", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "activity", body: "✓ run_command — Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", detail: "Run command")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: restart reset, wall kick, game over,", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual c", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", detail: "Assistant")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The single-file game has keyboard and touch co", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best sco", detail: "Assistant")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "text", body: "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best score, accessible status, and a responsive layout.", detail: "Assistant")),
]
private let kScriptedDurations: [TimeInterval] = [3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.0, 0.0, 0.0]

private let kStressEntryCount = 5_556
private let kStressTargetRows = 50_000
private let kStressMinMessageRows = 5
private let kStressMessageRowVariants = 4
private let kStressMediumMessageRows = 40
private let kStressMediumMessageInterval = 53
private let kStressLongMessageRows = 100
private let kStressLongMessageInterval = 211
private let kSimulationMessageTarget = 100
private let kScriptedMessageCount = 100
private let kSimulationEventCount =
    kScriptedSteps.count + kSimulationMessageTarget - kScriptedMessageCount
private let kSimulationDurationSeconds = 180.0

private final class Transcript: TranscriptUI
{
    private var entries: [TranscriptEntry] = []

    override init()
    {
        super.init()
        setAutoScroll(false)
        scrollView().setVirtualDataSource(VirtualScrollDataSource(
            itemCount: { [weak self] in self?.entries.count ?? 0 },
            itemKey: { index in AnyHashable(index) },
            materialize: { [weak self] index, recycled in
                let reusable = recycled as? ReusableElement ?? ReusableElement("entry")
                if let self
                {
                    Self.setEntry(reusable, self.entries[index])
                }
                return reusable
            }
        ))
    }

    func appendEntry(_ entry: TranscriptEntry)
    {
        let index = entries.count
        entries.append(entry)
        scrollView().virtualItemsInserted(at: index)
    }

    func replaceLastEntry(_ entry: TranscriptEntry)
    {
        guard !entries.isEmpty else
        {
            appendEntry(entry)
            return
        }
        let index = entries.count - 1
        entries[index] = entry
        scrollView().virtualItemsReplaced(at: index)
    }

    func replaceEntries(_ replacement: [TranscriptEntry])
    {
        clearEntries()
        entries = replacement
        if !entries.isEmpty
        {
            scrollView().virtualItemsInserted(at: 0, count: entries.count)
        }
    }

    func clearEntries()
    {
        let previousCount = entries.count
        entries.removeAll(keepingCapacity: true)
        if previousCount > 0
        {
            scrollView().virtualItemsRemoved(at: 0, count: previousCount)
        }
        scrollToTop()
    }

    func entryCount() -> Int
    {
        entries.count
    }

    private static func setEntry(_ reusable: ReusableElement, _ entry: TranscriptEntry)
    {
        if entry.role == "You"
        {
            let view = reusable.child as? UserEntryUI ?? UserEntryUI()
            if reusable.child !== view
            {
                reusable.setChild(view)
            }
            view.body.setText(entry.body)
        }
        else if entry.phase == "reasoning" || entry.phase == "status"
        {
            let view = reusable.child as? ReasoningEntryUI ?? ReasoningEntryUI()
            if reusable.child !== view
            {
                reusable.setChild(view)
            }
            view.heading.setText(entry.detail)
            view.body.setText(entry.body)
        }
        else if entry.phase == "table"
        {
            let view = reusable.child as? TableEntryUI ?? TableEntryUI()
            if reusable.child !== view
            {
                reusable.setChild(view)
            }
            view.heading.setText(entry.detail)
            view.table.setText(entry.body)
        }
        else if codePhases.contains(entry.phase)
        {
            let view = reusable.child as? CodeEntryUI ?? CodeEntryUI()
            if reusable.child !== view
            {
                reusable.setChild(view)
            }
            view.heading.setText(entry.detail)
            view.code.setText(entry.phase == "code" || entry.phase == "code · python"
                ? numberedCode(entry.body) : withoutCodeFence(entry.body))
        }
        else
        {
            let view = reusable.child as? ChatEntryUI ?? ChatEntryUI()
            if reusable.child !== view
            {
                reusable.setChild(view)
            }
            view.heading.setText(entry.role == "Tool" ? entry.detail :
                entry.role == "Agent" ? "Assistant" : entry.role)
            view.body.setText(entry.body)
        }
    }
}

private let codePhases: Set<String> = [
    "code",
    "code · python",
    "diff",
    "patch",
    "result",
    "search_code",
    "structured data",
    "test failure",
    "warning",
]

private func withoutCodeFence(_ text: String) -> String
{
    var lines = text.split(separator: "\n", omittingEmptySubsequences: false).map(String.init)
    if lines.first?.hasPrefix("```") == true
    {
        lines.removeFirst()
    }
    if lines.last?.hasPrefix("```") == true
    {
        lines.removeLast()
    }
    return lines.joined(separator: "\n")
}

private func numberedCode(_ text: String) -> String
{
    withoutCodeFence(text).split(separator: "\n", omittingEmptySubsequences: false)
        .enumerated().map { "\($0.offset + 1)  \($0.element)" }.joined(separator: "\n")
}

private final class AgentChat: AgentChatUI
{
    private let transcriptPanel = Transcript()
    private let clock: () -> TimeInterval
    private var nextStep = 0
    private var simulationRunning = false
    private var retainHistory = false
    private var followLive = false
    private var nextStepAt: TimeInterval?
    private var quitRequested = false

    override init()
    {
        clock = { ProcessInfo.processInfo.systemUptime }
        super.init()
        transcript.setChild(transcriptPanel)
        resetSimulation()
    }

    init(clock: @escaping () -> TimeInterval)
    {
        self.clock = clock
        super.init()
        transcript.setChild(transcriptPanel)
        resetSimulation()
    }

    override func onStartButtonClick()
    {
        startSimulation()
    }

    override func onPauseButtonClick()
    {
        pauseSimulation()
    }

    override func onNextStepButtonClick()
    {
        applyNextStep()
    }

    override func onResetButtonClick()
    {
        resetSimulation()
    }

    override func onLoadStressButtonClick()
    {
        loadStressTranscript()
    }

    override func onTopClick()
    {
        followLive = false
        transcriptPanel.scrollToTop()
    }

    override func onEndClick()
    {
        followLive = true
        transcriptPanel.scrollToBottom()
    }

    override func onQuitButtonClick()
    {
        quitRequested = true
    }

    override func shouldClose() -> Bool
    {
        quitRequested
    }

    func startSimulation()
    {
        if nextStep >= kSimulationEventCount
        {
            if retainHistory
            {
                nextStep = 0
            }
            else
            {
                resetSimulation()
            }
        }
        if nextStep == 0
        {
            if retainHistory
            {
                transcriptPanel.appendEntry(kInitialEntry)
            }
            applyNextTimelineEvent()
        }
        transcriptPanel.scrollToBottom()
        followLive = true
        if nextStep >= kSimulationEventCount
        {
            finishSimulation()
            return
        }
        simulationRunning = true
        nextStepAt = clock() + kScriptedDurations[nextStep - 1]
        setStatus("Running")
    }

    func pauseSimulation()
    {
        simulationRunning = false
        nextStepAt = nil
        setStatus("Paused")
    }

    func applyNextStep()
    {
        simulationRunning = false
        nextStepAt = nil
        guard nextStep < kSimulationEventCount else
        {
            setStatus("Scenario complete")
            return
        }
        let follow = followLive && atBottom()
        applyNextTimelineEvent()
        if follow
        {
            transcriptPanel.scrollToBottom()
        }
        setStatus(nextStep == kSimulationEventCount ? "Scenario complete" : "Step applied")
    }

    func resetSimulation()
    {
        simulationRunning = false
        nextStepAt = nil
        transcriptPanel.clearEntries()
        transcriptPanel.appendEntry(kInitialEntry)
        transcriptPanel.scrollToBottom()
        nextStep = 0
        retainHistory = false
        followLive = true
        setStatus("Ready")
    }

    func loadStressTranscript()
    {
        simulationRunning = false
        nextStepAt = nil
        transcriptPanel.replaceEntries((0..<kStressEntryCount).map(stressEntry))
        transcriptPanel.scrollToTop()
        nextStep = 0
        retainHistory = true
        followLive = false
        status.setText("Stress loaded · \(messageCountText(transcriptPanel.entryCount())) · ~\(kStressTargetRows) rows")
    }

    func entryCount() -> Int
    {
        transcriptPanel.entryCount()
    }

    override func tick() -> Bool
    {
        guard simulationRunning, let deadline = nextStepAt else
        {
            return false
        }
        let now = clock()
        var nextDeadline = deadline
        var applied = false
        let follow = followLive && atBottom()
        while now >= nextDeadline && nextStep < kSimulationEventCount
        {
            applyNextTimelineEvent()
            if nextStep < kSimulationEventCount
            {
                nextDeadline += kScriptedDurations[nextStep - 1]
            }
            applied = true
        }
        nextStepAt = nextDeadline
        guard applied else
        {
            return false
        }
        if follow
        {
            transcriptPanel.scrollToBottom()
        }
        if nextStep >= kSimulationEventCount
        {
            finishSimulation()
        }
        else
        {
            setStatus("Running")
        }
        return true
    }

    private func applyStep(_ step: ScriptedStep)
    {
        if step.replaceLast
        {
            transcriptPanel.replaceLastEntry(step.entry)
        }
        else
        {
            transcriptPanel.appendEntry(step.entry)
        }
    }

    private func applyNextTimelineEvent()
    {
        applyStep(kScriptedSteps[nextStep])
        nextStep += 1
    }

    private func atBottom() -> Bool
    {
        let position = transcriptPanel.scrollView().scrollPosition()
        return position.viewOffset == 0 && position.scrollOffset == 0
    }

    private func finishSimulation()
    {
        simulationRunning = false
        nextStepAt = nil
        setStatus("Scenario complete")
    }

    private func setStatus(_ prefix: String)
    {
        status.setText(prefix + " · " + messageCountText(transcriptPanel.entryCount()))
    }
}

private func messageCountText(_ count: Int) -> String
{
    "\(count) " + (count == 1 ? "message" : "messages")
}

private func stressEntry(_ index: Int) -> TranscriptEntry
{
    let detail = "SIMULATED STRESS DATA · no external action"
    let body = stressBody(index, stressEntryLineCount(index))
    switch index % 8
    {
    case 0:
        return TranscriptEntry(role: "You", phase: "request", body: body, detail: detail)
    case 1:
        return TranscriptEntry(role: "Agent", phase: "plan", body: body, detail: detail)
    case 2:
        return TranscriptEntry(role: "Tool", phase: "search_code", body: body, detail: detail)
    case 3:
        return TranscriptEntry(role: "Agent", phase: "code", body: body, detail: detail)
    case 4:
        return TranscriptEntry(role: "Tool", phase: "diff", body: body, detail: detail)
    case 5:
        return TranscriptEntry(role: "Agent", phase: "table", body: body, detail: detail)
    case 6:
        return TranscriptEntry(role: "Tool", phase: "warning", body: body, detail: detail)
    default:
        return TranscriptEntry(role: "Agent", phase: "result", body: body, detail: detail)
    }
}

private func stressEntryLineCount(_ index: Int) -> Int
{
    let ordinal = index + 1
    if ordinal % kStressLongMessageInterval == 0
    {
        return kStressLongMessageRows
    }
    if ordinal % kStressMediumMessageInterval == 0
    {
        return kStressMediumMessageRows
    }
    return kStressMinMessageRows + ((index * 17) % kStressMessageRowVariants)
}

private func stressBody(_ index: Int, _ lineCount: Int) -> String
{
    let variant = index % 8
    let lineNumber = (index % 997) + 1
    var lines: [String] = []
    lines.reserveCapacity(lineCount)
    switch variant
    {
    case 0:
        let prompts = [
            "- preserve the documented API",
            "- compare every supported port",
            "- keep the reproduction offline",
            "- report focused validation evidence",
        ]
        lines.append("Please investigate deterministic case \(index).")
        for offset in 0..<(lineCount - 1)
        {
            lines.append(prompts[offset % prompts.count])
        }
    case 1:
        let actions = ["inspect", "reproduce", "repair", "validate"]
        lines.append("## Plan for case \(index)")
        for offset in 0..<(lineCount - 1)
        {
            lines.append("\(offset + 1). \(actions[offset % actions.count]) the shared invariant")
        }
    case 2:
        lines.append("$ search_code case_\(index)")
        for offset in 0..<(lineCount - 2)
        {
            lines.append("src/module_\(offset % 7).py:\(lineNumber + offset): simulated match")
        }
        lines.append("result: \(lineCount - 2) matches")
    case 3:
        lines.append("```python")
        lines.append("def case_\(index)():")
        for offset in 0..<(lineCount - 4)
        {
            lines.append("    value_\(offset) = \(index + offset)")
        }
        lines.append("    return {\"ok\": True}")
        lines.append("```")
    case 4:
        lines.append("--- a/src/case_\(index).py")
        lines.append("+++ b/src/case_\(index).py")
        lines.append("@@ -1,2 +1,2 @@")
        var offset = 0
        while lines.count < lineCount
        {
            let prefix = offset % 2 == 0 ? "-" : "+"
            lines.append("\(prefix)value_\(offset) = \(index + offset)")
            offset += 1
        }
    case 5:
        lines.append("| Item | Value | State |")
        lines.append("|:-----|------:|:-----:|")
        var offset = 0
        while lines.count < lineCount
        {
            lines.append("| row \(offset) | \(index + offset) | pass |")
            offset += 1
        }
    case 6:
        lines.append("ERROR simulated_case_\(index)")
        for offset in 0..<(lineCount - 2)
        {
            lines.append("  at worker_\(offset % 5).py:\(lineNumber + offset): expected stable state")
        }
        lines.append("status: simulated failure; no test executed")
    default:
        let results = [
            "- [x] focused regression",
            "- [x] 1:1 parity audit",
            "- [x] backward compatibility",
            "Unicode: ľščťžýáíé · 東京 · 🙂",
        ]
        lines.append("### Result \(index) ✓")
        for offset in 0..<(lineCount - 1)
        {
            lines.append(results[offset % results.count])
        }
    }
    return lines.joined(separator: "\n")
}

private func logicTest()
{
    var now = 0.0
    let app = AgentChat(clock: { now })
    precondition(app.entryCount() == 1)
    precondition(app.status.text == "Ready · 1 message")
    app.applyNextStep()
    app.startSimulation()
    precondition(app.entryCount() == 2)
    precondition(app.status.text == "Running · 2 messages")
    app.pauseSimulation()
    now += kSimulationDurationSeconds
    precondition(!app.tick())
    precondition(app.entryCount() == 2)
    app.applyNextStep()
    app.startSimulation()
    now += kSimulationDurationSeconds
    precondition(app.tick())
    precondition(app.entryCount() == kSimulationMessageTarget)
    precondition(app.status.text == "Scenario complete · 100 messages")
}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            if CommandLine.arguments.contains("--logic-test")
            {
                logicTest()
                return 0
            }
            let app = AgentChat()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}
