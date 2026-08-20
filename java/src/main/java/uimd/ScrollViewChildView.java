package uimd;

public record ScrollViewChildView(
    Element element,
    Rect frame,
    boolean visible,
    int index,
    boolean clipped)
{
}
