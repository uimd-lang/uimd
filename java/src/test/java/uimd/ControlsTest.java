package uimd;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.List;
import java.util.Map;

import org.junit.jupiter.api.Test;

final class ControlsTest
{
    @Test
    void versionAndClipboardExposeTheCanonicalPublicRuntimeContracts()
    {
        assertTrue(Version.runtimeVersion().matches("\\d+\\.\\d+\\.\\d+"));

        assertFalse(RuntimeClipboard.copyTextToClipboard(""));
        assertEquals("", RuntimeClipboard.runtimeClipboardText());
        RuntimeClipboard.copyTextToClipboard("shared clipboard");
        assertEquals("shared clipboard", RuntimeClipboard.runtimeClipboardText());
    }

    @Test
    void scopedRenderTimeOverrideRestoresNestedStateExactlyOnce()
    {
        RenderHelpers.setRenderTimeOverrideMs(11L);
        ScopedRenderTimeOverride outer = new ScopedRenderTimeOverride(22L);
        assertEquals(22L, RenderHelpers.renderTimeOverrideMs());
        try (ScopedRenderTimeOverride nested = new ScopedRenderTimeOverride(33L))
        {
            assertEquals(33L, RenderHelpers.renderTimeOverrideMs());
            nested.restore();
        }
        assertEquals(22L, RenderHelpers.renderTimeOverrideMs());
        outer.restore();
        outer.close();
        assertEquals(11L, RenderHelpers.renderTimeOverrideMs());
        RenderHelpers.setRenderTimeOverrideMs(null);
    }

    @Test
    void colorsAndStylesExposeTheCanonicalPublicStateAndMergeContract()
    {
        Color unset = new Color();
        assertEquals(Color.Kind.UNSET, unset.kind());
        assertFalse(unset.isSet());
        assertEquals("", unset.value());
        assertTrue(unset.rgba().isEmpty());

        Color transparent = Color.transparent();
        assertEquals(Color.Kind.TRANSPARENT, transparent.kind());
        assertTrue(transparent.isSet());
        assertTrue(transparent.isTransparent());
        assertTrue(transparent.rgba().isEmpty());

        Color shortRgb = new Color("#aBc");
        assertEquals(Color.Kind.RGB, shortRgb.kind());
        assertEquals(new Rgba(0xaa, 0xbb, 0xcc, 0xff), shortRgb.rgba().orElseThrow());
        assertEquals("#aabbcc", shortRgb.toString());

        Color shortRgba = new Color("#f0c8");
        assertEquals(new Rgba(0xff, 0x00, 0xcc, 0x88), shortRgba.rgba().orElseThrow());
        assertEquals("#ff00cc", shortRgba.toString());

        Color named = Color.named("BLUE");
        assertEquals(Color.Kind.NAMED, named.kind());
        assertEquals("blue", named.value());
        assertEquals("blue", named.toString());

        Color constructed = Color.rgb(1, 2, 3, 4);
        assertEquals(Color.Kind.RGB, constructed.kind());
        assertEquals("", constructed.value());
        assertEquals("#010203", constructed.toString());
        assertEquals(new Color("#01020304"), constructed);
        assertThrows(IllegalArgumentException.class, () -> new Color("#12"));
        assertThrows(IllegalArgumentException.class, () -> new Color("#xyz"));

        Style base = Style.fromProperties("color", "#111111", "padding", "2");
        Style override = Style.fromProperties("background", "#222222", "gap", "3");
        Style merged = base.merged(override);
        assertNotSame(base, merged);
        assertEquals(new Color("#111111"), merged.color());
        assertEquals(new Color("#222222"), merged.background());
        assertEquals(2, merged.padding());
        assertEquals(3, merged.gap());
        assertNull(base.background());
        assertNull(base.gap());

        base.merge(override);
        assertEquals(new Color("#222222"), base.background());
        assertEquals(3, base.gap());
    }

    @Test
    void infoLabelAndTextGradientUseCanonicalDefaultsAndRespectOverrides()
    {
        TextGradient defaults = new TextGradient();
        assertEquals(70, defaults.intervalMs());
        assertEquals(1, defaults.step());
        assertEquals(1, defaults.segmentSize());

        InfoLabel label = new InfoLabel("info", "ab");
        List<List<TerminalCell>> rendered = RenderHelpers.withRenderTimeOverride(
            0L,
            () -> label.render(new Size(2, 1), new ElementRenderState()));
        assertEquals(new Color("#ffffff66"), rendered.get(0).get(0).foreground());
        assertEquals(new Color("#ffffff88"), rendered.get(0).get(1).foreground());

        TextGradient override = new TextGradient();
        override.colors().add(new Color("#123456"));
        Style style = new Style();
        style.setTextColorGradient(override);
        label.setStyle(style);
        List<List<TerminalCell>> overridden = RenderHelpers.withRenderTimeOverride(
            0L,
            () -> label.render(new Size(2, 1), new ElementRenderState()));
        assertEquals(new Color("#123456"), overridden.get(0).get(0).foreground());
        assertEquals(new Color("#123456"), overridden.get(0).get(1).foreground());
    }

    @Test
    void focusedButtonUsesTheCanonicalDecorationGeometry()
    {
        Button button = new Button("zero", "0");
        ElementRenderState state = new ElementRenderState();
        state.setFocused(true);

        List<List<TerminalCell>> rendered = button.render(new Size(8, 1), state);

        assertEquals(" [ 0  ] ", RenderHelpers.renderedText(rendered).get(0));
    }

    @Test
    void textInputSupportsEditingSelectionAndEscapeSnapshots()
    {
        TextInput input = new TextInput("input", "hello", 0);
        input.setCursor(5);
        assertTrue(input.handleKey("Backspace"));
        assertEquals("hell", input.value());
        input.selectRange(1, 3);
        input.insertText("i");
        assertEquals("hil", input.value());
        assertEquals(2, input.cursor());
    }

    @Test
    void textSelectionUsesOneAnchorAndCursorAndNumberDisplayUsesTheCanonicalName()
    {
        TextInput input = new TextInput("input", "abcd", 0);
        input.setCursor(2);
        assertEquals(2, input.selectionStart());
        assertEquals(2, input.selectionEnd());

        input.selectRange(1, 4);
        assertEquals(1, input.selectionStart());
        assertEquals(4, input.selectionEnd());
        assertEquals("bcd", input.selectedText());
        assertTrue(input.handleKey("Left"));
        assertEquals(1, input.cursor());
        assertEquals(1, input.selectionStart());
        assertEquals(1, input.selectionEnd());

        input.selectRange(4, 1);
        assertEquals(1, input.selectionStart());
        assertEquals(4, input.selectionEnd());
        input.insertText("X");
        assertEquals("aX", input.value());
        assertEquals(2, input.cursor());

        NumberInput number = new NumberInput("number", 12.5, 0.5);
        assertEquals("12.5", number.displayValue());
        assertEquals(number.displayValue(), number.displayText());
    }

    @Test
    void labelAndTextInputMapMousePointsToSelectionRanges()
    {
        Label label = new Label("author", "Ada Lovelace");
        label.setStyle(Style.fromProperties("user-select", "text"));
        int labelStart = label.textPositionFromPoint(0, 0, new Size(12, 1));
        int labelEnd = label.textPositionFromPoint(0, 3, new Size(12, 1));
        label.selectRange(labelStart, labelEnd);
        assertEquals("Ada", label.selectedText());

        TextInput input = new TextInput("name", "Grace Hopper", 0);
        int inputStart = input.cursorForPoint(0, 0, new Size(12, 1));
        int inputEnd = input.cursorForPoint(0, 5, new Size(12, 1));
        input.selectRange(inputStart, inputEnd);
        assertEquals("Grace", input.selectedText());
    }

    @Test
    void numberComboListAndCheckboxMatchSharedStateTransitions()
    {
        NumberInput number = new NumberInput("number", 0.0, 2.0);
        number.beginEdit();
        number.handleKey("4");
        number.handleKey("Enter");
        assertEquals(4.0, number.value());

        ComboBox combo = new ComboBox("combo", List.of("A", "B", "C"));
        assertTrue(combo.handleKey("Down"));
        assertEquals("B", combo.selectedText());

        ListBox list = new ListBox("list", List.of("A", "B", "C"), true);
        list.showActiveItem();
        list.handleKey("Down");
        list.handleKey("Enter");
        assertEquals(List.of("B"), list.selectedValues());

        CheckBox checkBox = new CheckBox("check", "Check", false);
        assertTrue(checkBox.handleKey("Enter"));
        assertTrue(checkBox.checked());
        assertFalse(checkBox.handleKey("Left"));
    }

    @Test
    void comboAndListCollectionsSelectionsAndExplicitScrollingMatchThePublicContract()
    {
        ComboBox emptyCombo = new ComboBox("empty");
        assertEquals(0, emptyCombo.selectedIndex());
        assertEquals("", emptyCombo.selectedText());
        assertThrows(UnsupportedOperationException.class, () -> emptyCombo.options().add("x"));

        ComboBox combo = new ComboBox("combo", List.of("A", "B"));
        combo.setSelectedIndex(1);
        assertEquals("B", combo.selectedText());

        ListBox list = new ListBox("list", List.of("A", "B", "C", "D"));
        list.setMultiple(true);
        list.setSelectedIndices(List.of(2, 0, 2));
        assertEquals(List.of(2, 0), list.selectedIndices());
        assertEquals(List.of("C", "A"), list.selectedValues());
        assertThrows(UnsupportedOperationException.class, () -> list.options().clear());
        assertThrows(UnsupportedOperationException.class, () -> list.selectedIndices().clear());

        list.scrollBy(2, 2);
        assertEquals(2, list.scrollOffset());
        list.scrollBy(-1, 2);
        assertEquals(1, list.scrollOffset());
        assertTrue(list.scrollLines(-1));
        assertEquals(2, list.scrollOffset());
    }

    @Test
    void generatedMultipleListBoxPreservesOneExplicitInitialSelection()
    {
        ListBox list = new ListBox("list", List.of("A", "B", "C"));
        list.setMultiple(true);
        list.setSelectedValues(List.of("B"));

        assertEquals(List.of("B"), list.selectedValues());
        assertEquals(1, list.activeIndex());
    }

    @Test
    void textRenderingUsesVisualCellsAndWordWrapping()
    {
        Style style = Style.fromProperties("text-align", "right");
        List<List<TerminalCell>> rendered = RenderHelpers.renderPlainText("alpha beta", 6, 2, style);
        assertEquals(" alpha", RenderHelpers.renderedText(rendered).get(0));
        assertEquals("  beta", RenderHelpers.renderedText(rendered).get(1));
        assertEquals("?", RenderHelpers.safeTerminalCellText("界"));
    }

    @Test
    void geometryAndTextVisualHelpersExposeTheCanonicalPublicContract()
    {
        Rect rect = new Rect(2, 3, 4, 5);
        assertFalse(rect.empty());
        assertEquals(new Point(2, 3), rect.origin());
        assertEquals(new Size(4, 5), rect.size());
        assertTrue(new Rect(0, 0, 0, 1).empty());
        assertTrue(new Rect(0, 0, 1, -1).empty());

        assertEquals(new VisualGlyph(" ", -1, -1), new VisualGlyph());
        assertEquals(4, RenderHelpers.TEXT_TAB_WIDTH);
        assertEquals(4, RenderHelpers.tabSpacesForColumn(-1));
        assertEquals(3, RenderHelpers.tabSpacesForColumn(1));
        assertEquals(4, RenderHelpers.tabSpacesForColumn(4));

        assertTrue(RenderHelpers.isUnsafeTerminalCodepoint('\n'));
        assertTrue(RenderHelpers.isUnsafeTerminalCodepoint(0x0301));
        assertTrue(RenderHelpers.isUnsafeTerminalCodepoint(0x754c));
        assertFalse(RenderHelpers.isUnsafeTerminalCodepoint('A'));
        assertEquals(" ", RenderHelpers.safeTerminalCellText(""));
        assertEquals(" ", RenderHelpers.safeTerminalCellText("\t"));
        assertEquals("?", RenderHelpers.safeTerminalCellText("\u0301"));
        assertEquals("A", RenderHelpers.safeTerminalCellText("Alpha"));

        assertEquals(
            List.of(
                new VisualGlyph("a", 7, 8),
                new VisualGlyph(" ", 8, 9),
                new VisualGlyph("b", 9, 10),
                new VisualGlyph("?", 11, 12)),
            RenderHelpers.visualGlyphs("a\tb\r\u0301", 2, 7));
        assertEquals(
            List.of(new VisualGlyph("?", 5, 7)),
            RenderHelpers.visualGlyphs("\ud83d\ude00", 0, 5));
        assertEquals("  A", RenderHelpers.visibleText("\tA", 2));
        assertEquals(3, RenderHelpers.visibleWidth("\tA", 2));
    }

    @Test
    void scrollViewStartsAtTopAndPreservesLineLevelScrollSemantics()
    {
        ScrollView scrollView = new ScrollView("scroll", 0);
        scrollView.setFrame(new Rect(0, 0, 4, 2));
        scrollView.addChild(RenderHelpers.renderPlainText("one\ntwo\nthree\nfour", 4, 4, new Style()));

        List<List<TerminalCell>> top = scrollView.render(
            new Size(4, 2),
            new ElementRenderState());
        assertEquals("one ", RenderHelpers.renderedText(top).get(0));
        assertEquals(2, scrollView.viewOffset());

        assertTrue(scrollView.handleWheel(-1, new Size(4, 2)));
        List<List<TerminalCell>> moved = scrollView.render(
            new Size(4, 2),
            new ElementRenderState());
        assertEquals("two^", RenderHelpers.renderedText(moved).get(0));
        assertEquals("thrv", RenderHelpers.renderedText(moved).get(1));

        assertTrue(scrollView.scrollToBottom(new Size(4, 2)));
        assertEquals(0, scrollView.viewOffset());
        assertTrue(scrollView.autoScroll());
    }

    @Test
    void scrollViewAppliesViewportPaddingExactlyOnce()
    {
        final int viewportWidth = 8;
        final int viewportHeight = 6;
        final int childHeight = 3;
        final int gap = 1;
        final int padding = 1;
        final int expectedTopOffset = 3;
        Size viewport = new Size(viewportWidth, viewportHeight);
        ScrollView scrollView = new ScrollView("scroll", gap);
        scrollView.setFrame(new Rect(0, 0, viewportWidth, viewportHeight));
        scrollView.setStyle(Style.fromProperties("padding", Integer.toString(padding)));
        scrollView.addChild(RenderHelpers.renderPlainText(
            "one\ntwo\nthree",
            viewportWidth,
            childHeight,
            new Style()));
        scrollView.addChild(RenderHelpers.renderPlainText(
            "four\nfive\nsix",
            viewportWidth,
            childHeight,
            new Style()));

        assertTrue(scrollView.scrollToTop(viewport));
        assertEquals(expectedTopOffset, scrollView.viewOffset());

        scrollView.render(viewport, new ElementRenderState());
        assertEquals(expectedTopOffset, scrollView.viewOffset());

        scrollView.childViews(viewport);
        assertEquals(expectedTopOffset, scrollView.viewOffset());

        assertTrue(scrollView.scrollLines(-1, viewport));
        assertEquals(expectedTopOffset - 1, scrollView.viewOffset());
    }

    @Test
    void scrollViewOverflowIndicatorsInferVisibleRowForeground()
    {
        Color foreground = new Color("#d1d5db");
        Size viewport = new Size(4, 2);
        ScrollView scrollView = new ScrollView("scroll", 0);
        scrollView.setFrame(new Rect(0, 0, viewport.width(), viewport.height()));
        List<List<TerminalCell>> child = RenderHelpers.renderPlainText(
            "one\ntwo\nthree\nfour",
            viewport.width(),
            4,
            Style.fromProperties("color", foreground.text()));
        child.get(1).get(viewport.width() - 1).setForeground(null);
        child.get(2).get(viewport.width() - 1).setForeground(new Color("transparent"));
        scrollView.addChild(child);

        scrollView.render(viewport, new ElementRenderState());
        assertTrue(scrollView.scrollLines(-1, viewport));
        List<List<TerminalCell>> rendered = scrollView.render(viewport, new ElementRenderState());

        assertEquals("^", rendered.get(0).get(viewport.width() - 1).text());
        assertEquals(foreground, rendered.get(0).get(viewport.width() - 1).foreground());
        assertEquals("v", rendered.get(1).get(viewport.width() - 1).text());
        assertEquals(foreground, rendered.get(1).get(viewport.width() - 1).foreground());
    }

    @Test
    void frameBufferViewMatchesMultiTargetPanDimAndInteractionContract()
    {
        Color fallbackForeground = new Color("#d1d5db");
        Color fallbackBackground = new Color("#0b1018");
        FrameBufferView view = new FrameBufferView("frame");
        view.setStyle(Style.fromProperties(
            "color", fallbackForeground.text(),
            "background", fallbackBackground.text()));
        view.setFrames(
            Map.of(
                "left", List.of(List.of(
                    new FrameBufferCell("AB", "#not-a-color", "#010203"),
                    new FrameBufferCell("L", "#ffffff", "#000000"))),
                "right", List.of(List.of(
                    new FrameBufferCell("R", "#38bdf8", "#111827"),
                    new FrameBufferCell("Z", "", "")))),
            List.of("left", "right"));

        List<List<TerminalCell>> split = view.render(new Size(4, 1), new ElementRenderState());
        assertTrue(view.focusable());
        assertEquals("ALRZ", RenderHelpers.renderedText(split).get(0));
        assertEquals(fallbackForeground, split.get(0).get(0).foreground());
        assertEquals(new Color("#010203"), split.get(0).get(0).background());
        assertEquals(fallbackForeground, split.get(0).get(3).foreground());
        assertEquals(fallbackBackground, split.get(0).get(3).background());

        view.setFrames(
            Map.of("panel", List.of(
                List.of(
                    new FrameBufferCell("0", "#ffffff", "#000000"),
                    new FrameBufferCell("1", "#ffffff", "#000000"),
                    new FrameBufferCell("2", "#ffffff", "#000000")),
                List.of(
                    new FrameBufferCell("3", "#ffffff", "#000000"),
                    new FrameBufferCell("4", "#ffffff", "#000000"),
                    new FrameBufferCell("5", "#ffffff", "#000000")))),
            List.of("panel"),
            Map.of("panel", new FrameBufferRegion(0, 1, 2, 2)));
        view.setPan(0, 1);
        view.setDimmed(true);
        List<List<TerminalCell>> panned = view.render(new Size(4, 2), new ElementRenderState());
        assertEquals(" 12 ", RenderHelpers.renderedText(panned).get(0));
        assertEquals(" 45 ", RenderHelpers.renderedText(panned).get(1));
        assertEquals(new Color("#999999"), panned.get(0).get(1).foreground());

        view.setDimmed(false);
        view.setFocusStyle(Style.fromProperties("color", "#facc15"));
        view.setEditStyle(Style.fromProperties("color", "#22c55e"));
        view.setFrames(
            Map.of("panel", List.of()),
            List.of("panel"),
            Map.of("panel", new FrameBufferRegion(0, 0, 5, 3)));
        view.setInteractionState(true, false);
        List<List<TerminalCell>> focused = view.render(new Size(5, 3), new ElementRenderState());
        assertEquals("┌─ ─┐", RenderHelpers.renderedText(focused).get(0));
        assertEquals(new Color("#facc15"), focused.get(0).get(0).foreground());
        view.setInteractionState(true, true);
        List<List<TerminalCell>> editing = view.render(new Size(5, 3), new ElementRenderState());
        assertEquals(new Color("#22c55e"), editing.get(2).get(4).foreground());

        view.clearFrames();
        assertEquals("     ", RenderHelpers.renderedText(
            view.render(new Size(5, 1), new ElementRenderState())).get(0));
    }

    @Test
    void scrollViewExposesTheCanonicalPublicWindowAndVisibilityContract()
    {
        Size viewport = new Size(4, 3);
        ScrollView scrollView = new ScrollView("scroll", 1);
        scrollView.setFrame(new Rect(0, 0, viewport.width(), viewport.height()));
        scrollView.addChild(RenderHelpers.renderPlainText("head", 4, 1, new Style()));
        Label first = scrollView.addChild(new Label("first", "one"));
        first.setFrame(new Rect(0, 0, 4, 2));
        Label second = scrollView.addChild(new Label("second", "two"));
        second.setFrame(new Rect(0, 0, 4, 2));

        assertEquals(List.of(1, 2, 2), scrollView.cachedChildHeights());
        scrollView.invalidateHeightCache();
        assertEquals(List.of(1, 2, 2), scrollView.cachedChildHeights());
        ScrollViewContentWindow top = scrollView.contentWindow(viewport);
        assertEquals(viewport, top.viewport());
        assertEquals(0, top.actualSkip());
        assertEquals(4, top.naturalSkip());
        assertEquals(1, top.gap());

        assertTrue(scrollView.ensureChildVisible(second, viewport));
        assertEquals(4, scrollView.contentWindow(viewport).actualSkip());
        assertTrue(scrollView.ensureChildVisible(first, viewport));
        assertEquals(2, scrollView.contentWindow(viewport).actualSkip());

        assertFalse(scrollView.canScrollUp(viewport));
        assertTrue(scrollView.canScrollDown(viewport));
        assertTrue(scrollView.scrollTo(1, viewport));
        assertTrue(scrollView.canScrollUp(viewport));
        assertFalse(scrollView.canScrollDown(viewport));
        assertTrue(scrollView.scrollHorizontal(2));
        assertEquals(2, scrollView.hViewOffset());
        assertEquals(scrollView.horizontalViewOffset(), scrollView.hViewOffset());
        assertTrue(scrollView.handleKey("End", viewport));
    }

    @Test
    void applicationWindowAndElementOwnershipMatchThePublicCoreContract()
    {
        Application application = new Application();
        assertNull(application.activeWindow());
        Window firstWindow = application.open(new Window("First"));
        Window secondWindow = application.open(new Window("Second"));
        assertSame(secondWindow, application.activeWindow());
        assertEquals(List.of(firstWindow, secondWindow), application.windows());
        assertThrows(UnsupportedOperationException.class, () -> application.windows().clear());
        assertThrows(IllegalArgumentException.class, () -> application.open(null));

        Control root = firstWindow.addElement(new Control("root"));
        Control child = root.addChild(new Control("child"));
        assertSame(root, child.parent());
        assertNotEquals(root.identity(), child.identity());
        assertThrows(UnsupportedOperationException.class, () -> root.children().clear());
        assertThrows(IllegalArgumentException.class, () -> root.addChild(null));
        assertThrows(IllegalArgumentException.class, () -> firstWindow.addElement(null));
        root.clearChildElements();
        assertNull(child.parent());
        assertTrue(root.children().isEmpty());
    }

    @Test
    void runtimeFocusabilityUsesTheCanonicalTypeFilterWithoutChangingPublicFlags()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Focus types");
        Label label = window.addElement(new Label("label", "Label"));
        Image image = window.addElement(new Image("image"));
        MessageTable table = window.addElement(new MessageTable("table"));
        FrameBufferView frameBuffer = window.addElement(new FrameBufferView("frame"));
        Button button = window.addElement(new Button("button", "Button"));

        assertTrue(label.focusable());
        assertTrue(image.focusable());
        assertTrue(table.focusable());
        assertTrue(frameBuffer.focusable());
        assertFalse(GeneratedWindowRuntime.isFocusable(label));
        assertFalse(GeneratedWindowRuntime.isFocusable(image));
        assertFalse(GeneratedWindowRuntime.isFocusable(table));
        assertFalse(GeneratedWindowRuntime.isFocusable(frameBuffer));
        assertTrue(GeneratedWindowRuntime.isFocusable(button));
        assertEquals(List.of(button), GeneratedWindowRuntime.focusableElements(window));
    }

    @Test
    void publicLayoutResolverMatchesFixedExpandedFitAndFlattenSemantics()
    {
        final int layoutWidth = 20;
        final int layoutHeight = 8;
        final int fixedWidth = 6;
        final int gap = 2;
        LayoutCell root = new LayoutCell()
            .setName("root")
            .setWidth(AxisDimension.fixed(layoutWidth))
            .setHeight(AxisDimension.fixed(layoutHeight))
            .setGap(gap)
            .setDirection(LayoutDirection.ROW)
            .addChild(new LayoutCell()
                .setName("fixed")
                .setWidth(AxisDimension.fixed(fixedWidth))
                .setHeight(AxisDimension.expanded())
                .addElement(new LayoutElement()
                    .setName("label")
                    .setRelative(new Rect(1, 2, 3, 1))))
            .addChild(new LayoutCell()
                .setName("fill")
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded()));
        LayoutResolver resolver = new LayoutResolver();
        ResolvedCell resolved = resolver.resolve(root, new Rect(4, 5, 0, 0));

        assertEquals(new Rect(4, 5, layoutWidth, layoutHeight), resolved.rect());
        assertEquals(fixedWidth, resolved.children().get(0).rect().width());
        assertEquals(layoutHeight, resolved.children().get(0).rect().height());
        assertEquals(fixedWidth + gap + 5, resolved.children().get(1).rect().col());
        assertEquals(layoutWidth - fixedWidth - gap, resolved.children().get(1).rect().width());
        assertSame(resolved.children().get(0), LayoutResolver.findCellByName(resolved, "fixed"));
        assertNull(LayoutResolver.findCellByName(resolved, "missing"));
        assertEquals(
            new Rect(5, 7, 3, 1),
            LayoutResolver.findElementByName(resolved, "label").rect());
        assertEquals(List.of("root", "fixed", "fill"), LayoutResolver.flattenCells(resolved)
            .stream().map(FlatResolvedCell::name).toList());
        assertEquals("fixed", LayoutResolver.flattenElements(resolved).get(0).cellName());

        final int contentWidth = 7;
        final int contentHeight = 2;
        final int padding = 1;
        LayoutCell fit = new LayoutCell()
            .setName("fit")
            .setWidth(AxisDimension.fitContent())
            .setHeight(AxisDimension.fitContent())
            .setFitContentSize(new Size(contentWidth, contentHeight))
            .setPadding(padding);
        assertEquals(
            new Rect(0, 0, contentWidth + padding * 2, contentHeight + padding * 2),
            resolver.resolve(fit, new Rect(0, 0, 0, 0)).rect());
    }

    @Test
    void messageTableOwnsOnlyElementTableStateAndRemainsFocusable()
    {
        MessageTable table = new MessageTable(
            "table",
            "| Name | Value |\n| :--- | ---: |\n| One | 1 |");
        assertEquals(Element.class, table.getClass().getSuperclass());
        assertTrue(table.focusable());
        assertEquals(5, table.fitContentHeight());
        assertEquals("┌────────┬─────────┐", RenderHelpers.renderedText(
            table.render(new Size(20, 5), new ElementRenderState())).get(0));
        table.setText("");
        assertEquals("", table.text());
        assertEquals(1, table.fitContentHeight());
    }

    @Test
    void reusableElementRendersOneGeneratedChildRuntime()
    {
        GeneratedWindowBase child = new GeneratedWindowBase("Child");
        child.setGeneratedWindowStyle(new Style());
        child.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("message")
                .setRelative(new Rect(0, 0, 4, 1))
                .setSourceCell(new Rect(0, 0, 4, 1))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))));
        child.addElement(new Label("message", "test"));
        ReusableElement reusable = new ReusableElement("host", child);

        List<List<TerminalCell>> rendered = reusable.render(
            new Size(4, 1),
            new ElementRenderState());
        assertEquals(List.of("test"), RenderHelpers.renderedText(rendered));
        assertEquals("fullscreen", child.mode());
    }

    @Test
    void reusableFocusBackgroundMatchesCanonicalTransparentAndOpaqueLayerCounts()
    {
        Color parentBackground = new Color("#172033");
        Color focusBackground = new Color("#ffffff14");

        Color transparentChild = renderedReusableFocusBackground(
            Color.rgb(0, 0, 0, 0),
            parentBackground,
            focusBackground);
        Color opaqueChild = renderedReusableFocusBackground(
            parentBackground,
            parentBackground,
            focusBackground);

        Color oneLayer = focusBackground.blendOver(parentBackground);
        Color twoLayers = focusBackground.blendOver(oneLayer);
        assertEquals(twoLayers, transparentChild);
        assertEquals(oneLayer, opaqueChild);
    }

    @Test
    void reusableRootScrollViewUsesItsFocusSurfaceOnlyWhileProxyIsFocused()
    {
        GeneratedScrollViewBase child = new GeneratedScrollViewBase("Scroll child");
        child.setGeneratedWindowStyle(Style.fromProperties("background", "#030712"));
        ScrollView scrollView = child.addElement(new ScrollView("__scrollview"));
        scrollView.setStyle(Style.fromProperties("background", "#030712"));
        scrollView.setFocusStyle(Style.fromProperties("background", "#1e3a5f"));
        scrollView.setDescendantFocusStyle(Style.fromProperties("background", "#1e3a5f"));
        child.setGeneratedScrollView(scrollView);
        child.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("__scrollview")
                .setRelative(new Rect(0, 0, 1, 1))
                .setSourceCell(new Rect(0, 0, 1, 1))
                .setWidth(AxisDimension.fixed(1))
                .setHeight(AxisDimension.fixed(1))
                .setCellWidth(AxisDimension.fixed(1))
                .setCellHeight(AxisDimension.fixed(1))
                .setCharsSize(new Size(1, 1))
                .setCellCharsSize(new Size(1, 1))
                .setCellStyle(Style.fromProperties("background", "#030712"))));

        GeneratedWindowBase host = new GeneratedWindowBase("Host");
        host.setGeneratedWindowStyle(Style.fromProperties("background", "#030712"));
        ReusableElement reusable = host.addElement(new ReusableElement("items", child));
        reusable.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        host.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("items")
                .setRelative(new Rect(0, 0, 1, 1))
                .setSourceCell(new Rect(0, 0, 1, 1))
                .setWidth(AxisDimension.fixed(1))
                .setHeight(AxisDimension.fixed(1))
                .setCellWidth(AxisDimension.fixed(1))
                .setCellHeight(AxisDimension.fixed(1))
                .setCharsSize(new Size(1, 1))
                .setCellCharsSize(new Size(1, 1))
                .setCellStyle(Style.fromProperties("background", "#030712"))));

        Color unfocused = GeneratedWindowRuntime.renderGeneratedWindowContent(
            host,
            new Size(1, 1),
            -1).get(0).get(0).background();
        Color focused = GeneratedWindowRuntime.renderGeneratedWindowContent(
            host,
            new Size(1, 1),
            0).get(0).get(0).background();

        assertEquals(new Color("#030712"), unfocused);
        assertEquals(new Color("#1e3a5f"), focused);
    }

    @Test
    void viewHostPageSwapKeepsRootScrollViewIndicatorInsideTheHostViewport()
    {
        ViewHost host = new ViewHost("page");
        host.setView(new GeneratedWindowBase("Initial page"));

        GeneratedScrollViewBase child = new GeneratedScrollViewBase("Scroll page");
        child.setGeneratedWindowStyle(Style.fromProperties("background", "#030712"));
        ScrollView scrollView = child.addElement(new ScrollView("__scrollview"));
        scrollView.setStyle(Style.fromProperties(
            "background", "#030712",
            "padding", "1"));
        scrollView.addChild(RenderHelpers.renderPlainText(
            "one\ntwo\nthree\nfour\nfive\nsix",
            6,
            6,
            new Style()));
        child.setGeneratedScrollView(scrollView);
        child.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("__scrollview")
                .setRelative(new Rect(0, 0, Integer.MAX_VALUE, Integer.MAX_VALUE))
                .setSourceCell(new Rect(0, 0, 6, 4))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(6, 4))
                .setCellCharsSize(new Size(6, 4))
                .setCellStyle(Style.fromProperties("background", "#030712"))));
        host.setView(child);

        List<List<TerminalCell>> rendered = host.render(
            new Size(4, 3),
            new ElementRenderState());

        assertTrue(scrollView.viewOffset() > 0);
        assertEquals("v", rendered.get(2).get(3).text());
    }

    @Test
    void reusableTransparentRootScrollViewKeepsRenderedContentBackgroundInEditMode()
    {
        GeneratedScrollViewBase child = new GeneratedScrollViewBase("Scroll child");
        child.setGeneratedWindowStyle(Style.fromProperties("background", "#00000000"));
        ScrollView scrollView = child.addElement(new ScrollView("__scrollview"));
        scrollView.setStyle(Style.fromProperties(
            "background", "#00000000",
            "padding-left", "1",
            "padding-right", "1"));
        scrollView.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        scrollView.setDynamicChildrenRenderer(width -> List.of(
            RenderHelpers.renderPlainText(
                "content",
                Math.max(1, width),
                1,
                Style.fromProperties("background", "#000000"))));
        child.setGeneratedScrollView(scrollView);
        child.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("__scrollview")
                .setRelative(new Rect(0, 0, 9, 1))
                .setSourceCell(new Rect(0, 0, 9, 1))
                .setWidth(AxisDimension.fixed(9))
                .setHeight(AxisDimension.fixed(1))
                .setCellWidth(AxisDimension.fixed(9))
                .setCellHeight(AxisDimension.fixed(1))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(9, 1))
                .setCellStyle(Style.fromProperties("background", "#000000"))));

        GeneratedWindowBase host = new GeneratedWindowBase("Host");
        host.setGeneratedWindowStyle(Style.fromProperties("background", "#000000"));
        ReusableElement reusable = host.addElement(new ReusableElement("viewer", child));
        reusable.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        host.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("viewer")
                .setRelative(new Rect(0, 0, 9, 1))
                .setSourceCell(new Rect(0, 0, 9, 1))
                .setWidth(AxisDimension.fixed(9))
                .setHeight(AxisDimension.fixed(1))
                .setCellWidth(AxisDimension.fixed(9))
                .setCellHeight(AxisDimension.fixed(1))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(9, 1))
                .setCellStyle(Style.fromProperties("background", "#000000"))));

        List<List<TerminalCell>> rendered = GeneratedWindowRuntime.renderGeneratedWindowContent(
            host,
            new Size(9, 1),
            0,
            true);

        assertEquals(new Color("#141414"), rendered.get(0).get(0).background());
        assertEquals(new Color("#000000"), rendered.get(0).get(1).background());
        assertEquals("c", rendered.get(0).get(1).text());
        assertEquals(new Color("#141414"), rendered.get(0).get(8).background());
    }

    @Test
    void activeNestedScrollViewDimsOnlyCellsOutsideItsResolvedViewport()
    {
        GeneratedScrollViewBase child = new GeneratedScrollViewBase("Scroll child");
        child.setGeneratedWindowStyle(Style.fromProperties("background", "#030712"));
        ScrollView scrollView = child.addElement(new ScrollView("__scrollview"));
        scrollView.setStyle(Style.fromProperties("background", "#030712"));
        scrollView.setFocusStyle(Style.fromProperties("background", "#1e3a5f"));
        child.setGeneratedScrollView(scrollView);
        child.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("__scrollview")
                .setRelative(new Rect(0, 0, 1, 1))
                .setSourceCell(new Rect(0, 0, 1, 1))
                .setWidth(AxisDimension.fixed(1))
                .setHeight(AxisDimension.fixed(1))
                .setCellWidth(AxisDimension.fixed(1))
                .setCellHeight(AxisDimension.fixed(1))
                .setCharsSize(new Size(1, 1))
                .setCellCharsSize(new Size(1, 1))
                .setCellStyle(Style.fromProperties("background", "#030712"))));

        GeneratedWindowBase host = new GeneratedWindowBase("Host");
        host.setGeneratedWindowStyle(Style.fromProperties("background", "#ffffff"));
        host.addElement(new ReusableElement("items", child));
        host.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("items")
                .setRelative(new Rect(0, 1, 1, 1))
                .setSourceCell(new Rect(0, 0, 3, 1))
                .setWidth(AxisDimension.fixed(1))
                .setHeight(AxisDimension.fixed(1))
                .setCellWidth(AxisDimension.fixed(3))
                .setCellHeight(AxisDimension.fixed(1))
                .setCharsSize(new Size(1, 1))
                .setCellCharsSize(new Size(3, 1))
                .setCellStyle(Style.fromProperties("background", "#ffffff"))));

        List<List<TerminalCell>> rendered = GeneratedWindowRuntime.renderGeneratedWindowContent(
            host,
            new Size(3, 1),
            0,
            true,
            scrollView,
            null,
            false);

        Color dimmed = new Color("#00000066").blendOver(new Color("#ffffff"));
        assertEquals(dimmed, rendered.get(0).get(0).background());
        assertEquals(new Color("#1e3a5f"), rendered.get(0).get(1).background());
        assertEquals(dimmed, rendered.get(0).get(2).background());
    }

    @Test
    void messageTableParsesAlignmentAndExpandsToItsElementWidth()
    {
        MessageTable table = new MessageTable(
            "table",
            "| Name | Score |\n| :--- | ---: |\n| Ada | 42 |");
        List<List<TerminalCell>> rendered = table.render(
            new Size(20, 5),
            new ElementRenderState());

        assertEquals(5, table.fitContentHeight());
        assertEquals(20, rendered.get(0).size());
        assertEquals("┌────────┬─────────┐", RenderHelpers.renderedText(rendered).get(0));
        assertEquals("│ Name   │   Score │", RenderHelpers.renderedText(rendered).get(1));
        assertEquals("│ Ada    │      42 │", RenderHelpers.renderedText(rendered).get(3));
    }

    private static Color renderedReusableFocusBackground(
        Color childCellBackground,
        Color parentBackground,
        Color focusBackground)
    {
        GeneratedWindowBase child = new GeneratedWindowBase("Child");
        child.setGeneratedFocusable(true);
        child.setGeneratedWindowStyle(Style.fromProperties(
            "background",
            parentBackground.text()));
        Label content = child.addElement(new Label("content", " "));
        content.setStyle(Style.fromProperties("background", "transparent"));
        child.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("content")
                .setRelative(new Rect(0, 0, 1, 1))
                .setSourceCell(new Rect(0, 0, 1, 1))
                .setWidth(AxisDimension.fixed(1))
                .setHeight(AxisDimension.fixed(1))
                .setCellWidth(AxisDimension.fixed(1))
                .setCellHeight(AxisDimension.fixed(1))
                .setCharsSize(new Size(1, 1))
                .setCellCharsSize(new Size(1, 1))
                .setCellStyle(Style.fromProperties(
                    "background",
                    childCellBackground.text()))));

        GeneratedWindowBase host = new GeneratedWindowBase("Host");
        host.setGeneratedWindowStyle(Style.fromProperties(
            "background",
            parentBackground.text()));
        ReusableElement reusable = host.addElement(new ReusableElement("card", child));
        reusable.setFocusStyle(Style.fromProperties(
            "background",
            focusBackground.text()));
        host.setGeneratedLayout(List.of(
            new GeneratedLayoutEntry()
                .setName("card")
                .setRelative(new Rect(0, 0, 1, 1))
                .setSourceCell(new Rect(0, 0, 1, 1))
                .setWidth(AxisDimension.fixed(1))
                .setHeight(AxisDimension.fixed(1))
                .setCellWidth(AxisDimension.fixed(1))
                .setCellHeight(AxisDimension.fixed(1))
                .setCharsSize(new Size(1, 1))
                .setCellCharsSize(new Size(1, 1))
                .setCellStyle(Style.fromProperties(
                    "background",
                    parentBackground.text()))));

        return GeneratedWindowRuntime.renderGeneratedWindowContent(
            host,
            new Size(1, 1),
            0).get(0).get(0).background();
    }
}
