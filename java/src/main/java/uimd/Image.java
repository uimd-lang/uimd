package uimd;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import javax.imageio.ImageIO;

public final class Image extends Element
{
    public static final String DEFAULT_FIT = "contain";
    public static final String DEFAULT_RENDER_MODE = "auto";
    public static final String DEFAULT_ALIGN = "center";
    public static final String DEFAULT_VERTICAL_ALIGN = "middle";

    private static final int DEFAULT_CELL_PIXEL_WIDTH = 8;
    private static final int DEFAULT_CELL_PIXEL_HEIGHT = 16;
    private static final int FALLBACK_VERTICAL_SAMPLES_PER_CELL = 2;
    private static final int SIXEL_BITS_PER_GLYPH = 6;
    private static final int SIXEL_COLOR_LEVELS = 4;
    private static final int SIXEL_COLOR_SCALE = 100;
    private static final int SIXEL_CHUNK_CELL_ROWS = 1;
    private static final int SIXEL_CACHE_MAX_ENTRIES = 512;
    private static final int SIXEL_CACHE_MAX_BYTES = 32 * 1024 * 1024;
    private static final int RASTER_CACHE_MAX_ENTRIES = 64;
    private static final int SAMPLE_GRID_SIZE = 3;
    private static final int SAMPLE_COLOR_QUANTUM = 64;
    private static final int DETERMINISTIC_COLOR_QUANTUM = 32;
    private static final int CHECKER_TILE_PIXELS = 4;
    private static final int CHECKER_LIGHT_ALPHA = 160;
    private static final int CHECKER_DARK_ALPHA = 0;
    private static final int OPAQUE_ALPHA = 255;
    static final String UPPER_HALF_BLOCK = "▀";
    private static final String FULL_BLOCK = "█";
    private static final String MISSING_PLACEHOLDER = "image";
    private static final String DETERMINISTIC_FALLBACK_ENV = "UIMD_DETERMINISTIC_IMAGE_FALLBACK";
    private static final String FORCE_SIXEL_ENV = "UIMD_FORCE_SIXEL";
    private static final String DISABLE_SIXEL_ENV = "UIMD_DISABLE_SIXEL";

    private static final ThreadLocal<Integer> FALLBACK_RENDERING_DEPTH = ThreadLocal.withInitial(() -> 0);
    private static final ThreadLocal<Integer> CELL_BACKGROUND_RENDERING_DEPTH = ThreadLocal.withInitial(() -> 0);
    private static final Map<RasterCacheKey, Raster> RASTER_CACHE = new LinkedHashMap<>(16, 0.75f, true);
    private static final Map<SixelCacheKey, String> SIXEL_CACHE = new LinkedHashMap<>(16, 0.75f, true);
    private static int sixelCacheBytes;
    private static volatile Size terminalCellPixels;

    private String source;
    private String alt;
    private String fit;
    private String renderMode;
    private String align;
    private String verticalAlign;

    public Image(String name)
    {
        this(name, "", "", DEFAULT_FIT, DEFAULT_RENDER_MODE, DEFAULT_ALIGN, DEFAULT_VERTICAL_ALIGN);
    }

    public Image(
        String name,
        String source,
        String alt,
        String fit,
        String renderMode,
        String align,
        String verticalAlign)
    {
        super(name);
        this.source = source == null ? "" : source;
        this.alt = alt == null ? "" : alt;
        this.fit = normalizedMode(fit, DEFAULT_FIT);
        this.renderMode = normalizedMode(renderMode, DEFAULT_RENDER_MODE);
        this.align = normalizedMode(align, DEFAULT_ALIGN);
        this.verticalAlign = normalizedMode(verticalAlign, DEFAULT_VERTICAL_ALIGN);
    }

    public String source()
    {
        return source;
    }

    public void setSource(String value)
    {
        source = value == null ? "" : value;
    }

    public String alt()
    {
        return alt;
    }

    public void setAlt(String value)
    {
        alt = value == null ? "" : value;
    }

    public String fit()
    {
        return fit;
    }

    public void setFit(String value)
    {
        fit = normalizedMode(value, DEFAULT_FIT);
    }

    public String renderMode()
    {
        return renderMode;
    }

    public void setRenderMode(String value)
    {
        renderMode = normalizedMode(value, DEFAULT_RENDER_MODE);
    }

    public String align()
    {
        return align;
    }

    public void setAlign(String value)
    {
        align = normalizedMode(value, DEFAULT_ALIGN);
    }

    public String verticalAlign()
    {
        return verticalAlign;
    }

    public void setVerticalAlign(String value)
    {
        verticalAlign = normalizedMode(value, DEFAULT_VERTICAL_ALIGN);
    }

    public static void setTerminalCellPixels(Size value)
    {
        setImageTerminalCellPixels(value);
    }

    public static void setImageTerminalCellPixels(Size value)
    {
        if (value != null && value.width() > 0 && value.height() > 0)
        {
            terminalCellPixels = value;
        }
    }

    public static boolean imageModeNeedsSixelFallbackWarning(String mode)
    {
        if (deterministicFallbackEnabled()
            || sixelDisabled()
            || truthyEnvironment(FORCE_SIXEL_ENV))
        {
            return false;
        }
        String resolved = normalizedMode(mode, DEFAULT_RENDER_MODE);
        return !"fallback".equals(resolved) && !terminalSupportsSixel();
    }

    public static void requireSixelForImageRendering()
    {
        // Java uses its built-in Sixel encoder and has no native library to load.
    }

    public boolean shouldWarnSixelFallback()
    {
        return imageModeNeedsSixelFallbackWarning(renderMode);
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        int width = Math.max(1, size.width());
        int height = Math.max(1, size.height());
        Style style = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        Raster raster = loadRaster(imagePath());
        if (raster.empty())
        {
            return placeholderContent(width, height, style);
        }
        Rgb background = letterboxRgb(style);
        if (CELL_BACKGROUND_RENDERING_DEPTH.get() > 0)
        {
            return cellBackgroundContent(raster, width, height, style, background);
        }
        if (FALLBACK_RENDERING_DEPTH.get() > 0)
        {
            return fallbackContent(raster, width, height, style, background);
        }
        if (deterministicFallbackEnabled())
        {
            return deterministicContent(raster, width, height, style, background, !"fallback".equals(renderMode));
        }
        if (shouldRenderSixel())
        {
            List<List<TerminalCell>> sixel = sixelContent(
                raster,
                width,
                height,
                style,
                background,
                effectiveState);
            if (sixel != null)
            {
                return sixel;
            }
        }
        return fallbackContent(raster, width, height, style, background);
    }

    public ImageRenderInfo renderInfo(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        int width = Math.max(1, size.width());
        int height = Math.max(1, size.height());
        Style style = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        Size cellPixels = terminalCellPixels();
        ImageRenderInfo info = new ImageRenderInfo();
        info.setSource(source);
        info.setFit(fit);
        info.setConfiguredRenderMode(renderMode);
        info.setResolvedRenderMode(resolvedRenderMode());
        info.setElementWidth(width);
        info.setElementHeight(height);
        info.setCellPixelWidth(cellPixels.width());
        info.setCellPixelHeight(cellPixels.height());
        Raster raster = loadRaster(imagePath());
        if (raster.empty())
        {
            info.setResolvedRenderMode("placeholder");
            return info;
        }
        CellRegion region = imageCellRegion(width, height, raster.width(), raster.height());
        int visibleTop = region.rowOffset();
        int visibleBottom = region.rowOffset() + region.rows();
        if (effectiveState.clipTop() != null || effectiveState.clipBottom() != null)
        {
            visibleTop = Math.max(visibleTop, effectiveState.clipTop() == null ? 0 : effectiveState.clipTop());
            visibleBottom = Math.min(
                visibleBottom,
                effectiveState.clipBottom() == null ? height : effectiveState.clipBottom());
        }
        int visibleRows = Math.max(0, visibleBottom - visibleTop);
        String regionFit = "contain".equals(fit) ? "cover" : fit;
        int signatureRows = "sixel".equals(info.resolvedRenderMode())
            ? Math.max(1, visibleRows)
            : region.rows();
        info.setSourceLoaded(true);
        info.setSourceWidth(raster.width());
        info.setSourceHeight(raster.height());
        info.setImageLeft(region.colOffset());
        info.setImageTop(region.rowOffset());
        info.setImageWidth(region.cols());
        info.setImageHeight(region.rows());
        info.setVisibleLeft(region.colOffset());
        info.setVisibleTop(visibleTop);
        info.setVisibleWidth(visibleRows > 0 ? region.cols() : 0);
        info.setVisibleHeight(visibleRows);
        info.setRawExpected("sixel".equals(info.resolvedRenderMode()) && visibleRows > 0);
        info.setRawPresent(info.rawExpected());
        info.setSampleSignature(imageInfoSampleSignature(
            raster,
            region.cols(),
            Math.max(1, signatureRows),
            regionFit,
            align,
            verticalAlign,
            letterboxRgb(style)));
        return info;
    }

    static void pushFallbackRendering()
    {
        FALLBACK_RENDERING_DEPTH.set(FALLBACK_RENDERING_DEPTH.get() + 1);
    }

    static void popFallbackRendering()
    {
        FALLBACK_RENDERING_DEPTH.set(Math.max(0, FALLBACK_RENDERING_DEPTH.get() - 1));
    }

    static void pushCellBackgroundRendering()
    {
        CELL_BACKGROUND_RENDERING_DEPTH.set(CELL_BACKGROUND_RENDERING_DEPTH.get() + 1);
    }

    static void popCellBackgroundRendering()
    {
        CELL_BACKGROUND_RENDERING_DEPTH.set(Math.max(0, CELL_BACKGROUND_RENDERING_DEPTH.get() - 1));
    }

    private String resolvedRenderMode()
    {
        if (CELL_BACKGROUND_RENDERING_DEPTH.get() > 0)
        {
            return "cell_background";
        }
        if (FALLBACK_RENDERING_DEPTH.get() > 0)
        {
            return "fallback";
        }
        if (deterministicFallbackEnabled())
        {
            return "deterministic";
        }
        return shouldRenderSixel() ? "sixel" : "fallback";
    }

    private boolean shouldRenderSixel()
    {
        return ("sixel".equals(renderMode) || "auto".equals(renderMode)) && terminalSupportsSixel();
    }

    private static boolean terminalSupportsSixel()
    {
        if (truthyEnvironment(FORCE_SIXEL_ENV))
        {
            return true;
        }
        if (sixelDisabled())
        {
            return false;
        }
        String termProgram = environment("TERM_PROGRAM");
        String term = environment("TERM");
        String colorTerm = environment("COLORTERM");
        String itermSession = environment("ITERM_SESSION_ID");
        String lcTerminal = environment("LC_TERMINAL");
        if (termProgram.contains("apple_terminal"))
        {
            return false;
        }
        return !itermSession.isEmpty()
            || lcTerminal.contains("iterm")
            || termProgram.contains("iterm")
            || termProgram.contains("wezterm")
            || termProgram.contains("mlterm")
            || termProgram.contains("foot")
            || termProgram.contains("contour")
            || term.contains("sixel")
            || colorTerm.contains("sixel")
            || term.contains("mlterm")
            || term.contains("foot")
            || term.contains("contour");
    }

    private static boolean sixelDisabled()
    {
        return truthyEnvironment(DISABLE_SIXEL_ENV);
    }

    private static boolean deterministicFallbackEnabled()
    {
        return truthyEnvironment(DETERMINISTIC_FALLBACK_ENV);
    }

    private static boolean truthyEnvironment(String name)
    {
        String value = environment(name);
        return "1".equals(value) || "true".equals(value) || "yes".equals(value) || "on".equals(value);
    }

    private static String environment(String name)
    {
        String value = System.getenv(name);
        return value == null ? "" : value.strip().toLowerCase(Locale.ROOT);
    }

    private Path imagePath()
    {
        if (source.isEmpty())
        {
            return Path.of("");
        }
        Path path = Path.of(source);
        return path.isAbsolute() ? path.normalize() : path.toAbsolutePath().normalize();
    }

    private List<List<TerminalCell>> fallbackContent(
        Raster raster,
        int width,
        int height,
        Style style,
        Rgb background)
    {
        List<List<TerminalCell>> content = blankContent(width, height, style);
        CellRegion region = imageCellRegion(width, height, raster.width(), raster.height());
        String regionFit = "contain".equals(fit) ? "cover" : fit;
        Raster resized = resizeRasterRows(
            raster,
            region.cols(),
            region.rows() * FALLBACK_VERTICAL_SAMPLES_PER_CELL,
            0,
            region.rows() * FALLBACK_VERTICAL_SAMPLES_PER_CELL,
            regionFit,
            align,
            verticalAlign,
            background);
        for (int row = 0; row < region.rows(); ++row)
        {
            int targetRow = region.rowOffset() + row;
            int topY = row * FALLBACK_VERTICAL_SAMPLES_PER_CELL;
            int bottomY = Math.min(resized.height() - 1, topY + 1);
            for (int col = 0; col < region.cols(); ++col)
            {
                int targetCol = region.colOffset() + col;
                Rgb top = resized.pixel(col, topY).rgb();
                Rgb bottom = resized.pixel(col, bottomY).rgb();
                TerminalCell cell = content.get(targetRow).get(targetCol);
                cell.setText(UPPER_HALF_BLOCK);
                cell.setForeground(top.color());
                cell.setBackground(bottom.color());
            }
        }
        return content;
    }

    private List<List<TerminalCell>> deterministicContent(
        Raster raster,
        int width,
        int height,
        Style style,
        Rgb background,
        boolean checker)
    {
        List<List<TerminalCell>> content = blankContent(width, height, style);
        CellRegion region = imageCellRegion(width, height, raster.width(), raster.height());
        String regionFit = "contain".equals(fit) ? "cover" : fit;
        int sampleHeight = region.rows() * FALLBACK_VERTICAL_SAMPLES_PER_CELL;
        for (int row = region.rowOffset(); row < region.rowOffset() + region.rows(); ++row)
        {
            if (row < 0 || row >= height)
            {
                continue;
            }
            for (int col = region.colOffset(); col < region.colOffset() + region.cols(); ++col)
            {
                if (col < 0 || col >= width)
                {
                    continue;
                }
                int sampleCol = col - region.colOffset();
                int sampleTop = (row - region.rowOffset()) * FALLBACK_VERTICAL_SAMPLES_PER_CELL;
                TerminalCell cell = content.get(row).get(col);
                cell.setText(UPPER_HALF_BLOCK);
                cell.setForeground(testFallbackSampleColor(
                    raster,
                    sampleCol,
                    sampleTop,
                    region.cols(),
                    sampleHeight,
                    regionFit,
                    align,
                    verticalAlign,
                    background,
                    checker).color());
                cell.setBackground(testFallbackSampleColor(
                    raster,
                    sampleCol,
                    sampleTop + 1,
                    region.cols(),
                    sampleHeight,
                    regionFit,
                    align,
                    verticalAlign,
                    background,
                    checker).color());
            }
        }
        return content;
    }

    private List<List<TerminalCell>> cellBackgroundContent(
        Raster raster,
        int width,
        int height,
        Style style,
        Rgb background)
    {
        List<List<TerminalCell>> content = blankContent(width, height, style);
        CellRegion region = imageCellRegion(width, height, raster.width(), raster.height());
        String regionFit = "contain".equals(fit) ? "cover" : fit;
        Raster resized = resizeRasterRows(
            raster,
            region.cols(),
            region.rows(),
            0,
            region.rows(),
            regionFit,
            align,
            verticalAlign,
            background);
        for (int row = 0; row < region.rows(); ++row)
        {
            for (int col = 0; col < region.cols(); ++col)
            {
                Color color = resized.pixel(col, row).rgb().color();
                TerminalCell cell = content.get(region.rowOffset() + row).get(region.colOffset() + col);
                cell.setText(FULL_BLOCK);
                cell.setForeground(color);
                cell.setBackground(color);
            }
        }
        return content;
    }

    private List<List<TerminalCell>> sixelContent(
        Raster raster,
        int width,
        int height,
        Style style,
        Rgb background,
        ElementRenderState state)
    {
        CellRegion region = imageCellRegion(width, height, raster.width(), raster.height());
        int visibleTop = region.rowOffset();
        int visibleBottom = region.rowOffset() + region.rows();
        if (state.clipTop() != null || state.clipBottom() != null)
        {
            visibleTop = Math.max(visibleTop, state.clipTop() == null ? 0 : state.clipTop());
            visibleBottom = Math.min(visibleBottom, state.clipBottom() == null ? height : state.clipBottom());
        }
        int visibleRows = Math.max(0, visibleBottom - visibleTop);
        if (visibleRows <= 0)
        {
            return blankContent(width, height, style);
        }
        List<List<TerminalCell>> content = blankContent(width, height, style);
        for (int row = visibleTop; row < visibleBottom; ++row)
        {
            for (int col = region.colOffset(); col < region.colOffset() + region.cols(); ++col)
            {
                content.get(row).get(col).setRawSkip(true);
            }
        }
        String regionFit = "contain".equals(fit) ? "cover" : fit;
        int cropTop = visibleTop - region.rowOffset();
        int cropBottom = cropTop + visibleRows;
        int firstChunk = cropTop / SIXEL_CHUNK_CELL_ROWS * SIXEL_CHUNK_CELL_ROWS;
        boolean rawPresent = false;
        for (int chunkTop = firstChunk; chunkTop < cropBottom; chunkTop += SIXEL_CHUNK_CELL_ROWS)
        {
            int segmentTop = Math.max(cropTop, chunkTop);
            int segmentBottom = Math.min(cropBottom, chunkTop + SIXEL_CHUNK_CELL_ROWS);
            int segmentRows = segmentBottom - segmentTop;
            String raw = cachedSixelPayload(
                raster,
                region.cols(),
                segmentRows,
                regionFit,
                align,
                verticalAlign,
                background,
                region.rows(),
                segmentTop);
            if (raw.isEmpty())
            {
                continue;
            }
            int anchorRow = visibleTop + segmentTop - cropTop;
            TerminalCell anchor = content.get(anchorRow).get(region.colOffset());
            anchor.setRaw(raw);
            anchor.setRawWidth(region.cols());
            anchor.setRawHeight(segmentRows);
            anchor.setRawSkip(false);
            rawPresent = true;
        }
        return rawPresent ? content : null;
    }

    private String cachedSixelPayload(
        Raster raster,
        int width,
        int height,
        String payloadFit,
        String payloadAlign,
        String payloadVerticalAlign,
        Rgb background,
        int sourceRows,
        int cropTop)
    {
        Size cellPixels = terminalCellPixels();
        int targetPixelWidth = Math.max(1, width) * cellPixels.width();
        int visiblePixelHeight = Math.max(1, height) * cellPixels.height();
        int sourcePixelHeight = Math.max(1, sourceRows) * cellPixels.height();
        int cropTopPixels = Math.max(0, cropTop) * cellPixels.height();
        SixelCacheKey key = new SixelCacheKey(
            raster.identity(),
            targetPixelWidth,
            visiblePixelHeight,
            payloadFit,
            payloadAlign,
            payloadVerticalAlign,
            background,
            sourcePixelHeight,
            cropTopPixels);
        synchronized (SIXEL_CACHE)
        {
            String cached = SIXEL_CACHE.get(key);
            if (cached != null)
            {
                return cached;
            }
        }
        Raster payload = resizeRasterRows(
            raster,
            targetPixelWidth,
            sourcePixelHeight,
            cropTopPixels,
            visiblePixelHeight,
            payloadFit,
            payloadAlign,
            payloadVerticalAlign,
            background);
        String encoded = sixelPayload(payload);
        synchronized (SIXEL_CACHE)
        {
            if (encoded.length() <= SIXEL_CACHE_MAX_BYTES)
            {
                String previous = SIXEL_CACHE.put(key, encoded);
                sixelCacheBytes += encoded.length() - (previous == null ? 0 : previous.length());
                while (SIXEL_CACHE.size() > SIXEL_CACHE_MAX_ENTRIES
                    || sixelCacheBytes > SIXEL_CACHE_MAX_BYTES)
                {
                    Map.Entry<SixelCacheKey, String> oldest = SIXEL_CACHE.entrySet().iterator().next();
                    sixelCacheBytes -= oldest.getValue().length();
                    SIXEL_CACHE.remove(oldest.getKey());
                }
            }
        }
        return encoded;
    }

    private static String sixelPayload(Raster raster)
    {
        if (raster.empty())
        {
            return "";
        }
        int[] indexes = new int[raster.width() * raster.height()];
        Set<Integer> colors = new LinkedHashSet<>();
        for (int y = 0; y < raster.height(); ++y)
        {
            for (int x = 0; x < raster.width(); ++x)
            {
                Rgb color = quantizeSixel(raster.pixel(x, y).rgb());
                int index = sixelColorIndex(color);
                indexes[y * raster.width() + x] = index;
                colors.add(index);
            }
        }
        StringBuilder output = new StringBuilder();
        output.append("\u001bPq\"1;1;").append(raster.width()).append(';').append(raster.height());
        for (int index : colors)
        {
            Rgb color = sixelColor(index);
            output.append('#').append(index).append(";2;")
                .append(sixelComponent(color.red())).append(';')
                .append(sixelComponent(color.green())).append(';')
                .append(sixelComponent(color.blue()));
        }
        for (int y = 0; y < raster.height(); y += SIXEL_BITS_PER_GLYPH)
        {
            for (int color : colors)
            {
                StringBuilder run = new StringBuilder();
                char previous = 0;
                int count = 0;
                boolean hasPixels = false;
                for (int x = 0; x < raster.width(); ++x)
                {
                    int bits = 0;
                    for (int bit = 0; bit < SIXEL_BITS_PER_GLYPH; ++bit)
                    {
                        int py = y + bit;
                        if (py < raster.height() && indexes[py * raster.width() + x] == color)
                        {
                            bits |= 1 << bit;
                        }
                    }
                    hasPixels |= bits != 0;
                    char character = (char) (63 + bits);
                    if (character == previous)
                    {
                        ++count;
                    }
                    else
                    {
                        appendSixelRun(run, previous, count);
                        previous = character;
                        count = 1;
                    }
                }
                if (hasPixels)
                {
                    appendSixelRun(run, previous, count);
                    output.append('#').append(color).append(run).append('$');
                }
            }
            output.append('-');
        }
        return output.append("\u001b\\").toString();
    }

    private static void appendSixelRun(StringBuilder output, char character, int count)
    {
        if (count <= 0)
        {
            return;
        }
        if (count >= 4)
        {
            output.append('!').append(count).append(character);
        }
        else
        {
            output.append(String.valueOf(character).repeat(count));
        }
    }

    private CellRegion imageCellRegion(int width, int height, int sourceWidth, int sourceHeight)
    {
        if (!"contain".equals(fit) || sourceWidth <= 0 || sourceHeight <= 0)
        {
            return new CellRegion(width, height, 0, 0);
        }
        Size pixels = terminalCellPixels();
        double scale = Math.min(
            width * pixels.width() / (double) sourceWidth,
            height * pixels.height() / (double) sourceHeight);
        int cols = Math.max(1, Math.min(
            width,
            (int) Math.rint(sourceWidth * scale / pixels.width())));
        int rows = Math.max(1, Math.min(
            height,
            (int) Math.rint(sourceHeight * scale / pixels.height())));
        return new CellRegion(
            cols,
            rows,
            alignmentOffset(width, cols, align, "left", "right"),
            alignmentOffset(height, rows, verticalAlign, "top", "bottom"));
    }

    private static Raster resizeRasterRows(
        Raster source,
        int targetWidth,
        int targetHeight,
        int firstTargetRow,
        int targetRowCount,
        String fit,
        String align,
        String verticalAlign,
        Rgb background)
    {
        int width = Math.max(1, targetWidth);
        int height = Math.max(1, targetHeight);
        int firstRow = clamp(firstTargetRow, 0, height);
        int rowCount = clamp(targetRowCount, 0, height - firstRow);
        int[] pixels = new int[width * rowCount];
        String resolvedFit = normalizedMode(fit, DEFAULT_FIT);
        boolean stretch = "stretch".equals(resolvedFit);
        boolean cover = "cover".equals(resolvedFit);
        double scale = stretch
            ? 0.0
            : cover
                ? Math.max(width / (double) source.width(), height / (double) source.height())
                : Math.min(width / (double) source.width(), height / (double) source.height());
        double drawnWidth = stretch ? width : Math.max(1.0, source.width() * scale);
        double drawnHeight = stretch ? height : Math.max(1.0, source.height() * scale);
        double xOffset = stretch
            ? 0.0
            : cover
                ? alignmentOffsetFloat(drawnWidth, width, align, "left", "right")
                : alignmentOffsetFloat(width, drawnWidth, align, "left", "right");
        double yOffset = stretch
            ? 0.0
            : cover
                ? alignmentOffsetFloat(drawnHeight, height, verticalAlign, "top", "bottom")
                : alignmentOffsetFloat(height, drawnHeight, verticalAlign, "top", "bottom");
        for (int y = 0; y < rowCount; ++y)
        {
            int targetY = firstRow + y;
            for (int x = 0; x < width; ++x)
            {
                double sourceLeft;
                double sourceRight;
                double sourceTop;
                double sourceBottom;
                if (stretch)
                {
                    sourceLeft = x * source.width() / (double) width;
                    sourceRight = (x + 1) * source.width() / (double) width;
                    sourceTop = targetY * source.height() / (double) height;
                    sourceBottom = (targetY + 1) * source.height() / (double) height;
                }
                else if (cover)
                {
                    sourceLeft = (x + xOffset) / scale;
                    sourceRight = (x + 1 + xOffset) / scale;
                    sourceTop = (targetY + yOffset) / scale;
                    sourceBottom = (targetY + 1 + yOffset) / scale;
                }
                else
                {
                    sourceLeft = (x - xOffset) / scale;
                    sourceRight = (x + 1 - xOffset) / scale;
                    sourceTop = (targetY - yOffset) / scale;
                    sourceBottom = (targetY + 1 - yOffset) / scale;
                }
                Rgb sample = sampleRasterArea(
                    source,
                    sourceLeft,
                    sourceTop,
                    sourceRight,
                    sourceBottom,
                    background);
                pixels[y * width + x] = new RgbaSample(
                    sample.red(),
                    sample.green(),
                    sample.blue(),
                    OPAQUE_ALPHA).argb();
            }
        }
        return new Raster(width, rowCount, pixels, source.identity());
    }

    private static Rgb sampleRasterArea(
        Raster source,
        double left,
        double top,
        double right,
        double bottom,
        Rgb background)
    {
        double fullArea = Math.max(0.000001, (right - left) * (bottom - top));
        if (source.empty()
            || right <= 0.0
            || bottom <= 0.0
            || left >= source.width()
            || top >= source.height())
        {
            return background;
        }
        double red = background.red() * fullArea;
        double green = background.green() * fullArea;
        double blue = background.blue() * fullArea;
        int startX = Math.max(0, (int) Math.floor(left));
        int endX = Math.min(source.width(), (int) Math.ceil(right));
        int startY = Math.max(0, (int) Math.floor(top));
        int endY = Math.min(source.height(), (int) Math.ceil(bottom));
        for (int y = startY; y < endY; ++y)
        {
            double yWeight = intervalOverlap(top, bottom, y, y + 1.0);
            if (yWeight <= 0.0)
            {
                continue;
            }
            for (int x = startX; x < endX; ++x)
            {
                double xWeight = intervalOverlap(left, right, x, x + 1.0);
                if (xWeight <= 0.0)
                {
                    continue;
                }
                double weight = xWeight * yWeight;
                RgbaSample pixel = source.pixel(x, y);
                double alpha = pixel.alpha() / (double) OPAQUE_ALPHA;
                red += (pixel.red() - background.red()) * alpha * weight;
                green += (pixel.green() - background.green()) * alpha * weight;
                blue += (pixel.blue() - background.blue()) * alpha * weight;
            }
        }
        return new Rgb(
            (int) Math.round(red / fullArea),
            (int) Math.round(green / fullArea),
            (int) Math.round(blue / fullArea));
    }

    private static double intervalOverlap(
        double firstStart,
        double firstEnd,
        double secondStart,
        double secondEnd)
    {
        return Math.max(0.0, Math.min(firstEnd, secondEnd) - Math.max(firstStart, secondStart));
    }

    private static Raster loadRaster(Path path)
    {
        if (path == null || path.toString().isEmpty() || !Files.isRegularFile(path))
        {
            return Raster.EMPTY;
        }
        try
        {
            BasicFileAttributes attributes = Files.readAttributes(path, BasicFileAttributes.class);
            RasterCacheKey key = new RasterCacheKey(
                path.toString(),
                attributes.lastModifiedTime().toMillis(),
                attributes.size());
            synchronized (RASTER_CACHE)
            {
                Raster cached = RASTER_CACHE.get(key);
                if (cached != null)
                {
                    return cached;
                }
            }
            Raster loaded = path.getFileName().toString().toLowerCase(Locale.ROOT).endsWith(".tga")
                ? loadTga(path, key.toString())
                : loadImageIo(path, key.toString());
            synchronized (RASTER_CACHE)
            {
                RASTER_CACHE.put(key, loaded);
                while (RASTER_CACHE.size() > RASTER_CACHE_MAX_ENTRIES)
                {
                    RASTER_CACHE.remove(RASTER_CACHE.entrySet().iterator().next().getKey());
                }
            }
            return loaded;
        }
        catch (IOException exception)
        {
            return Raster.EMPTY;
        }
    }

    private static Raster loadImageIo(Path path, String identity) throws IOException
    {
        BufferedImage image = ImageIO.read(path.toFile());
        if (image == null || image.getWidth() <= 0 || image.getHeight() <= 0)
        {
            return Raster.EMPTY;
        }
        int[] pixels = image.getColorModel().getColorSpace().getType() == java.awt.color.ColorSpace.TYPE_GRAY
            ? rawGrayscalePixels(image)
            : image.getRGB(0, 0, image.getWidth(), image.getHeight(), null, 0, image.getWidth());
        return new Raster(image.getWidth(), image.getHeight(), pixels, identity);
    }

    private static int[] rawGrayscalePixels(BufferedImage image)
    {
        java.awt.image.Raster source = image.getRaster();
        int[] pixels = new int[image.getWidth() * image.getHeight()];
        int alphaBand = image.getColorModel().hasAlpha() ? source.getNumBands() - 1 : -1;
        int colorBits = source.getSampleModel().getSampleSize(0);
        int alphaBits = alphaBand >= 0 ? source.getSampleModel().getSampleSize(alphaBand) : 0;
        for (int y = 0; y < image.getHeight(); ++y)
        {
            for (int x = 0; x < image.getWidth(); ++x)
            {
                int gray = sampleToByte(source.getSample(x, y, 0), colorBits);
                int alpha = alphaBand >= 0
                    ? sampleToByte(source.getSample(x, y, alphaBand), alphaBits)
                    : OPAQUE_ALPHA;
                pixels[y * image.getWidth() + x] = alpha << 24 | gray << 16 | gray << 8 | gray;
            }
        }
        return pixels;
    }

    private static int sampleToByte(int sample, int sampleBits)
    {
        if (sampleBits <= 0 || sampleBits > Integer.SIZE - 1)
        {
            return clamp(sample, 0, OPAQUE_ALPHA);
        }
        long maximum = (1L << sampleBits) - 1L;
        long resolved = Math.max(0L, Math.min(maximum, sample));
        return (int) (resolved * OPAQUE_ALPHA / maximum);
    }

    private static Raster loadTga(Path path, String identity) throws IOException
    {
        byte[] data = Files.readAllBytes(path);
        if (data.length < 18)
        {
            return Raster.EMPTY;
        }
        int idLength = unsigned(data[0]);
        int colorMapType = unsigned(data[1]);
        int imageType = unsigned(data[2]);
        int width = littleEndian(data, 12);
        int height = littleEndian(data, 14);
        int depth = unsigned(data[16]);
        int descriptor = unsigned(data[17]);
        if (colorMapType != 0
            || (imageType != 2 && imageType != 10)
            || (depth != 24 && depth != 32)
            || width <= 0
            || height <= 0)
        {
            return Raster.EMPTY;
        }
        int bytesPerPixel = depth / 8;
        int offset = 18 + idLength;
        int[] sourcePixels = new int[width * height];
        int pixel = 0;
        while (pixel < sourcePixels.length && offset < data.length)
        {
            if (imageType == 2)
            {
                if (offset + bytesPerPixel > data.length)
                {
                    return Raster.EMPTY;
                }
                sourcePixels[pixel++] = tgaPixel(data, offset, bytesPerPixel);
                offset += bytesPerPixel;
                continue;
            }
            int packet = unsigned(data[offset++]);
            int count = (packet & 0x7f) + 1;
            if ((packet & 0x80) != 0)
            {
                if (offset + bytesPerPixel > data.length)
                {
                    return Raster.EMPTY;
                }
                int color = tgaPixel(data, offset, bytesPerPixel);
                offset += bytesPerPixel;
                for (int index = 0; index < count && pixel < sourcePixels.length; ++index)
                {
                    sourcePixels[pixel++] = color;
                }
            }
            else
            {
                for (int index = 0; index < count && pixel < sourcePixels.length; ++index)
                {
                    if (offset + bytesPerPixel > data.length)
                    {
                        return Raster.EMPTY;
                    }
                    sourcePixels[pixel++] = tgaPixel(data, offset, bytesPerPixel);
                    offset += bytesPerPixel;
                }
            }
        }
        if (pixel != sourcePixels.length)
        {
            return Raster.EMPTY;
        }
        boolean topOrigin = (descriptor & 0x20) != 0;
        boolean rightOrigin = (descriptor & 0x10) != 0;
        int[] oriented = new int[sourcePixels.length];
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int targetY = topOrigin ? y : height - 1 - y;
                int targetX = rightOrigin ? width - 1 - x : x;
                oriented[targetY * width + targetX] = sourcePixels[y * width + x];
            }
        }
        return new Raster(width, height, oriented, identity);
    }

    private static int tgaPixel(byte[] data, int offset, int bytesPerPixel)
    {
        int blue = unsigned(data[offset]);
        int green = unsigned(data[offset + 1]);
        int red = unsigned(data[offset + 2]);
        int alpha = bytesPerPixel == 4 ? unsigned(data[offset + 3]) : OPAQUE_ALPHA;
        return alpha << 24 | red << 16 | green << 8 | blue;
    }

    private static int littleEndian(byte[] data, int offset)
    {
        return unsigned(data[offset]) | unsigned(data[offset + 1]) << 8;
    }

    private static int unsigned(byte value)
    {
        return value & 0xff;
    }

    private Rgb letterboxRgb(Style style)
    {
        Color background = style.background();
        if (background == null
            || background.isTransparent()
            || background.rgba().map(value -> value.alpha() < OPAQUE_ALPHA).orElse(true))
        {
            Color parent = Element.renderingParentBackground();
            if (background != null && !background.isTransparent()
                && background.rgba().isPresent() && parent != null)
            {
                background = background.blendOver(parent);
            }
            else if (parent != null)
            {
                background = parent;
            }
        }
        return background == null || background.rgba().isEmpty()
            ? new Rgb(0, 0, 0)
            : new Rgb(
                background.rgba().orElseThrow().red(),
                background.rgba().orElseThrow().green(),
                background.rgba().orElseThrow().blue());
    }

    private List<List<TerminalCell>> placeholderContent(int width, int height, Style style)
    {
        Path fileName = imagePath().getFileName();
        String text = alt.isEmpty() && fileName != null ? fileName.toString() : alt;
        if (text.isEmpty())
        {
            text = MISSING_PLACEHOLDER;
        }
        List<List<TerminalCell>> content = blankContent(width, height, style);
        int row = height / 2;
        int col = Math.max(0, (width - RenderHelpers.visibleWidth(text)) / 2);
        for (VisualGlyph glyph : RenderHelpers.visualGlyphs(text, 0, 0))
        {
            if (col >= width)
            {
                break;
            }
            content.get(row).get(col++).setText(glyph.text());
        }
        return content;
    }

    private static List<List<TerminalCell>> blankContent(int width, int height, Style style)
    {
        return RenderHelpers.renderPlainText("", width, height, style);
    }

    private static List<String> imageInfoSampleSignature(
        Raster raster,
        int width,
        int height,
        String fit,
        String align,
        String verticalAlign,
        Rgb background)
    {
        Raster fitted = resizeRasterRows(
            raster,
            Math.max(1, width),
            Math.max(1, height),
            0,
            Math.max(1, height),
            fit,
            align,
            verticalAlign,
            background);
        List<String> result = new ArrayList<>();
        for (int row : samplePositions(fitted.height()))
        {
            for (int col : samplePositions(fitted.width()))
            {
                Rgb color = fitted.pixel(col, row).rgb().quantized(SAMPLE_COLOR_QUANTUM);
                result.add(String.format(Locale.ROOT, "#%02x%02x%02x", color.red(), color.green(), color.blue()));
            }
        }
        return result;
    }

    private static List<Integer> samplePositions(int extent)
    {
        int size = Math.max(1, extent);
        List<Integer> result = new ArrayList<>();
        for (int index = 0; index < SAMPLE_GRID_SIZE; ++index)
        {
            result.add(clamp(
                (int) Math.rint(index * (size - 1) / (double) (SAMPLE_GRID_SIZE - 1)),
                0,
                size - 1));
        }
        return result;
    }

    private static Rgb testFallbackSampleColor(
        Raster raster,
        int x,
        int y,
        int targetWidth,
        int targetHeight,
        String fit,
        String align,
        String verticalAlign,
        Rgb background,
        boolean checker)
    {
        int width = Math.max(1, targetWidth);
        int height = Math.max(1, targetHeight);
        int sampleX = clamp(x, 0, width - 1);
        int sampleY = clamp(y, 0, height - 1);
        Rgb base = checker ? checkerTint(background, sampleX, sampleY) : background;
        if (raster.empty())
        {
            return base.quantized(DETERMINISTIC_COLOR_QUANTUM);
        }
        String resolvedFit = normalizedMode(fit, DEFAULT_FIT);
        int sourceX;
        int sourceY;
        if ("stretch".equals(resolvedFit))
        {
            sourceX = (int) (sampleX * raster.width() / (double) width);
            sourceY = (int) (sampleY * raster.height() / (double) height);
        }
        else
        {
            double scale = "cover".equals(resolvedFit)
                ? Math.max(width / (double) raster.width(), height / (double) raster.height())
                : Math.min(width / (double) raster.width(), height / (double) raster.height());
            double drawnWidth = raster.width() * scale;
            double drawnHeight = raster.height() * scale;
            double xOffset = alignmentOffsetFloat(width, drawnWidth, align, "left", "right");
            double yOffset = alignmentOffsetFloat(height, drawnHeight, verticalAlign, "top", "bottom");
            double sourceXFloat = (sampleX - xOffset) / scale;
            double sourceYFloat = (sampleY - yOffset) / scale;
            if (sourceXFloat < 0.0
                || sourceYFloat < 0.0
                || sourceXFloat >= raster.width()
                || sourceYFloat >= raster.height())
            {
                return base.quantized(DETERMINISTIC_COLOR_QUANTUM);
            }
            sourceX = (int) sourceXFloat;
            sourceY = (int) sourceYFloat;
        }
        RgbaSample pixel = raster.pixel(
            clamp(sourceX, 0, raster.width() - 1),
            clamp(sourceY, 0, raster.height() - 1));
        Rgb color = blendRgb(pixel.rgb(), base, pixel.alpha());
        if (checker)
        {
            color = checkerTint(color, sampleX, sampleY);
        }
        return color.quantized(DETERMINISTIC_COLOR_QUANTUM);
    }

    private static Rgb checkerTint(Rgb color, int x, int y)
    {
        int alpha = ((x / CHECKER_TILE_PIXELS) + (y / CHECKER_TILE_PIXELS)) % 2 == 0
            ? CHECKER_LIGHT_ALPHA
            : CHECKER_DARK_ALPHA;
        return blendRgb(new Rgb(255, 255, 255), color, alpha);
    }

    private static Rgb blendRgb(Rgb source, Rgb destination, int alpha)
    {
        int resolvedAlpha = clamp(alpha, 0, OPAQUE_ALPHA);
        return new Rgb(
            (source.red() * resolvedAlpha + destination.red() * (OPAQUE_ALPHA - resolvedAlpha))
                / OPAQUE_ALPHA,
            (source.green() * resolvedAlpha + destination.green() * (OPAQUE_ALPHA - resolvedAlpha))
                / OPAQUE_ALPHA,
            (source.blue() * resolvedAlpha + destination.blue() * (OPAQUE_ALPHA - resolvedAlpha))
                / OPAQUE_ALPHA);
    }

    private static Rgb quantizeSixel(Rgb color)
    {
        return new Rgb(
            quantizeSixelChannel(color.red()),
            quantizeSixelChannel(color.green()),
            quantizeSixelChannel(color.blue()));
    }

    private static int quantizeSixelChannel(int value)
    {
        int level = (int) Math.round(value * (SIXEL_COLOR_LEVELS - 1) / 255.0);
        return clamp(level, 0, SIXEL_COLOR_LEVELS - 1) * 255 / (SIXEL_COLOR_LEVELS - 1);
    }

    private static int sixelColorIndex(Rgb color)
    {
        int red = color.red() * (SIXEL_COLOR_LEVELS - 1) / 255;
        int green = color.green() * (SIXEL_COLOR_LEVELS - 1) / 255;
        int blue = color.blue() * (SIXEL_COLOR_LEVELS - 1) / 255;
        return red * SIXEL_COLOR_LEVELS * SIXEL_COLOR_LEVELS + green * SIXEL_COLOR_LEVELS + blue;
    }

    private static Rgb sixelColor(int index)
    {
        int blue = index % SIXEL_COLOR_LEVELS;
        int green = index / SIXEL_COLOR_LEVELS % SIXEL_COLOR_LEVELS;
        int red = index / (SIXEL_COLOR_LEVELS * SIXEL_COLOR_LEVELS);
        return new Rgb(
            red * 255 / (SIXEL_COLOR_LEVELS - 1),
            green * 255 / (SIXEL_COLOR_LEVELS - 1),
            blue * 255 / (SIXEL_COLOR_LEVELS - 1));
    }

    private static int sixelComponent(int value)
    {
        return clamp((int) Math.round(value * SIXEL_COLOR_SCALE / 255.0), 0, SIXEL_COLOR_SCALE);
    }

    private static Size terminalCellPixels()
    {
        Size override = terminalCellPixels;
        return override == null
            ? new Size(DEFAULT_CELL_PIXEL_WIDTH, DEFAULT_CELL_PIXEL_HEIGHT)
            : override;
    }

    private static String normalizedMode(String value, String fallback)
    {
        String result = value == null ? "" : value.strip().toLowerCase(Locale.ROOT);
        return result.isEmpty() ? fallback : result;
    }

    private static int alignmentOffset(int outer, int inner, String value, String start, String end)
    {
        int span = Math.max(0, outer - inner);
        String mode = normalizedMode(value, "");
        if (start.equals(mode))
        {
            return 0;
        }
        return end.equals(mode) ? span : span / 2;
    }

    private static double alignmentOffsetFloat(
        double outer,
        double inner,
        String value,
        String start,
        String end)
    {
        double span = Math.max(0.0, outer - inner);
        String mode = normalizedMode(value, "");
        if (start.equals(mode))
        {
            return 0.0;
        }
        return end.equals(mode) ? span : span / 2.0;
    }

    private static int clamp(int value, int minimum, int maximum)
    {
        return Math.max(minimum, Math.min(maximum, value));
    }

    private record CellRegion(int cols, int rows, int colOffset, int rowOffset)
    {
    }

    private record RasterCacheKey(String path, long modified, long size)
    {
    }

    private record SixelCacheKey(
        String source,
        int pixelWidth,
        int pixelHeight,
        String fit,
        String align,
        String verticalAlign,
        Rgb background,
        int sourcePixelHeight,
        int cropTopPixels)
    {
    }

    private record Rgb(int red, int green, int blue)
    {
        Rgb
        {
            red = clamp(red, 0, 255);
            green = clamp(green, 0, 255);
            blue = clamp(blue, 0, 255);
        }

        Color color()
        {
            return Color.rgb(red, green, blue);
        }

        Rgb quantized(int quantum)
        {
            return new Rgb(
                red / quantum * quantum,
                green / quantum * quantum,
                blue / quantum * quantum);
        }
    }

    private record RgbaSample(int red, int green, int blue, int alpha)
    {
        RgbaSample
        {
            red = clamp(red, 0, 255);
            green = clamp(green, 0, 255);
            blue = clamp(blue, 0, 255);
            alpha = clamp(alpha, 0, 255);
        }

        Rgb rgb()
        {
            return new Rgb(red, green, blue);
        }

        int argb()
        {
            return alpha << 24 | red << 16 | green << 8 | blue;
        }

    }

    private static final class Raster
    {
        private static final Raster EMPTY = new Raster(0, 0, new int[0], "");

        private final int width;
        private final int height;
        private final int[] pixels;
        private final String identity;

        Raster(int width, int height, int[] pixels, String identity)
        {
            this.width = width;
            this.height = height;
            this.pixels = Arrays.copyOf(pixels, pixels.length);
            this.identity = identity;
        }

        int width()
        {
            return width;
        }

        int height()
        {
            return height;
        }

        String identity()
        {
            return identity;
        }

        boolean empty()
        {
            return width <= 0 || height <= 0 || pixels.length == 0;
        }

        RgbaSample pixel(int x, int y)
        {
            int argb = pixels[clamp(y, 0, height - 1) * width + clamp(x, 0, width - 1)];
            return new RgbaSample(
                argb >> 16 & 0xff,
                argb >> 8 & 0xff,
                argb & 0xff,
                argb >>> 24);
        }
    }
}
