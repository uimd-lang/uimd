package uimd;

public final class ScopedImageCellBackgroundRendering implements AutoCloseable
{
    private boolean active = true;

    public ScopedImageCellBackgroundRendering()
    {
        Image.pushCellBackgroundRendering();
    }

    @Override
    public void close()
    {
        if (active)
        {
            active = false;
            Image.popCellBackgroundRendering();
        }
    }
}
