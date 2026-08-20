package uimd;

public record Rect(int row, int col, int width, int height)
{
    public boolean contains(Point point)
    {
        return point.row() >= row && point.row() < row + height
            && point.col() >= col && point.col() < col + width;
    }

    public boolean empty()
    {
        return width <= 0 || height <= 0;
    }

    public Point origin()
    {
        return new Point(row, col);
    }

    public Size size()
    {
        return new Size(width, height);
    }
}
