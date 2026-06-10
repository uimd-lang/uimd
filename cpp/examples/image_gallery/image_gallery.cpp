#include "image_gallery_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <cassert>
#include <string_view>

namespace
{

constexpr std::size_t kExpectedLayoutEntries = 18;

class ImageGallery : public ImageGalleryUI
{
protected:
    void onCloseBtnClick() override
    {
        finished_ = true;
    }

    bool shouldClose() const override
    {
        return finished_;
    }

private:
    bool finished_ = false;
};

void smoke()
{
    ImageGalleryUI window;
    assert(window.title() == "Image Gallery");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(window.camera_fb != nullptr);
    assert(window.astro_fb != nullptr);
    assert(window.coffee_fb != nullptr);
    assert(window.coins_fb != nullptr);
    assert(window.camera_ctn != nullptr);
    assert(window.astro_ctn != nullptr);
    assert(window.coffee_ctn != nullptr);
    assert(window.coins_ctn != nullptr);
    assert(window.camera_str != nullptr);
    assert(window.astro_str != nullptr);
    assert(window.coffee_str != nullptr);
    assert(window.coins_str != nullptr);
    const ui::RenderedContent coins = window.coins_fb->render(ui::Size{8, 10});
    assert(coins.size() == 10);
    assert(coins.front().size() == 8);
    assert(!coins.front().front().text.empty());
}

}  // namespace

int main(int argc, char** argv)
{
    ImageGallery app;
    if (argc > 1 && std::string_view(argv[1]) == "--smoke")
    {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--logic-test")
    {
        smoke();
        return 0;
    }

    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}
