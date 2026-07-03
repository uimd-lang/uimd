# Tables

This document focuses on MessageTable rendering. The viewer parses simple
Markdown tables and renders them as terminal tables inside the scroll view.

## Runtime elements

The table below is parsed from Markdown and rendered by MessageTable.

| Element | Runtime role | Interactive |
|:--------|:-------------|------------:|
| Label | Static selectable text | no |
| SpanLabel | Per-character colored text | no |
| TextInput | Editable text | yes |
| NumberInput | Numeric text input | yes |
| CheckBox | Boolean toggle | yes |
| Button | Action trigger | yes |
| ComboBox | Single value picker | yes |
| ListBox | Selection list | yes |
| UIScrollView | Scroll container | yes |
| MessageTable | Markdown table renderer | no |

## Example dataset

Tables are useful for compact summaries in terminal layouts. A table block can
be mixed with paragraphs and code blocks in the same scroll view.

| File | Demonstrates | Notes |
|:-----|:-------------|:------|
| overview.markdown | Long document | Scrollable paragraphs |
| styles.markdown | Styled blocks | Selection and spans |
| tables.markdown | MessageTable | Multiple tables |
| code.markdown | Code block | Syntax-colored spans |

## Contact-like data

This table resembles data that could be displayed by the contacts manager
example. It is not editable here; it is rendered as read-only Markdown content.

| Name | Role | City | Status |
|:-----|:-----|:-----|:-------|
| Anna Kovac | Designer | Bratislava | active |
| Martin Hrasko | Backend developer | Kosice | active |
| Lucia Benda | Product manager | Prague | invited |
| Peter Novak | Support | Zilina | archived |
| Eva Urbanova | QA engineer | Trnava | active |
| Tomas Barta | DevOps | Brno | active |

## Alignment check

The separator row controls alignment. Left, center, and right alignment should
be visible in the rendered output.

| Metric | Owner | Value |
|:-------|:-----:|------:|
| Open tasks | Team A | 12 |
| Closed tasks | Team A | 48 |
| Warnings | Team B | 3 |
| Errors | Team B | 0 |
| Runtime widgets | Core | 11 |

## Narrow columns

Small tables should still render cleanly. The table should expand to the
available element width, while cell content keeps its alignment.

| Key | Value |
|:----|------:|
| a | 1 |
| b | 2 |
| c | 3 |
| d | 4 |

## Wider text

This paragraph appears between tables and verifies that the document view can
continue rendering normal content after a table block. It should not overlap
the bottom border of the previous table.

| Scenario | Expected behavior | Manual check |
|:---------|:------------------|:-------------|
| First render | table appears with header and rows | no clipped rows |
| Window resize | table width follows viewport | columns remain visible |
| Scroll down | table moves as one block | no duplicated borders |
| Scroll up | previous content returns | status line stays fixed |
| Switch document | new blocks replace old ones | list focus remains |

## Table block sizing

The table block reports its natural height based on the number of table rows.
That height must include the header line, separator line, all data rows, and the
bottom border.

If this measurement is wrong, the scroll view can clip the table or show empty
space where content should be. This document deliberately includes several
tables to make such problems visible.

## Release checklist

This table is wider and placed late in the document. It verifies that table
rendering remains readable after multiple previous table blocks.

| Step | Responsible area | Result |
|:-----|:-----------------|:------:|
| Parse source file | markdown_viewer.py | pass |
| Build block list | document_view.py | pass |
| Measure natural height | table_block.py | pass |
| Render borders | MessageTable | pass |
| Preserve focus | ListBox event handling | pass |
| Keep status fixed | main window layout | pass |

The paragraph after the checklist is just as important as the table. It checks
that normal content resumes below a late table and that the scroll view can
still reach the final section without clipping.

## Final comparison

The next table is small but placed near the end of the document. It helps check
that bottom scrolling still works after several earlier table blocks.

| Area | OK |
|:-----|:--:|
| parsing | yes |
| sizing | yes |
| rendering | yes |
| scrolling | yes |

## End

The end of the table document should only be visible after scrolling through
multiple tables. If all tables are visible at once, the terminal is too tall for
this manual test.
