package uimd;

public record VirtualScrollDiagnostics(
    int logicalItems,
    int materializedRows,
    int recycledRows,
    int lastRowsMeasured,
    int lastRowsConsidered,
    int lastRowsRendered)
{
}
