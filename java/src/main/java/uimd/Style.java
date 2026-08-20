package uimd;

public final class Style
{
    private Color color;
    private Color background;
    private String backgroundTexture;
    private Color backgroundTextureColor;
    private Color scopeDimBackground = new Color("#00000066");
    private Color borderColor;
    private Integer borderWidthHorizontal;
    private Integer borderWidthVertical;
    private Integer padding;
    private Integer paddingTop;
    private Integer paddingRight;
    private Integer paddingBottom;
    private Integer paddingLeft;
    private Integer margin;
    private Integer marginTop;
    private Integer marginRight;
    private Integer marginBottom;
    private Integer marginLeft;
    private Integer gap;
    private String textAlign = "";
    private String userSelect = "";
    private Boolean scrollX;
    private Boolean scrollY;
    private TextGradient textColorGradient;
    private TextGradient textBackgroundGradient;

    public static Style fromProperties(String... properties)
    {
        if (properties == null || properties.length % 2 != 0)
        {
            throw new IllegalArgumentException("style properties must be key/value pairs");
        }
        Style result = new Style();
        for (int index = 0; index < properties.length; index += 2)
        {
            String key = properties[index];
            String value = properties[index + 1];
            switch (key)
            {
                case "color" -> result.setColor(new Color(value));
                case "background" -> result.setBackground(new Color(value));
                case "background-texture" -> result.setBackgroundTexture(value);
                case "background-texture-color" -> result.setBackgroundTextureColor(new Color(value));
                case "scope-dim-background" -> result.setScopeDimBackground(new Color(value));
                case "border-color" -> result.setBorderColor(new Color(value));
                case "border-width-horizontal" -> result.setBorderWidthHorizontal(parseInteger(value));
                case "border-width-vertical" -> result.setBorderWidthVertical(parseInteger(value));
                case "padding" -> result.setPadding(parseInteger(value));
                case "padding-top" -> result.setPaddingTop(parseInteger(value));
                case "padding-right" -> result.setPaddingRight(parseInteger(value));
                case "padding-bottom" -> result.setPaddingBottom(parseInteger(value));
                case "padding-left" -> result.setPaddingLeft(parseInteger(value));
                case "margin" -> result.setMargin(parseInteger(value));
                case "margin-top" -> result.setMarginTop(parseInteger(value));
                case "margin-right" -> result.setMarginRight(parseInteger(value));
                case "margin-bottom" -> result.setMarginBottom(parseInteger(value));
                case "margin-left" -> result.setMarginLeft(parseInteger(value));
                case "gap" -> result.setGap(parseInteger(value));
                case "text-align" -> result.setTextAlign(value);
                case "user-select" -> result.setUserSelect(value);
                case "scroll-x" -> result.setScrollX(parseBoolean(value));
                case "scroll-y" -> result.setScrollY(parseBoolean(value));
                default -> throw new IllegalArgumentException("unsupported style property: " + key);
            }
        }
        return result;
    }

    public Style copy()
    {
        Style result = new Style();
        result.color = color;
        result.background = background;
        result.backgroundTexture = backgroundTexture;
        result.backgroundTextureColor = backgroundTextureColor;
        result.scopeDimBackground = scopeDimBackground;
        result.borderColor = borderColor;
        result.borderWidthHorizontal = borderWidthHorizontal;
        result.borderWidthVertical = borderWidthVertical;
        result.padding = padding;
        result.paddingTop = paddingTop;
        result.paddingRight = paddingRight;
        result.paddingBottom = paddingBottom;
        result.paddingLeft = paddingLeft;
        result.margin = margin;
        result.marginTop = marginTop;
        result.marginRight = marginRight;
        result.marginBottom = marginBottom;
        result.marginLeft = marginLeft;
        result.gap = gap;
        result.textAlign = textAlign;
        result.userSelect = userSelect;
        result.scrollX = scrollX;
        result.scrollY = scrollY;
        result.textColorGradient = textColorGradient;
        result.textBackgroundGradient = textBackgroundGradient;
        return result;
    }

    public void merge(Style other)
    {
        if (other == null)
        {
            return;
        }
        color = other.color == null ? color : other.color;
        background = other.background == null ? background : other.background;
        backgroundTexture = other.backgroundTexture == null ? backgroundTexture : other.backgroundTexture;
        backgroundTextureColor = other.backgroundTextureColor == null ? backgroundTextureColor : other.backgroundTextureColor;
        scopeDimBackground = other.scopeDimBackground == null ? scopeDimBackground : other.scopeDimBackground;
        borderColor = other.borderColor == null ? borderColor : other.borderColor;
        borderWidthHorizontal = other.borderWidthHorizontal == null ? borderWidthHorizontal : other.borderWidthHorizontal;
        borderWidthVertical = other.borderWidthVertical == null ? borderWidthVertical : other.borderWidthVertical;
        padding = other.padding == null ? padding : other.padding;
        paddingTop = other.paddingTop == null ? paddingTop : other.paddingTop;
        paddingRight = other.paddingRight == null ? paddingRight : other.paddingRight;
        paddingBottom = other.paddingBottom == null ? paddingBottom : other.paddingBottom;
        paddingLeft = other.paddingLeft == null ? paddingLeft : other.paddingLeft;
        margin = other.margin == null ? margin : other.margin;
        marginTop = other.marginTop == null ? marginTop : other.marginTop;
        marginRight = other.marginRight == null ? marginRight : other.marginRight;
        marginBottom = other.marginBottom == null ? marginBottom : other.marginBottom;
        marginLeft = other.marginLeft == null ? marginLeft : other.marginLeft;
        gap = other.gap == null ? gap : other.gap;
        textAlign = other.textAlign == null || other.textAlign.isEmpty() ? textAlign : other.textAlign;
        userSelect = other.userSelect == null || other.userSelect.isEmpty() ? userSelect : other.userSelect;
        scrollX = other.scrollX == null ? scrollX : other.scrollX;
        scrollY = other.scrollY == null ? scrollY : other.scrollY;
        textColorGradient = other.textColorGradient == null ? textColorGradient : other.textColorGradient;
        textBackgroundGradient = other.textBackgroundGradient == null ? textBackgroundGradient : other.textBackgroundGradient;
    }

    public Style merged(Style other)
    {
        Style result = copy();
        result.merge(other);
        return result;
    }

    public Color color()
    {
        return color;
    }

    public void setColor(Color value)
    {
        color = value;
    }

    public Color background()
    {
        return background;
    }

    public void setBackground(Color value)
    {
        background = value;
    }

    public String backgroundTexture()
    {
        return backgroundTexture;
    }

    public void setBackgroundTexture(String value)
    {
        backgroundTexture = value;
    }

    public Color backgroundTextureColor()
    {
        return backgroundTextureColor;
    }

    public void setBackgroundTextureColor(Color value)
    {
        backgroundTextureColor = value;
    }

    public Color scopeDimBackground()
    {
        return scopeDimBackground;
    }

    public void setScopeDimBackground(Color value)
    {
        scopeDimBackground = value;
    }

    public Color borderColor()
    {
        return borderColor;
    }

    public void setBorderColor(Color value)
    {
        borderColor = value;
    }

    public Integer borderWidthHorizontal()
    {
        return borderWidthHorizontal;
    }

    public void setBorderWidthHorizontal(Integer value)
    {
        borderWidthHorizontal = value;
    }

    public Integer borderWidthVertical()
    {
        return borderWidthVertical;
    }

    public void setBorderWidthVertical(Integer value)
    {
        borderWidthVertical = value;
    }

    public Integer padding()
    {
        return padding;
    }

    public void setPadding(Integer value)
    {
        padding = value;
    }

    public Integer paddingTop()
    {
        return paddingTop;
    }

    public void setPaddingTop(Integer value)
    {
        paddingTop = value;
    }

    public Integer paddingRight()
    {
        return paddingRight;
    }

    public void setPaddingRight(Integer value)
    {
        paddingRight = value;
    }

    public Integer paddingBottom()
    {
        return paddingBottom;
    }

    public void setPaddingBottom(Integer value)
    {
        paddingBottom = value;
    }

    public Integer paddingLeft()
    {
        return paddingLeft;
    }

    public void setPaddingLeft(Integer value)
    {
        paddingLeft = value;
    }

    public Integer margin()
    {
        return margin;
    }

    public void setMargin(Integer value)
    {
        margin = value;
    }

    public Integer marginTop()
    {
        return marginTop;
    }

    public void setMarginTop(Integer value)
    {
        marginTop = value;
    }

    public Integer marginRight()
    {
        return marginRight;
    }

    public void setMarginRight(Integer value)
    {
        marginRight = value;
    }

    public Integer marginBottom()
    {
        return marginBottom;
    }

    public void setMarginBottom(Integer value)
    {
        marginBottom = value;
    }

    public Integer marginLeft()
    {
        return marginLeft;
    }

    public void setMarginLeft(Integer value)
    {
        marginLeft = value;
    }

    public Integer gap()
    {
        return gap;
    }

    public void setGap(Integer value)
    {
        gap = value;
    }

    public String textAlign()
    {
        return textAlign;
    }

    public void setTextAlign(String value)
    {
        textAlign = value == null ? "" : value;
    }

    public String userSelect()
    {
        return userSelect;
    }

    public void setUserSelect(String value)
    {
        userSelect = value == null ? "" : value;
    }

    public Boolean scrollX()
    {
        return scrollX;
    }

    public void setScrollX(Boolean value)
    {
        scrollX = value;
    }

    public Boolean scrollY()
    {
        return scrollY;
    }

    public void setScrollY(Boolean value)
    {
        scrollY = value;
    }

    public TextGradient textColorGradient()
    {
        return textColorGradient;
    }

    public void setTextColorGradient(TextGradient value)
    {
        textColorGradient = value;
    }

    public TextGradient textBackgroundGradient()
    {
        return textBackgroundGradient;
    }

    public void setTextBackgroundGradient(TextGradient value)
    {
        textBackgroundGradient = value;
    }

    private static int parseInteger(String value)
    {
        return Integer.parseInt(value);
    }

    private static boolean parseBoolean(String value)
    {
        return "true".equalsIgnoreCase(value)
            || "1".equals(value)
            || "yes".equalsIgnoreCase(value)
            || "on".equalsIgnoreCase(value);
    }
}
