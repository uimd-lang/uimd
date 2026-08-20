package uimd;

public final class ScopedElementParentBackground implements AutoCloseable
{
    private final Color previous;

    public ScopedElementParentBackground(Color background)
    {
        previous = Element.renderingParentBackground();
        Element.setRenderingParentBackground(background);
    }

    @Override
    public void close()
    {
        Element.setRenderingParentBackground(previous);
    }
}
