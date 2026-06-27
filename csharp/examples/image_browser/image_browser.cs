using Uimd;

public sealed class ImageRecord
{
    public ImageRecord(string name, string source, bool selected = false)
    {
        Name = name;
        Source = source;
        Selected = selected;
    }

    public string Name { get; set; }
    public string Source { get; set; }
    public bool Selected { get; set; }
}

public static class ImageBrowserStyles
{
    public static void SetLayoutCellBackground(GeneratedWindowBase window, string background)
    {
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            entry.CellStyle.Background = new Color(background);
        }
    }

    public static int MeasuredLayoutHeight(GeneratedWindowBase window)
    {
        Size naturalSize = GeneratedWindowRuntime.GeneratedWindowContentSize(window);
        Size measuredSize = GeneratedWindowRuntime.GeneratedWindowContentSizeForWidth(window, naturalSize.Width);
        return Math.Max(1, measuredSize.Height);
    }
}

public sealed class ImageView : ImageViewUI
{
    public ImageView(string source, string captionText, string renderMode = "sixel")
    {
        photo.SetSource(source);
        photo.SetAlt(captionText);
        photo.SetRenderMode(renderMode);
        caption.SetText(captionText);
    }
}

public sealed class ImageButton : ImageButtonUI
{
    private const string TileNormalBackground = "transparent";
    private const string TileSelectedBackground = "#2563eb";
    private Action? onClick;

    public ImageButton()
    {
        photo.Focusable = false;
    }

    public void SetSource(string source)
    {
        photo.SetSource(source);
    }

    public void SetAlt(string alt)
    {
        photo.SetAlt(alt);
    }

    public void SetRenderMode(string mode)
    {
        photo.SetRenderMode(mode);
    }

    public void SetSelected(bool selected)
    {
        ImageBrowserStyles.SetLayoutCellBackground(this, selected ? TileSelectedBackground : TileNormalBackground);
    }

    public void SetOnClick(Action callback)
    {
        onClick = callback;
    }

    public override bool ActivateGeneratedControl()
    {
        onPhotoClick();
        return onClick is not null;
    }

    protected override void onPhotoClick()
    {
        onClick?.Invoke();
    }
}

public sealed class GalleryMosaic : GalleryMosaicUI
{
    private const string TileNormalBackground = "#172033";
    private const string TileSelectedBackground = "#2563eb";
    private Action? onClick;

    public GalleryMosaic()
    {
        foreach (Image image in Images())
        {
            image.Focusable = false;
        }
    }

    public void SetOnGalleryClick(Action callback)
    {
        onClick = callback;
    }

    public void SetRenderMode(string mode)
    {
        foreach (Image image in Images())
        {
            image.SetRenderMode(mode);
        }
    }

    public void SetSelected(bool selected)
    {
        ImageBrowserStyles.SetLayoutCellBackground(this, selected ? TileSelectedBackground : TileNormalBackground);
    }

    public override bool ActivateGeneratedControl()
    {
        Fire();
        return onClick is not null;
    }

    protected override void onGal1Click()
    {
        Fire();
    }

    protected override void onGal2Click()
    {
        Fire();
    }

    protected override void onGal3Click()
    {
        Fire();
    }

    protected override void onGal4Click()
    {
        Fire();
    }

    protected override void onGal5Click()
    {
        Fire();
    }

    private void Fire()
    {
        onClick?.Invoke();
    }

    private IEnumerable<Image> Images()
    {
        yield return gal_1;
        yield return gal_2;
        yield return gal_3;
        yield return gal_4;
        yield return gal_5;
    }
}

public sealed class ImageListButton : ImageListButtonUI
{
    private const string TileNormalBackground = "#172033";
    private const string TileSelectedBackground = "#2563eb";
    private Action? onClick;

    public ImageListButton()
    {
        foreach (Image image in Images())
        {
            image.Focusable = false;
        }
    }

    public void SetImages(IReadOnlyList<ImageRecord> images)
    {
        Image[] previews = { prw_1, prw_2, prw_3 };
        for (int index = 0; index < previews.Length && index < images.Count; ++index)
        {
            previews[index].SetSource(images[index].Source);
            previews[index].SetAlt(images[index].Name);
        }
    }

    public void SetRenderMode(string mode)
    {
        foreach (Image image in Images())
        {
            image.SetRenderMode(mode);
        }
    }

    public void SetSelected(bool selected)
    {
        ImageBrowserStyles.SetLayoutCellBackground(this, selected ? TileSelectedBackground : TileNormalBackground);
    }

    public void SetOnClick(Action callback)
    {
        onClick = callback;
    }

    public override bool ActivateGeneratedControl()
    {
        Fire();
        return onClick is not null;
    }

    protected override void onPrw1Click()
    {
        Fire();
    }

    protected override void onPrw2Click()
    {
        Fire();
    }

    protected override void onPrw3Click()
    {
        Fire();
    }

    private void Fire()
    {
        onClick?.Invoke();
    }

    private IEnumerable<Image> Images()
    {
        yield return prw_1;
        yield return prw_2;
        yield return prw_3;
    }
}

public sealed class GalleryItem : GalleryItemUI
{
    public GalleryItem(string source, string captionText, string renderMode)
    {
        photo.SetSource(source);
        photo.SetRenderMode(renderMode);
        caption.SetText(captionText);
    }
}

public sealed class GalleryScroll : GalleryScrollUI
{
    public void Populate(string renderMode)
    {
        ClearChildren();
        List<ImageRecord> images = ImageBrowser.ImageLibrary();
        for (int index = 0; index < Math.Min(images.Count, 5); ++index)
        {
            GalleryItem item = new(images[index].Source, images[index].Name, renderMode);
            ReusableElement reusable = new("gallery_item");
            reusable.SetChild(item);
            item.Mode = "expand_width";
            reusable.Frame = new Rect(0, 0, 0, ImageBrowserStyles.MeasuredLayoutHeight(item));
            AddChild(reusable);
        }
        ScrollToTop();
    }
}

public sealed class ImageShowDialog : ImageShowDialogUI
{
    private bool closed;

    public ImageShowDialog(string source, string captionText, string renderMode)
    {
        title.SetText(captionText);
        photo.SetSource(source);
        photo.SetAlt(captionText);
        photo.SetRenderMode(renderMode);
    }

    public override GeneratedWindowRuntimeOptions RuntimeOptions()
    {
        GeneratedWindowRuntimeOptions options = base.RuntimeOptions();
        options.InitialFocusName = "close_btn";
        options.OnKey = key =>
        {
            if (key == "Escape")
            {
                closed = true;
                CloseWindow();
                return true;
            }
            return false;
        };
        return options;
    }

    protected override void onCloseBtnClick()
    {
        closed = true;
        CloseWindow();
    }

    protected override bool shouldClose()
    {
        return closed;
    }
}

public sealed class ImageListItem : ImageListItemUI
{
    private readonly ImageRecord record;
    private readonly Action<ImageRecord>? onShow;
    private readonly Action<ImageRecord>? onBrowse;
    private readonly Action<ImageRecord>? onDelete;
    private readonly Action<ImageRecord>? onChange;

    public ImageListItem(
        ImageRecord record,
        string renderMode,
        Action<ImageRecord>? onShow,
        Action<ImageRecord>? onBrowse,
        Action<ImageRecord>? onDelete,
        Action<ImageRecord>? onChange)
    {
        this.record = record;
        this.onShow = onShow;
        this.onBrowse = onBrowse;
        this.onDelete = onDelete;
        this.onChange = onChange;
        SyncFromRecord(renderMode);
    }

    protected override void onSelChange(string value)
    {
        record.Selected = value == "true";
        onChange?.Invoke(record);
    }

    protected override void onShowBtnClick()
    {
        onShow?.Invoke(record);
    }

    protected override void onBrowseBtnClick()
    {
        onBrowse?.Invoke(record);
    }

    protected override void onDeleteBtnClick()
    {
        onDelete?.Invoke(record);
    }

    private void SyncFromRecord(string renderMode)
    {
        thumb.SetSource(record.Source);
        thumb.SetAlt(record.Name);
        thumb.SetFit("contain");
        thumb.SetRenderMode(renderMode);
        name_label.SetText(record.Name);
        path_label.SetText(ImageDisplayPath(record.Source));
        sel.SetChecked(record.Selected);
    }

    private static string ImageDisplayPath(string source)
    {
        return ImageBrowser.ImageDisplayPath(source);
    }
}

public sealed class ImageListScroll : ImageListScrollUI
{
    public void SetItems(
        IList<ImageRecord> records,
        string renderMode,
        Action<ImageRecord>? onShow,
        Action<ImageRecord>? onBrowse,
        Action<ImageRecord>? onDelete,
        Action<ImageRecord>? onChange,
        bool scrollToEnd = false,
        bool preserveScroll = false)
    {
        ScrollViewPosition position = ScrollPosition();
        ClearChildren();
        SetAutoScroll(scrollToEnd);
        for (int index = 0; index < records.Count; ++index)
        {
            ImageListItem item = new(records[index], renderMode, onShow, onBrowse, onDelete, onChange);
            ReusableElement reusable = new("items[" + index + "]");
            reusable.SetChild(item);
            item.Mode = "expand_width";
            reusable.Frame = new Rect(0, 0, 0, ImageBrowserStyles.MeasuredLayoutHeight(item));
            AddChild(reusable);
        }
        if (scrollToEnd)
        {
            ScrollToBottom();
        }
        else if (preserveScroll)
        {
            RestoreScrollPosition(position);
        }
        else
        {
            ScrollToTop();
        }
    }
}

public sealed class ImageListView : ImageListViewUI
{
    private readonly IList<ImageRecord> records;
    private readonly ImageListScroll scroll;
    private readonly Action<GeneratedWindowBase, GeneratedWindowRuntimeOptions> openModal;
    private readonly Action closeModal;
    private string renderMode;
    private FileBrowser? browser;
    private MessageBoxYesNo? deleteDialog;
    private ImageShowDialog? showDialog;
    private ImageRecord? pendingBrowseRecord;
    private ImageRecord? pendingDeleteRecord;

    public ImageListView(
        IList<ImageRecord> records,
        string renderMode,
        Action<GeneratedWindowBase, GeneratedWindowRuntimeOptions> openModal,
        Action closeModal)
    {
        this.records = records;
        this.renderMode = renderMode;
        this.openModal = openModal;
        this.closeModal = closeModal;
        scroll = new ImageListScroll();
        items.SetChild(scroll);
        RefreshItems();
    }

    public void SetRenderMode(string mode)
    {
        renderMode = mode;
        RefreshItems(preserveScroll: true);
    }

    protected override void onAddBtnClick()
    {
        OpenBrowser(null);
    }

    private void RefreshItems(bool scrollToEnd = false, bool preserveScroll = false)
    {
        scroll.SetItems(
            records,
            renderMode,
            ShowImage,
            OpenBrowser,
            ConfirmDelete,
            _ => { },
            scrollToEnd,
            preserveScroll);
    }

    private void ShowImage(ImageRecord record)
    {
        CloseShowDialog();
        showDialog = new ImageShowDialog(record.Source, record.Name, renderMode);
        GeneratedWindowRuntimeOptions options = showDialog.RuntimeOptions();
        options.InitialFocusName = "close_btn";
        options.OnButton = name =>
        {
            if (name == "close_btn")
            {
                CloseShowDialog();
            }
        };
        options.OnKey = key =>
        {
            if (key == "Escape")
            {
                CloseShowDialog();
                return true;
            }
            return false;
        };
        openModal(showDialog, options);
    }

    private void OpenBrowser(ImageRecord? record)
    {
        CloseBrowser();
        pendingBrowseRecord = record;
        string startPath = record is null ? ImageBrowser.ImageSampleDir() : record.Source;
        browser = new FileBrowser(
            ImageBrowser.ProjectRoot(),
            startPath,
            "open",
            OnBrowserClosed,
            "",
            ImageBrowser.ImageExtensionFilter);
        openModal(browser, browser.RuntimeOptions());
    }

    private void OnBrowserClosed(string path)
    {
        if (string.IsNullOrEmpty(path))
        {
            CloseBrowser();
            return;
        }
        if (pendingBrowseRecord is null)
        {
            records.Add(new ImageRecord(ImageBrowser.ImageNameFromPath(path), path));
            CloseBrowser();
            RefreshItems(scrollToEnd: true);
            return;
        }
        pendingBrowseRecord.Name = ImageBrowser.ImageNameFromPath(path);
        pendingBrowseRecord.Source = path;
        CloseBrowser();
        RefreshItems(preserveScroll: true);
    }

    private void ConfirmDelete(ImageRecord record)
    {
        CloseDeleteDialog();
        pendingDeleteRecord = record;
        deleteDialog = new MessageBoxYesNo("Delete Image", "Delete " + record.Name + "?");
        deleteDialog.OnClose = DeleteConfirmed;
        openModal(deleteDialog, deleteDialog.RuntimeOptions());
    }

    private void DeleteConfirmed(bool confirmed)
    {
        ImageRecord? record = pendingDeleteRecord;
        deleteDialog = null;
        pendingDeleteRecord = null;
        if (!confirmed || record is null)
        {
            return;
        }
        records.Remove(record);
        RefreshItems(preserveScroll: true);
    }

    private void CloseBrowser()
    {
        browser = null;
        pendingBrowseRecord = null;
    }

    private void CloseDeleteDialog()
    {
        deleteDialog = null;
        pendingDeleteRecord = null;
    }

    private void CloseShowDialog()
    {
        if (showDialog is null)
        {
            return;
        }
        showDialog = null;
        closeModal();
    }
}

public sealed class GalleryView : GalleryViewUI
{
    private readonly GalleryScroll scroll;

    public GalleryView(string renderMode)
    {
        scroll = new GalleryScroll();
        gallery_scroll.SetChild(scroll);
        scroll.Populate(renderMode);
    }

    public void SetRenderMode(string renderMode)
    {
        scroll.Populate(renderMode);
    }
}

public sealed class ImageBrowser : ImageBrowserUI
{
    private const string RenderModeSixel = "sixel";
    private const string RenderModeFallback = "fallback";
    private readonly List<ImageRecord> images = ImageLibrary();
    private readonly List<ImageRecord> imageRecords;
    private readonly ImageButton[] thumbs = new ImageButton[3];
    private readonly GalleryMosaic galleryMosaicControl;
    private readonly ImageListButton imageListButtonControl;
    private string renderMode = RenderModeSixel;
    private bool finished;

    public ImageBrowser()
    {
        imageRecords = images
            .Take(20)
            .Select(record => new ImageRecord(record.Name, record.Source, record.Selected))
            .ToList();
        galleryMosaicControl = new GalleryMosaic();
        gallery_mosaic.SetChild(galleryMosaicControl);
        galleryMosaicControl.SetOnGalleryClick(ShowGallery);

        imageListButtonControl = new ImageListButton();
        image_list_btn.SetChild(imageListButtonControl);
        imageListButtonControl.SetImages(images);
        imageListButtonControl.SetOnClick(ShowImageList);

        ReusableElement[] slots = { camera_thumb, astro_thumb, coffee_thumb };
        for (int index = 0; index < slots.Length; ++index)
        {
            int imageIndex = index;
            ImageButton button = new();
            slots[index].SetChild(button);
            button.SetSource(images[index].Source);
            button.SetAlt(images[index].Name);
            button.SetOnClick(() => ShowImage(imageIndex));
            thumbs[index] = button;
        }
        ApplyRenderMode(RenderModeSixel);
        ShowImage(0);
    }

    public override GeneratedWindowRuntimeOptions RuntimeOptions()
    {
        GeneratedWindowRuntimeOptions options = base.RuntimeOptions();
        options.InitialFocusName = "camera_thumb";
        return options;
    }

    protected override void onCloseBtnClick()
    {
        finished = true;
    }

    protected override void onModeSelectChange(string value)
    {
        ApplyRenderMode(value == "Normal" ? RenderModeSixel : RenderModeFallback);
    }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "camera_thumb")
        {
            ShowImage(0);
            return true;
        }
        if (name == "astro_thumb")
        {
            ShowImage(1);
            return true;
        }
        if (name == "coffee_thumb")
        {
            ShowImage(2);
            return true;
        }
        if (name == "image_list_btn")
        {
            ShowImageList();
            return true;
        }
        if (name == "gallery_mosaic")
        {
            ShowGallery();
            return true;
        }
        return base.HandleGeneratedButton(name);
    }

    protected override bool shouldClose()
    {
        return finished;
    }

    private void ShowImage(int index)
    {
        SelectThumb(index);
        galleryMosaicControl.SetSelected(false);
        imageListButtonControl.SetSelected(false);
        main.SetChild(new ImageView(images[index].Source, images[index].Name, renderMode));
    }

    private void ShowImageList()
    {
        DeselectAllThumbs();
        galleryMosaicControl.SetSelected(false);
        imageListButtonControl.SetSelected(true);
        ImageListView view = new(
            imageRecords,
            renderMode,
            (window, options) => OpenWindow(window, options),
            CloseWindow);
        main.SetChild(view);
    }

    private void ShowGallery()
    {
        DeselectAllThumbs();
        galleryMosaicControl.SetSelected(true);
        imageListButtonControl.SetSelected(false);
        main.SetChild(new GalleryView(renderMode));
    }

    private void ApplyRenderMode(string mode)
    {
        renderMode = mode;
        foreach (ImageButton thumb in thumbs)
        {
            thumb?.SetRenderMode(renderMode);
        }
        galleryMosaicControl.SetRenderMode(renderMode);
        imageListButtonControl.SetRenderMode(renderMode);
        if (main.Child is ImageView imageView)
        {
            imageView.photo.SetRenderMode(renderMode);
        }
        else if (main.Child is GalleryView galleryView)
        {
            galleryView.SetRenderMode(renderMode);
        }
        else if (main.Child is ImageListView listView)
        {
            listView.SetRenderMode(renderMode);
        }
    }

    private void SelectThumb(int selectedIndex)
    {
        for (int index = 0; index < thumbs.Length; ++index)
        {
            thumbs[index]?.SetSelected(index == selectedIndex);
        }
    }

    private void DeselectAllThumbs()
    {
        for (int index = 0; index < thumbs.Length; ++index)
        {
            thumbs[index]?.SetSelected(false);
        }
    }

    public static List<ImageRecord> ImageLibrary()
    {
        List<ImageRecord> result = new()
        {
            new("Camera", "shared/assets/image_samples/camera.png"),
            new("Astronaut", "shared/assets/image_samples/astronaut.png"),
            new("Coffee", "shared/assets/image_samples/coffee.png"),
            new("Coins", "shared/assets/image_samples/coins.png"),
            new("Chelsea", "shared/assets/image_samples/chelsea.png"),
        };
        for (int index = 6; index <= 50; ++index)
        {
            result.Add(new ImageRecord("Sample " + index, "shared/assets/image_samples/sample_" + index + SampleExtension(index)));
        }
        return result;
    }

    private static string SampleExtension(int index)
    {
        return (index % 6) switch
        {
            0 => ".bmp",
            1 => ".gif",
            2 => ".png",
            3 => ".tga",
            _ => ".jpg",
        };
    }

    public const string ImageExtensionFilter = "(png|jpg|jpeg|gif|bmp|tga|ppm|pgm)";

    public static string ProjectRoot()
    {
        return Directory.GetCurrentDirectory();
    }

    public static string ImageSampleDir()
    {
        return Path.Combine(ProjectRoot(), "shared", "assets", "image_samples");
    }

    public static string ImageDisplayPath(string path)
    {
        if (string.IsNullOrEmpty(path))
        {
            return "";
        }
        string absoluteSourcePath = Path.GetFullPath(path);
        string root = ProjectRoot();
        if (PathStartsWith(absoluteSourcePath, root))
        {
            return Path.GetRelativePath(root, absoluteSourcePath);
        }
        return path;
    }

    public static string ImageNameFromPath(string path)
    {
        string stem = Path.GetFileNameWithoutExtension(path);
        if (string.IsNullOrEmpty(stem))
        {
            return path;
        }
        stem = stem.Replace('_', ' ').Replace('-', ' ');
        string[] words = stem.Split(' ', StringSplitOptions.RemoveEmptyEntries);
        if (words.Length == 0)
        {
            return stem;
        }
        for (int index = 0; index < words.Length; ++index)
        {
            string word = words[index];
            if (word.Length > 0)
            {
                words[index] = char.ToUpperInvariant(word[0]) + word[1..];
            }
        }
        return string.Join(" ", words);
    }

    private static bool PathStartsWith(string path, string root)
    {
        string normalizedPath = Path.GetFullPath(path)
            .TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar) + Path.DirectorySeparatorChar;
        string normalizedRoot = Path.GetFullPath(root)
            .TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar) + Path.DirectorySeparatorChar;
        return normalizedPath.StartsWith(normalizedRoot, StringComparison.Ordinal);
    }

}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            ImageBrowser app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
