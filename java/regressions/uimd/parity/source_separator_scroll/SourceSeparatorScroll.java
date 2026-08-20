import uimd.GeneratedWindowRuntime;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;

public final class SourceSeparatorScroll extends SourceSeparatorScrollUI
{
    public SourceSeparatorScroll()
    {
        clearChildren();
        SourceSeparatorRowUI row = new SourceSeparatorRowUI();
        row.setMode("expand_width");
        Size natural = GeneratedWindowRuntime.generatedWindowContentSize(row);
        ReusableElement reusable = new ReusableElement("row", row);
        reusable.setFrame(new Rect(0, 0, natural.width(), natural.height()));
        addChild(reusable);
    }

    public static void main(String[] arguments)
    {
        System.exit(GeneratedWindowRuntime.runGeneratedWindow(
            new SourceSeparatorScroll(),
            arguments));
    }
}
