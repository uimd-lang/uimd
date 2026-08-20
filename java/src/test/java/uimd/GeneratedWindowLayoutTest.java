package uimd;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;
import java.util.List;

import org.junit.jupiter.api.Test;

final class GeneratedWindowLayoutTest
{
    private static final int LEFT_WIDTH = 4;
    private static final int RIGHT_WIDTH = 3;
    private static final int SOURCE_HEIGHT = 2;
    private static final int SOURCE_SEPARATOR = 1;
    private static final int EXPANDED_HEIGHT = 6;
    private static final int REUSABLE_CHILD_SOURCE_WIDTH = 3;
    private static final int REUSABLE_HOST_WIDTH = 12;
    private static final int FAILURE_EXIT_CODE = 1;

    @Test
    void adjacentCellsUseRuntimeSeparatorsInsteadOfSourceGridOffsets()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Adjacent");
        Label left = window.addElement(new Label("left", "left"));
        Label right = window.addElement(new Label("right", "right"));
        window.setGeneratedLayout(List.of(
            entry("left", new Rect(0, 0, LEFT_WIDTH, SOURCE_HEIGHT)),
            entry("right", new Rect(
                0,
                LEFT_WIDTH + SOURCE_SEPARATOR,
                RIGHT_WIDTH,
                SOURCE_HEIGHT))));

        assertEquals(
            new Size(LEFT_WIDTH + RIGHT_WIDTH, SOURCE_HEIGHT),
            GeneratedWindowRuntime.generatedWindowContentSize(window));

        GeneratedWindowRuntime.renderGeneratedWindowContent(
            window,
            new Size(LEFT_WIDTH + RIGHT_WIDTH, SOURCE_HEIGHT),
            0,
            false);

        assertEquals(new Rect(0, 0, LEFT_WIDTH, SOURCE_HEIGHT), left.frame());
        assertEquals(new Rect(0, LEFT_WIDTH, RIGHT_WIDTH, SOURCE_HEIGHT), right.frame());
    }

    @Test
    void expandedCellReceivesRemainingSpaceBeforeTrailingFixedCell()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Expanded");
        Label expanded = window.addElement(new Label("expanded", "expanded"));
        Label fixed = window.addElement(new Label("fixed", "fixed"));
        GeneratedLayoutEntry first = entry(
            "expanded",
            new Rect(0, 0, LEFT_WIDTH, SOURCE_HEIGHT));
        first.setCellHeight(AxisDimension.expanded());
        first.setHeight(AxisDimension.expanded());
        GeneratedLayoutEntry second = entry(
            "fixed",
            new Rect(SOURCE_HEIGHT + SOURCE_SEPARATOR, 0, LEFT_WIDTH, 1));
        second.setCellHeight(AxisDimension.auto(1));
        second.setHeight(AxisDimension.auto(1));
        window.setGeneratedLayout(List.of(first, second));

        GeneratedWindowRuntime.renderGeneratedWindowContent(
            window,
            new Size(LEFT_WIDTH, EXPANDED_HEIGHT),
            0,
            false);

        assertEquals(new Rect(0, 0, LEFT_WIDTH, EXPANDED_HEIGHT - 1), expanded.frame());
        assertEquals(new Rect(EXPANDED_HEIGHT - 1, 0, LEFT_WIDTH, 1), fixed.frame());
    }

    @Test
    void reusableChildUsesFullscreenDistributionForFixedColumns()
    {
        GeneratedWindowBase child = new GeneratedWindowBase("Reusable child");
        Label first = child.addElement(new Label("first", "first"));
        Label second = child.addElement(new Label("second", "second"));
        Label third = child.addElement(new Label("third", "third"));
        GeneratedLayoutEntry firstEntry = entry(
            "first",
            new Rect(0, 0, REUSABLE_CHILD_SOURCE_WIDTH, 1));
        GeneratedLayoutEntry secondEntry = entry(
            "second",
            new Rect(
                0,
                REUSABLE_CHILD_SOURCE_WIDTH + SOURCE_SEPARATOR,
                REUSABLE_CHILD_SOURCE_WIDTH,
                1));
        GeneratedLayoutEntry thirdEntry = entry(
            "third",
            new Rect(
                0,
                (REUSABLE_CHILD_SOURCE_WIDTH + SOURCE_SEPARATOR) * 2,
                REUSABLE_CHILD_SOURCE_WIDTH,
                1));
        firstEntry.setWidth(AxisDimension.expanded());
        secondEntry.setWidth(AxisDimension.expanded());
        thirdEntry.setWidth(AxisDimension.expanded());
        child.setGeneratedLayout(List.of(firstEntry, secondEntry, thirdEntry));
        ReusableElement reusable = new ReusableElement("reusable");
        reusable.setChild(child);

        reusable.render(
            new Size(REUSABLE_HOST_WIDTH, 1),
            new ElementRenderState());

        int resolvedWidth = REUSABLE_HOST_WIDTH / 3;
        assertEquals(new Rect(0, 0, resolvedWidth, 1), first.frame());
        assertEquals(new Rect(0, resolvedWidth, resolvedWidth, 1), second.frame());
        assertEquals(new Rect(0, resolvedWidth * 2, resolvedWidth, 1), third.frame());
    }

    @Test
    void editedComboBoxDropdownOverlaysFollowingSiblingRows()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Combo overlay");
        ComboBox theme = window.addElement(new ComboBox("theme", List.of("Light", "Dark")));
        window.addElement(new Label("under", "Mode"));
        GeneratedLayoutEntry comboEntry = entry("theme", new Rect(0, 0, 8, 1));
        comboEntry.setType("combobox");
        window.setGeneratedLayout(List.of(
            comboEntry,
            entry("under", new Rect(2, 0, 8, 1))));

        List<List<TerminalCell>> content = GeneratedWindowRuntime.renderGeneratedWindowContent(
            window,
            new Size(8, 3),
            0,
            true);

        assertEquals(new Rect(0, 0, 8, 3), theme.frame());
        assertEquals(" ", content.get(1).get(0).text());
        assertEquals(" ", content.get(1).get(1).text());
        assertEquals("L", content.get(1).get(2).text());
    }

    @Test
    void transparentAndPartialAlphaElementColorsCompositeOverPaintedCell()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Compositing");
        window.setGeneratedWindowStyle(Style.fromProperties("background", "#102040"));
        Label label = window.addElement(new Label("label", "X"));
        label.setStyle(Style.fromProperties(
            "background", "transparent",
            "color", "#ffffff80"));
        window.setGeneratedLayout(List.of(entry("label", new Rect(0, 0, 1, 1))));

        TerminalCell cell = GeneratedWindowRuntime.renderGeneratedWindowContent(
            window,
            new Size(1, 1),
            -1).get(0).get(0);
        Color expectedBackground = new Color("#102040");

        assertEquals(expectedBackground, cell.background());
        assertEquals(new Color("#ffffff80").blendOver(expectedBackground), cell.foreground());
    }

    @Test
    void emptyLayoutEntryKeepsCellBackgroundTexture()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Texture");
        GeneratedLayoutEntry texture = entry("", new Rect(0, 0, LEFT_WIDTH, 1));
        texture.setCellStyle(Style.fromProperties(
            "background", "#102040",
            "background-texture", "ab",
            "background-texture-color", "#ffffff"));
        window.setGeneratedLayout(List.of(texture));

        List<TerminalCell> row = GeneratedWindowRuntime.renderGeneratedWindowContent(
            window,
            new Size(LEFT_WIDTH, 1),
            -1).get(0);
        StringBuilder text = new StringBuilder();
        for (TerminalCell cell : row)
        {
            text.append(cell.text());
            assertEquals(new Color("#102040"), cell.background());
            assertEquals(new Color("#ffffff"), cell.foreground());
        }
        assertEquals("abab", text.toString());
    }

    @Test
    void generatedMetadataLookupUsesTheReadOnlyCanonicalCollection()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Metadata");
        GeneratedElementMetadata visible = new GeneratedElementMetadata(
            "visible",
            "Visible element",
            true);
        window.setMcpMetadata(
            true,
            "MetadataWindow",
            "# Source",
            "Metadata test",
            List.of(visible));

        assertSame(visible, window.mcpElementMetadata("visible"));
        assertNull(window.mcpElementMetadata("missing"));
        assertNull(window.mcpElementMetadata(null));
        assertEquals("MetadataWindow", window.runtimeOptions().className());
        assertThrows(
            UnsupportedOperationException.class,
            () -> window.mcpElementMetadata().clear());
    }

    @Test
    void resolvedSizeAndPublicRenderHelpersUseTheSuppliedSurface()
    {
        Size surfaceSize = new Size(5, 3);
        Color background = new Color("#808080");
        GeneratedWindowBase window = new GeneratedWindowBase("Render helper");
        window.setMode("fullscreen");
        window.setGeneratedWindowStyle(Style.fromProperties(
            "background",
            background.toString()));
        Label label = window.addElement(new Label("label", "X"));
        label.setStyle(Style.fromProperties("background", background.toString()));
        GeneratedLayoutEntry layout = entry("label", new Rect(0, 0, 1, 1));
        layout.setWidth(AxisDimension.expanded());
        layout.setHeight(AxisDimension.expanded());
        layout.setCellWidth(AxisDimension.expanded());
        layout.setCellHeight(AxisDimension.expanded());
        window.setGeneratedLayout(List.of(layout));
        TerminalBuffer buffer = new TerminalBuffer(surfaceSize.width(), surfaceSize.height());

        assertEquals(
            surfaceSize,
            GeneratedWindowRuntime.generatedWindowResolvedContentSize(
                window,
                surfaceSize));
        GeneratedWindowRuntime.renderGeneratedWindow(window, buffer);

        assertEquals("X", buffer.cell(0, 0).text());
        assertEquals(background, buffer.cell(0, 0).background());
        GeneratedWindowRuntime.dimGeneratedWindowModalBackground(buffer);
        assertEquals(new Color("#404040"), buffer.cell(0, 0).background());
    }

    @Test
    void advancedContentRenderPropagatesTheCanonicalVerticalClip()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Clip helper");
        ClipRecordingElement element = window.addElement(
            new ClipRecordingElement("clipped"));
        window.setGeneratedLayout(List.of(
            entry("clipped", new Rect(0, 0, 3, 3))));

        GeneratedWindowRuntime.renderGeneratedWindowContent(
            window,
            new Size(3, 3),
            -1,
            false,
            null,
            null,
            null,
            null,
            1,
            2,
            true,
            false,
            false,
            false);

        assertEquals(1, element.clipTop);
        assertEquals(2, element.clipBottom);
    }

    @Test
    void generatedAppErrorHelpersReportKnownAndUnknownFailures()
    {
        ByteArrayOutputStream errors = new ByteArrayOutputStream();
        PrintStream previous = System.err;
        try
        {
            System.setErr(new PrintStream(errors, true, StandardCharsets.UTF_8));
            assertEquals(
                FAILURE_EXIT_CODE,
                GeneratedWindowRuntime.reportGeneratedAppUnhandledException(
                    new IllegalStateException("known failure")));
            assertEquals(
                FAILURE_EXIT_CODE,
                GeneratedWindowRuntime.reportGeneratedAppUnhandledException());
        }
        finally
        {
            System.setErr(previous);
        }

        String output = errors.toString(StandardCharsets.UTF_8);
        assertTrue(output.contains("uimd: error: known failure"));
        assertTrue(output.contains("uimd: error: unhandled Java exception"));
    }

    private static GeneratedLayoutEntry entry(String name, Rect source)
    {
        return new GeneratedLayoutEntry()
            .setName(name)
            .setType("label")
            .setRelative(new Rect(0, 0, source.width(), source.height()))
            .setSourceCell(source)
            .setWidth(AxisDimension.auto(source.width()))
            .setHeight(AxisDimension.auto(source.height()))
            .setCellWidth(AxisDimension.auto(source.width()))
            .setCellHeight(AxisDimension.auto(source.height()))
            .setCharsSize(new Size(source.width(), source.height()))
            .setCellCharsSize(new Size(source.width(), source.height()));
    }

    private static final class ClipRecordingElement extends Element
    {
        private Integer clipTop;
        private Integer clipBottom;

        ClipRecordingElement(String name)
        {
            super(name);
        }

        @Override
        public List<List<TerminalCell>> render(Size size, ElementRenderState state)
        {
            clipTop = state.clipTop();
            clipBottom = state.clipBottom();
            return super.render(size, state);
        }
    }
}
