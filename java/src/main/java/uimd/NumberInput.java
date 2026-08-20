package uimd;

import java.math.BigDecimal;
import java.util.List;

public final class NumberInput extends Element
{
    private double value;
    private final double stepSize;
    private String editText = "";
    private int editCursor;
    private boolean editing;
    private double editOriginalValue;
    private boolean replaceOnFirstTextInput;

    public NumberInput(String name)
    {
        this(name, 0.0, 1.0);
    }

    public NumberInput(String name, double value, double stepSize)
    {
        super(name);
        this.value = value;
        this.stepSize = stepSize;
    }

    public double value()
    {
        return value;
    }

    public void setValue(double nextValue)
    {
        boolean wasEditing = editing;
        value = nextValue;
        editOriginalValue = nextValue;
        editText = wasEditing ? displayValue() : "";
        editCursor = wasEditing ? editText.length() : 0;
        editing = wasEditing;
        replaceOnFirstTextInput = false;
    }

    public double stepSize()
    {
        return stepSize;
    }

    public String displayText()
    {
        return displayValue();
    }

    public void beginEdit()
    {
        if (editing)
        {
            return;
        }
        editOriginalValue = value;
        editText = displayValue();
        editCursor = editText.length();
        editing = true;
        replaceOnFirstTextInput = value == 0.0;
    }

    public void cancelEdit()
    {
        if (!editing)
        {
            return;
        }
        value = editOriginalValue;
        clearEditState();
    }

    public void commitEdit()
    {
        ensureEditText();
        if (editText.isEmpty())
        {
            value = 0.0;
        }
        else
        {
            try
            {
                value = Double.parseDouble(editText);
            }
            catch (NumberFormatException exception)
            {
                value = editOriginalValue;
            }
        }
        clearEditState();
    }

    public void setEditCursor(int cursor)
    {
        setEditCursor(cursor, false);
    }

    public void setEditCursor(int cursor, boolean preserveReplaceOnFirstTextInput)
    {
        ensureEditText();
        editCursor = clamp(cursor, 0, editText.length());
        if (!preserveReplaceOnFirstTextInput)
        {
            replaceOnFirstTextInput = false;
        }
    }

    public void setEditText(String text)
    {
        editText = text == null ? "" : text;
        editCursor = editText.length();
        editing = true;
        replaceOnFirstTextInput = false;
    }

    @Override
    public boolean handleKey(String key)
    {
        ensureEditText();
        if ("Up".equals(key))
        {
            value += stepSize;
            editText = displayValue();
            editCursor = editText.length();
            replaceOnFirstTextInput = false;
            return true;
        }
        if ("Down".equals(key))
        {
            value -= stepSize;
            editText = displayValue();
            editCursor = editText.length();
            replaceOnFirstTextInput = false;
            return true;
        }
        if ("Left".equals(key))
        {
            setEditCursor(editCursor - 1);
            return true;
        }
        if ("Right".equals(key))
        {
            setEditCursor(editCursor + 1);
            return true;
        }
        if ("Home".equals(key))
        {
            setEditCursor(0);
            return true;
        }
        if ("End".equals(key))
        {
            setEditCursor(editText.length());
            return true;
        }
        if ("Backspace".equals(key))
        {
            replaceOnFirstTextInput = false;
            if (editCursor > 0)
            {
                editText = editText.substring(0, editCursor - 1) + editText.substring(editCursor);
                --editCursor;
            }
            return true;
        }
        if ("Delete".equals(key))
        {
            replaceOnFirstTextInput = false;
            if (editCursor < editText.length())
            {
                editText = editText.substring(0, editCursor) + editText.substring(editCursor + 1);
            }
            return true;
        }
        if ("Enter".equals(key))
        {
            commitEdit();
            return true;
        }
        if (key != null && key.length() == 1
            && (Character.isDigit(key.charAt(0)) || ".".equals(key) || "-".equals(key)))
        {
            if (replaceOnFirstTextInput)
            {
                editText = "";
                editCursor = 0;
                replaceOnFirstTextInput = false;
            }
            editText = editText.substring(0, editCursor) + key + editText.substring(editCursor);
            ++editCursor;
            return true;
        }
        return false;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        if (effectiveState.editMode())
        {
            ensureEditText();
        }
        String text = effectiveState.editMode() ? editText : displayValue();
        int width = Math.max(1, size.width());
        Style style = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        List<List<TerminalCell>> rendered = RenderHelpers.renderPlainText(text, width, 1, style);
        if (effectiveState.editMode())
        {
            Style cursor = style.copy();
            if (cursorStyle() != null)
            {
                cursor.merge(cursorStyle());
            }
            if (replaceOnFirstTextInput)
            {
                for (int col = 0; col < Math.min(editText.length(), width); ++col)
                {
                    applyStyle(rendered.get(0).get(col), cursor);
                }
            }
            else
            {
                int cursorCol = clamp(editCursor, 0, width - 1);
                applyStyle(rendered.get(0).get(cursorCol), cursor);
            }
        }
        return rendered;
    }

    public String displayValue()
    {
        if (!Double.isFinite(value))
        {
            return Double.toString(value);
        }
        return BigDecimal.valueOf(value).stripTrailingZeros().toPlainString();
    }

    private void ensureEditText()
    {
        if (!editing)
        {
            beginEdit();
        }
    }

    private void clearEditState()
    {
        editText = "";
        editCursor = 0;
        editing = false;
        replaceOnFirstTextInput = false;
    }

    private static void applyStyle(TerminalCell cell, Style style)
    {
        cell.setForeground(style.color());
        cell.setBackground(style.background());
    }

    private static int clamp(int value, int minimum, int maximum)
    {
        return Math.max(minimum, Math.min(maximum, value));
    }
}
