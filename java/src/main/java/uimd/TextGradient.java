package uimd;

import java.util.ArrayList;
import java.util.List;

public final class TextGradient
{
    private static final int DEFAULT_INTERVAL_MS = 70;
    private static final int DEFAULT_STEP = 1;
    private static final int DEFAULT_SEGMENT_SIZE = 1;

    private int intervalMs = DEFAULT_INTERVAL_MS;
    private int step = DEFAULT_STEP;
    private int segmentSize = DEFAULT_SEGMENT_SIZE;
    private final List<Color> colors = new ArrayList<>();

    public int intervalMs()
    {
        return intervalMs;
    }

    public void setIntervalMs(int value)
    {
        intervalMs = value;
    }

    public int step()
    {
        return step;
    }

    public void setStep(int value)
    {
        step = value;
    }

    public int segmentSize()
    {
        return segmentSize;
    }

    public void setSegmentSize(int value)
    {
        segmentSize = value;
    }

    public List<Color> colors()
    {
        return colors;
    }
}
