package uimd;

import java.util.List;
import java.util.function.IntFunction;

public class GeneratedScrollViewBase extends GeneratedWindowBase
{
    private ScrollView generatedScrollView;

    public GeneratedScrollViewBase()
    {
        this("");
    }

    public GeneratedScrollViewBase(String title)
    {
        super(title);
    }

    public ScrollView scrollView()
    {
        if (generatedScrollView == null)
        {
            throw new IllegalStateException("generated scroll view has not been initialized");
        }
        return generatedScrollView;
    }

    @Override
    public ScrollView generatedScrollView()
    {
        return generatedScrollView;
    }

    public void setGeneratedScrollView(ScrollView value)
    {
        generatedScrollView = value;
    }

    public void setAutoScroll(boolean value)
    {
        scrollView().setAutoScroll(value);
    }

    public Rect frame()
    {
        return scrollView().frame();
    }

    public int viewOffset()
    {
        return scrollView().viewOffset();
    }

    public int scrollOffset()
    {
        return scrollView().scrollOffset();
    }

    public int contentHeight()
    {
        return scrollView().contentHeight();
    }

    public int contentHeight(int width)
    {
        return scrollView().contentHeight(width);
    }

    public boolean handleKey(String key)
    {
        ScrollView view = scrollView();
        return view.handleKey(
            key,
            new Size(view.frame().width(), view.frame().height()));
    }

    public boolean scrollToTop()
    {
        ScrollView view = scrollView();
        return view.scrollToTop(new Size(view.frame().width(), view.frame().height()));
    }

    public boolean scrollToBottom()
    {
        ScrollView view = scrollView();
        return view.scrollToBottom(new Size(view.frame().width(), view.frame().height()));
    }

    public boolean scrollToTop(Size viewport)
    {
        return scrollView().scrollToTop(viewport);
    }

    public boolean scrollToBottom(Size viewport)
    {
        return scrollView().scrollToBottom(viewport);
    }

    public ScrollViewPosition scrollPosition()
    {
        return scrollView().scrollPosition();
    }

    public void restoreScrollPosition(ScrollViewPosition position)
    {
        scrollView().restoreScrollPosition(position);
    }

    public void clearChildren()
    {
        scrollView().clearChildren();
    }

    public <T extends Element> T addChild(T child)
    {
        return scrollView().addChild(child);
    }

    public void addChild(List<List<TerminalCell>> child)
    {
        scrollView().addChild(child);
    }

    public void setDynamicChildrenRenderer(IntFunction<List<List<List<TerminalCell>>>> renderer)
    {
        scrollView().setDynamicChildrenRenderer(renderer);
    }

    public void invalidateDynamicChildren()
    {
        scrollView().invalidateDynamicChildren();
    }
}
