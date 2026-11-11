import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.function.DoubleSupplier;

import uimd.GeneratedWindowRuntime;
import uimd.ReusableElement;
import uimd.VirtualScrollDataSource;

public final class AgentChat extends AgentChatUI
{
    private static final int STRESS_ENTRY_COUNT = 5_556;
    private static final int STRESS_TARGET_ROWS = 50_000;
    private static final int STRESS_MIN_MESSAGE_ROWS = 5;
    private static final int STRESS_MESSAGE_ROW_VARIANTS = 4;
    private static final int STRESS_MEDIUM_MESSAGE_ROWS = 40;
    private static final int STRESS_MEDIUM_MESSAGE_INTERVAL = 53;
    private static final int STRESS_LONG_MESSAGE_ROWS = 100;
    private static final int STRESS_LONG_MESSAGE_INTERVAL = 211;
    private static final int SIMULATION_MESSAGE_TARGET = 100;
    private static final int SCRIPTED_MESSAGE_COUNT = 100;
    private static final double SIMULATION_DURATION_SECONDS = 180.0;

    // Generated from python/examples/agent_chat/scenario.json.
    private static final TranscriptEntry INITIAL_ENTRY = new TranscriptEntry("You", "user", "Write me a playable Tetris game in one HTML file. Use canvas, keyboard controls, and no external assets.", "You");
    private static final List<ScriptedStep> SCRIPTED_STEPS = List.of(
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "Thinking through the board model, piece rotation, collision checks, and the game loop. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… read_file index.html", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ read_file — simulated project view: index.html", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "I will work on the board model, piece rotation, collisio", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the curr", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "The important invariant is that the board model, piece rotation, collision checks, and the game loop. I can keep the controls wired to the same game state.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… search_code query=\"rotation at wall\"", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ search_code — found the relevant functions in index.html; no network access.", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, ", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return pie", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return piece.cells.some(([x, y]) => board[y + piece.y + dy]?.[x + piece.x + dx] !== 0);\n}\n```", "Code · JavaScript")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 1.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… apply_patch index.html", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n+requestAnimationFrame(tick);", "Patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html.", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "table", "| Check | Expected | Observed |\n|---|---|---|\n| Spawn | Centered piece | Pass |\n| Soft drop | One row | Pass |", "Verification table")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "I should check rotation at wall, line clear, game over and report the limits of this simulated run accurately.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ run_command — Syntax check passed; browser gameplay remains to be exercised.", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "Checks considered: rotation at wall, lin", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: rotation at wall, line clear, game over. Syntax check passed; ", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: rotation at wall, line clear, game over. Syntax check passed; browser gameplay remains to be exercised.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "The first version has a playable board", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The first version has a playable board, seven tetrominoes, arrow-key movement", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The first version has a playable board, seven tetrominoes, arrow-key movement, rotation, line clearing, and restart.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("You", "user", "Add a ghost piece, a hold slot, and a three-piece next queue. Keep the controls discoverable.", "You")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "Thinking through a projection that never mutates the active piece and a once-per-drop hold rule. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… read_file game.js, index.html", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ read_file — simulated project view: game.js, index.html", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "I will work on a projection that never mutates the active pi", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the c", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "The important invariant is that a projection that never mutates the active piece and a once-per-drop hold rule. I can keep the controls wired to the same game state.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… search_code query=\"ghost landing\"", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ search_code — found the relevant functions in game.js, index.html; no network access.", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while ", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold() { if (holdUsed) return; swapHeldPiece(); holdUsed = true; }\n```", "Code · JavaScript")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 2.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… apply_patch game.js, index.html", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "diff", "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+drawGhost(active, ghostY(active));\n+drawQueue(nextPieces.slice(0, 3));\n+holdButton.addEventListener('click', hold);", "Patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ apply_patch — simulated changes recorded for game.js, index.html.", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "table", "| Feature | Input | State |\n|---|---|---|\n| Hold | C | Ready |\n| Ghost | Shadow | Visible |\n| Queue | Three pieces | Filled |", "Verification table")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "I should check ghost landing, hold once, queue refill and report the limits of this simulated run accurately.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ run_command — Syntax check passed; hold is locked until the active piece lands.", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "Checks considered: ghost landing, hold on", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; ho", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; hold is locked until the active piece lands.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "The ghost marks the landing row, C h", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The ghost marks the landing row, C holds a piece once per drop, and the ", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The ghost marks the landing row, C holds a piece once per drop, and the sidebar shows the next three pieces.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("You", "user", "Make it work on phones. Add touch controls, pause, and accessible labels without changing keyboard play.", "You")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "Thinking through pointer-sized on-screen controls, pause state, and accessible status announcements. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… read_file index.html, styles.css, game.js", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ read_file — simulated project view: index.html, styles.css, game.js", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "I will work on pointer-sized on-screen controls, pause state,", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "The important invariant is that pointer-sized on-screen controls, pause state, and accessible status announcements. I can keep the controls wired to the same game state.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… search_code query=\"touch left/right\"", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pau", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener(", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener('click', () => actions[button.dataset.action]());\n}\nstatus.setAttribute('aria-live', 'polite');\n```", "Code · JavaScript")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 3.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… apply_patch index.html, styles.css, game.js", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+<button data-action=\"pause\" aria-label=\"Pause or resume game\">Pause</button>\n+<p id=\"status\" role=\"status\" aria-live=\"polite\"></p>", "Patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "table", "| Control | Keyboard | Touch |\n|---|---|---|\n| Rotate | Up | Rotate button |\n| Pause | P | Pause button |", "Verification table")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "I should check touch left/right, pause, resume, screen-reader status and report the limits of this simulated run accurately.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ run_command — Syntax check passed; pointer controls and key controls share the same action functions.", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "Checks considered: touch left/right, pause, resume, s", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer cont", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer controls and key controls share the same action functions.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "Touch buttons call the same actions a", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Touch buttons call the same actions as keys. Pause freezes the loop, and s", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Touch buttons call the same actions as keys. Pause freezes the loop, and status changes are announced politely.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("You", "user", "Add scoring, levels, and a saved high score. Handle blocked localStorage gracefully.", "You")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "Thinking through line-clear points, speed by level, and optional persistent storage. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… read_file game.js, index.html", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ read_file — simulated project view: game.js, index.html", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "I will work on line-clear points, speed by level, and op", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the curre", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "The important invariant is that line-clear points, speed by level, and optional persistent storage. I can keep the controls wired to the same game state.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… search_code query=\"single/Tetris points\"", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ search_code — found the relevant functions in game.js, index.html; no network access.", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POIN", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.s", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.setItem('tetris-best', String(best)); }\ncatch { storageAvailable = false; }\n```", "Code · JavaScript")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 4.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… apply_patch game.js, index.html", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "diff", "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+scoreLabel.textContent = String(score);\n+levelLabel.textContent = String(level);\n+bestLabel.textContent = String(best);", "Patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ apply_patch — simulated changes recorded for game.js, index.html.", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "table", "| Lines | Base points | At level 2 |\n|---:|---:|---:|\n| 1 | 100 | 200 |\n| 4 | 800 | 1600 |", "Verification table")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "I should check single/Tetris points, level-up, storage exception and report the limits of this simulated run accurately.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ run_command — Syntax check passed; storage denial keeps the in-memory score usable.", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "Checks considered: single/Tetris points, level", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; s", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; storage denial keeps the in-memory score usable.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "Score now follows line clears and level,", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Score now follows line clears and level, gravity accelerates by level, and high ", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Score now follows line clears and level, gravity accelerates by level, and high score persists when storage is available.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("You", "user", "Polish the visual design and check the finished game. Summarize the controls and any remaining caveats.", "You")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "Thinking through contrast, responsive layout, restart flow, and edge-case verification. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… read_file index.html, styles.css, game.js", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ read_file — simulated project view: index.html, styles.css, game.js", "Read files")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "I will work on contrast, responsive layout, restart flow,", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the curr", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "The important invariant is that contrast, responsive layout, restart flow, and edge-case verification. I can keep the controls wired to the same game state.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… search_code query=\"restart reset\"", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", "Search code")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  ", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; ", "Code · JavaScript")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; refillQueue(); spawnPiece();\n  render(); announce('New game started');\n}\n```", "Code · JavaScript")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 5.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… apply_patch index.html, styles.css, game.js", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+@media (max-width: 640px) { .game { grid-template-columns: 1fr; } }\n+.button:focus-visible { outline: 3px solid #fde047; }", "Patch")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", "Apply patch")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "table", "| Surface | Check | Outcome |\n|---|---|---|\n| Desktop | Keyboard play | Pass |\n| Mobile | Touch layout | Pass |\n| Offline | No assets | Pass |", "Verification table")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "reasoning", "I should check restart reset, wall kick, game over, narrow layout and report the limits of this simulated run accurately.", "Thinking")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "activity", "✓ run_command — Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", "Run command")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "Checks considered: restart reset, wall kick, game over,", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual c", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", "Assistant")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "text", "The single-file game has keyboard and touch co", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best sco", "Assistant")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "text", "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best score, accessible status, and a responsive layout.", "Assistant")));
    private static final double[] SCRIPTED_DURATIONS = {3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.0, 0.0, 0.0};

    private final Transcript transcriptPanel;
    private final DoubleSupplier clock;
    private int nextStep;
    private boolean simulationRunning;
    private boolean retainHistory;
    private boolean followLive;
    private Double nextStepAt;
    private boolean closeRequested;

    private static int simulationEventCount()
    {
        return SCRIPTED_STEPS.size() + SIMULATION_MESSAGE_TARGET - SCRIPTED_MESSAGE_COUNT;
    }

    public AgentChat()
    {
        this(() -> System.nanoTime() / 1_000_000_000.0);
    }

    AgentChat(DoubleSupplier clock)
    {
        this.clock = clock;
        transcriptPanel = new Transcript();
        transcript.setChild(transcriptPanel);
        resetSimulation();
    }

    @Override
    protected void onStartButtonClick()
    {
        startSimulation();
    }

    @Override
    protected void onPauseButtonClick()
    {
        pauseSimulation();
    }

    @Override
    protected void onNextStepButtonClick()
    {
        applyNextStep();
    }

    @Override
    protected void onResetButtonClick()
    {
        resetSimulation();
    }

    @Override
    protected void onLoadStressButtonClick()
    {
        loadStressTranscript();
    }

    @Override
    protected void onTopClick()
    {
        followLive = false;
        transcriptPanel.scrollToTop();
    }

    @Override
    protected void onEndClick()
    {
        followLive = true;
        transcriptPanel.scrollToBottom();
    }

    @Override
    protected void onQuitButtonClick()
    {
        closeRequested = true;
    }

    @Override
    protected boolean shouldClose()
    {
        return closeRequested;
    }

    public void startSimulation()
    {
        if (nextStep >= simulationEventCount())
        {
            if (retainHistory)
            {
                nextStep = 0;
            }
            else
            {
                resetSimulation();
            }
        }
        if (nextStep == 0)
        {
            if (retainHistory)
            {
                transcriptPanel.appendEntry(INITIAL_ENTRY);
            }
            applyNextTimelineEvent();
        }
        transcriptPanel.scrollToBottom();
        followLive = true;
        if (nextStep >= simulationEventCount())
        {
            finishSimulation();
            return;
        }
        simulationRunning = true;
        nextStepAt = clock.getAsDouble() + SCRIPTED_DURATIONS[nextStep - 1];
        setStatus("Running");
    }

    public void pauseSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        setStatus("Paused");
    }

    public void applyNextStep()
    {
        simulationRunning = false;
        nextStepAt = null;
        if (nextStep >= simulationEventCount())
        {
            setStatus("Scenario complete");
            return;
        }
        boolean follow = followLive && atBottom();
        applyNextTimelineEvent();
        if (follow)
        {
            transcriptPanel.scrollToBottom();
        }
        setStatus(nextStep == simulationEventCount() ? "Scenario complete" : "Step applied");
    }

    public void resetSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        transcriptPanel.clearEntries();
        transcriptPanel.appendEntry(INITIAL_ENTRY);
        transcriptPanel.scrollToBottom();
        nextStep = 0;
        retainHistory = false;
        followLive = true;
        setStatus("Ready");
    }

    public void loadStressTranscript()
    {
        simulationRunning = false;
        nextStepAt = null;
        List<TranscriptEntry> entries = new ArrayList<>(STRESS_ENTRY_COUNT);
        for (int index = 0; index < STRESS_ENTRY_COUNT; ++index)
        {
            entries.add(stressEntry(index));
        }
        transcriptPanel.replaceEntries(entries);
        transcriptPanel.scrollToTop();
        nextStep = 0;
        retainHistory = true;
        followLive = false;
        status.setText(
            "Stress loaded · " + messageCountText(transcriptPanel.entryCount()) +
            " · ~" + STRESS_TARGET_ROWS + " rows");
    }

    public int entryCount()
    {
        return transcriptPanel.entryCount();
    }

    @Override
    public boolean tick()
    {
        if (!simulationRunning || nextStepAt == null)
        {
            return false;
        }
        double now = clock.getAsDouble();
        boolean applied = false;
        boolean follow = followLive && atBottom();
        while (now >= nextStepAt && nextStep < simulationEventCount())
        {
            applyNextTimelineEvent();
            if (nextStep < simulationEventCount())
            {
                nextStepAt += SCRIPTED_DURATIONS[nextStep - 1];
            }
            applied = true;
        }
        if (!applied)
        {
            return false;
        }
        if (follow)
        {
            transcriptPanel.scrollToBottom();
        }
        if (nextStep >= simulationEventCount())
        {
            finishSimulation();
        }
        else
        {
            setStatus("Running");
        }
        return true;
    }

    private void applyStep(ScriptedStep step)
    {
        if (step.replaceLast())
        {
            transcriptPanel.replaceLastEntry(step.entry());
        }
        else
        {
            transcriptPanel.appendEntry(step.entry());
        }
    }

    private void applyNextTimelineEvent()
    {
        applyStep(SCRIPTED_STEPS.get(nextStep));
        ++nextStep;
    }

    private boolean atBottom()
    {
        return transcriptPanel.scrollView().viewOffset() == 0 &&
            transcriptPanel.scrollView().scrollOffset() == 0;
    }

    private void finishSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        setStatus("Scenario complete");
    }

    private void setStatus(String prefix)
    {
        status.setText(prefix + " · " + messageCountText(transcriptPanel.entryCount()));
    }

    private static String messageCountText(int count)
    {
        return count + (count == 1 ? " message" : " messages");
    }

    private static TranscriptEntry stressEntry(int index)
    {
        String detail = "SIMULATED STRESS DATA · no external action";
        String body = stressBody(index, stressEntryLineCount(index));
        return switch (index % 8)
        {
            case 0 -> new TranscriptEntry("You", "request", body, detail);
            case 1 -> new TranscriptEntry("Agent", "plan", body, detail);
            case 2 -> new TranscriptEntry("Tool", "search_code", body, detail);
            case 3 -> new TranscriptEntry("Agent", "code", body, detail);
            case 4 -> new TranscriptEntry("Tool", "diff", body, detail);
            case 5 -> new TranscriptEntry("Agent", "table", body, detail);
            case 6 -> new TranscriptEntry("Tool", "warning", body, detail);
            default -> new TranscriptEntry("Agent", "result", body, detail);
        };
    }

    private static int stressEntryLineCount(int index)
    {
        int ordinal = index + 1;
        if (ordinal % STRESS_LONG_MESSAGE_INTERVAL == 0)
        {
            return STRESS_LONG_MESSAGE_ROWS;
        }
        if (ordinal % STRESS_MEDIUM_MESSAGE_INTERVAL == 0)
        {
            return STRESS_MEDIUM_MESSAGE_ROWS;
        }
        return STRESS_MIN_MESSAGE_ROWS + ((index * 17) % STRESS_MESSAGE_ROW_VARIANTS);
    }

    private static String stressBody(int index, int lineCount)
    {
        int variant = index % 8;
        int lineNumber = (index % 997) + 1;
        List<String> lines = new ArrayList<>(lineCount);
        switch (variant)
        {
            case 0 ->
            {
                List<String> prompts = List.of(
                    "- preserve the documented API",
                    "- compare every supported port",
                    "- keep the reproduction offline",
                    "- report focused validation evidence");
                lines.add("Please investigate deterministic case " + index + ".");
                for (int offset = 0; offset < lineCount - 1; ++offset)
                {
                    lines.add(prompts.get(offset % prompts.size()));
                }
            }
            case 1 ->
            {
                List<String> actions = List.of("inspect", "reproduce", "repair", "validate");
                lines.add("## Plan for case " + index);
                for (int offset = 0; offset < lineCount - 1; ++offset)
                {
                    lines.add(
                        (offset + 1) + ". " + actions.get(offset % actions.size()) +
                        " the shared invariant");
                }
            }
            case 2 ->
            {
                lines.add("$ search_code case_" + index);
                for (int offset = 0; offset < lineCount - 2; ++offset)
                {
                    lines.add(
                        "src/module_" + (offset % 7) + ".py:" + (lineNumber + offset) +
                        ": simulated match");
                }
                lines.add("result: " + (lineCount - 2) + " matches");
            }
            case 3 ->
            {
                lines.add("```python");
                lines.add("def case_" + index + "():");
                for (int offset = 0; offset < lineCount - 4; ++offset)
                {
                    lines.add("    value_" + offset + " = " + (index + offset));
                }
                lines.add("    return {\"ok\": True}");
                lines.add("```");
            }
            case 4 ->
            {
                lines.add("--- a/src/case_" + index + ".py");
                lines.add("+++ b/src/case_" + index + ".py");
                lines.add("@@ -1,2 +1,2 @@");
                for (int offset = 0; lines.size() < lineCount; ++offset)
                {
                    String prefix = offset % 2 == 0 ? "-" : "+";
                    lines.add(prefix + "value_" + offset + " = " + (index + offset));
                }
            }
            case 5 ->
            {
                lines.add("| Item | Value | State |");
                lines.add("|:-----|------:|:-----:|");
                for (int offset = 0; lines.size() < lineCount; ++offset)
                {
                    lines.add("| row " + offset + " | " + (index + offset) + " | pass |");
                }
            }
            case 6 ->
            {
                lines.add("ERROR simulated_case_" + index);
                for (int offset = 0; offset < lineCount - 2; ++offset)
                {
                    lines.add(
                        "  at worker_" + (offset % 5) + ".py:" + (lineNumber + offset) +
                        ": expected stable state");
                }
                lines.add("status: simulated failure; no test executed");
            }
            default ->
            {
                List<String> results = List.of(
                    "- [x] focused regression",
                    "- [x] 1:1 parity audit",
                    "- [x] backward compatibility",
                    "Unicode: ľščťžýáíé · 東京 · 🙂");
                lines.add("### Result " + index + " ✓");
                for (int offset = 0; offset < lineCount - 1; ++offset)
                {
                    lines.add(results.get(offset % results.size()));
                }
            }
        }
        return String.join("\n", lines);
    }

    private static void logicTest()
    {
        double[] now = {0.0};
        AgentChat application = new AgentChat(() -> now[0]);
        if (application.entryCount() != 1 || !"Ready · 1 message".equals(application.status.text()))
        {
            throw new IllegalStateException("agent chat initial state failed");
        }
        application.applyNextStep();
        application.startSimulation();
        if (application.entryCount() != 2 || !"Running · 2 messages".equals(application.status.text()))
        {
            throw new IllegalStateException("agent chat timed start failed");
        }
        application.pauseSimulation();
        now[0] += SIMULATION_DURATION_SECONDS;
        if (application.tick() || application.entryCount() != 2)
        {
            throw new IllegalStateException("agent chat pause failed");
        }
        application.applyNextStep();
        application.startSimulation();
        now[0] += SIMULATION_DURATION_SECONDS;
        if (!application.tick()
            || application.entryCount() != SIMULATION_MESSAGE_TARGET
            || !"Scenario complete · 100 messages".equals(application.status.text()))
        {
            throw new IllegalStateException("agent chat scenario failed");
        }
    }

    public static void main(String[] arguments)
    {
        if (arguments.length > 0 && "--logic-test".equals(arguments[0]))
        {
            logicTest();
            return;
        }
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            AgentChat application = new AgentChat();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }

    private record TranscriptEntry(String role, String phase, String body, String detail)
    {
    }

    private record ScriptedStep(boolean replaceLast, TranscriptEntry entry)
    {
    }

    private static final class Transcript extends TranscriptUI
    {
        private static final Set<String> CODE_PHASES = Set.of(
            "code",
            "code · python",
            "diff",
            "patch",
            "result",
            "search_code",
            "structured data",
            "test failure",
            "warning");

        private final List<TranscriptEntry> entries = new ArrayList<>();

        Transcript()
        {
            setAutoScroll(false);
            scrollView().setVirtualDataSource(new VirtualScrollDataSource(
                entries::size,
                index -> index,
                (index, recycled) ->
                {
                    ReusableElement reusable = recycled instanceof ReusableElement existing
                        ? existing
                        : new ReusableElement("entry");
                    setEntry(reusable, entries.get(index));
                    return reusable;
                }));
        }

        void appendEntry(TranscriptEntry entry)
        {
            int index = entries.size();
            entries.add(entry);
            scrollView().virtualItemsInserted(index, 1);
        }

        void replaceLastEntry(TranscriptEntry entry)
        {
            if (entries.isEmpty())
            {
                appendEntry(entry);
                return;
            }
            int index = entries.size() - 1;
            entries.set(index, entry);
            scrollView().virtualItemsReplaced(index, 1);
        }

        void replaceEntries(List<TranscriptEntry> replacement)
        {
            clearEntries();
            entries.addAll(replacement);
            if (!entries.isEmpty())
            {
                scrollView().virtualItemsInserted(0, entries.size());
            }
        }

        void clearEntries()
        {
            int previousCount = entries.size();
            entries.clear();
            if (previousCount > 0)
            {
                scrollView().virtualItemsRemoved(0, previousCount);
            }
            scrollToTop();
        }

        int entryCount()
        {
            return entries.size();
        }

        private static void setEntry(ReusableElement reusable, TranscriptEntry entry)
        {
            if ("You".equals(entry.role()))
            {
                UserEntryUI view;
                if (reusable.child() instanceof UserEntryUI existing)
                {
                    view = existing;
                }
                else
                {
                    view = new UserEntryUI();
                    reusable.setChild(view);
                }
                view.body.setText(entry.body());
            }
            else if ("reasoning".equals(entry.phase()) || "status".equals(entry.phase()))
            {
                ReasoningEntryUI view;
                if (reusable.child() instanceof ReasoningEntryUI existing)
                {
                    view = existing;
                }
                else
                {
                    view = new ReasoningEntryUI();
                    reusable.setChild(view);
                }
                view.heading.setText(entry.detail());
                view.body.setText(entry.body());
            }
            else if ("table".equals(entry.phase()))
            {
                TableEntryUI view;
                if (reusable.child() instanceof TableEntryUI existing)
                {
                    view = existing;
                }
                else
                {
                    view = new TableEntryUI();
                    reusable.setChild(view);
                }
                view.heading.setText(entry.detail());
                view.table.setText(entry.body());
            }
            else if (CODE_PHASES.contains(entry.phase()))
            {
                CodeEntryUI view;
                if (reusable.child() instanceof CodeEntryUI existing)
                {
                    view = existing;
                }
                else
                {
                    view = new CodeEntryUI();
                    reusable.setChild(view);
                }
                view.heading.setText(entry.detail());
                view.code.setText("code".equals(entry.phase()) || "code · python".equals(entry.phase())
                    ? numberedCode(entry.body()) : withoutCodeFence(entry.body()));
            }
            else
            {
                ChatEntryUI view;
                if (reusable.child() instanceof ChatEntryUI existing)
                {
                    view = existing;
                }
                else
                {
                    view = new ChatEntryUI();
                    reusable.setChild(view);
                }
                view.heading.setText("Tool".equals(entry.role()) ? entry.detail() :
                    "Agent".equals(entry.role()) ? "Assistant" : entry.role());
                view.body.setText(entry.body());
            }
        }

        private static String withoutCodeFence(String text)
        {
            List<String> lines = new ArrayList<>(List.of(text.split("\\n", -1)));
            if (!lines.isEmpty() && lines.get(0).startsWith("```"))
            {
                lines.remove(0);
            }
            if (!lines.isEmpty() && lines.get(lines.size() - 1).startsWith("```"))
            {
                lines.remove(lines.size() - 1);
            }
            return String.join("\n", lines);
        }

        private static String numberedCode(String text)
        {
            String[] lines = withoutCodeFence(text).split("\n", -1);
            for (int index = 0; index < lines.length; ++index)
            {
                lines[index] = (index + 1) + "  " + lines[index];
            }
            return String.join("\n", lines);
        }
    }
}
