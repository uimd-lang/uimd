#include "edit_page_ui.hpp"
#include "edit_field_row_ui.hpp"
#include "edit_fields_scroll_ui.hpp"
#include "item_row_ui.hpp"
#include "list_page_ui.hpp"
#include "stale_scrollview_focus_ui.hpp"

#include "ui/elements/ReusableElement.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace
{

constexpr int kMinimumRenderHeight = 1;
constexpr std::size_t kFieldCount = 40;
constexpr std::size_t kPrimaryFieldIndex = 0;
constexpr std::size_t kBodyFieldIndex = 1;
constexpr std::size_t kDisplayIndexOffset = 1;
constexpr std::size_t kFieldNumberPaddingThreshold = 10;

struct ReproItem
{
    std::string_view name;
    std::string_view summary;
    std::string_view body;
};

constexpr std::array<ReproItem, 5> kItems{{
    {
        "Alpha Apartments",
        "First row, visible without scrolling",
        "Alpha item was opened from the top of the ScrollView.\n"
        "The edit page should not keep any stale list focus background.",
    },
    {
        "Bravo Offices",
        "Second row, used by the automated repro",
        "Bravo item was opened by pressing Enter on a row button.\n"
        "The old ScrollView page must be gone before this page renders.",
    },
    {
        "Charlie Lofts",
        "Third row, keeps the source list taller",
        "Charlie item keeps the source page closer to the reported list shape.\n"
        "The opened edit page still owns the new nested ScrollView.",
    },
    {
        "Delta Studios",
        "Fourth row, manual repro filler",
        "Delta item is extra manual test data for scroll and focus traversal.\n"
        "Activating Open should replace the whole page cleanly.",
    },
    {
        "Echo Retail",
        "Fifth row, enough rows to browse",
        "Echo item makes the source ScrollView visibly list-like.\n"
        "The edit form below contains many alternating controls.",
    },
}};

using OpenCallback = std::function<void(const ReproItem&)>;
using BackCallback = std::function<void()>;
using FieldActionCallback = std::function<void(std::size_t, std::string_view)>;

[[nodiscard]] std::string fieldNumber(std::size_t index)
{
    const std::size_t displayIndex = index + kDisplayIndexOffset;
    std::string value = std::to_string(displayIndex);
    if (displayIndex < kFieldNumberPaddingThreshold)
    {
        value = "0" + value;
    }
    return value;
}

[[nodiscard]] std::string firstLine(std::string_view value)
{
    const std::size_t lineBreak = value.find('\n');
    return std::string{value.substr(0, lineBreak)};
}

[[nodiscard]] std::string fieldValue(const ReproItem& item, std::size_t index)
{
    if (index == kPrimaryFieldIndex)
    {
        return std::string{item.name} + " primary field";
    }
    if (index == kBodyFieldIndex)
    {
        return firstLine(item.body);
    }
    return std::string{item.name} + " detail field " + fieldNumber(index);
}

template <typename RowWindow>
void addReusableScrollChild(ui::GeneratedScrollViewBase& scrollView, std::unique_ptr<RowWindow> row)
{
    const ui::Size naturalSize = ui::generatedWindowContentSize(*row);
    auto reusable = std::make_unique<ui::ReusableElement>("row", std::move(row));
    reusable->setFrame(ui::Rect{0, 0, 0, std::max(kMinimumRenderHeight, naturalSize.height)});
    scrollView.addChild(std::move(reusable));
}

class ItemRowWindow : public ItemRowUI
{
public:
    ItemRowWindow(ReproItem item, OpenCallback onOpen)
        : item_(item),
          onOpen_(std::move(onOpen))
    {
        name->setText(std::string{item_.name});
        summary->setText(std::string{item_.summary});
    }

    void onOpenBtnClick() override
    {
        if (onOpen_)
        {
            onOpen_(item_);
        }
    }

private:
    ReproItem item_;
    OpenCallback onOpen_;
};

class ListPageWindow : public ListPageUI
{
public:
    explicit ListPageWindow(OpenCallback onOpen)
        : onOpen_(std::move(onOpen))
    {
        populate();
    }

private:
    void populate()
    {
        clearChildren();
        for (const ReproItem& item : kItems)
        {
            addReusableScrollChild(*this, std::make_unique<ItemRowWindow>(item, onOpen_));
        }
        const ui::Rect currentFrame = frame();
        if (currentFrame.width > 0 && currentFrame.height > 0)
        {
            (void)scrollToTop(ui::Size{currentFrame.width, currentFrame.height});
        }
    }

    OpenCallback onOpen_;
};

class EditFieldRowWindow : public EditFieldRowUI
{
public:
    EditFieldRowWindow(std::size_t index, std::string value, FieldActionCallback onAction)
        : index_(index),
          onAction_(std::move(onAction))
    {
        field_input->setValue(std::move(value));
        field_btn->setTitle("Action " + fieldNumber(index_));
    }

    void onFieldBtnClick() override
    {
        if (onAction_)
        {
            onAction_(index_, field_input->value());
        }
    }

private:
    std::size_t index_;
    FieldActionCallback onAction_;
};

class EditFieldsScrollWindow : public EditFieldsScrollUI
{
public:
    EditFieldsScrollWindow(ReproItem item, FieldActionCallback onAction)
        : item_(item),
          onAction_(std::move(onAction))
    {
        populate();
    }

private:
    void populate()
    {
        clearChildren();
        for (std::size_t index = 0; index < kFieldCount; ++index)
        {
            addReusableScrollChild(
                *this,
                std::make_unique<EditFieldRowWindow>(index, fieldValue(item_, index), onAction_)
            );
        }
        const ui::Rect currentFrame = frame();
        if (currentFrame.width > 0 && currentFrame.height > 0)
        {
            (void)scrollToTop(ui::Size{currentFrame.width, currentFrame.height});
        }
    }

    ReproItem item_;
    FieldActionCallback onAction_;
};

class EditPageWindow : public EditPageUI
{
public:
    EditPageWindow(ReproItem item, BackCallback onBack)
        : onBack_(std::move(onBack))
    {
        edit_title->setText("Edit " + std::string{item.name});
        name_input->setValue(std::string{item.name});
        fields->setView(std::make_unique<EditFieldsScrollWindow>(
            item,
            [this](std::size_t index, std::string_view value)
            {
                status->setText("Action " + fieldNumber(index) + ": " + std::string{value});
            }
        ));
        auto* fieldScroll = dynamic_cast<EditFieldsScrollWindow*>(fields->child());
        if (fieldScroll != nullptr)
        {
            const ui::Size fieldsSize = ui::generatedWindowContentSize(*fieldScroll);
            if (fieldsSize.width > 0 && fieldsSize.height > 0)
            {
                ui::ScrollViewPosition topPosition;
                topPosition.scrollOffset = 0;
                topPosition.viewOffset = fieldScroll->scrollView().contentHeight(fieldsSize.width);
                topPosition.autoScroll = false;
                fieldScroll->scrollView().restoreScrollPosition(topPosition);
            }
        }
    }

    void onBackBtnClick() override
    {
        if (onBack_)
        {
            onBack_();
        }
    }

    void onSaveBtnClick() override
    {
        status->setText("Saved in memory");
    }

private:
    BackCallback onBack_;
};

class StaleScrollviewFocusApp : public StaleScrollviewFocusUI
{
public:
    StaleScrollviewFocusApp()
    {
        showListPage();
    }

    void onTabListClick() override
    {
        showListPage();
    }

    void onTabOtherClick() override
    {
        hint->setText("Other tab intentionally leaves the list unchanged.");
    }

    void onQuitBtnClick() override
    {
        quitRequested_ = true;
    }

    bool shouldClose() const override
    {
        return quitRequested_;
    }

private:
    void showListPage()
    {
        page->setView(std::make_unique<ListPageWindow>(
            [this](const ReproItem& item)
            {
                showEditPage(item);
            }
        ));
        hint->setText("Focus list, press Enter, choose Open.");
    }

    void showEditPage(const ReproItem& item)
    {
        page->setView(std::make_unique<EditPageWindow>(
            item,
            [this]()
            {
                showListPage();
            }
        ));
        hint->setText("Edit page opened from ScrollView row.");
    }

    bool quitRequested_ = false;
};

}  // namespace

int main(int argc, char** argv)
{
    StaleScrollviewFocusApp app;
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}
