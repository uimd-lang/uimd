// Auto-generated UI code for task_board - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class TaskBoardUI extends GeneratedWindowBase
{
    public Label title;
    public Button quit_btn;
    public ReusableElement filters;
    public Button new_task_btn;
    public Button mark_all_done_btn;
    public Button clear_board_btn;
    public ReusableElement board;
    public Label status;

    public TaskBoardUI()
    {
        super("Task Board");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "TaskBoard",
            "# Task Board\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.\"\ntags: [example, mcp]\n```\n\n## Definition\n\n```yaml\nuses:\n  - task_filters/task_filters\n  - task_list/task_list\n  - task_dialog/task_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Task Board\"\n  description: \"Application title.\"\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the task list example.\"\n\nfilters:\n  type: task_filters\n  cpp-class: task_board_example::TaskFilters\n  cpp-header: \"task_filters/task_filters.hpp\"\n  description: \"Reusable task filter panel.\"\n\nnew_task_btn:\n  type: button\n  title: New task\n  description: \"Open a dialog for creating a task.\"\n\nmark_all_done_btn:\n  type: button\n  title: Mark all done\n  description: \"Ask before marking every task as done.\"\n\nclear_board_btn:\n  type: button\n  title: Clear board\n  description: \"Ask before deleting every task.\"\n\nboard:\n  type: task_list\n  cpp-class: task_board_example::TaskList\n  cpp-header: \"task_list/task_list.hpp\"\n  description: \"Scrollable list of interactive task rows.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Current board status.\"\n```\n\n## Tools\n\n```yaml\nquery_tasks:\n  description: \"Return tasks matching optional title, status, assignee, priority, and done filters.\"\n  input:\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    done: bool\n  output:\n    tasks:\n      - id: string\n        title: string\n        status: string\n        assignee: string\n        priority: string\n        description: string\n        done: bool\n\nget_task:\n  description: \"Return one task by id.\"\n  input:\n    id: string\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\nupdate_task:\n  description: \"Update a task by id and return the updated task.\"\n  input:\n    id: string\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    description: string\n    done: bool\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\ndelete_task:\n  description: \"Delete a task by id and return whether it was removed.\"\n  input:\n    id: string\n    required: [id]\n  output:\n    deleted: bool\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@filter_cell:\n  background: \"#15171c\"\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@board_cell:\n  background: \"#030712\"\n@filters:\n  background: \"#15171c\"\n@board:\n  background: \"#030712\"\n  focus-background: \"#07111f\"\n  edit-background: \"#0f172a\"\n  scope-dim-background: \"#0000004d\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@new_task_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@mark_all_done_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@clear_board_btn:\n  background: \"#711212\"\n  focus-background: \"#b91c1c\"\n@status:\n  background: \"#111827\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**................ ..................................... quit_btn.... |\n+-filter_cell-**---------------------------------------------------------------+\n| filters..**................................................................. |\n| *........................................................................... |\n| *........................................................................... |\n| ............................................................................ |\n| ............................................................................ |\n| ............................................................................ |\n+-actions-24----------------+-board_cell-**------------------------------------+\n| new_task_btn..**......... | board..**....................................... |\n|                           | ................................................ |\n* mark_all_done_btn..**.... | ................................................ |\n*                           | ................................................ |\n| clear_board_btn..**...... | ................................................ |\n|                           | *............................................... |\n| status..**............... | *............................................... |\n| ......................... | ................................................ |\n| ......................... | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n+---------------------------+--------------------------------------------------+\n```\n",
            "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.",
            List.of(
            new GeneratedElementMetadata("title", "Application title.", false),
            new GeneratedElementMetadata("quit_btn", "Close the task list example.", true),
            new GeneratedElementMetadata("filters", "Reusable task filter panel.", true),
            new GeneratedElementMetadata("new_task_btn", "Open a dialog for creating a task.", true),
            new GeneratedElementMetadata("mark_all_done_btn", "Ask before marking every task as done.", true),
            new GeneratedElementMetadata("clear_board_btn", "Ask before deleting every task.", true),
            new GeneratedElementMetadata("board", "Scrollable list of interactive task rows.", true),
            new GeneratedElementMetadata("status", "Current board status.", true)));
        setMcpAppTools(List.of(
            new GeneratedAppToolMetadata("delete_task", "Delete a task by id and return whether it was removed.", "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"}},\"required\":[\"id\"]}", "{\"type\":\"object\",\"properties\":{\"deleted\":{\"type\":\"boolean\"}}}"),
            new GeneratedAppToolMetadata("get_task", "Return one task by id.", "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"}},\"required\":[\"id\"]}", "{\"type\":\"object\",\"properties\":{\"task\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}"),
            new GeneratedAppToolMetadata("query_tasks", "Return tasks matching optional title, status, assignee, priority, and done filters.", "{\"type\":\"object\",\"properties\":{\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}", "{\"type\":\"object\",\"properties\":{\"tasks\":{\"type\":\"array\",\"items\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}}"),
            new GeneratedAppToolMetadata("update_task", "Update a task by id and return the updated task.", "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}},\"required\":[\"id\"]}", "{\"type\":\"object\",\"properties\":{\"task\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}")));
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        title = addElement(new Label("title", "Task Board"));
        title.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center"));
        title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        quit_btn = addElement(new Button("quit_btn", "Quit"));
        quit_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        quit_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        quit_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        filters = addElement(new ReusableElement("filters", "task_filters"));
        filters.setChild(new TaskFiltersUI());
        filters.setStyle(Style.fromProperties("background", "#15171c"));
        filters.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        filters.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        filters.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        new_task_btn = addElement(new Button("new_task_btn", "New task"));
        new_task_btn.setStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"));
        new_task_btn.setFocusStyle(Style.fromProperties("background", "#16a34a", "color", "#ffffff"));
        new_task_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        mark_all_done_btn = addElement(new Button("mark_all_done_btn", "Mark all done"));
        mark_all_done_btn.setStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"));
        mark_all_done_btn.setFocusStyle(Style.fromProperties("background", "#16a34a", "color", "#ffffff"));
        mark_all_done_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        clear_board_btn = addElement(new Button("clear_board_btn", "Clear board"));
        clear_board_btn.setStyle(Style.fromProperties("background", "#711212", "color", "#e2e8f0"));
        clear_board_btn.setFocusStyle(Style.fromProperties("background", "#b91c1c", "color", "#ffffff"));
        clear_board_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        board = addElement(new ReusableElement("board", "task_list"));
        board.setChild(new TaskListUI());
        board.setStyle(Style.fromProperties("background", "#030712", "scope-dim-background", "#0000004d"));
        board.setFocusStyle(Style.fromProperties("background", "#07111f"));
        board.setEditStyle(Style.fromProperties("background", "#0f172a"));
        board.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        status = addElement(new Label("status", "Ready"));
        status.setStyle(Style.fromProperties("background", "#111827", "color", "#93c5fd"));
        status.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
    }

    private static TextGradient makeTextGradient(
        int intervalMs,
        int step,
        int segmentSize,
        String... colors)
    {
        TextGradient gradient = new TextGradient();
        gradient.setIntervalMs(intervalMs);
        gradient.setStep(step);
        gradient.setSegmentSize(segmentSize);
        for (String color : colors)
        {
            gradient.colors().add(new Color(color));
        }
        return gradient;
    }

    private static Style styleWithGradients(
        Style style,
        TextGradient colorGradient,
        TextGradient backgroundGradient)
    {
        style.setTextColorGradient(colorGradient);
        style.setTextBackgroundGradient(backgroundGradient);
        return style;
    }

    private static List<GeneratedLayoutEntry> buildLayout()
    {
        return List.of(
            new GeneratedLayoutEntry()
                .setName("board")
                .setType("task_list")
                .setCellName("board_cell")
                .setRelative(new Rect(0, 1, 536870911, 536870911))
                .setSourceCell(new Rect(9, 28, 50, 12))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(12))
                .setCharsSize(new Size(48, 12))
                .setCellCharsSize(new Size(50, 12))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712"))
                .setElementStyle(Style.fromProperties("background", "#030712", "scope-dim-background", "#0000004d")),
            new GeneratedLayoutEntry()
                .setName("filters")
                .setType("task_filters")
                .setCellName("filter_cell")
                .setRelative(new Rect(0, 1, 536870911, 536870911))
                .setSourceCell(new Rect(2, 0, 78, 6))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(6))
                .setCharsSize(new Size(76, 6))
                .setCellCharsSize(new Size(78, 6))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#15171c"))
                .setElementStyle(Style.fromProperties("background", "#15171c")),
            new GeneratedLayoutEntry()
                .setName("new_task_btn")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(9, 0, 27, 12))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.fixed(24))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(25, 1))
                .setCellCharsSize(new Size(27, 12))
                .setMarginRight(1)
                .setMarginBottom(11)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("title")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 78, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(25, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(52)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("quit_btn")
                .setType("button")
                .setCellName("header")
                .setRelative(new Rect(0, 65, 12, 1))
                .setSourceCell(new Rect(0, 0, 78, 1))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("mark_all_done_btn")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(2, 1, 536870911, 1))
                .setSourceCell(new Rect(9, 0, 27, 12))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.fixed(24))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(25, 1))
                .setCellCharsSize(new Size(27, 12))
                .setMarginRight(1)
                .setMarginBottom(9)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("clear_board_btn")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(4, 1, 536870911, 1))
                .setSourceCell(new Rect(9, 0, 27, 12))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.fixed(24))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(25, 1))
                .setCellCharsSize(new Size(27, 12))
                .setMarginRight(1)
                .setMarginBottom(7)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#711212", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("status")
                .setType("label")
                .setCellName("actions")
                .setRelative(new Rect(6, 1, 536870911, 3))
                .setSourceCell(new Rect(9, 0, 27, 12))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.fixed(24))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(25, 3))
                .setCellCharsSize(new Size(27, 12))
                .setMarginRight(1)
                .setMarginBottom(3)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#111827", "color", "#93c5fd"))
        );
    }

    protected void onQuitBtnClick()
    {
    }

    protected void onNewTaskBtnClick()
    {
    }

    protected void onMarkAllDoneBtnClick()
    {
    }

    protected void onClearBoardBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("quit_btn".equals(name))
        {
            onQuitBtnClick();
            return true;
        }
        if ("new_task_btn".equals(name))
        {
            onNewTaskBtnClick();
            return true;
        }
        if ("mark_all_done_btn".equals(name))
        {
            onMarkAllDoneBtnClick();
            return true;
        }
        if ("clear_board_btn".equals(name))
        {
            onClearBoardBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        return false;
    }
}
