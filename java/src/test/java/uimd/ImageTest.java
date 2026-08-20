package uimd;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

import javax.imageio.ImageIO;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

final class ImageTest
{
    private static final int DEFAULT_CELL_PIXEL_WIDTH = 8;
    private static final int DEFAULT_CELL_PIXEL_HEIGHT = 16;
    private static final int OPAQUE_RED = 0xffff0000;
    private static final int OPAQUE_GREEN = 0xff00ff00;
    private static final int OPAQUE_BLUE = 0xff0000ff;
    private static final int HALF_ALPHA_RED = 0x80ff0000;
    private static final int GRAYSCALE_SAMPLE = 200;
    private static final int TGA_HEADER_BYTES = 18;
    private static final int TGA_PIXEL_BYTES = 3;
    private static final int TGA_IMAGE_TYPE_OFFSET = 2;
    private static final int TGA_DIMENSIONS_OFFSET = 12;
    private static final int TGA_PIXEL_DEPTH = 24;
    private static final int TGA_TOP_ORIGIN = 0x20;
    private static final int TGA_TRUE_COLOR_IMAGE = 2;

    @TempDir
    Path temporaryDirectory;

    @BeforeEach
    void resetCellPixelGeometry()
    {
        Image.setTerminalCellPixels(new Size(DEFAULT_CELL_PIXEL_WIDTH, DEFAULT_CELL_PIXEL_HEIGHT));
    }

    @Test
    void missingSourceRendersOneCenteredPlaceholderGlyphPerCell()
    {
        Image image = new Image("picture", "", "", "contain", "fallback", "center", "middle");

        List<List<TerminalCell>> rendered = image.render(new Size(5, 1), new ElementRenderState());

        assertEquals(List.of("image"), RenderHelpers.renderedText(rendered));
    }

    @Test
    void canonicalImageRuntimeHelpersExposeThePureJavaSixelAdapter()
    {
        final int cellWidth = 9;
        final int cellHeight = 18;
        Image.setImageTerminalCellPixels(new Size(cellWidth, cellHeight));
        Image.requireSixelForImageRendering();
        assertFalse(Image.imageModeNeedsSixelFallbackWarning("fallback"));

        Image image = new Image("missing");
        ImageRenderInfo info = image.renderInfo(new Size(1, 1), new ElementRenderState());
        assertEquals(cellWidth, info.cellPixelWidth());
        assertEquals(cellHeight, info.cellPixelHeight());
    }

    @Test
    void cellBackgroundUsesAreaSamplingAndAlphaComposition() throws IOException
    {
        Path averagedPath = temporaryDirectory.resolve("averaged.png");
        BufferedImage averaged = new BufferedImage(2, 1, BufferedImage.TYPE_INT_ARGB);
        averaged.setRGB(0, 0, OPAQUE_RED);
        averaged.setRGB(1, 0, OPAQUE_BLUE);
        assertTrue(ImageIO.write(averaged, "png", averagedPath.toFile()));

        Image averagedImage = new Image(
            "averaged",
            averagedPath.toString(),
            "",
            "stretch",
            "fallback",
            "center",
            "middle");
        List<List<TerminalCell>> averagedCells = renderWithCellBackground(averagedImage, new Size(1, 1));
        assertEquals(new Color("#800080"), averagedCells.get(0).get(0).foreground());

        Path alphaPath = temporaryDirectory.resolve("alpha.png");
        BufferedImage alpha = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB);
        alpha.setRGB(0, 0, HALF_ALPHA_RED);
        assertTrue(ImageIO.write(alpha, "png", alphaPath.toFile()));
        Image alphaImage = new Image(
            "alpha",
            alphaPath.toString(),
            "",
            "stretch",
            "fallback",
            "center",
            "middle");
        alphaImage.setStyle(Style.fromProperties("background", "#0000ff"));
        List<List<TerminalCell>> alphaCells = renderWithCellBackground(alphaImage, new Size(1, 1));
        assertEquals(new Color("#80007f"), alphaCells.get(0).get(0).foreground());
    }

    @Test
    void grayscalePngKeepsRawSamplesLikePillowAndStb() throws IOException
    {
        Path path = temporaryDirectory.resolve("grayscale.png");
        BufferedImage source = new BufferedImage(1, 1, BufferedImage.TYPE_BYTE_GRAY);
        source.getRaster().setSample(0, 0, 0, GRAYSCALE_SAMPLE);
        assertTrue(ImageIO.write(source, "png", path.toFile()));
        Image image = new Image("grayscale", path.toString(), "", "stretch", "fallback", "center", "middle");

        List<List<TerminalCell>> rendered = renderWithCellBackground(image, new Size(1, 1));
        ImageRenderInfo info = image.renderInfo(new Size(1, 1), new ElementRenderState());

        assertEquals(new Color("#c8c8c8"), rendered.get(0).get(0).foreground());
        assertEquals("#c0c0c0", info.sampleSignature().get(0));
    }

    @Test
    void renderInfoUsesPhysicalCellGeometryAndStableSamples() throws IOException
    {
        Path path = temporaryDirectory.resolve("geometry.png");
        BufferedImage source = new BufferedImage(3, 2, BufferedImage.TYPE_INT_ARGB);
        for (int y = 0; y < source.getHeight(); ++y)
        {
            for (int x = 0; x < source.getWidth(); ++x)
            {
                source.setRGB(x, y, x == 0 ? OPAQUE_RED : OPAQUE_BLUE);
            }
        }
        assertTrue(ImageIO.write(source, "png", path.toFile()));
        Image image = new Image("geometry", path.toString(), "", "contain", "fallback", "center", "middle");

        ImageRenderInfo info = image.renderInfo(new Size(10, 10), new ElementRenderState());

        assertTrue(info.sourceLoaded());
        assertEquals(3, info.sourceWidth());
        assertEquals(2, info.sourceHeight());
        assertEquals(DEFAULT_CELL_PIXEL_WIDTH, info.cellPixelWidth());
        assertEquals(DEFAULT_CELL_PIXEL_HEIGHT, info.cellPixelHeight());
        assertEquals(10, info.imageWidth());
        assertEquals(3, info.imageHeight());
        assertEquals(0, info.imageLeft());
        assertEquals(3, info.imageTop());
        assertEquals(9, info.sampleSignature().size());
        assertEquals("#c00000", info.sampleSignature().get(0));
        assertEquals("#0000c0", info.sampleSignature().get(2));
    }

    @Test
    void decodesUncompressedTopOriginTga() throws IOException
    {
        Path path = temporaryDirectory.resolve("sample.tga");
        ByteBuffer data = ByteBuffer
            .allocate(TGA_HEADER_BYTES + 2 * TGA_PIXEL_BYTES)
            .order(ByteOrder.LITTLE_ENDIAN);
        data.position(TGA_IMAGE_TYPE_OFFSET);
        data.put((byte) TGA_TRUE_COLOR_IMAGE);
        data.position(TGA_DIMENSIONS_OFFSET);
        data.putShort((short) 2);
        data.putShort((short) 1);
        data.put((byte) TGA_PIXEL_DEPTH);
        data.put((byte) TGA_TOP_ORIGIN);
        data.position(TGA_HEADER_BYTES);
        data.put((byte) 0).put((byte) 0).put((byte) 255);
        data.put((byte) 0).put((byte) 255).put((byte) 0);
        Files.write(path, data.array());

        Image image = new Image("tga", path.toString(), "", "stretch", "fallback", "center", "middle");
        ImageRenderInfo info = image.renderInfo(new Size(2, 1), new ElementRenderState());
        List<List<TerminalCell>> rendered = renderWithCellBackground(image, new Size(2, 1));

        assertTrue(info.sourceLoaded());
        assertEquals(2, info.sourceWidth());
        assertEquals(1, info.sourceHeight());
        assertEquals(new Color("#ff0000"), rendered.get(0).get(0).foreground());
        assertEquals(new Color("#00ff00"), rendered.get(0).get(1).foreground());
    }

    @Test
    void sixelModeWritesOneRawAnchorPerVisibleCellRow() throws IOException
    {
        assumeTrue("1".equals(System.getenv("UIMD_FORCE_SIXEL")));
        Image.setTerminalCellPixels(new Size(1, 6));
        Path path = temporaryDirectory.resolve("sixel.png");
        BufferedImage source = new BufferedImage(2, 2, BufferedImage.TYPE_INT_ARGB);
        source.setRGB(0, 0, OPAQUE_RED);
        source.setRGB(1, 0, OPAQUE_GREEN);
        source.setRGB(0, 1, OPAQUE_BLUE);
        source.setRGB(1, 1, OPAQUE_RED);
        assertTrue(ImageIO.write(source, "png", path.toFile()));
        Image image = new Image("sixel", path.toString(), "", "stretch", "sixel", "center", "middle");
        ElementRenderState state = new ElementRenderState();
        state.setClipTop(1);
        state.setClipBottom(3);

        List<List<TerminalCell>> rendered = image.render(new Size(2, 3), state);
        ImageRenderInfo info = image.renderInfo(new Size(2, 3), state);

        assertTrue(rendered.get(0).get(0).raw().isEmpty());
        assertTrue(rendered.get(1).get(0).raw().startsWith("\u001bPq\"1;1;2;6"));
        assertEquals(2, rendered.get(1).get(0).rawWidth());
        assertEquals(1, rendered.get(1).get(0).rawHeight());
        assertTrue(rendered.get(1).get(1).rawSkip());
        assertTrue(rendered.get(2).get(0).raw().startsWith("\u001bPq\"1;1;2;6"));
        assertEquals(1, info.visibleTop());
        assertEquals(2, info.visibleHeight());
        assertTrue(info.rawExpected());
        assertTrue(info.rawPresent());
    }

    private static List<List<TerminalCell>> renderWithCellBackground(Image image, Size size)
    {
        ScopedImageCellBackgroundRendering scope = new ScopedImageCellBackgroundRendering();
        try
        {
            return image.render(size, new ElementRenderState());
        }
        finally
        {
            scope.close();
        }
    }
}
