package uimd;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.nio.charset.StandardCharsets;

import org.junit.jupiter.api.Test;

final class TerminalBufferTest
{
    @Test
    void diffTracksChangedRunsAndFullRedraws()
    {
        TerminalBuffer buffer = new TerminalBuffer(4, 2);
        TerminalCell cell = new TerminalCell();
        cell.setText("A");
        cell.setForeground(new Color("#ffffff"));
        buffer.setCell(0, 0, cell);

        String first = buffer.renderDiff();
        assertTrue(first.contains("A"));
        assertFalse(buffer.renderDiff().contains("A"));

        buffer.requestFullRedraw();
        assertTrue(buffer.renderDiff().contains("A"));
    }

    @Test
    void copyPreservesCanonicalCellValueIdentity()
    {
        TerminalCell cell = new TerminalCell();
        cell.setText("");
        cell.setRaw("sixel");
        cell.setRawWidth(0);
        cell.setRawHeight(-2);
        assertEquals(cell, cell.copy());
        assertEquals("", cell.copy().text());
        assertEquals(0, cell.copy().rawWidth());
        assertEquals(-2, cell.copy().rawHeight());
    }

    @Test
    void dimensionsAllowEmptyBuffersAndRejectNegativeValues()
    {
        TerminalBuffer buffer = new TerminalBuffer(0, 0);
        assertEquals(0, buffer.width());
        assertEquals(0, buffer.height());
        buffer.beginRenderStats();
        assertEquals("", buffer.renderDiff());
        assertTrue(buffer.renderStats().fullRedraw());
        assertThrows(IllegalArgumentException.class, () -> new TerminalBuffer(-1, 0));
        assertThrows(IllegalArgumentException.class, () -> buffer.resize(0, -1));
    }

    @Test
    void renderStatsAccumulateAcrossRegionsRawCellsAndScrollOutput()
    {
        TerminalBuffer buffer = new TerminalBuffer(3, 2);
        buffer.renderDiff();
        buffer.beginRenderStats();

        TerminalCell first = new TerminalCell();
        first.setText("A");
        buffer.setCell(0, 0, first);
        String firstOutput = buffer.renderDiffRegion(0, 0, 0, 0, 1, 1);
        TerminalCell second = new TerminalCell();
        second.setText("B");
        buffer.setCell(1, 0, second);
        String secondOutput = buffer.renderDiffRegion(0, 0, 1, 0, 1, 1);

        assertEquals(2, buffer.renderStats().changedCells());
        assertEquals(2, buffer.renderStats().changedRuns());
        assertEquals(
            utf8Length(firstOutput) + utf8Length(secondOutput),
            buffer.renderStats().outputBytes());
        assertFalse(buffer.renderStats().fullRedraw());

        TerminalBuffer rawBuffer = new TerminalBuffer(3, 2);
        rawBuffer.renderDiff();
        rawBuffer.beginRenderStats();
        TerminalCell raw = new TerminalCell();
        raw.setRaw("sixel");
        raw.setRawWidth(2);
        raw.setRawHeight(2);
        rawBuffer.setCell(0, 0, raw);
        rawBuffer.renderDiff();
        assertEquals(4, rawBuffer.renderStats().changedCells());
        assertEquals(5, rawBuffer.renderStats().changedRuns());

        TerminalBuffer skipBuffer = new TerminalBuffer(1, 1);
        skipBuffer.renderDiff();
        skipBuffer.beginRenderStats();
        TerminalCell skip = new TerminalCell();
        skip.setRawSkip(true);
        skipBuffer.setCell(0, 0, skip);
        assertEquals("", skipBuffer.renderDiff());
        assertEquals(1, skipBuffer.renderStats().changedCells());

        TerminalBuffer scrollBuffer = new TerminalBuffer(2, 3);
        scrollBuffer.renderDiff();
        scrollBuffer.beginRenderStats();
        String scroll = scrollBuffer.renderScrollRegion(0, 0, 3, -1);
        assertFalse(scroll.isEmpty());
        assertEquals(utf8Length(scroll), scrollBuffer.renderStats().outputBytes());
    }

    private static int utf8Length(String value)
    {
        return value.getBytes(StandardCharsets.UTF_8).length;
    }
}
