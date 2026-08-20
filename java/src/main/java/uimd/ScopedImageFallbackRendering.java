package uimd;

public final class ScopedImageFallbackRendering implements AutoCloseable
{
    private boolean active = true;

    public ScopedImageFallbackRendering()
    {
        Image.pushFallbackRendering();
    }

    @Override
    public void close()
    {
        if (active)
        {
            active = false;
            Image.popFallbackRendering();
        }
    }
}
