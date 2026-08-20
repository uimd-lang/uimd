import uimd.GeneratedWindowRuntime;
import uimd.ElementRenderState;
import uimd.Size;

public final class ImageGallery extends ImageGalleryUI
{
    private static final int EXPECTED_LAYOUT_ENTRIES = 18;
    private static final int SMOKE_IMAGE_WIDTH = 8;
    private static final int SMOKE_IMAGE_HEIGHT = 10;

    private boolean closeRequested;

    @Override
    protected void onCloseBtnClick()
    {
        closeRequested = true;
    }

    @Override
    protected boolean shouldClose()
    {
        return closeRequested;
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() -> run(arguments));
        System.exit(exitCode);
    }

    private static int run(String[] arguments)
    {
        if (arguments.length > 0
            && ("--smoke".equals(arguments[0]) || "--logic-test".equals(arguments[0])))
        {
            smoke();
            return 0;
        }
        ImageGallery application = new ImageGallery();
        return GeneratedWindowRuntime.runGeneratedWindow(
            application,
            application.runtimeOptions(),
            arguments);
    }

    private static void smoke()
    {
        ImageGallery window = new ImageGallery();
        require("Image Gallery".equals(window.title()), "unexpected title");
        require(window.generatedLayout().size() == EXPECTED_LAYOUT_ENTRIES, "unexpected layout size");
        require(window.camera_fb != null, "camera_fb missing");
        require(window.astro_fb != null, "astro_fb missing");
        require(window.coffee_fb != null, "coffee_fb missing");
        require(window.coins_fb != null, "coins_fb missing");
        require(window.camera_ctn != null, "camera_ctn missing");
        require(window.astro_ctn != null, "astro_ctn missing");
        require(window.coffee_ctn != null, "coffee_ctn missing");
        require(window.coins_ctn != null, "coins_ctn missing");
        require(window.camera_str != null, "camera_str missing");
        require(window.astro_str != null, "astro_str missing");
        require(window.coffee_str != null, "coffee_str missing");
        require(window.coins_str != null, "coins_str missing");
        require(window.coins_fb.render(
            new Size(SMOKE_IMAGE_WIDTH, SMOKE_IMAGE_HEIGHT),
            new ElementRenderState()).size() == SMOKE_IMAGE_HEIGHT, "unexpected fallback height");
    }

    private static void require(boolean condition, String message)
    {
        if (!condition)
        {
            throw new IllegalStateException(message);
        }
    }
}
