"""Deterministic, offline agent chat simulation example."""

import os
import sys
import time

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(_here)))

from runtime import UIApplication
from agent_chat_ui import AgentChatUI


# Generated from scenario.json by tools/generate_agent_chat_replay.py.
INITIAL_ENTRY = ('You', 'user', 'Write me a playable Tetris game in one HTML file. Use canvas, keyboard controls, and no external assets.', 'You')
SCRIPTED_STEPS = (
    (False, 'Agent', 'reasoning', 'Thinking through the board model, piece rotation, collision checks, and the game loop. I should keep the implementation in one offline HTML document and check interactions before summarizing.', 'Thinking'),
    (False, 'Agent', 'status', 'Checking the available files and preparing the next tool action.', 'Preparing action'),
    (False, 'Tool', 'activity', '… read_file index.html', 'Read files'),
    (False, 'Tool', 'activity', '✓ read_file — simulated project view: index.html', 'Read files'),
    (False, 'Agent', 'text', 'I will work on the board model, piece rotation, collisio', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the curr', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the current state, then I will make the smallest cohesive update.', 'Assistant'),
    (False, 'Agent', 'reasoning', 'The important invariant is that the board model, piece rotation, collision checks, and the game loop. I can keep the controls wired to the same game state.', 'Thinking'),
    (False, 'Tool', 'activity', '… search_code query="rotation at wall"', 'Search code'),
    (False, 'Tool', 'activity', '✓ search_code — found the relevant functions in index.html; no network access.', 'Search code'),
    (False, 'Agent', 'code', '```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, ', 'Code · JavaScript'),
    (True, 'Agent', 'code', '```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return pie', 'Code · JavaScript'),
    (True, 'Agent', 'code', '```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return piece.cells.some(([x, y]) => board[y + piece.y + dy]?.[x + piece.x + dx] !== 0);\n}\n```', 'Code · JavaScript'),
    (False, 'Agent', 'text', 'The implementation keeps the game loop', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the ', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 1.', 'Assistant'),
    (False, 'Tool', 'activity', '… apply_patch index.html', 'Apply patch'),
    (False, 'Tool', 'diff', '--- a/index.html\n+++ b/index.html\n@@ game update @@\n+const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n+requestAnimationFrame(tick);', 'Patch'),
    (False, 'Tool', 'activity', '✓ apply_patch — simulated changes recorded for index.html.', 'Apply patch'),
    (False, 'Agent', 'table', '| Check | Expected | Observed |\n|---|---|---|\n| Spawn | Centered piece | Pass |\n| Soft drop | One row | Pass |', 'Verification table'),
    (False, 'Agent', 'reasoning', 'I should check rotation at wall, line clear, game over and report the limits of this simulated run accurately.', 'Thinking'),
    (False, 'Tool', 'activity', '… run_command node --check game.js', 'Run command'),
    (False, 'Tool', 'activity', '✓ run_command — Syntax check passed; browser gameplay remains to be exercised.', 'Run command'),
    (False, 'Agent', 'text', 'Checks considered: rotation at wall, lin', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: rotation at wall, line clear, game over. Syntax check passed; ', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: rotation at wall, line clear, game over. Syntax check passed; browser gameplay remains to be exercised.', 'Assistant'),
    (False, 'Agent', 'text', 'The first version has a playable board', 'Assistant'),
    (True, 'Agent', 'text', 'The first version has a playable board, seven tetrominoes, arrow-key movement', 'Assistant'),
    (True, 'Agent', 'text', 'The first version has a playable board, seven tetrominoes, arrow-key movement, rotation, line clearing, and restart.', 'Assistant'),
    (False, 'You', 'user', 'Add a ghost piece, a hold slot, and a three-piece next queue. Keep the controls discoverable.', 'You'),
    (False, 'Agent', 'reasoning', 'Thinking through a projection that never mutates the active piece and a once-per-drop hold rule. I should keep the implementation in one offline HTML document and check interactions before summarizing.', 'Thinking'),
    (False, 'Agent', 'status', 'Checking the available files and preparing the next tool action.', 'Preparing action'),
    (False, 'Tool', 'activity', '… read_file game.js, index.html', 'Read files'),
    (False, 'Tool', 'activity', '✓ read_file — simulated project view: game.js, index.html', 'Read files'),
    (False, 'Agent', 'text', 'I will work on a projection that never mutates the active pi', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the c', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the current state, then I will make the smallest cohesive update.', 'Assistant'),
    (False, 'Agent', 'reasoning', 'The important invariant is that a projection that never mutates the active piece and a once-per-drop hold rule. I can keep the controls wired to the same game state.', 'Thinking'),
    (False, 'Tool', 'activity', '… search_code query="ghost landing"', 'Search code'),
    (False, 'Tool', 'activity', '✓ search_code — found the relevant functions in game.js, index.html; no network access.', 'Search code'),
    (False, 'Agent', 'code', '```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while ', 'Code · JavaScript'),
    (True, 'Agent', 'code', '```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold', 'Code · JavaScript'),
    (True, 'Agent', 'code', '```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold() { if (holdUsed) return; swapHeldPiece(); holdUsed = true; }\n```', 'Code · JavaScript'),
    (False, 'Agent', 'text', 'The implementation keeps the game loop', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the ', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 2.', 'Assistant'),
    (False, 'Tool', 'activity', '… apply_patch game.js, index.html', 'Apply patch'),
    (False, 'Tool', 'diff', "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+drawGhost(active, ghostY(active));\n+drawQueue(nextPieces.slice(0, 3));\n+holdButton.addEventListener('click', hold);", 'Patch'),
    (False, 'Tool', 'activity', '✓ apply_patch — simulated changes recorded for game.js, index.html.', 'Apply patch'),
    (False, 'Agent', 'table', '| Feature | Input | State |\n|---|---|---|\n| Hold | C | Ready |\n| Ghost | Shadow | Visible |\n| Queue | Three pieces | Filled |', 'Verification table'),
    (False, 'Agent', 'reasoning', 'I should check ghost landing, hold once, queue refill and report the limits of this simulated run accurately.', 'Thinking'),
    (False, 'Tool', 'activity', '… run_command node --check game.js', 'Run command'),
    (False, 'Tool', 'activity', '✓ run_command — Syntax check passed; hold is locked until the active piece lands.', 'Run command'),
    (False, 'Agent', 'text', 'Checks considered: ghost landing, hold on', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: ghost landing, hold once, queue refill. Syntax check passed; ho', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: ghost landing, hold once, queue refill. Syntax check passed; hold is locked until the active piece lands.', 'Assistant'),
    (False, 'Agent', 'text', 'The ghost marks the landing row, C h', 'Assistant'),
    (True, 'Agent', 'text', 'The ghost marks the landing row, C holds a piece once per drop, and the ', 'Assistant'),
    (True, 'Agent', 'text', 'The ghost marks the landing row, C holds a piece once per drop, and the sidebar shows the next three pieces.', 'Assistant'),
    (False, 'You', 'user', 'Make it work on phones. Add touch controls, pause, and accessible labels without changing keyboard play.', 'You'),
    (False, 'Agent', 'reasoning', 'Thinking through pointer-sized on-screen controls, pause state, and accessible status announcements. I should keep the implementation in one offline HTML document and check interactions before summarizing.', 'Thinking'),
    (False, 'Agent', 'status', 'Checking the available files and preparing the next tool action.', 'Preparing action'),
    (False, 'Tool', 'activity', '… read_file index.html, styles.css, game.js', 'Read files'),
    (False, 'Tool', 'activity', '✓ read_file — simulated project view: index.html, styles.css, game.js', 'Read files'),
    (False, 'Agent', 'text', 'I will work on pointer-sized on-screen controls, pause state,', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the current state, then I will make the smallest cohesive update.', 'Assistant'),
    (False, 'Agent', 'reasoning', 'The important invariant is that pointer-sized on-screen controls, pause state, and accessible status announcements. I can keep the controls wired to the same game state.', 'Thinking'),
    (False, 'Tool', 'activity', '… search_code query="touch left/right"', 'Search code'),
    (False, 'Tool', 'activity', '✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.', 'Search code'),
    (False, 'Agent', 'code', '```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pau', 'Code · JavaScript'),
    (True, 'Agent', 'code', "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener(", 'Code · JavaScript'),
    (True, 'Agent', 'code', "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener('click', () => actions[button.dataset.action]());\n}\nstatus.setAttribute('aria-live', 'polite');\n```", 'Code · JavaScript'),
    (False, 'Agent', 'text', 'The implementation keeps the game loop', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the ', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 3.', 'Assistant'),
    (False, 'Tool', 'activity', '… apply_patch index.html, styles.css, game.js', 'Apply patch'),
    (False, 'Tool', 'diff', '--- a/index.html\n+++ b/index.html\n@@ game update @@\n+<button data-action="pause" aria-label="Pause or resume game">Pause</button>\n+<p id="status" role="status" aria-live="polite"></p>', 'Patch'),
    (False, 'Tool', 'activity', '✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.', 'Apply patch'),
    (False, 'Agent', 'table', '| Control | Keyboard | Touch |\n|---|---|---|\n| Rotate | Up | Rotate button |\n| Pause | P | Pause button |', 'Verification table'),
    (False, 'Agent', 'reasoning', 'I should check touch left/right, pause, resume, screen-reader status and report the limits of this simulated run accurately.', 'Thinking'),
    (False, 'Tool', 'activity', '… run_command node --check game.js', 'Run command'),
    (False, 'Tool', 'activity', '✓ run_command — Syntax check passed; pointer controls and key controls share the same action functions.', 'Run command'),
    (False, 'Agent', 'text', 'Checks considered: touch left/right, pause, resume, s', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer cont', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer controls and key controls share the same action functions.', 'Assistant'),
    (False, 'Agent', 'text', 'Touch buttons call the same actions a', 'Assistant'),
    (True, 'Agent', 'text', 'Touch buttons call the same actions as keys. Pause freezes the loop, and s', 'Assistant'),
    (True, 'Agent', 'text', 'Touch buttons call the same actions as keys. Pause freezes the loop, and status changes are announced politely.', 'Assistant'),
    (False, 'You', 'user', 'Add scoring, levels, and a saved high score. Handle blocked localStorage gracefully.', 'You'),
    (False, 'Agent', 'reasoning', 'Thinking through line-clear points, speed by level, and optional persistent storage. I should keep the implementation in one offline HTML document and check interactions before summarizing.', 'Thinking'),
    (False, 'Agent', 'status', 'Checking the available files and preparing the next tool action.', 'Preparing action'),
    (False, 'Tool', 'activity', '… read_file game.js, index.html', 'Read files'),
    (False, 'Tool', 'activity', '✓ read_file — simulated project view: game.js, index.html', 'Read files'),
    (False, 'Agent', 'text', 'I will work on line-clear points, speed by level, and op', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the curre', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the current state, then I will make the smallest cohesive update.', 'Assistant'),
    (False, 'Agent', 'reasoning', 'The important invariant is that line-clear points, speed by level, and optional persistent storage. I can keep the controls wired to the same game state.', 'Thinking'),
    (False, 'Tool', 'activity', '… search_code query="single/Tetris points"', 'Search code'),
    (False, 'Tool', 'activity', '✓ search_code — found the relevant functions in game.js, index.html; no network access.', 'Search code'),
    (False, 'Agent', 'code', '```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POIN', 'Code · JavaScript'),
    (True, 'Agent', 'code', '```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.s', 'Code · JavaScript'),
    (True, 'Agent', 'code', "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.setItem('tetris-best', String(best)); }\ncatch { storageAvailable = false; }\n```", 'Code · JavaScript'),
    (False, 'Agent', 'text', 'The implementation keeps the game loop', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the ', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 4.', 'Assistant'),
    (False, 'Tool', 'activity', '… apply_patch game.js, index.html', 'Apply patch'),
    (False, 'Tool', 'diff', '--- a/game.js\n+++ b/game.js\n@@ game update @@\n+scoreLabel.textContent = String(score);\n+levelLabel.textContent = String(level);\n+bestLabel.textContent = String(best);', 'Patch'),
    (False, 'Tool', 'activity', '✓ apply_patch — simulated changes recorded for game.js, index.html.', 'Apply patch'),
    (False, 'Agent', 'table', '| Lines | Base points | At level 2 |\n|---:|---:|---:|\n| 1 | 100 | 200 |\n| 4 | 800 | 1600 |', 'Verification table'),
    (False, 'Agent', 'reasoning', 'I should check single/Tetris points, level-up, storage exception and report the limits of this simulated run accurately.', 'Thinking'),
    (False, 'Tool', 'activity', '… run_command node --check game.js', 'Run command'),
    (False, 'Tool', 'activity', '✓ run_command — Syntax check passed; storage denial keeps the in-memory score usable.', 'Run command'),
    (False, 'Agent', 'text', 'Checks considered: single/Tetris points, level', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; s', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; storage denial keeps the in-memory score usable.', 'Assistant'),
    (False, 'Agent', 'text', 'Score now follows line clears and level,', 'Assistant'),
    (True, 'Agent', 'text', 'Score now follows line clears and level, gravity accelerates by level, and high ', 'Assistant'),
    (True, 'Agent', 'text', 'Score now follows line clears and level, gravity accelerates by level, and high score persists when storage is available.', 'Assistant'),
    (False, 'You', 'user', 'Polish the visual design and check the finished game. Summarize the controls and any remaining caveats.', 'You'),
    (False, 'Agent', 'reasoning', 'Thinking through contrast, responsive layout, restart flow, and edge-case verification. I should keep the implementation in one offline HTML document and check interactions before summarizing.', 'Thinking'),
    (False, 'Agent', 'status', 'Checking the available files and preparing the next tool action.', 'Preparing action'),
    (False, 'Tool', 'activity', '… read_file index.html, styles.css, game.js', 'Read files'),
    (False, 'Tool', 'activity', '✓ read_file — simulated project view: index.html, styles.css, game.js', 'Read files'),
    (False, 'Agent', 'text', 'I will work on contrast, responsive layout, restart flow,', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the curr', 'Assistant'),
    (True, 'Agent', 'text', 'I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the current state, then I will make the smallest cohesive update.', 'Assistant'),
    (False, 'Agent', 'reasoning', 'The important invariant is that contrast, responsive layout, restart flow, and edge-case verification. I can keep the controls wired to the same game state.', 'Thinking'),
    (False, 'Tool', 'activity', '… search_code query="restart reset"', 'Search code'),
    (False, 'Tool', 'activity', '✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.', 'Search code'),
    (False, 'Agent', 'code', '```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  ', 'Code · JavaScript'),
    (True, 'Agent', 'code', '```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; ', 'Code · JavaScript'),
    (True, 'Agent', 'code', "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; refillQueue(); spawnPiece();\n  render(); announce('New game started');\n}\n```", 'Code · JavaScript'),
    (False, 'Agent', 'text', 'The implementation keeps the game loop', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the ', 'Assistant'),
    (True, 'Agent', 'text', 'The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 5.', 'Assistant'),
    (False, 'Tool', 'activity', '… apply_patch index.html, styles.css, game.js', 'Apply patch'),
    (False, 'Tool', 'diff', '--- a/index.html\n+++ b/index.html\n@@ game update @@\n+@media (max-width: 640px) { .game { grid-template-columns: 1fr; } }\n+.button:focus-visible { outline: 3px solid #fde047; }', 'Patch'),
    (False, 'Tool', 'activity', '✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.', 'Apply patch'),
    (False, 'Agent', 'table', '| Surface | Check | Outcome |\n|---|---|---|\n| Desktop | Keyboard play | Pass |\n| Mobile | Touch layout | Pass |\n| Offline | No assets | Pass |', 'Verification table'),
    (False, 'Agent', 'reasoning', 'I should check restart reset, wall kick, game over, narrow layout and report the limits of this simulated run accurately.', 'Thinking'),
    (False, 'Tool', 'activity', '… run_command node --check game.js', 'Run command'),
    (False, 'Tool', 'activity', '✓ run_command — Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.', 'Run command'),
    (False, 'Agent', 'text', 'Checks considered: restart reset, wall kick, game over,', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual c', 'Assistant'),
    (True, 'Agent', 'text', 'Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.', 'Assistant'),
    (False, 'Agent', 'text', 'The single-file game has keyboard and touch co', 'Assistant'),
    (True, 'Agent', 'text', 'The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best sco', 'Assistant'),
    (True, 'Agent', 'text', 'The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best score, accessible status, and a responsive layout.', 'Assistant'),
)
SCRIPTED_DURATIONS = (3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.0, 0.0, 0.0)

STRESS_ENTRY_COUNT = 5_556
STRESS_TARGET_ROWS = 50_000
STRESS_MIN_MESSAGE_ROWS = 5
STRESS_MESSAGE_ROW_VARIANTS = 4
STRESS_MEDIUM_MESSAGE_ROWS = 40
STRESS_MEDIUM_MESSAGE_INTERVAL = 53
STRESS_LONG_MESSAGE_ROWS = 100
STRESS_LONG_MESSAGE_INTERVAL = 211
SIMULATION_MESSAGE_TARGET = 100
SIMULATION_DURATION_SECONDS = 180.0
STRESS_TEMPLATES = (
    ("You", "request", "Investigate deterministic case {index}.\n- preserve API\n- compare ports"),
    ("Agent", "plan", "## Plan {index}\n1. inspect\n2. reproduce\n3. repair\n4. validate"),
    ("Tool", "search_code", "$ search_code case_{index}\nsrc/module.py:{line}: match\nresult: simulated"),
    ("Agent", "code", "```python\ndef case_{index}():\n    return {{\"ok\": True}}\n```"),
    ("Tool", "diff", "--- before_{index}\n+++ after_{index}\n@@ -1 +1 @@\n-old\n+new"),
    ("Agent", "table", "| item | value |\n|------|-------|\n| id | {index} |\n| state | pass |"),
    ("Tool", "warning", "WARNING simulated retry {index}\ntrace: worker -> parser -> validator\nno external action"),
    ("Agent", "result", "{{\"id\": {index}, \"status\": \"ok\"}}\nUnicode: čau · 東京 · 🙂"),
)


def stress_entry_line_count(index):
    """Return a deterministic pseudo-random message height from 5 to 100 rows."""
    ordinal = int(index) + 1
    if ordinal % STRESS_LONG_MESSAGE_INTERVAL == 0:
        return STRESS_LONG_MESSAGE_ROWS
    if ordinal % STRESS_MEDIUM_MESSAGE_INTERVAL == 0:
        return STRESS_MEDIUM_MESSAGE_ROWS
    return STRESS_MIN_MESSAGE_ROWS + ((int(index) * 17) % STRESS_MESSAGE_ROW_VARIANTS)


def _stress_body(index, line_count):
    variant = index % len(STRESS_TEMPLATES)
    line_number = (index % 997) + 1
    if variant == 0:
        lines = [f"Please investigate deterministic case {index}."]
        prompts = (
            "- preserve the documented API",
            "- compare every supported port",
            "- keep the reproduction offline",
            "- report focused validation evidence",
        )
        lines.extend(prompts[offset % len(prompts)] for offset in range(line_count - 1))
        return "\n".join(lines)
    if variant == 1:
        lines = [f"## Plan for case {index}"]
        actions = ("inspect", "reproduce", "repair", "validate")
        lines.extend(
            f"{offset + 1}. {actions[offset % len(actions)]} the shared invariant"
            for offset in range(line_count - 1)
        )
        return "\n".join(lines)
    if variant == 2:
        lines = [f"$ search_code case_{index}"]
        lines.extend(
            f"src/module_{offset % 7}.py:{line_number + offset}: simulated match"
            for offset in range(line_count - 2)
        )
        lines.append(f"result: {max(0, line_count - 2)} matches")
        return "\n".join(lines)
    if variant == 3:
        lines = ["```python", f"def case_{index}():"]
        lines.extend(
            f"    value_{offset} = {index + offset}"
            for offset in range(max(0, line_count - 4))
        )
        lines.extend(("    return {\"ok\": True}", "```"))
        return "\n".join(lines)
    if variant == 4:
        lines = [f"--- a/src/case_{index}.py", f"+++ b/src/case_{index}.py", "@@ -1,2 +1,2 @@"]
        for offset in range(max(0, line_count - len(lines))):
            prefix = "-" if offset % 2 == 0 else "+"
            lines.append(f"{prefix}value_{offset} = {index + offset}")
        return "\n".join(lines)
    if variant == 5:
        lines = ["| Item | Value | State |", "|:-----|------:|:-----:|"]
        lines.extend(
            f"| row {offset} | {index + offset} | pass |"
            for offset in range(max(1, line_count - 2))
        )
        return "\n".join(lines)
    if variant == 6:
        lines = [f"ERROR simulated_case_{index}"]
        lines.extend(
            f"  at worker_{offset % 5}.py:{line_number + offset}: expected stable state"
            for offset in range(line_count - 2)
        )
        lines.append("status: simulated failure; no test executed")
        return "\n".join(lines)
    lines = [f"### Result {index} ✓"]
    results = (
        "- [x] focused regression",
        "- [x] 1:1 parity audit",
        "- [x] backward compatibility",
        "Unicode: ľščťžýáíé · 東京 · 🙂",
    )
    lines.extend(results[offset % len(results)] for offset in range(line_count - 1))
    return "\n".join(lines)


def stress_entry(index):
    """Return one deterministic rich stress entry without I/O or random state."""
    role, phase, _template = STRESS_TEMPLATES[index % len(STRESS_TEMPLATES)]
    return (
        role,
        phase,
        _stress_body(index, stress_entry_line_count(index)),
        "SIMULATED STRESS DATA · no external action",
    )


SCRIPTED_MESSAGE_COUNT = 1 + sum(1 for step in SCRIPTED_STEPS if not step[0])
SIMULATION_EVENT_COUNT = (
    len(SCRIPTED_STEPS) + SIMULATION_MESSAGE_TARGET - SCRIPTED_MESSAGE_COUNT
)
class AgentChatSimulation(AgentChatUI):
    """Run a deterministic visual chat scenario without external work."""

    def __init__(self, clock=None):
        super().__init__()
        self._clock = clock or time.monotonic
        self._next_step = 0
        self._running_simulation = False
        self._next_step_at = None
        self._retain_history = False
        self._follow_live = False

    def open(self):
        super().open()
        if self.transcript.child.entry_count() == 0:
            self.reset_simulation()

    def on_start_button_click(self):
        self.start_simulation()

    def on_pause_button_click(self):
        self.pause_simulation()

    def on_next_step_button_click(self):
        self.next_step()

    def on_reset_button_click(self):
        self.reset_simulation()

    def on_load_stress_button_click(self):
        self.load_stress_transcript()

    def on_top_click(self):
        self._follow_live = False
        self.transcript.child.scroll_to_top()

    def on_end_click(self):
        self._follow_live = True
        self.transcript.child.scroll_to_bottom()

    def on_quit_button_click(self):
        self.quit()

    def start_simulation(self):
        if self._next_step >= SIMULATION_EVENT_COUNT:
            if self._retain_history:
                self._next_step = 0
            else:
                self.reset_simulation()
        if self._next_step == 0:
            if self._retain_history:
                self.transcript.child.append_entry(*INITIAL_ENTRY)
            self._apply_next_timeline_event()
        self.transcript.child.scroll_to_bottom()
        self._follow_live = True
        if self._next_step >= SIMULATION_EVENT_COUNT:
            self._finish_simulation()
            return
        self._running_simulation = True
        self._next_step_at = self._clock() + SCRIPTED_DURATIONS[self._next_step - 1]
        self._set_status("Running")

    def pause_simulation(self):
        self._running_simulation = False
        self._next_step_at = None
        self._set_status("Paused")

    def next_step(self):
        self._running_simulation = False
        self._next_step_at = None
        if self._next_step >= SIMULATION_EVENT_COUNT:
            self._set_status("Scenario complete")
            return
        follow = self._follow_live and self.transcript.child.scroll_position()["view_offset"] == 0
        self._apply_next_timeline_event()
        if follow:
            self.transcript.child.scroll_to_bottom()
        prefix = "Scenario complete" if self._next_step == SIMULATION_EVENT_COUNT else "Step applied"
        self._set_status(prefix)

    def reset_simulation(self):
        self._running_simulation = False
        self._next_step_at = None
        transcript = self.transcript.child
        transcript.clear_entries()
        transcript.append_entry(*INITIAL_ENTRY)
        transcript.scroll_to_bottom()
        self._next_step = 0
        self._retain_history = False
        self._follow_live = True
        self._set_status("Ready")

    def load_stress_transcript(self):
        self._running_simulation = False
        self._next_step_at = None
        transcript = self.transcript.child
        transcript.replace_entries(stress_entry(index) for index in range(STRESS_ENTRY_COUNT))
        transcript.scroll_to_top()
        self._next_step = 0
        self._retain_history = True
        self._follow_live = False
        self.status.text = (
            f"Stress loaded · {transcript.entry_count()} messages · "
            f"~{STRESS_TARGET_ROWS} rows"
        )

    def _apply_step(self, step):
        replace_last, role, phase, body, detail = step
        transcript = self.transcript.child
        if replace_last:
            transcript.replace_last_entry(role, phase, body, detail)
        else:
            transcript.append_entry(role, phase, body, detail)

    def _apply_next_timeline_event(self):
        self._apply_step(SCRIPTED_STEPS[self._next_step])
        self._next_step += 1

    def _finish_simulation(self):
        self._running_simulation = False
        self._next_step_at = None
        self._set_status("Scenario complete")

    def tick(self):
        changed = bool(super().tick())
        if not self._running_simulation or self._next_step_at is None:
            return changed
        now = self._clock()
        applied = False
        follow = self._follow_live and self.transcript.child.scroll_position()["view_offset"] == 0
        while now >= self._next_step_at and self._next_step < SIMULATION_EVENT_COUNT:
            self._apply_next_timeline_event()
            if self._next_step < SIMULATION_EVENT_COUNT:
                self._next_step_at += SCRIPTED_DURATIONS[self._next_step - 1]
            applied = True
        if not applied:
            return changed
        if follow:
            self.transcript.child.scroll_to_bottom()
        if self._next_step >= SIMULATION_EVENT_COUNT:
            self._finish_simulation()
        else:
            self._set_status("Running")
        return True

    def _set_status(self, prefix):
        count = self.transcript.child.entry_count()
        noun = "message" if count == 1 else "messages"
        self.status.text = f"{prefix} · {count} {noun}"


def main():
    app = UIApplication()
    window = AgentChatSimulation()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
