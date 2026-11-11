using System.Diagnostics;
using Uimd;

internal readonly record struct TranscriptEntry(string Role, string Phase, string Body, string Detail);

internal readonly record struct ScriptedStep(bool ReplaceLast, TranscriptEntry Entry);

internal sealed class Transcript : TranscriptUI
{
    private static readonly HashSet<string> CodePhases = new(StringComparer.Ordinal)
    {
        "code",
        "code · python",
        "diff",
        "patch",
        "result",
        "search_code",
        "structured data",
        "test failure",
        "warning",
    };

    private readonly List<TranscriptEntry> entries = new();

    public Transcript()
    {
        SetAutoScroll(false);
        ScrollView().SetVirtualDataSource(new VirtualScrollDataSource
        {
            ItemCount = () => entries.Count,
            ItemKey = index => index,
            Materialize = (index, recycled) =>
            {
                ReusableElement reusable = recycled as ReusableElement ?? new ReusableElement("entry");
                SetEntry(reusable, entries[index]);
                return reusable;
            },
        });
    }

    public void AppendEntry(TranscriptEntry entry)
    {
        int index = entries.Count;
        entries.Add(entry);
        ScrollView().VirtualItemsInserted(index);
    }

    public void ReplaceLastEntry(TranscriptEntry entry)
    {
        if (entries.Count == 0)
        {
            AppendEntry(entry);
            return;
        }
        entries[^1] = entry;
        ScrollView().VirtualItemsReplaced(entries.Count - 1);
    }

    public void ReplaceEntries(IEnumerable<TranscriptEntry> replacement)
    {
        ClearEntries();
        entries.AddRange(replacement);
        if (entries.Count > 0)
        {
            ScrollView().VirtualItemsInserted(0, entries.Count);
        }
    }

    public void ClearEntries()
    {
        int previousCount = entries.Count;
        entries.Clear();
        if (previousCount > 0)
        {
            ScrollView().VirtualItemsRemoved(0, previousCount);
        }
        _ = ScrollToTop();
    }

    public int EntryCount()
    {
        return entries.Count;
    }

    private static void SetEntry(ReusableElement reusable, TranscriptEntry entry)
    {
        if (entry.Role == "You")
        {
            UserEntryUI view = reusable.Child as UserEntryUI ?? new UserEntryUI();
            if (!ReferenceEquals(view, reusable.Child)) reusable.SetChild(view);
            view.body.SetText(entry.Body);
        }
        else if (entry.Phase is "reasoning" or "status")
        {
            ReasoningEntryUI view = reusable.Child as ReasoningEntryUI ?? new ReasoningEntryUI();
            if (!ReferenceEquals(view, reusable.Child)) reusable.SetChild(view);
            view.heading.SetText(entry.Detail);
            view.body.SetText(entry.Body);
        }
        else if (entry.Phase == "table")
        {
            TableEntryUI view = reusable.Child as TableEntryUI ?? new TableEntryUI();
            if (!ReferenceEquals(view, reusable.Child))
            {
                reusable.SetChild(view);
            }
            view.heading.SetText(entry.Detail);
            view.table.SetText(entry.Body);
        }
        else if (CodePhases.Contains(entry.Phase))
        {
            CodeEntryUI view = reusable.Child as CodeEntryUI ?? new CodeEntryUI();
            if (!ReferenceEquals(view, reusable.Child))
            {
                reusable.SetChild(view);
            }
            view.heading.SetText(entry.Detail);
            view.code.SetText(entry.Phase is "code" or "code · python"
                ? NumberedCode(entry.Body) : WithoutCodeFence(entry.Body));
        }
        else
        {
            ChatEntryUI view = reusable.Child as ChatEntryUI ?? new ChatEntryUI();
            if (!ReferenceEquals(view, reusable.Child))
            {
                reusable.SetChild(view);
            }
            view.heading.SetText(entry.Role == "Tool" ? entry.Detail :
                entry.Role == "Agent" ? "Assistant" : entry.Role);
            view.body.SetText(entry.Body);
        }
    }

    private static string WithoutCodeFence(string text)
    {
        List<string> lines = text.Split('\n').ToList();
        if (lines.Count > 0 && lines[0].StartsWith("```", StringComparison.Ordinal))
        {
            lines.RemoveAt(0);
        }
        if (lines.Count > 0 && lines[^1].StartsWith("```", StringComparison.Ordinal))
        {
            lines.RemoveAt(lines.Count - 1);
        }
        return string.Join("\n", lines);
    }

    private static string NumberedCode(string text)
    {
        return string.Join("\n", WithoutCodeFence(text).Split('\n')
            .Select((line, index) => $"{index + 1}  {line}"));
    }
}

public sealed class AgentChat : AgentChatUI
{
    private const int StressEntryCount = 5_556;
    private const int StressTargetRows = 50_000;
    private const int StressMinMessageRows = 5;
    private const int StressMessageRowVariants = 4;
    private const int StressMediumMessageRows = 40;
    private const int StressMediumMessageInterval = 53;
    private const int StressLongMessageRows = 100;
    private const int StressLongMessageInterval = 211;
    internal const int SimulationMessageTarget = 100;
    private const int ScriptedMessageCount = 100;
    internal const double SimulationDurationSeconds = 180.0;

    // Generated from python/examples/agent_chat/scenario.json.
    private static readonly TranscriptEntry InitialEntry = new("You", "user", "Write me a playable Tetris game in one HTML file. Use canvas, keyboard controls, and no external assets.", "You");
    private static readonly ScriptedStep[] ScriptedSteps =
    {
        new(false, new("Agent", "reasoning", "Thinking through the board model, piece rotation, collision checks, and the game loop. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new(false, new("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new(false, new("Tool", "activity", "… read_file index.html", "Read files")),
        new(false, new("Tool", "activity", "✓ read_file — simulated project view: index.html", "Read files")),
        new(false, new("Agent", "text", "I will work on the board model, piece rotation, collisio", "Assistant")),
        new(true, new("Agent", "text", "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the curr", "Assistant")),
        new(true, new("Agent", "text", "I will work on the board model, piece rotation, collision checks, and the game loop. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new(false, new("Agent", "reasoning", "The important invariant is that the board model, piece rotation, collision checks, and the game loop. I can keep the controls wired to the same game state.", "Thinking")),
        new(false, new("Tool", "activity", "… search_code query=\"rotation at wall\"", "Search code")),
        new(false, new("Tool", "activity", "✓ search_code — found the relevant functions in index.html; no network access.", "Search code")),
        new(false, new("Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, ", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return pie", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nconst COLS = 10, ROWS = 20;\nconst board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\nfunction collides(piece, dx = 0, dy = 0) {\n  return piece.cells.some(([x, y]) => board[y + piece.y + dy]?.[x + piece.x + dx] !== 0);\n}\n```", "Code · JavaScript")),
        new(false, new("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 1.", "Assistant")),
        new(false, new("Tool", "activity", "… apply_patch index.html", "Apply patch")),
        new(false, new("Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+const board = Array.from({length: ROWS}, () => Array(COLS).fill(0));\n+requestAnimationFrame(tick);", "Patch")),
        new(false, new("Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html.", "Apply patch")),
        new(false, new("Agent", "table", "| Check | Expected | Observed |\n|---|---|---|\n| Spawn | Centered piece | Pass |\n| Soft drop | One row | Pass |", "Verification table")),
        new(false, new("Agent", "reasoning", "I should check rotation at wall, line clear, game over and report the limits of this simulated run accurately.", "Thinking")),
        new(false, new("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new(false, new("Tool", "activity", "✓ run_command — Syntax check passed; browser gameplay remains to be exercised.", "Run command")),
        new(false, new("Agent", "text", "Checks considered: rotation at wall, lin", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: rotation at wall, line clear, game over. Syntax check passed; ", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: rotation at wall, line clear, game over. Syntax check passed; browser gameplay remains to be exercised.", "Assistant")),
        new(false, new("Agent", "text", "The first version has a playable board", "Assistant")),
        new(true, new("Agent", "text", "The first version has a playable board, seven tetrominoes, arrow-key movement", "Assistant")),
        new(true, new("Agent", "text", "The first version has a playable board, seven tetrominoes, arrow-key movement, rotation, line clearing, and restart.", "Assistant")),
        new(false, new("You", "user", "Add a ghost piece, a hold slot, and a three-piece next queue. Keep the controls discoverable.", "You")),
        new(false, new("Agent", "reasoning", "Thinking through a projection that never mutates the active piece and a once-per-drop hold rule. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new(false, new("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new(false, new("Tool", "activity", "… read_file game.js, index.html", "Read files")),
        new(false, new("Tool", "activity", "✓ read_file — simulated project view: game.js, index.html", "Read files")),
        new(false, new("Agent", "text", "I will work on a projection that never mutates the active pi", "Assistant")),
        new(true, new("Agent", "text", "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the c", "Assistant")),
        new(true, new("Agent", "text", "I will work on a projection that never mutates the active piece and a once-per-drop hold rule. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new(false, new("Agent", "reasoning", "The important invariant is that a projection that never mutates the active piece and a once-per-drop hold rule. I can keep the controls wired to the same game state.", "Thinking")),
        new(false, new("Tool", "activity", "… search_code query=\"ghost landing\"", "Search code")),
        new(false, new("Tool", "activity", "✓ search_code — found the relevant functions in game.js, index.html; no network access.", "Search code")),
        new(false, new("Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while ", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nfunction ghostY(piece) {\n  let y = piece.y;\n  while (!collides({...piece, y: y + 1})) y++;\n  return y;\n}\nfunction hold() { if (holdUsed) return; swapHeldPiece(); holdUsed = true; }\n```", "Code · JavaScript")),
        new(false, new("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 2.", "Assistant")),
        new(false, new("Tool", "activity", "… apply_patch game.js, index.html", "Apply patch")),
        new(false, new("Tool", "diff", "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+drawGhost(active, ghostY(active));\n+drawQueue(nextPieces.slice(0, 3));\n+holdButton.addEventListener('click', hold);", "Patch")),
        new(false, new("Tool", "activity", "✓ apply_patch — simulated changes recorded for game.js, index.html.", "Apply patch")),
        new(false, new("Agent", "table", "| Feature | Input | State |\n|---|---|---|\n| Hold | C | Ready |\n| Ghost | Shadow | Visible |\n| Queue | Three pieces | Filled |", "Verification table")),
        new(false, new("Agent", "reasoning", "I should check ghost landing, hold once, queue refill and report the limits of this simulated run accurately.", "Thinking")),
        new(false, new("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new(false, new("Tool", "activity", "✓ run_command — Syntax check passed; hold is locked until the active piece lands.", "Run command")),
        new(false, new("Agent", "text", "Checks considered: ghost landing, hold on", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; ho", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: ghost landing, hold once, queue refill. Syntax check passed; hold is locked until the active piece lands.", "Assistant")),
        new(false, new("Agent", "text", "The ghost marks the landing row, C h", "Assistant")),
        new(true, new("Agent", "text", "The ghost marks the landing row, C holds a piece once per drop, and the ", "Assistant")),
        new(true, new("Agent", "text", "The ghost marks the landing row, C holds a piece once per drop, and the sidebar shows the next three pieces.", "Assistant")),
        new(false, new("You", "user", "Make it work on phones. Add touch controls, pause, and accessible labels without changing keyboard play.", "You")),
        new(false, new("Agent", "reasoning", "Thinking through pointer-sized on-screen controls, pause state, and accessible status announcements. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new(false, new("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new(false, new("Tool", "activity", "… read_file index.html, styles.css, game.js", "Read files")),
        new(false, new("Tool", "activity", "✓ read_file — simulated project view: index.html, styles.css, game.js", "Read files")),
        new(false, new("Agent", "text", "I will work on pointer-sized on-screen controls, pause state,", "Assistant")),
        new(true, new("Agent", "text", "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the", "Assistant")),
        new(true, new("Agent", "text", "I will work on pointer-sized on-screen controls, pause state, and accessible status announcements. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new(false, new("Agent", "reasoning", "The important invariant is that pointer-sized on-screen controls, pause state, and accessible status announcements. I can keep the controls wired to the same game state.", "Thinking")),
        new(false, new("Tool", "activity", "… search_code query=\"touch left/right\"", "Search code")),
        new(false, new("Tool", "activity", "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", "Search code")),
        new(false, new("Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pau", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener(", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nconst actions = {left: moveLeft, right: moveRight, down: softDrop, rotate, hold, pause};\nfor (const button of document.querySelectorAll('[data-action]')) {\n  button.addEventListener('click', () => actions[button.dataset.action]());\n}\nstatus.setAttribute('aria-live', 'polite');\n```", "Code · JavaScript")),
        new(false, new("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 3.", "Assistant")),
        new(false, new("Tool", "activity", "… apply_patch index.html, styles.css, game.js", "Apply patch")),
        new(false, new("Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+<button data-action=\"pause\" aria-label=\"Pause or resume game\">Pause</button>\n+<p id=\"status\" role=\"status\" aria-live=\"polite\"></p>", "Patch")),
        new(false, new("Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", "Apply patch")),
        new(false, new("Agent", "table", "| Control | Keyboard | Touch |\n|---|---|---|\n| Rotate | Up | Rotate button |\n| Pause | P | Pause button |", "Verification table")),
        new(false, new("Agent", "reasoning", "I should check touch left/right, pause, resume, screen-reader status and report the limits of this simulated run accurately.", "Thinking")),
        new(false, new("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new(false, new("Tool", "activity", "✓ run_command — Syntax check passed; pointer controls and key controls share the same action functions.", "Run command")),
        new(false, new("Agent", "text", "Checks considered: touch left/right, pause, resume, s", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer cont", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: touch left/right, pause, resume, screen-reader status. Syntax check passed; pointer controls and key controls share the same action functions.", "Assistant")),
        new(false, new("Agent", "text", "Touch buttons call the same actions a", "Assistant")),
        new(true, new("Agent", "text", "Touch buttons call the same actions as keys. Pause freezes the loop, and s", "Assistant")),
        new(true, new("Agent", "text", "Touch buttons call the same actions as keys. Pause freezes the loop, and status changes are announced politely.", "Assistant")),
        new(false, new("You", "user", "Add scoring, levels, and a saved high score. Handle blocked localStorage gracefully.", "You")),
        new(false, new("Agent", "reasoning", "Thinking through line-clear points, speed by level, and optional persistent storage. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new(false, new("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new(false, new("Tool", "activity", "… read_file game.js, index.html", "Read files")),
        new(false, new("Tool", "activity", "✓ read_file — simulated project view: game.js, index.html", "Read files")),
        new(false, new("Agent", "text", "I will work on line-clear points, speed by level, and op", "Assistant")),
        new(true, new("Agent", "text", "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the curre", "Assistant")),
        new(true, new("Agent", "text", "I will work on line-clear points, speed by level, and optional persistent storage. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new(false, new("Agent", "reasoning", "The important invariant is that line-clear points, speed by level, and optional persistent storage. I can keep the controls wired to the same game state.", "Thinking")),
        new(false, new("Tool", "activity", "… search_code query=\"single/Tetris points\"", "Search code")),
        new(false, new("Tool", "activity", "✓ search_code — found the relevant functions in game.js, index.html; no network access.", "Search code")),
        new(false, new("Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POIN", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.s", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nconst LINE_POINTS = [0, 100, 300, 500, 800];\nscore += LINE_POINTS[cleared] * level;\nlevel = Math.floor(lines / 10) + 1;\ntry { localStorage.setItem('tetris-best', String(best)); }\ncatch { storageAvailable = false; }\n```", "Code · JavaScript")),
        new(false, new("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 4.", "Assistant")),
        new(false, new("Tool", "activity", "… apply_patch game.js, index.html", "Apply patch")),
        new(false, new("Tool", "diff", "--- a/game.js\n+++ b/game.js\n@@ game update @@\n+scoreLabel.textContent = String(score);\n+levelLabel.textContent = String(level);\n+bestLabel.textContent = String(best);", "Patch")),
        new(false, new("Tool", "activity", "✓ apply_patch — simulated changes recorded for game.js, index.html.", "Apply patch")),
        new(false, new("Agent", "table", "| Lines | Base points | At level 2 |\n|---:|---:|---:|\n| 1 | 100 | 200 |\n| 4 | 800 | 1600 |", "Verification table")),
        new(false, new("Agent", "reasoning", "I should check single/Tetris points, level-up, storage exception and report the limits of this simulated run accurately.", "Thinking")),
        new(false, new("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new(false, new("Tool", "activity", "✓ run_command — Syntax check passed; storage denial keeps the in-memory score usable.", "Run command")),
        new(false, new("Agent", "text", "Checks considered: single/Tetris points, level", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; s", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: single/Tetris points, level-up, storage exception. Syntax check passed; storage denial keeps the in-memory score usable.", "Assistant")),
        new(false, new("Agent", "text", "Score now follows line clears and level,", "Assistant")),
        new(true, new("Agent", "text", "Score now follows line clears and level, gravity accelerates by level, and high ", "Assistant")),
        new(true, new("Agent", "text", "Score now follows line clears and level, gravity accelerates by level, and high score persists when storage is available.", "Assistant")),
        new(false, new("You", "user", "Polish the visual design and check the finished game. Summarize the controls and any remaining caveats.", "You")),
        new(false, new("Agent", "reasoning", "Thinking through contrast, responsive layout, restart flow, and edge-case verification. I should keep the implementation in one offline HTML document and check interactions before summarizing.", "Thinking")),
        new(false, new("Agent", "status", "Checking the available files and preparing the next tool action.", "Preparing action")),
        new(false, new("Tool", "activity", "… read_file index.html, styles.css, game.js", "Read files")),
        new(false, new("Tool", "activity", "✓ read_file — simulated project view: index.html, styles.css, game.js", "Read files")),
        new(false, new("Agent", "text", "I will work on contrast, responsive layout, restart flow,", "Assistant")),
        new(true, new("Agent", "text", "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the curr", "Assistant")),
        new(true, new("Agent", "text", "I will work on contrast, responsive layout, restart flow, and edge-case verification. First I am checking the current state, then I will make the smallest cohesive update.", "Assistant")),
        new(false, new("Agent", "reasoning", "The important invariant is that contrast, responsive layout, restart flow, and edge-case verification. I can keep the controls wired to the same game state.", "Thinking")),
        new(false, new("Tool", "activity", "… search_code query=\"restart reset\"", "Search code")),
        new(false, new("Tool", "activity", "✓ search_code — found the relevant functions in index.html, styles.css, game.js; no network access.", "Search code")),
        new(false, new("Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  ", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; ", "Code · JavaScript")),
        new(true, new("Agent", "code", "```javascript\nfunction resetGame() {\n  board.forEach(row => row.fill(0));\n  score = 0; lines = 0; level = 1; holdUsed = false;\n  nextPieces.length = 0; refillQueue(); spawnPiece();\n  render(); announce('New game started');\n}\n```", "Code · JavaScript")),
        new(false, new("Agent", "text", "The implementation keeps the game loop", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the ", "Assistant")),
        new(true, new("Agent", "text", "The implementation keeps the game loop deterministic. I will now update the UI and the event wiring for request 5.", "Assistant")),
        new(false, new("Tool", "activity", "… apply_patch index.html, styles.css, game.js", "Apply patch")),
        new(false, new("Tool", "diff", "--- a/index.html\n+++ b/index.html\n@@ game update @@\n+@media (max-width: 640px) { .game { grid-template-columns: 1fr; } }\n+.button:focus-visible { outline: 3px solid #fde047; }", "Patch")),
        new(false, new("Tool", "activity", "✓ apply_patch — simulated changes recorded for index.html, styles.css, game.js.", "Apply patch")),
        new(false, new("Agent", "table", "| Surface | Check | Outcome |\n|---|---|---|\n| Desktop | Keyboard play | Pass |\n| Mobile | Touch layout | Pass |\n| Offline | No assets | Pass |", "Verification table")),
        new(false, new("Agent", "reasoning", "I should check restart reset, wall kick, game over, narrow layout and report the limits of this simulated run accurately.", "Thinking")),
        new(false, new("Tool", "activity", "… run_command node --check game.js", "Run command")),
        new(false, new("Tool", "activity", "✓ run_command — Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", "Run command")),
        new(false, new("Agent", "text", "Checks considered: restart reset, wall kick, game over,", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual c", "Assistant")),
        new(true, new("Agent", "text", "Checks considered: restart reset, wall kick, game over, narrow layout. Syntax check passed; simulated manual checks cover play, pause, restart, and responsive layout.", "Assistant")),
        new(false, new("Agent", "text", "The single-file game has keyboard and touch co", "Assistant")),
        new(true, new("Agent", "text", "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best sco", "Assistant")),
        new(true, new("Agent", "text", "The single-file game has keyboard and touch controls, hold and ghost, scoring, saved best score, accessible status, and a responsive layout.", "Assistant")),
    };
    private static readonly double[] ScriptedDurations = {3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.958, 0.958, 1.28, 2.264, 3.196, 0.799, 0.932, 1.132, 0.679, 0.679, 0.906, 2.664, 0.866, 0.932, 1.118, 1.118, 1.493, 0.599, 0.599, 0.8, 0.866, 2.198, 0.999, 2.664, 2.198, 0.866, 1.265, 0.619, 0.619, 0.826, 0.0, 0.0, 0.0};

    private readonly Transcript transcriptPanel = new();
    private readonly Func<double> clock;
    private int nextStep;
    private bool simulationRunning;
    private bool retainHistory;
    private bool followLive;
    private double? nextStepAt;
    private bool quitRequested;

    private static int SimulationEventCount =>
        ScriptedSteps.Length + SimulationMessageTarget - ScriptedMessageCount;

    private static double SimulationStepIntervalSeconds =>
        SimulationDurationSeconds / (SimulationEventCount - 1);

    public AgentChat(Func<double>? clock = null)
    {
        this.clock = clock ?? MonotonicSeconds;
        transcript.SetChild(transcriptPanel);
        ResetSimulation();
    }

    public void StartSimulation()
    {
        if (nextStep >= SimulationEventCount)
        {
            if (retainHistory) nextStep = 0;
            else ResetSimulation();
        }
        if (nextStep == 0)
        {
            if (retainHistory) transcriptPanel.AppendEntry(InitialEntry);
            ApplyNextTimelineEvent();
        }
        _ = transcriptPanel.ScrollToBottom();
        followLive = true;
        if (nextStep >= SimulationEventCount)
        {
            FinishSimulation();
            return;
        }
        simulationRunning = true;
        nextStepAt = clock() + ScriptedDurations[nextStep - 1];
        SetStatus("Running");
    }

    public void PauseSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        SetStatus("Paused");
    }

    public void NextStep()
    {
        simulationRunning = false;
        nextStepAt = null;
        if (nextStep >= SimulationEventCount)
        {
            SetStatus("Scenario complete");
            return;
        }
        bool follow = followLive && AtBottom();
        ApplyNextTimelineEvent();
        if (follow) _ = transcriptPanel.ScrollToBottom();
        SetStatus(nextStep == SimulationEventCount ? "Scenario complete" : "Step applied");
    }

    public void ResetSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        transcriptPanel.ClearEntries();
        transcriptPanel.AppendEntry(InitialEntry);
        _ = transcriptPanel.ScrollToBottom();
        nextStep = 0;
        retainHistory = false;
        followLive = true;
        SetStatus("Ready");
    }

    public void LoadStressTranscript()
    {
        simulationRunning = false;
        nextStepAt = null;
        transcriptPanel.ReplaceEntries(Enumerable.Range(0, StressEntryCount).Select(StressEntry));
        _ = transcriptPanel.ScrollToTop();
        nextStep = 0;
        retainHistory = true;
        followLive = false;
        status.SetText($"Stress loaded · {MessageCountText(transcriptPanel.EntryCount())} · ~{StressTargetRows} rows");
    }

    public int EntryCount()
    {
        return transcriptPanel.EntryCount();
    }

    public override bool Tick()
    {
        if (!simulationRunning || nextStepAt is null)
        {
            return false;
        }
        double now = clock();
        bool applied = false;
        bool follow = followLive && AtBottom();
        while (now >= nextStepAt.Value && nextStep < SimulationEventCount)
        {
            ApplyNextTimelineEvent();
            if (nextStep < SimulationEventCount) nextStepAt += ScriptedDurations[nextStep - 1];
            applied = true;
        }
        if (!applied)
        {
            return false;
        }
        if (follow) _ = transcriptPanel.ScrollToBottom();
        if (nextStep >= SimulationEventCount)
        {
            FinishSimulation();
        }
        else
        {
            SetStatus("Running");
        }
        return true;
    }

    protected override void onStartButtonClick()
    {
        StartSimulation();
    }

    protected override void onPauseButtonClick()
    {
        PauseSimulation();
    }

    protected override void onNextStepButtonClick()
    {
        NextStep();
    }

    protected override void onResetButtonClick()
    {
        ResetSimulation();
    }

    protected override void onLoadStressButtonClick()
    {
        LoadStressTranscript();
    }

    protected override void onTopClick()
    {
        followLive = false;
        _ = transcriptPanel.ScrollToTop();
    }

    protected override void onEndClick()
    {
        followLive = true;
        _ = transcriptPanel.ScrollToBottom();
    }

    protected override void onQuitButtonClick()
    {
        quitRequested = true;
    }

    protected override bool shouldClose()
    {
        return quitRequested;
    }

    private void ApplyStep(ScriptedStep step)
    {
        if (step.ReplaceLast)
        {
            transcriptPanel.ReplaceLastEntry(step.Entry);
        }
        else
        {
            transcriptPanel.AppendEntry(step.Entry);
        }
    }

    private void ApplyNextTimelineEvent()
    {
        ApplyStep(ScriptedSteps[nextStep]);
        ++nextStep;
    }

    private bool AtBottom() =>
        transcriptPanel.ScrollView().ViewOffset == 0 &&
        transcriptPanel.ScrollView().ScrollOffset == 0;

    private void FinishSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        SetStatus("Scenario complete");
    }

    private void SetStatus(string prefix)
    {
        status.SetText(prefix + " · " + MessageCountText(transcriptPanel.EntryCount()));
    }

    private static string MessageCountText(int count)
    {
        return count + (count == 1 ? " message" : " messages");
    }

    private static double MonotonicSeconds()
    {
        return (double)Stopwatch.GetTimestamp() / Stopwatch.Frequency;
    }

    private static TranscriptEntry StressEntry(int index)
    {
        string body = StressBody(index, StressEntryLineCount(index));
        return (index % 8) switch
        {
            0 => new("You", "request", body, "SIMULATED STRESS DATA · no external action"),
            1 => new("Agent", "plan", body, "SIMULATED STRESS DATA · no external action"),
            2 => new("Tool", "search_code", body, "SIMULATED STRESS DATA · no external action"),
            3 => new("Agent", "code", body, "SIMULATED STRESS DATA · no external action"),
            4 => new("Tool", "diff", body, "SIMULATED STRESS DATA · no external action"),
            5 => new("Agent", "table", body, "SIMULATED STRESS DATA · no external action"),
            6 => new("Tool", "warning", body, "SIMULATED STRESS DATA · no external action"),
            _ => new("Agent", "result", body, "SIMULATED STRESS DATA · no external action"),
        };
    }

    private static int StressEntryLineCount(int index)
    {
        int ordinal = index + 1;
        if (ordinal % StressLongMessageInterval == 0)
        {
            return StressLongMessageRows;
        }
        if (ordinal % StressMediumMessageInterval == 0)
        {
            return StressMediumMessageRows;
        }
        return StressMinMessageRows + ((index * 17) % StressMessageRowVariants);
    }

    private static string StressBody(int index, int lineCount)
    {
        int variant = index % 8;
        int lineNumber = (index % 997) + 1;
        List<string> lines = new(lineCount);
        if (variant == 0)
        {
            string[] prompts =
            {
                "- preserve the documented API",
                "- compare every supported port",
                "- keep the reproduction offline",
                "- report focused validation evidence",
            };
            lines.Add($"Please investigate deterministic case {index}.");
            for (int offset = 0; offset < lineCount - 1; ++offset)
            {
                lines.Add(prompts[offset % prompts.Length]);
            }
        }
        else if (variant == 1)
        {
            string[] actions = { "inspect", "reproduce", "repair", "validate" };
            lines.Add($"## Plan for case {index}");
            for (int offset = 0; offset < lineCount - 1; ++offset)
            {
                lines.Add($"{offset + 1}. {actions[offset % actions.Length]} the shared invariant");
            }
        }
        else if (variant == 2)
        {
            lines.Add($"$ search_code case_{index}");
            for (int offset = 0; offset < lineCount - 2; ++offset)
            {
                lines.Add($"src/module_{offset % 7}.py:{lineNumber + offset}: simulated match");
            }
            lines.Add($"result: {lineCount - 2} matches");
        }
        else if (variant == 3)
        {
            lines.Add("```python");
            lines.Add($"def case_{index}():");
            for (int offset = 0; offset < lineCount - 4; ++offset)
            {
                lines.Add($"    value_{offset} = {index + offset}");
            }
            lines.Add("    return {\"ok\": True}");
            lines.Add("```");
        }
        else if (variant == 4)
        {
            lines.Add($"--- a/src/case_{index}.py");
            lines.Add($"+++ b/src/case_{index}.py");
            lines.Add("@@ -1,2 +1,2 @@");
            for (int offset = 0; lines.Count < lineCount; ++offset)
            {
                string prefix = offset % 2 == 0 ? "-" : "+";
                lines.Add($"{prefix}value_{offset} = {index + offset}");
            }
        }
        else if (variant == 5)
        {
            lines.Add("| Item | Value | State |");
            lines.Add("|:-----|------:|:-----:|");
            for (int offset = 0; lines.Count < lineCount; ++offset)
            {
                lines.Add($"| row {offset} | {index + offset} | pass |");
            }
        }
        else if (variant == 6)
        {
            lines.Add($"ERROR simulated_case_{index}");
            for (int offset = 0; offset < lineCount - 2; ++offset)
            {
                lines.Add($"  at worker_{offset % 5}.py:{lineNumber + offset}: expected stable state");
            }
            lines.Add("status: simulated failure; no test executed");
        }
        else
        {
            string[] results =
            {
                "- [x] focused regression",
                "- [x] 1:1 parity audit",
                "- [x] backward compatibility",
                "Unicode: ľščťžýáíé · 東京 · 🙂",
            };
            lines.Add($"### Result {index} ✓");
            for (int offset = 0; offset < lineCount - 1; ++offset)
            {
                lines.Add(results[offset % results.Length]);
            }
        }
        return string.Join("\n", lines);
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        if (args.Length > 0 && args[0] == "--logic-test")
        {
            double now = 0.0;
            AgentChat test = new(() => now);
            if (test.EntryCount() != 1 || test.status.Text != "Ready · 1 message")
            {
                throw new InvalidOperationException("agent chat initial state failed");
            }
            test.NextStep();
            test.StartSimulation();
            if (test.EntryCount() != 2 || test.status.Text != "Running · 2 messages")
            {
                throw new InvalidOperationException("agent chat timed start failed");
            }
            test.PauseSimulation();
            now += AgentChat.SimulationDurationSeconds;
            if (test.Tick() || test.EntryCount() != 2)
            {
                throw new InvalidOperationException("agent chat pause failed");
            }
            test.NextStep();
            test.StartSimulation();
            now += AgentChat.SimulationDurationSeconds;
            if (!test.Tick() ||
                test.EntryCount() != AgentChat.SimulationMessageTarget ||
                test.status.Text != "Scenario complete · 100 messages")
            {
                throw new InvalidOperationException("agent chat scenario failed");
            }
            return 0;
        }
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            AgentChat app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
