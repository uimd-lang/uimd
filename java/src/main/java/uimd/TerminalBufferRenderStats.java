package uimd;

public record TerminalBufferRenderStats(
    long changedCells,
    long changedRuns,
    long outputBytes,
    boolean fullRedraw)
{
}
