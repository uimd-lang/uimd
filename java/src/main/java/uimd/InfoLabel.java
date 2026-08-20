package uimd;

import java.util.List;

public final class InfoLabel extends Label
{
    private static final int GRADIENT_INTERVAL_MS = 70;
    private static final int GRADIENT_STEP = 1;
    private static final int GRADIENT_SEGMENT_SIZE = 1;

    public InfoLabel(String name)
    {
        super(name);
    }

    public InfoLabel(String name, String text)
    {
        super(name, text);
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        Style style = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        if (style.textColorGradient() == null)
        {
            style.setTextColorGradient(defaultGradient());
        }
        int width = Math.max(1, size.width() > 0 ? size.width() : text().length());
        int height = Math.max(1, size.height() > 0 ? size.height() : 1);
        return RenderHelpers.renderPlainText(text(), width, height, style);
    }

    private static TextGradient defaultGradient()
    {
        TextGradient gradient = new TextGradient();
        gradient.setIntervalMs(GRADIENT_INTERVAL_MS);
        gradient.setStep(GRADIENT_STEP);
        gradient.setSegmentSize(GRADIENT_SEGMENT_SIZE);
        gradient.colors().addAll(List.of(
            new Color("#ffffff66"),
            new Color("#ffffff88"),
            new Color("#ffffffaa"),
            new Color("#ffffffcc"),
            new Color("#ffffffff"),
            new Color("#ffffffcc"),
            new Color("#ffffffaa"),
            new Color("#ffffff88"),
            new Color("#ffffff66")));
        return gradient;
    }
}
