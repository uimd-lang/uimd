package uimd;

import java.util.Objects;
import java.util.function.BiFunction;
import java.util.function.IntFunction;
import java.util.function.IntSupplier;
import java.util.function.IntUnaryOperator;

public final class VirtualScrollDataSource
{
    private final IntSupplier itemCount;
    private final IntFunction<Object> itemKey;
    private final BiFunction<Integer, Element, Element> materialize;
    private final IntUnaryOperator estimatedHeight;

    public VirtualScrollDataSource(
        IntSupplier itemCount,
        IntFunction<Object> itemKey,
        BiFunction<Integer, Element, Element> materialize,
        IntUnaryOperator estimatedHeight)
    {
        this.itemCount = Objects.requireNonNull(itemCount, "itemCount");
        this.itemKey = Objects.requireNonNull(itemKey, "itemKey");
        this.materialize = Objects.requireNonNull(materialize, "materialize");
        this.estimatedHeight = estimatedHeight;
    }

    public VirtualScrollDataSource(
        IntSupplier itemCount,
        IntFunction<Object> itemKey,
        BiFunction<Integer, Element, Element> materialize)
    {
        this(itemCount, itemKey, materialize, null);
    }

    int itemCount()
    {
        return itemCount.getAsInt();
    }

    Object itemKey(int index)
    {
        return itemKey.apply(index);
    }

    Element materialize(int index, Element recycled)
    {
        return materialize.apply(index, recycled);
    }

    Integer estimatedHeight(int width)
    {
        return estimatedHeight == null ? null : estimatedHeight.applyAsInt(width);
    }
}
