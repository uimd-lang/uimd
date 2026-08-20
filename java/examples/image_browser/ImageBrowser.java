import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

import uimd.Color;
import uimd.ElementRenderState;
import uimd.FileBrowser;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedScrollViewBase;
import uimd.GeneratedWindowBase;
import uimd.GeneratedWindowRuntime;
import uimd.GeneratedWindowRuntimeOptions;
import uimd.GeneratedWindowStack;
import uimd.Image;
import uimd.MessageBoxYesNo;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.ScrollViewPosition;
import uimd.Size;
import uimd.TerminalCell;

public final class ImageBrowser extends ImageBrowserUI
{
    private static final int INITIAL_LIST_COUNT = 20;
    private static final int FIRST_GENERATED_SAMPLE_INDEX = 6;
    private static final int LAST_GENERATED_SAMPLE_INDEX = 50;
    private static final int SIDEBAR_IMAGE_COUNT = 3;
    private static final int GALLERY_IMAGE_COUNT = 5;
    private static final int SMOKE_IMAGE_WIDTH = 8;
    private static final int SMOKE_IMAGE_HEIGHT = 4;
    private static final String IMAGE_SAMPLE_DIR = "shared/assets/image_samples";
    private static final String IMAGE_EXTENSION_FILTER = "(png|jpg|jpeg|gif|bmp|tga|ppm|pgm)";
    private static final String THUMB_NORMAL_BACKGROUND = "transparent";
    private static final String THUMB_SELECTED_BACKGROUND = "#2563eb";
    private static final String MOSAIC_NORMAL_BACKGROUND = "#172033";
    private static final String RENDER_MODE_SIXEL = "sixel";
    private static final String RENDER_MODE_FALLBACK = "fallback";
    private static final String NORMAL_MODE_LABEL = "Normal";
    private static final String UPPER_HALF_BLOCK = "▀";

    private final List<ImageRecord> images = imageLibrary();
    private final List<ImageRecord> imageRecords = initialImageRecords(images);
    private final ImageButton[] thumbs = new ImageButton[SIDEBAR_IMAGE_COUNT];
    private final GalleryMosaic galleryMosaicControl;
    private final ImageListButton imageListButtonControl;
    private final GeneratedWindowStack modalStack = new GeneratedWindowStack();
    private String renderMode = RENDER_MODE_SIXEL;
    private boolean closeRequested;

    public ImageBrowser()
    {
        galleryMosaicControl = new GalleryMosaic(this::showGallery);
        gallery_mosaic.setChild(galleryMosaicControl);

        imageListButtonControl = new ImageListButton();
        image_list_btn.setChild(imageListButtonControl);
        imageListButtonControl.setImages(images);
        imageListButtonControl.setOnClick(this::showImageList);

        List<ReusableElement> slots = List.of(camera_thumb, astro_thumb, coffee_thumb);
        for (int index = 0; index < slots.size(); ++index)
        {
            int imageIndex = index;
            ImageButton button = new ImageButton();
            slots.get(index).setChild(button);
            button.setSource(images.get(index).source());
            button.setAlt(images.get(index).name());
            button.setOnClick(() -> showImage(imageIndex));
            thumbs[index] = button;
        }
        applyRenderMode(RENDER_MODE_SIXEL);
        showImage(0);
    }

    @Override
    public GeneratedWindowRuntimeOptions runtimeOptions()
    {
        GeneratedWindowRuntimeOptions options = super.runtimeOptions();
        options.setInitialFocusName("camera_thumb");
        options.setWindowStack(modalStack);
        return options;
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        switch (name)
        {
            case "camera_thumb" -> showImage(0);
            case "astro_thumb" -> showImage(1);
            case "coffee_thumb" -> showImage(2);
            case "gallery_mosaic" -> showGallery();
            case "image_list_btn" -> showImageList();
            default ->
            {
                return super.handleGeneratedButton(name);
            }
        }
        return true;
    }

    @Override
    protected void onCloseBtnClick()
    {
        closeRequested = true;
    }

    @Override
    protected void onModeSelectChange(String value)
    {
        applyRenderMode(NORMAL_MODE_LABEL.equals(value) ? RENDER_MODE_SIXEL : RENDER_MODE_FALLBACK);
    }

    @Override
    protected boolean shouldClose()
    {
        return closeRequested;
    }

    void showImage(int index)
    {
        selectThumb(index);
        galleryMosaicControl.setSelected(false);
        imageListButtonControl.setSelected(false);
        ImageRecord image = images.get(index);
        main.setView(new ImageView(image.source(), image.name(), renderMode));
    }

    void showGallery()
    {
        deselectAllThumbs();
        galleryMosaicControl.setSelected(true);
        imageListButtonControl.setSelected(false);
        main.setView(new GalleryView(renderMode));
    }

    void showImageList()
    {
        deselectAllThumbs();
        galleryMosaicControl.setSelected(false);
        imageListButtonControl.setSelected(true);
        main.setView(new ImageListView(imageRecords, renderMode, modalStack));
    }

    void applyRenderMode(String mode)
    {
        renderMode = mode;
        for (ImageButton thumb : thumbs)
        {
            if (thumb != null)
            {
                thumb.setRenderMode(renderMode);
            }
        }
        galleryMosaicControl.setRenderMode(renderMode);
        imageListButtonControl.setRenderMode(renderMode);
        if (main.currentView() instanceof ImageView imageView)
        {
            imageView.photo.setRenderMode(renderMode);
        }
        else if (main.currentView() instanceof GalleryView galleryView)
        {
            galleryView.setRenderMode(renderMode);
        }
        else if (main.currentView() instanceof ImageListView listView)
        {
            listView.setRenderMode(renderMode);
        }
    }

    int imageRecordCount()
    {
        return imageRecords.size();
    }

    private void selectThumb(int selectedIndex)
    {
        for (int index = 0; index < thumbs.length; ++index)
        {
            thumbs[index].setSelected(index == selectedIndex);
        }
    }

    private void deselectAllThumbs()
    {
        for (ImageButton thumb : thumbs)
        {
            thumb.setSelected(false);
        }
    }

    static List<ImageRecord> imageLibrary()
    {
        List<ImageRecord> result = new ArrayList<>();
        result.add(new ImageRecord("Camera", "shared/assets/image_samples/camera.png"));
        result.add(new ImageRecord("Astronaut", "shared/assets/image_samples/astronaut.png"));
        result.add(new ImageRecord("Coffee", "shared/assets/image_samples/coffee.png"));
        result.add(new ImageRecord("Coins", "shared/assets/image_samples/coins.png"));
        result.add(new ImageRecord("Chelsea", "shared/assets/image_samples/chelsea.png"));
        for (int index = FIRST_GENERATED_SAMPLE_INDEX; index <= LAST_GENERATED_SAMPLE_INDEX; ++index)
        {
            result.add(new ImageRecord(
                "Sample " + index,
                "shared/assets/image_samples/sample_" + index + sampleExtension(index)));
        }
        return result;
    }

    static Path projectRoot()
    {
        Path current = Path.of("").toAbsolutePath().normalize();
        Path root = findProjectRoot(current);
        if (root != null)
        {
            return root;
        }
        try
        {
            Path location = Path.of(
                ImageBrowser.class.getProtectionDomain().getCodeSource().getLocation().toURI());
            root = findProjectRoot(Files.isDirectory(location) ? location : location.getParent());
            if (root != null)
            {
                return root;
            }
        }
        catch (URISyntaxException | SecurityException ignored)
        {
            // Fall back to the process directory when code-source metadata is unavailable.
        }
        return current;
    }

    static Path imageSampleDir()
    {
        return projectRoot().resolve(IMAGE_SAMPLE_DIR).normalize();
    }

    static String imageDisplayPath(String source)
    {
        if (source == null || source.isEmpty())
        {
            return "";
        }
        Path root = projectRoot();
        Path sourcePath = Path.of(source);
        Path absolute = (sourcePath.isAbsolute() ? sourcePath : root.resolve(sourcePath))
            .toAbsolutePath()
            .normalize();
        return absolute.startsWith(root) ? root.relativize(absolute).toString() : source;
    }

    static String imageNameFromPath(Path path)
    {
        if (path == null || path.getFileName() == null)
        {
            return path == null ? "" : path.toString();
        }
        String fileName = path.getFileName().toString();
        int dot = fileName.lastIndexOf('.');
        String stem = dot > 0 ? fileName.substring(0, dot) : fileName;
        String normalized = stem.replace('_', ' ').replace('-', ' ').trim();
        if (normalized.isEmpty())
        {
            return stem;
        }
        String[] words = normalized.split("\\s+");
        StringBuilder result = new StringBuilder();
        for (String word : words)
        {
            if (!word.isEmpty())
            {
                if (!result.isEmpty())
                {
                    result.append(' ');
                }
                result.append(Character.toUpperCase(word.charAt(0)));
                result.append(word.substring(1));
            }
        }
        return result.toString();
    }

    private static Path findProjectRoot(Path start)
    {
        for (Path candidate = start; candidate != null; candidate = candidate.getParent())
        {
            if (Files.isDirectory(candidate.resolve(IMAGE_SAMPLE_DIR)))
            {
                return candidate;
            }
        }
        return null;
    }

    private static List<ImageRecord> initialImageRecords(List<ImageRecord> library)
    {
        List<ImageRecord> result = new ArrayList<>();
        int count = Math.min(INITIAL_LIST_COUNT, library.size());
        for (int index = 0; index < count; ++index)
        {
            ImageRecord record = library.get(index);
            result.add(new ImageRecord(record.name(), record.source(), record.selected()));
        }
        return result;
    }

    private static String sampleExtension(int index)
    {
        return switch (index % 6)
        {
            case 0 -> ".bmp";
            case 1 -> ".gif";
            case 2 -> ".png";
            case 3 -> ".tga";
            default -> ".jpg";
        };
    }

    private static boolean hasArgument(String[] arguments, String value)
    {
        for (String argument : arguments)
        {
            if (value.equals(argument))
            {
                return true;
            }
        }
        return false;
    }

    private static void logicTest()
    {
        require("Chelsea".equals(imageNameFromPath(Path.of("/tmp/chelsea.png"))), "unexpected image name");
        require(
            imageDisplayPath("shared/assets/image_samples/camera.png")
                .equals(Path.of("shared", "assets", "image_samples", "camera.png").toString()),
            "unexpected display path");
        List<ImageRecord> library = imageLibrary();
        require(library.get(5).source().endsWith(".bmp"), "BMP sample extension missing");
        require(library.get(6).source().endsWith(".gif"), "GIF sample extension missing");
        require(library.get(7).source().endsWith(".png"), "PNG sample extension missing");
        require(library.get(8).source().endsWith(".tga"), "TGA sample extension missing");
        Image sample = new Image(
            "sample",
            library.get(5).source(),
            library.get(5).name(),
            "contain",
            RENDER_MODE_FALLBACK,
            "center",
            "middle");
        List<List<TerminalCell>> rendered = sample.render(
            new Size(SMOKE_IMAGE_WIDTH, SMOKE_IMAGE_HEIGHT),
            new ElementRenderState());
        require(
            rendered.stream().flatMap(List::stream).anyMatch(cell -> UPPER_HALF_BLOCK.equals(cell.text())),
            "mixed-format image did not render");

        ImageBrowser shell = new ImageBrowser();
        require(shell.main.currentView() instanceof ImageView, "initial image view missing");
        for (int index = 0; index < SIDEBAR_IMAGE_COUNT; ++index)
        {
            shell.showImage(index);
            require(shell.main.currentView() instanceof ImageView, "sidebar image view missing");
        }
        shell.applyRenderMode(RENDER_MODE_FALLBACK);
        shell.applyRenderMode(RENDER_MODE_SIXEL);
        shell.showGallery();
        require(shell.main.currentView() instanceof GalleryView, "gallery view missing");
        shell.showImageList();
        require(shell.main.currentView() instanceof ImageListView, "image list view missing");
        require(shell.imageRecordCount() == INITIAL_LIST_COUNT, "unexpected image record count");
        require(!shell.galleryMosaicControl.gal_1.focusable(), "gallery image should not be focusable");
        require("camera_thumb".equals(shell.runtimeOptions().initialFocusName()), "initial focus mismatch");
        require(shell.runtimeOptions().windowStack() == shell.modalStack, "modal stack missing");
        require(shell.galleryMosaicControl.activateGeneratedControl(), "gallery activation failed");
        require(shell.main.currentView() instanceof GalleryView, "gallery activation did not switch view");
        require(shell.thumbs[0].activateGeneratedControl(), "thumbnail activation failed");
        require(shell.main.currentView() instanceof ImageView, "thumbnail activation did not switch view");
    }

    private static void require(boolean condition, String message)
    {
        if (!condition)
        {
            throw new IllegalStateException(message);
        }
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            if (hasArgument(arguments, "--logic-test") || hasArgument(arguments, "--smoke"))
            {
                logicTest();
                return 0;
            }
            ImageBrowser application = new ImageBrowser();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }

    private static final class ImageRecord
    {
        private String name;
        private String source;
        private boolean selected;

        ImageRecord(String name, String source)
        {
            this(name, source, false);
        }

        ImageRecord(String name, String source, boolean selected)
        {
            this.name = name;
            this.source = source;
            this.selected = selected;
        }

        String name()
        {
            return name;
        }

        void setName(String value)
        {
            name = value;
        }

        String source()
        {
            return source;
        }

        void setSource(String value)
        {
            source = value;
        }

        boolean selected()
        {
            return selected;
        }

        void setSelected(boolean value)
        {
            selected = value;
        }
    }

    private static final class ImageView extends ImageViewUI
    {
        ImageView(String source, String captionText, String imageRenderMode)
        {
            photo.setSource(source);
            photo.setAlt(captionText);
            photo.setRenderMode(imageRenderMode);
            caption.setText(captionText);
        }
    }

    private static final class ImageButton extends ImageButtonUI
    {
        private Runnable onClick;

        ImageButton()
        {
            photo.setFocusable(false);
        }

        void setSource(String value)
        {
            photo.setSource(value);
        }

        void setAlt(String value)
        {
            photo.setAlt(value);
        }

        void setRenderMode(String value)
        {
            photo.setRenderMode(value);
        }

        void setSelected(boolean selected)
        {
            setLayoutCellBackground(this, selected ? THUMB_SELECTED_BACKGROUND : THUMB_NORMAL_BACKGROUND);
        }

        void setOnClick(Runnable value)
        {
            onClick = value;
        }

        @Override
        public boolean activateGeneratedControl()
        {
            onPhotoClick();
            return onClick != null;
        }

        @Override
        protected void onPhotoClick()
        {
            if (onClick != null)
            {
                onClick.run();
            }
        }
    }

    private static final class GalleryMosaic extends GalleryMosaicUI
    {
        private final Runnable onClick;

        GalleryMosaic(Runnable onClick)
        {
            this.onClick = onClick;
            for (Image image : images())
            {
                image.setFocusable(false);
            }
        }

        void setRenderMode(String value)
        {
            for (Image image : images())
            {
                image.setRenderMode(value);
            }
        }

        void setSelected(boolean selected)
        {
            setLayoutCellBackground(this, selected ? THUMB_SELECTED_BACKGROUND : MOSAIC_NORMAL_BACKGROUND);
        }

        @Override
        public boolean activateGeneratedControl()
        {
            fire();
            return onClick != null;
        }

        @Override
        protected void onGal1Click()
        {
            fire();
        }

        @Override
        protected void onGal2Click()
        {
            fire();
        }

        @Override
        protected void onGal3Click()
        {
            fire();
        }

        @Override
        protected void onGal4Click()
        {
            fire();
        }

        @Override
        protected void onGal5Click()
        {
            fire();
        }

        private void fire()
        {
            if (onClick != null)
            {
                onClick.run();
            }
        }

        private List<Image> images()
        {
            return List.of(gal_1, gal_2, gal_3, gal_4, gal_5);
        }
    }

    private static final class ImageListButton extends ImageListButtonUI
    {
        private Runnable onClick;

        ImageListButton()
        {
            for (Image image : images())
            {
                image.setFocusable(false);
            }
        }

        void setImages(List<ImageRecord> records)
        {
            List<Image> previews = images();
            for (int index = 0; index < previews.size() && index < records.size(); ++index)
            {
                previews.get(index).setSource(records.get(index).source());
                previews.get(index).setAlt(records.get(index).name());
            }
        }

        void setRenderMode(String value)
        {
            for (Image image : images())
            {
                image.setRenderMode(value);
            }
        }

        void setSelected(boolean selected)
        {
            setLayoutCellBackground(this, selected ? THUMB_SELECTED_BACKGROUND : MOSAIC_NORMAL_BACKGROUND);
        }

        void setOnClick(Runnable value)
        {
            onClick = value;
        }

        @Override
        public boolean activateGeneratedControl()
        {
            fire();
            return onClick != null;
        }

        @Override
        protected void onPrw1Click()
        {
            fire();
        }

        @Override
        protected void onPrw2Click()
        {
            fire();
        }

        @Override
        protected void onPrw3Click()
        {
            fire();
        }

        private void fire()
        {
            if (onClick != null)
            {
                onClick.run();
            }
        }

        private List<Image> images()
        {
            return List.of(prw_1, prw_2, prw_3);
        }
    }

    private static final class GalleryItem extends GalleryItemUI
    {
        GalleryItem(String source, String captionText, String imageRenderMode)
        {
            photo.setSource(source);
            photo.setRenderMode(imageRenderMode);
            caption.setText(captionText);
        }
    }

    private static final class GalleryScroll extends GalleryScrollUI
    {
        void populate(String imageRenderMode)
        {
            clearChildren();
            List<ImageRecord> library = imageLibrary();
            int count = Math.min(library.size(), GALLERY_IMAGE_COUNT);
            for (int index = 0; index < count; ++index)
            {
                ImageRecord record = library.get(index);
                GalleryItem item = new GalleryItem(record.source(), record.name(), imageRenderMode);
                item.setMode("expand_width");
                ReusableElement reusable = new ReusableElement("gallery_item", item);
                reusable.setFrame(new Rect(0, 0, 0, measuredLayoutHeight(item)));
                addChild(reusable);
            }
            scrollToTop();
        }
    }

    private static final class GalleryView extends GalleryViewUI
    {
        private final GalleryScroll scroll;

        GalleryView(String imageRenderMode)
        {
            scroll = new GalleryScroll();
            gallery_scroll.setChild(scroll);
            scroll.populate(imageRenderMode);
        }

        void setRenderMode(String value)
        {
            scroll.populate(value);
        }
    }

    private static final class ImageShowDialog extends ImageShowDialogUI
    {
        ImageShowDialog(String source, String captionText, String imageRenderMode)
        {
            title.setText(captionText);
            photo.setSource(source);
            photo.setAlt(captionText);
            photo.setRenderMode(imageRenderMode);
        }
    }

    private static final class ImageListItem extends ImageListItemUI
    {
        private final ImageRecord record;
        private final Consumer<ImageRecord> onShow;
        private final Consumer<ImageRecord> onBrowse;
        private final Consumer<ImageRecord> onDelete;
        private final Consumer<ImageRecord> onChange;

        ImageListItem(
            ImageRecord record,
            String imageRenderMode,
            Consumer<ImageRecord> onShow,
            Consumer<ImageRecord> onBrowse,
            Consumer<ImageRecord> onDelete,
            Consumer<ImageRecord> onChange)
        {
            this.record = record;
            this.onShow = onShow;
            this.onBrowse = onBrowse;
            this.onDelete = onDelete;
            this.onChange = onChange;
            syncFromRecord(imageRenderMode);
        }

        @Override
        protected void onSelChange(String value)
        {
            record.setSelected(sel.checked());
            accept(onChange, record);
        }

        @Override
        protected void onShowBtnClick()
        {
            accept(onShow, record);
        }

        @Override
        protected void onBrowseBtnClick()
        {
            accept(onBrowse, record);
        }

        @Override
        protected void onDeleteBtnClick()
        {
            accept(onDelete, record);
        }

        private void syncFromRecord(String imageRenderMode)
        {
            thumb.setSource(record.source());
            thumb.setAlt(record.name());
            thumb.setFit("contain");
            thumb.setRenderMode(imageRenderMode);
            name_label.setText(record.name());
            path_label.setText(imageDisplayPath(record.source()));
            sel.setChecked(record.selected());
        }
    }

    private static final class ImageListScroll extends ImageListScrollUI
    {
        void setItems(
            List<ImageRecord> records,
            String imageRenderMode,
            Consumer<ImageRecord> onShow,
            Consumer<ImageRecord> onBrowse,
            Consumer<ImageRecord> onDelete,
            Consumer<ImageRecord> onChange,
            boolean scrollToEnd,
            boolean preserveScroll)
        {
            ScrollViewPosition position = scrollPosition();
            clearChildren();
            setAutoScroll(scrollToEnd);
            for (int index = 0; index < records.size(); ++index)
            {
                ImageListItem item = new ImageListItem(
                    records.get(index),
                    imageRenderMode,
                    onShow,
                    onBrowse,
                    onDelete,
                    onChange);
                item.setMode("expand_width");
                ReusableElement reusable = new ReusableElement("items[" + index + "]", item);
                reusable.setFrame(new Rect(0, 0, 0, measuredLayoutHeight(item)));
                addChild(reusable);
            }
            if (scrollToEnd)
            {
                scrollToBottom();
            }
            else if (preserveScroll)
            {
                restoreScrollPosition(position);
            }
            else
            {
                scrollToTop();
            }
        }
    }

    private static final class ImageListView extends ImageListViewUI
    {
        private final List<ImageRecord> records;
        private final ImageListScroll scroll;
        private final GeneratedWindowStack modalStack;
        private String renderMode;
        private FileBrowser browser;
        private MessageBoxYesNo deleteDialog;
        private ImageShowDialog showDialog;
        private ImageRecord pendingBrowseRecord;
        private ImageRecord pendingDeleteRecord;

        ImageListView(
            List<ImageRecord> records,
            String imageRenderMode,
            GeneratedWindowStack modalStack)
        {
            this.records = records;
            renderMode = imageRenderMode;
            this.modalStack = modalStack;
            scroll = new ImageListScroll();
            items.setChild(scroll);
            refreshItems(false, false);
        }

        void setRenderMode(String value)
        {
            renderMode = value;
            refreshItems(false, true);
        }

        @Override
        protected void onAddBtnClick()
        {
            openBrowser(null);
        }

        private void refreshItems(boolean scrollToEnd, boolean preserveScroll)
        {
            scroll.setItems(
                records,
                renderMode,
                this::showImage,
                this::openBrowser,
                this::confirmDelete,
                ignored -> { },
                scrollToEnd,
                preserveScroll);
        }

        private void showImage(ImageRecord record)
        {
            closeShowDialog();
            showDialog = new ImageShowDialog(record.source(), record.name(), renderMode);
            GeneratedWindowRuntimeOptions options = showDialog.runtimeOptions();
            options.setInitialFocusName("close_btn");
            options.setWindowStack(modalStack);
            options.setOnButton(name ->
            {
                if ("close_btn".equals(name))
                {
                    closeShowDialog();
                }
            });
            options.setOnKey(key ->
            {
                if ("Escape".equals(key))
                {
                    closeShowDialog();
                    return true;
                }
                return false;
            });
            modalStack.push(showDialog, options);
        }

        private void openBrowser(ImageRecord record)
        {
            closeBrowser();
            pendingBrowseRecord = record;
            Path startPath = record == null
                ? imageSampleDir()
                : resolveSourcePath(record.source());
            browser = new FileBrowser(
                projectRoot(),
                startPath,
                FileBrowser.MODE_OPEN,
                this::onBrowserClosed,
                "",
                IMAGE_EXTENSION_FILTER);
            modalStack.push(browser, browser.runtimeOptions(modalStack));
        }

        private void onBrowserClosed(Path path)
        {
            if (path == null)
            {
                closeBrowser();
                return;
            }
            if (pendingBrowseRecord == null)
            {
                records.add(new ImageRecord(imageNameFromPath(path), path.toString()));
                closeBrowser();
                refreshItems(true, false);
                return;
            }
            pendingBrowseRecord.setName(imageNameFromPath(path));
            pendingBrowseRecord.setSource(path.toString());
            closeBrowser();
            refreshItems(false, true);
        }

        private void confirmDelete(ImageRecord record)
        {
            closeDeleteDialog();
            pendingDeleteRecord = record;
            deleteDialog = new MessageBoxYesNo(
                "Delete Image",
                "Delete " + record.name() + "?",
                this::deleteConfirmed);
            GeneratedWindowRuntimeOptions options = deleteDialog.runtimeOptions();
            options.setWindowStack(modalStack);
            modalStack.push(deleteDialog, options);
        }

        private void deleteConfirmed(String result)
        {
            ImageRecord record = pendingDeleteRecord;
            closeDeleteDialog();
            if (!"yes".equals(result) || record == null)
            {
                return;
            }
            records.removeIf(item -> item == record);
            refreshItems(false, true);
        }

        private void closeBrowser()
        {
            if (browser != null)
            {
                modalStack.remove(browser);
                browser = null;
            }
            pendingBrowseRecord = null;
        }

        private void closeDeleteDialog()
        {
            if (deleteDialog != null)
            {
                modalStack.remove(deleteDialog);
                deleteDialog = null;
            }
            pendingDeleteRecord = null;
        }

        private void closeShowDialog()
        {
            if (showDialog != null)
            {
                modalStack.remove(showDialog);
                showDialog = null;
            }
        }
    }

    private static void setLayoutCellBackground(GeneratedWindowBase window, String background)
    {
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            entry.cellStyle().setBackground(new Color(background));
        }
    }

    private static int measuredLayoutHeight(GeneratedWindowBase window)
    {
        Size natural = GeneratedWindowRuntime.generatedWindowContentSize(window);
        Size measured = GeneratedWindowRuntime.generatedWindowContentSizeForWidth(window, natural.width());
        return Math.max(1, measured.height());
    }

    private static Path resolveSourcePath(String source)
    {
        Path path = Path.of(source);
        return (path.isAbsolute() ? path : projectRoot().resolve(path)).normalize();
    }

    private static void accept(Consumer<ImageRecord> callback, ImageRecord record)
    {
        if (callback != null)
        {
            callback.accept(record);
        }
    }
}
