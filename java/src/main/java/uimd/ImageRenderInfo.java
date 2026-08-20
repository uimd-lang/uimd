package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class ImageRenderInfo
{
    private String source = "";
    private String fit = "";
    private String configuredRenderMode = "";
    private String resolvedRenderMode = "";
    private boolean sourceLoaded;
    private int sourceWidth;
    private int sourceHeight;
    private int elementWidth;
    private int elementHeight;
    private int cellPixelWidth;
    private int cellPixelHeight;
    private int imageLeft;
    private int imageTop;
    private int imageWidth;
    private int imageHeight;
    private int visibleLeft;
    private int visibleTop;
    private int visibleWidth;
    private int visibleHeight;
    private boolean rawExpected;
    private boolean rawPresent;
    private final List<String> sampleSignature = new ArrayList<>();

    public String source()
    {
        return source;
    }

    public void setSource(String value)
    {
        source = value;
    }

    public String fit()
    {
        return fit;
    }

    public void setFit(String value)
    {
        fit = value;
    }

    public String configuredRenderMode()
    {
        return configuredRenderMode;
    }

    public void setConfiguredRenderMode(String value)
    {
        configuredRenderMode = value;
    }

    public String resolvedRenderMode()
    {
        return resolvedRenderMode;
    }

    public void setResolvedRenderMode(String value)
    {
        resolvedRenderMode = value;
    }

    public boolean sourceLoaded()
    {
        return sourceLoaded;
    }

    public void setSourceLoaded(boolean value)
    {
        sourceLoaded = value;
    }

    public int sourceWidth()
    {
        return sourceWidth;
    }

    public void setSourceWidth(int value)
    {
        sourceWidth = value;
    }

    public int sourceHeight()
    {
        return sourceHeight;
    }

    public void setSourceHeight(int value)
    {
        sourceHeight = value;
    }

    public int elementWidth()
    {
        return elementWidth;
    }

    public void setElementWidth(int value)
    {
        elementWidth = value;
    }

    public int elementHeight()
    {
        return elementHeight;
    }

    public void setElementHeight(int value)
    {
        elementHeight = value;
    }

    public int cellPixelWidth()
    {
        return cellPixelWidth;
    }

    public void setCellPixelWidth(int value)
    {
        cellPixelWidth = value;
    }

    public int cellPixelHeight()
    {
        return cellPixelHeight;
    }

    public void setCellPixelHeight(int value)
    {
        cellPixelHeight = value;
    }

    public int imageLeft()
    {
        return imageLeft;
    }

    public void setImageLeft(int value)
    {
        imageLeft = value;
    }

    public int imageTop()
    {
        return imageTop;
    }

    public void setImageTop(int value)
    {
        imageTop = value;
    }

    public int imageWidth()
    {
        return imageWidth;
    }

    public void setImageWidth(int value)
    {
        imageWidth = value;
    }

    public int imageHeight()
    {
        return imageHeight;
    }

    public void setImageHeight(int value)
    {
        imageHeight = value;
    }

    public int visibleLeft()
    {
        return visibleLeft;
    }

    public void setVisibleLeft(int value)
    {
        visibleLeft = value;
    }

    public int visibleTop()
    {
        return visibleTop;
    }

    public void setVisibleTop(int value)
    {
        visibleTop = value;
    }

    public int visibleWidth()
    {
        return visibleWidth;
    }

    public void setVisibleWidth(int value)
    {
        visibleWidth = value;
    }

    public int visibleHeight()
    {
        return visibleHeight;
    }

    public void setVisibleHeight(int value)
    {
        visibleHeight = value;
    }

    public boolean rawExpected()
    {
        return rawExpected;
    }

    public void setRawExpected(boolean value)
    {
        rawExpected = value;
    }

    public boolean rawPresent()
    {
        return rawPresent;
    }

    public void setRawPresent(boolean value)
    {
        rawPresent = value;
    }

    public List<String> sampleSignature()
    {
        return Collections.unmodifiableList(sampleSignature);
    }

    public void setSampleSignature(List<String> value)
    {
        sampleSignature.clear();
        sampleSignature.addAll(value == null ? List.of() : value);
    }
}
