#include "image_browser_ui.hpp"
#include "image_view_ui.hpp"
#include "image_button_ui.hpp"
#include "gallery_item_ui.hpp"
#include "gallery_mosaic_ui.hpp"
#include "gallery_scroll_ui.hpp"
#include "gallery_view_ui.hpp"
#include "image_list_button_ui.hpp"
#include "image_list_item_ui.hpp"
#include "image_list_scroll_ui.hpp"
#include "image_list_view_ui.hpp"
#include "image_show_dialog_ui.hpp"

#include "file_browser.hpp"
#include "message_box.hpp"
#include "ui/elements/CheckBox.hpp"
#include "ui/elements/Image.hpp"
#include "ui/elements/ReusableElement.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <filesystem>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{

struct ImageEntry
{
    std::string caption;
    std::string source;
};

struct SidebarImageEntry
{
    std::string_view memberName;
    std::string caption;
    std::string source;
};

struct ImageRecord
{
    std::string name;
    std::string source;
    bool selected = false;
};

constexpr std::size_t kInitialListCount = 20;
constexpr int kFirstGeneratedSampleIndex = 6;
constexpr int kLastGeneratedSampleIndex = 50;
constexpr std::string_view kImageSampleDir = "shared/assets/image_samples";
constexpr std::string_view kImageExtensionFilter = "(png|jpg|jpeg|gif|bmp|tga|ppm|pgm)";
constexpr std::string_view kThumbNormalBg = "transparent";
constexpr std::string_view kThumbSelectedBg = "#2563eb";
constexpr std::string_view kMosaicNormalBg = "#172033";
constexpr std::string_view kRenderModeSixel = "sixel";
constexpr std::string_view kRenderModeFallback = "fallback";

std::string sampleExtension(int index)
{
    switch (index % 6)
    {
        case 0:
            return ".bmp";
        case 1:
            return ".gif";
        case 2:
            return ".png";
        case 3:
            return ".tga";
        default:
            return ".jpg";
    }
}

std::vector<ImageEntry> imageLibrary()
{
    std::vector<ImageEntry> images{
        {"Camera", "shared/assets/image_samples/camera.png"},
        {"Astronaut", "shared/assets/image_samples/astronaut.png"},
        {"Coffee", "shared/assets/image_samples/coffee.png"},
        {"Coins", "shared/assets/image_samples/coins.png"},
        {"Chelsea", "shared/assets/image_samples/chelsea.png"},
    };
    for (int index = kFirstGeneratedSampleIndex; index <= kLastGeneratedSampleIndex; ++index)
    {
        images.push_back({
            "Sample " + std::to_string(index),
            "shared/assets/image_samples/sample_" + std::to_string(index) + sampleExtension(index)});
    }
    return images;
}

std::array<SidebarImageEntry, 3> sidebarImages()
{
    const std::vector<ImageEntry> images = imageLibrary();
    return {{
        {"camera_thumb", images[0].caption, images[0].source},
        {"astro_thumb", images[1].caption, images[1].source},
        {"coffee_thumb", images[2].caption, images[2].source},
    }};
}

std::vector<ImageRecord> initialImageRecords()
{
    const std::vector<ImageEntry> images = imageLibrary();
    std::vector<ImageRecord> records;
    for (std::size_t index = 0; index < std::min(kInitialListCount, images.size()); ++index)
    {
        records.push_back({images[index].caption, images[index].source, false});
    }
    return records;
}

std::string imageNameFromPath(const std::string& path)
{
    std::string stem = std::filesystem::path(path).stem().string();
    if (stem.empty())
    {
        return path;
    }
    std::replace(stem.begin(), stem.end(), '_', ' ');
    std::replace(stem.begin(), stem.end(), '-', ' ');
    std::istringstream stream(stem);
    std::string word;
    std::string result;
    while (stream >> word)
    {
        if (!word.empty())
        {
            word[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
        }
        if (!result.empty())
        {
            result += " ";
        }
        result += word;
    }
    return result.empty() ? stem : result;
}

std::filesystem::path projectRoot()
{
    return std::filesystem::current_path().lexically_normal();
}

std::filesystem::path imageSampleDir()
{
    return projectRoot() / std::string{kImageSampleDir};
}

bool pathStartsWith(const std::filesystem::path& path, const std::filesystem::path& root)
{
    const std::string absolute = std::filesystem::absolute(path).lexically_normal().string();
    const std::string base = std::filesystem::absolute(root).lexically_normal().string();
    return absolute == base || absolute.rfind(base + std::string{std::filesystem::path::preferred_separator}, 0) == 0;
}

std::string imageDisplayPath(const std::string& path)
{
    if (path.empty())
    {
        return {};
    }
    const std::filesystem::path sourcePath{path};
    if (sourcePath.is_absolute() && pathStartsWith(sourcePath, projectRoot()))
    {
        return std::filesystem::relative(sourcePath, projectRoot()).generic_string();
    }
    return path;
}

class ImageView : public ImageViewUI
{
public:
    ImageView(std::string source, std::string captionText, std::string renderMode)
        : source_(std::move(source)),
          captionText_(std::move(captionText)),
          renderMode_(std::move(renderMode))
    {
        photo->setSource(source_);
        photo->setRenderMode(renderMode_);
        caption->setText(captionText_);
    }

private:
    std::string source_;
    std::string captionText_;
    std::string renderMode_;
};

class ImageButton : public ImageButtonUI
{
public:
    using ClickCallback = std::function<void()>;

    ImageButton()
    {
        photo->setFocusable(false);
    }

    void setSource(std::string source) { photo->setSource(std::move(source)); }
    void setAlt(std::string alt) { photo->setAlt(std::move(alt)); }
    void setRenderMode(std::string_view mode) { photo->setRenderMode(std::string{mode}); }

    void setSelected(bool selected)
    {
        std::vector<ui::GeneratedLayoutEntry> layout = generatedLayout();
        const ui::Color background{std::string{selected ? kThumbSelectedBg : kThumbNormalBg}};
        for (ui::GeneratedLayoutEntry& entry : layout)
        {
            entry.cellStyle.background = background;
        }
        setGeneratedLayout(std::move(layout));
    }

    void setOnClick(ClickCallback onClick) { onClick_ = std::move(onClick); }

    bool activateGeneratedControl() override
    {
        onPhotoClick();
        return onClick_ != nullptr;
    }

protected:
    void onPhotoClick() override { if (onClick_) onClick_(); }

private:
    ClickCallback onClick_;
};

class GalleryMosaic : public GalleryMosaicUI
{
public:
    using ClickCallback = std::function<void()>;

    explicit GalleryMosaic(ClickCallback onClick)
        : onClick_(std::move(onClick))
    {
        for (auto* img : {gal_1, gal_2, gal_3, gal_4, gal_5})
        {
            img->setFocusable(false);
        }
    }

    void setRenderMode(std::string_view mode)
    {
        for (auto* img : {gal_1, gal_2, gal_3, gal_4, gal_5})
        {
            img->setRenderMode(std::string{mode});
        }
    }

    void setSelected(bool selected)
    {
        std::vector<ui::GeneratedLayoutEntry> layout = generatedLayout();
        const ui::Color background{std::string{selected ? kThumbSelectedBg : kMosaicNormalBg}};
        for (ui::GeneratedLayoutEntry& entry : layout)
        {
            entry.cellStyle.background = background;
        }
        setGeneratedLayout(std::move(layout));
    }

    bool activateGeneratedControl() override
    {
        fire();
        return onClick_ != nullptr;
    }

protected:
    void onGal1Click() override { fire(); }
    void onGal2Click() override { fire(); }
    void onGal3Click() override { fire(); }
    void onGal4Click() override { fire(); }
    void onGal5Click() override { fire(); }

private:
    void fire() { if (onClick_) onClick_(); }
    ClickCallback onClick_;
};

class ImageListButton : public ImageListButtonUI
{
public:
    using ClickCallback = std::function<void()>;

    ImageListButton()
    {
        for (auto* image : {prw_1, prw_2, prw_3})
        {
            image->setFocusable(false);
        }
    }

    void setImages(const std::vector<ImageEntry>& images)
    {
        const std::array<ui::Image*, 3> previews{prw_1, prw_2, prw_3};
        for (std::size_t index = 0; index < previews.size() && index < images.size(); ++index)
        {
            previews[index]->setSource(images[index].source);
            previews[index]->setAlt(images[index].caption);
        }
    }

    void setRenderMode(std::string_view mode)
    {
        for (auto* image : {prw_1, prw_2, prw_3})
        {
            image->setRenderMode(std::string{mode});
        }
    }

    void setSelected(bool selected)
    {
        std::vector<ui::GeneratedLayoutEntry> layout = generatedLayout();
        const ui::Color background{std::string{selected ? kThumbSelectedBg : kMosaicNormalBg}};
        for (ui::GeneratedLayoutEntry& entry : layout)
        {
            entry.cellStyle.background = background;
        }
        setGeneratedLayout(std::move(layout));
    }

    void setOnClick(ClickCallback onClick) { onClick_ = std::move(onClick); }

    bool activateGeneratedControl() override
    {
        fire();
        return onClick_ != nullptr;
    }

protected:
    void onPrw1Click() override { fire(); }
    void onPrw2Click() override { fire(); }
    void onPrw3Click() override { fire(); }

private:
    void fire() { if (onClick_) onClick_(); }
    ClickCallback onClick_;
};

class GalleryItem : public GalleryItemUI
{
public:
    GalleryItem(std::string source, std::string captionText, std::string renderMode)
    {
        photo->setSource(std::move(source));
        photo->setRenderMode(std::move(renderMode));
        caption->setText(std::move(captionText));
    }
};

class GalleryScroll : public GalleryScrollUI
{
public:
    void populate(std::string_view renderMode)
    {
        clearChildren();
        const std::vector<ImageEntry> images = imageLibrary();
        for (std::size_t index = 0; index < std::min<std::size_t>(images.size(), 5); ++index)
        {
            auto item = std::make_unique<GalleryItem>(
                images[index].source,
                images[index].caption,
                std::string{renderMode});
            const ui::Size naturalSize = ui::generatedWindowContentSize(*item);
            const ui::Size measuredSize = ui::generatedWindowContentSizeForWidth(*item, naturalSize.width);
            auto reusable = std::make_unique<ui::ReusableElement>("gallery_item", std::move(item));
            reusable->setFrame(ui::Rect{0, 0, 0, std::max(1, measuredSize.height)});
            addChild(std::move(reusable));
        }
        (void)scrollToTop();
    }
};

class GalleryView : public GalleryViewUI
{
public:
    explicit GalleryView(std::string_view renderMode)
    {
        auto scroll = std::make_unique<GalleryScroll>();
        galleryScroll_ = scroll.get();
        gallery_scroll->setChild(std::move(scroll));
        galleryScroll_->populate(renderMode);
    }

    void setRenderMode(std::string_view renderMode)
    {
        if (galleryScroll_ != nullptr)
        {
            galleryScroll_->populate(renderMode);
        }
    }

private:
    GalleryScroll* galleryScroll_ = nullptr;
};

class ImageShowDialog : public ImageShowDialogUI
{
public:
    ImageShowDialog(std::string source, std::string captionText, std::string renderMode)
        : source_(std::move(source)),
          captionText_(std::move(captionText)),
          renderMode_(std::move(renderMode))
    {
        title->setText(captionText_);
        photo->setSource(source_);
        photo->setAlt(captionText_);
        photo->setRenderMode(renderMode_);
    }

private:
    std::string source_;
    std::string captionText_;
    std::string renderMode_;
};

class ImageListItem : public ImageListItemUI
{
public:
    using RecordCallback = std::function<void(ImageRecord&)>;

    ImageListItem(ImageRecord& record, std::string renderMode,
                  RecordCallback onShow, RecordCallback onBrowse,
                  RecordCallback onDelete, RecordCallback onChange)
        : record_(record),
          renderMode_(std::move(renderMode)),
          onShow_(std::move(onShow)),
          onBrowse_(std::move(onBrowse)),
          onDelete_(std::move(onDelete)),
          onChange_(std::move(onChange))
    {
        syncFromRecord();
    }

protected:
    void onSelChange(std::string_view) override
    {
        record_.selected = sel->checked();
        if (onChange_)
        {
            onChange_(record_);
        }
    }

    void onShowBtnClick() override { if (onShow_) onShow_(record_); }
    void onBrowseBtnClick() override { if (onBrowse_) onBrowse_(record_); }
    void onDeleteBtnClick() override { if (onDelete_) onDelete_(record_); }

private:
    void syncFromRecord()
    {
        thumb->setSource(record_.source);
        thumb->setAlt(record_.name);
        thumb->setFit("contain");
        thumb->setRenderMode(renderMode_);
        name_label->setText(record_.name);
        path_label->setText(imageDisplayPath(record_.source));
        sel->setChecked(record_.selected);
    }

    ImageRecord& record_;
    std::string renderMode_;
    RecordCallback onShow_;
    RecordCallback onBrowse_;
    RecordCallback onDelete_;
    RecordCallback onChange_;
};

class ImageListScroll : public ImageListScrollUI
{
public:
    using RecordCallback = ImageListItem::RecordCallback;

    void setItems(std::vector<ImageRecord>& records,
                  std::string_view renderMode,
                  RecordCallback onShow,
                  RecordCallback onBrowse,
                  RecordCallback onDelete,
                  RecordCallback onChange,
                  bool scrollToEnd = false,
                  bool preserveScroll = false)
    {
        const ui::ScrollViewPosition position = scrollPosition();
        clearChildren();
        setAutoScroll(scrollToEnd);
        for (ImageRecord& record : records)
        {
            auto item = std::make_unique<ImageListItem>(
                record,
                std::string{renderMode},
                onShow,
                onBrowse,
                onDelete,
                onChange);
            const ui::Size naturalSize = ui::generatedWindowContentSize(*item);
            const ui::Size measuredSize = ui::generatedWindowContentSizeForWidth(*item, naturalSize.width);
            auto reusable = std::make_unique<ui::ReusableElement>("image_list_item", std::move(item));
            reusable->setFrame(ui::Rect{0, 0, 0, std::max(1, measuredSize.height)});
            addChild(std::move(reusable));
        }
        if (scrollToEnd)
        {
            (void)scrollToBottom();
        }
        else if (preserveScroll)
        {
            restoreScrollPosition(position);
        }
        else
        {
            (void)scrollToTop();
        }
    }
};

class ImageListView : public ImageListViewUI
{
public:
    ImageListView(std::vector<ImageRecord>& records, std::string renderMode, ui::GeneratedWindowStack& modalStack)
        : records_(records),
          renderMode_(std::move(renderMode)),
          modalStack_(modalStack)
    {
        auto scroll = std::make_unique<ImageListScroll>();
        itemsScroll_ = scroll.get();
        items->setChild(std::move(scroll));
        refreshItems();
    }

    ~ImageListView() override = default;

    void setRenderMode(std::string_view renderMode)
    {
        renderMode_ = renderMode;
        refreshItems(false, true);
    }

protected:
    void onAddBtnClick() override
    {
        openBrowser(nullptr);
    }

private:
    void refreshItems(bool scrollToEnd = false, bool preserveScroll = false)
    {
        itemsScroll_->setItems(
            records_,
            renderMode_,
            [this](ImageRecord& selectedRecord) { showImage(selectedRecord); },
            [this](ImageRecord& selectedRecord) { openBrowser(&selectedRecord); },
            [this](ImageRecord& selectedRecord) { confirmDelete(selectedRecord); },
            [](ImageRecord&) {},
            scrollToEnd,
            preserveScroll);
    }

    void showImage(ImageRecord& record)
    {
        closeShowDialog();
        showDialog_ = std::make_unique<ImageShowDialog>(record.source, record.name, renderMode_);
        ui::GeneratedWindowFrameOptions frame;
        frame.className = "ImageShowDialog";
        frame.initialFocusName = "close_btn";
        frame.onButton = [this](std::string_view name)
        {
            if (name == "close_btn")
            {
                closeShowDialog();
            }
        };
        frame.onKey = [this](std::string_view key)
        {
            if (key == "Escape")
            {
                closeShowDialog();
                return true;
            }
            return false;
        };
        modalStack_.push(*showDialog_, std::move(frame));
    }

    void openBrowser(ImageRecord* record)
    {
        closeBrowser();
        pendingBrowseRecord_ = record;
        const std::filesystem::path startPath = record == nullptr
            ? imageSampleDir()
            : std::filesystem::path(record->source);
        browser_ = std::make_unique<ui::dialogs::FileBrowser>(
            projectRoot(),
            startPath,
            "open",
            [this](std::string path)
            {
                onBrowserClosed(std::move(path));
            },
            "",
            std::string{kImageExtensionFilter});
        modalStack_.push(*browser_, browser_->stackFrameOptions());
    }

    void onBrowserClosed(std::string path)
    {
        if (path.empty())
        {
            closeBrowser();
            return;
        }
        if (pendingBrowseRecord_ == nullptr)
        {
            records_.push_back({imageNameFromPath(path), path, false});
            closeBrowser();
            refreshItems(true);
            return;
        }
        pendingBrowseRecord_->name = imageNameFromPath(path);
        pendingBrowseRecord_->source = std::move(path);
        closeBrowser();
        refreshItems(false, true);
    }

    void confirmDelete(ImageRecord& record)
    {
        closeDeleteDialog();
        pendingDeleteRecord_ = &record;
        deleteDialog_ = std::make_unique<ui::dialogs::MessageBoxYesNo>(
            "Delete Image",
            "Delete " + record.name + "?");
        ui::GeneratedWindowFrameOptions frame;
        frame.className = "MessageBoxYesNo";
        frame.initialFocusName = "no_btn";
        frame.onButton = [this](std::string_view name)
        {
            if (name == "yes_btn")
            {
                deleteConfirmed(true);
            }
            else if (name == "no_btn")
            {
                deleteConfirmed(false);
            }
        };
        modalStack_.push(*deleteDialog_, std::move(frame));
    }

    void deleteConfirmed(bool confirmed)
    {
        ImageRecord* record = pendingDeleteRecord_;
        closeDeleteDialog();
        if (!confirmed || record == nullptr)
        {
            return;
        }
        records_.erase(
            std::remove_if(records_.begin(), records_.end(), [record](const ImageRecord& item)
            {
                return &item == record;
            }),
            records_.end());
        refreshItems(false, true);
    }

    void closeBrowser()
    {
        if (browser_ == nullptr)
        {
            return;
        }
        modalStack_.remove(*browser_);
        closingBrowser_ = std::move(browser_);
        pendingBrowseRecord_ = nullptr;
    }

    void closeDeleteDialog()
    {
        if (deleteDialog_ == nullptr)
        {
            return;
        }
        modalStack_.remove(*deleteDialog_);
        deleteDialog_.reset();
        pendingDeleteRecord_ = nullptr;
    }

    void closeShowDialog()
    {
        if (showDialog_ == nullptr)
        {
            return;
        }
        modalStack_.remove(*showDialog_);
        showDialog_.reset();
    }

    std::vector<ImageRecord>& records_;
    std::string renderMode_;
    ui::GeneratedWindowStack& modalStack_;
    ImageListScroll* itemsScroll_ = nullptr;
    std::unique_ptr<ui::dialogs::FileBrowser> browser_;
    std::unique_ptr<ui::dialogs::FileBrowser> closingBrowser_;
    std::unique_ptr<ui::dialogs::MessageBoxYesNo> deleteDialog_;
    std::unique_ptr<ImageShowDialog> showDialog_;
    ImageRecord* pendingBrowseRecord_ = nullptr;
    ImageRecord* pendingDeleteRecord_ = nullptr;
};

class ImageBrowser : public ImageBrowserUI
{
public:
    ImageBrowser()
        : imageRecords_(initialImageRecords())
    {
        const std::vector<ImageEntry> images = imageLibrary();
        gallery_mosaic->setChild(std::make_unique<GalleryMosaic>([this]() { showGallery(); }));
        auto listButton = std::make_unique<ImageListButton>();
        imageListButton_ = listButton.get();
        image_list_btn->setChild(std::move(listButton));
        imageListButton_->setImages(images);
        imageListButton_->setOnClick([this]() { showImageList(); });

        const std::array<ui::ReusableElement*, 3> slots{camera_thumb, astro_thumb, coffee_thumb};
        const std::array<SidebarImageEntry, 3> sidebar = sidebarImages();
        for (std::size_t i = 0; i < slots.size(); ++i)
        {
            auto button = std::make_unique<ImageButton>();
            ImageButton* ptr = button.get();
            slots[i]->setChild(std::move(button));
            ptr->setSource(sidebar[i].source);
            ptr->setAlt(sidebar[i].caption);
            ptr->setOnClick([this, i]() { showImage(i); });
            thumbs_[i] = ptr;
        }
        applyRenderMode(kRenderModeSixel);
        showImage(0);
    }

    bool shouldClose() const override
    {
        return quitRequested_;
    }

    [[nodiscard]] ui::GeneratedWindowRuntimeOptions runtimeOptions() override
    {
        ui::GeneratedWindowRuntimeOptions options = ImageBrowserUI::runtimeOptions();
        options.initialFocusName = "camera_thumb";
        options.windowStack = &modalStack_;
        return options;
    }

    void showImage(std::size_t index)
    {
        const std::array<SidebarImageEntry, 3> sidebar = sidebarImages();
        selectThumb(index);
        setMosaicSelected(false);
        if (imageListButton_ != nullptr)
        {
            imageListButton_->setSelected(false);
        }
        main->setView(std::make_unique<ImageView>(
            sidebar[index].source,
            sidebar[index].caption,
            std::string{renderMode_}));
    }

    void showGallery()
    {
        deselectAllThumbs();
        setMosaicSelected(true);
        if (imageListButton_ != nullptr)
        {
            imageListButton_->setSelected(false);
        }
        main->setView(std::make_unique<GalleryView>(renderMode_));
    }

    void showImageList()
    {
        deselectAllThumbs();
        setMosaicSelected(false);
        if (imageListButton_ != nullptr)
        {
            imageListButton_->setSelected(true);
        }
        main->setView(std::make_unique<ImageListView>(imageRecords_, std::string{renderMode_}, modalStack_));
    }

    void applyRenderMode(std::string_view mode)
    {
        renderMode_ = mode;
        for (ImageButton* thumb : thumbs_)
        {
            thumb->setRenderMode(renderMode_);
        }
        if (auto* mosaic = dynamic_cast<GalleryMosaic*>(gallery_mosaic->child()))
        {
            mosaic->setRenderMode(renderMode_);
        }
        if (imageListButton_ != nullptr)
        {
            imageListButton_->setRenderMode(renderMode_);
        }
        if (auto* view = dynamic_cast<ImageView*>(main->currentView()))
        {
            view->photo->setRenderMode(std::string{renderMode_});
        }
        else if (dynamic_cast<GalleryView*>(main->currentView()) != nullptr)
        {
            main->setView(std::make_unique<GalleryView>(renderMode_));
        }
        else if (auto* list = dynamic_cast<ImageListView*>(main->currentView()))
        {
            list->setRenderMode(renderMode_);
        }
    }

    [[nodiscard]] std::size_t imageRecordCount() const
    {
        return imageRecords_.size();
    }

protected:
    void onCloseBtnClick() override { quitRequested_ = true; }

    void onModeSelectChange(std::string_view value) override
    {
        applyRenderMode((value == "Normal") ? kRenderModeSixel : kRenderModeFallback);
    }

private:
    void setMosaicSelected(bool selected)
    {
        if (auto* mosaic = dynamic_cast<GalleryMosaic*>(gallery_mosaic->child()))
        {
            mosaic->setSelected(selected);
        }
    }

    void selectThumb(std::size_t selectedIndex)
    {
        for (std::size_t i = 0; i < thumbs_.size(); ++i)
        {
            thumbs_[i]->setSelected(i == selectedIndex);
        }
    }

    void deselectAllThumbs()
    {
        for (ImageButton* thumb : thumbs_)
        {
            thumb->setSelected(false);
        }
    }

    std::array<ImageButton*, 3> thumbs_{};
    ImageListButton* imageListButton_ = nullptr;
    std::vector<ImageRecord> imageRecords_;
    ui::GeneratedWindowStack modalStack_;
    std::string_view renderMode_ = kRenderModeSixel;
    bool quitRequested_ = false;
};

void runLogicTest()
{
    assert(imageNameFromPath("/tmp/chelsea.png") == "Chelsea");
    assert(imageDisplayPath((projectRoot() / "shared/assets/image_samples/chelsea.png").string()) ==
           "shared/assets/image_samples/chelsea.png");
    const std::vector<ImageEntry> images = imageLibrary();
    assert(images[5].source.ends_with(".bmp"));
    assert(images[6].source.ends_with(".gif"));
    assert(images[7].source.ends_with(".png"));
    assert(images[8].source.ends_with(".tga"));
    ui::Image mixedSample{"sample", images[5].source, "Sample 6", "contain", "fallback"};
    const ui::RenderedContent mixedSampleRender = mixedSample.render(ui::Size{8, 4});
    assert(std::any_of(mixedSampleRender.begin(), mixedSampleRender.end(), [](const ui::RenderedRow& row)
    {
        return std::any_of(row.begin(), row.end(), [](const ui::TerminalCell& cell)
        {
            return cell.text == "\xE2\x96\x80";
        });
    }));
    ui::CheckBox emptyCheckbox{"sel", "", false};
    const ui::RenderedContent emptyCheckboxRender = emptyCheckbox.render(ui::Size{3, 1}, ui::ElementRenderState{.focused = true});
    assert(emptyCheckboxRender.size() == 1);
    assert(emptyCheckboxRender[0].size() == 3);
    ImageBrowser shell;
    assert(shell.main->currentView() != nullptr);
    assert(dynamic_cast<ImageView*>(shell.main->currentView()) != nullptr);
    for (std::size_t i = 0; i < sidebarImages().size(); ++i)
    {
        shell.showImage(i);
        assert(dynamic_cast<ImageView*>(shell.main->currentView()) != nullptr);
    }
    shell.applyRenderMode(kRenderModeFallback);
    shell.applyRenderMode(kRenderModeSixel);
    shell.showGallery();
    assert(dynamic_cast<GalleryView*>(shell.main->currentView()) != nullptr);
    shell.showImageList();
    assert(dynamic_cast<ImageListView*>(shell.main->currentView()) != nullptr);
    assert(shell.imageRecordCount() == kInitialListCount);
    auto* mosaic = dynamic_cast<GalleryMosaic*>(shell.gallery_mosaic->child());
    assert(mosaic != nullptr);
    assert(!mosaic->gal_1->focusable());
    assert(!mosaic->gal_2->focusable());
    assert(!mosaic->gal_3->focusable());
    assert(!mosaic->gal_4->focusable());
    assert(!mosaic->gal_5->focusable());
    assert(shell.runtimeOptions().initialFocusName == "camera_thumb");
    assert(shell.runtimeOptions().windowStack != nullptr);
    assert(mosaic->activateGeneratedControl());
    assert(dynamic_cast<GalleryView*>(shell.main->currentView()) != nullptr);
    auto* camera = dynamic_cast<ImageButton*>(shell.camera_thumb->child());
    assert(camera != nullptr);
    assert(!camera->photo->focusable());
    assert(camera->activateGeneratedControl());
    assert(dynamic_cast<ImageView*>(shell.main->currentView()) != nullptr);
}

}  // namespace

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (std::string_view{argv[i]} == "--logic-test" || std::string_view{argv[i]} == "--smoke")
        {
            runLogicTest();
            return 0;
        }
    }

    ImageBrowser app;
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}
