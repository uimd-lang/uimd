# Control Interaction Parity

This file defines the expected generated UI control behavior shared by the
Python and C++ runtimes.

## Global Runtime

- Generated terminal apps run in alternate screen mode.
- Runtime enables mouse tracking, SGR mouse coordinates, bracketed paste, and
  modified-key reporting where the terminal supports it.
- `Ctrl+C` closes the app globally, before app-specific key handlers.
- `Ctrl+C` must work both as raw ETX (`0x03`) and as a modified-key CSI
  sequence when modify-key reporting is enabled.
- Bracketed paste events insert the pasted text into the focused editable
  control.
- Copied text is stored in the runtime clipboard and copied to the system
  clipboard on a best-effort basis (`pbcopy`, `wl-copy`, `xclip`, or `xsel`).
- `Cmd+C` copies selected focused text, or the focused text value when there is
  no active selection.
- `Cmd+V` pastes from the runtime clipboard when the terminal reports it as a
  modified key event; native terminal bracketed paste remains the primary paste
  path.
- `Esc` does not close the app in navigation mode.
- In edit mode, `Esc` immediately discards edits and exits edit mode.
- In edit mode, `Enter` commits edits and exits edit mode, except multiline
  insertion uses `Alt+Enter`.
- `Tab` and `Shift+Tab` cycle focus in navigation mode.
- Arrow keys move focus spatially in navigation mode.

## TextInput

- `Enter` in navigation mode enters edit mode.
- Text is inserted at the cursor in edit mode.
- `Left`, `Right`, `Home`, `End`, `Up`, and `Down` move the cursor.
- `Shift+Left`, `Shift+Right`, `Shift+Up`, and `Shift+Down` extend the text
  selection from the selection anchor.
- Typing over a selection replaces it.
- `Backspace` deletes the selection or the character before the cursor.
- `Alt+Enter` inserts a newline.
- Mouse press enters edit mode and moves the cursor to the clicked character.
- Mouse drag extends the selection from the press anchor.
- Releasing a non-empty mouse selection copies it to the runtime/system
  clipboard and shows a transient `Copied to clipboard` notification.

## NumberInput

- `Enter` in navigation mode enters edit mode.
- A visible cursor is shown in edit mode.
- The first typed numeric character replaces the initial display value.
- Empty edit buffers are allowed while editing and commit as `0`.
- Invalid numeric edits roll back to the original value on commit.
- `Esc` rolls back to the value captured when edit mode was entered.
- Mouse press enters edit mode and moves the cursor to the clicked column.
- Leaving the control by clicking another element commits the current edit.

## Button

- `Enter` in navigation mode activates the button.
- Mouse press activates the button.
- Focused buttons render `[` and `]` one character inside the button when there
  is enough width, without covering the title.

## CheckBox

- `Enter` or Space toggles the value.
- Mouse press toggles the value.
- Checked and unchecked indicators use their dedicated style states.

## ComboBox

- `Enter` in navigation mode opens edit/dropdown mode.
- `Up` and `Down` change the selected item in edit mode.
- `Enter` commits the highlighted item and exits edit mode.
- `Esc` rolls back to the item captured when edit mode was entered.
- Mouse press on a closed combobox enters edit mode.
- Mouse press on an open option selects the option and exits edit mode.
- The `v` indicator is pinned to the right edge of the closed row.

## ListBox

- `Enter` in navigation mode enters edit mode.
- `Up` and `Down` change selection in edit mode and keep it visible.
- `Esc` rolls back to the item captured when edit mode was entered.
- Mouse press selects the clicked visible row and enters edit mode.
- Mouse wheel and trackpad wheel scroll the visible list without requiring the
  selected item to remain visible.
- If there are hidden rows above or below the viewport, `^` and `v` indicators
  render at the right edge of the first or last visible row.

## Label

- Labels are not focusable by default.
- Selectable label text is controlled by `user-select: text`.
- Mouse drag over selectable labels creates a text selection.
