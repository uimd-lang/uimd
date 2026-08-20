package uimd;

public final class ViewHost extends ReusableElement
{
    public ViewHost(String name)
    {
        super(name);
    }

    public void setView(GeneratedWindowBase view)
    {
        setChild(view);
    }

    public void clearView()
    {
        setChild(null);
    }

    public GeneratedWindowBase currentView()
    {
        return child();
    }
}
