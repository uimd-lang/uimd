package uimd;

public final class Event
{
    private final EventType type;
    private final String key;
    private final String text;
    private final Point position;
    private final int wheelDelta;
    private final Size size;

    public Event()
    {
        this(EventType.KEY, "", "", new Point(0, 0), 0, new Size(0, 0));
    }

    public Event(
        EventType type,
        String key,
        String text,
        Point position,
        int wheelDelta,
        Size size)
    {
        this.type = type == null ? EventType.KEY : type;
        this.key = key == null ? "" : key;
        this.text = text == null ? "" : text;
        this.position = position == null ? new Point(0, 0) : position;
        this.wheelDelta = wheelDelta;
        this.size = size == null ? new Size(0, 0) : size;
    }

    public static Event key(String value)
    {
        return new Event(EventType.KEY, value, "", null, 0, null);
    }

    public static Event paste(String value)
    {
        return new Event(EventType.PASTE, "", value, null, 0, null);
    }

    public static Event mouse(EventType type, Point position)
    {
        if (type != EventType.MOUSE_PRESS
            && type != EventType.MOUSE_DRAG
            && type != EventType.MOUSE_RELEASE)
        {
            throw new IllegalArgumentException("mouse event type is invalid");
        }
        return new Event(type, "", "", position, 0, null);
    }

    public static Event wheel(Point position, int delta)
    {
        return new Event(EventType.MOUSE_WHEEL, "", "", position, delta, null);
    }

    public static Event resize(Size size)
    {
        return new Event(EventType.RESIZE, "", "", null, 0, size);
    }

    public EventType type()
    {
        return type;
    }

    public String key()
    {
        return key;
    }

    public String text()
    {
        return text;
    }

    public Point position()
    {
        return position;
    }

    public int wheelDelta()
    {
        return wheelDelta;
    }

    public Size size()
    {
        return size;
    }
}
