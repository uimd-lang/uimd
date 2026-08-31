package uimd;

public record KeyEvent(String key, String focusedElementId, boolean editMode)
{
    public KeyEvent
    {
        key = key == null ? "" : key;
        focusedElementId = focusedElementId == null ? "" : focusedElementId;
    }
}
