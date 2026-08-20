package uimd;

public final class ScopedRenderTimeOverride implements AutoCloseable
{
    private final Long previous;
    private boolean restored;

    public ScopedRenderTimeOverride(Long timeMilliseconds)
    {
        previous = RenderHelpers.renderTimeOverrideMs();
        RenderHelpers.setRenderTimeOverrideMs(timeMilliseconds);
    }

    public void restore()
    {
        if (restored)
        {
            return;
        }
        RenderHelpers.setRenderTimeOverrideMs(previous);
        restored = true;
    }

    @Override
    public void close()
    {
        restore();
    }
}
