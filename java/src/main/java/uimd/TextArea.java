package uimd;

public final class TextArea extends TextInput
{
    public TextArea(String name)
    {
        this(name, "", 0);
    }

    public TextArea(String name, String value, int maxLength)
    {
        super(name, value, maxLength, true);
        setCommitMode(COMMIT_MODE_LEAVE);
    }
}
