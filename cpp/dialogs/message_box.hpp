#pragma once

#include "message_box_ok_cancel_ui.hpp"
#include "message_box_ok_ui.hpp"
#include "message_box_yes_no_cancel_ui.hpp"
#include "message_box_yes_no_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace ui::dialogs {

using MessageBoxCallback = std::function<void(std::string)>;

class MessageBoxBehavior {
public:
    void configure(std::string header, std::string message, MessageBoxCallback onClose,
                   std::unordered_map<std::string, std::string> resultByButton,
                   std::string defaultResult = {});

    [[nodiscard]] const std::string& result() const {
        return result_;
    }

    [[nodiscard]] bool closed() const {
        return closed_;
    }

protected:
    void applyText(Label* header, Label* message);
    bool handleButton(std::string_view name);
    bool handleKey(std::string_view key);
    [[nodiscard]] GeneratedWindowRuntimeOptions runtimeOptions();

private:
    [[nodiscard]] std::string buttonNameForResult(std::string_view result) const;
    void close(std::string result);

    std::string header_;
    std::string message_;
    MessageBoxCallback onClose_;
    std::unordered_map<std::string, std::string> resultByButton_;
    std::string defaultResult_;
    std::string result_;
    bool closed_ = false;
};

class MessageBoxOk : public MessageBoxOkUI, private MessageBoxBehavior {
public:
    MessageBoxOk(std::string header = "Message", std::string message = {}, MessageBoxCallback onClose = {});
    int run();
    using MessageBoxBehavior::closed;
    using MessageBoxBehavior::result;
};

class MessageBoxOkCancel : public MessageBoxOkCancelUI, private MessageBoxBehavior {
public:
    MessageBoxOkCancel(std::string header = "Message", std::string message = {}, MessageBoxCallback onClose = {});
    int run();
    using MessageBoxBehavior::closed;
    using MessageBoxBehavior::result;
};

class MessageBoxYesNo : public MessageBoxYesNoUI, private MessageBoxBehavior {
public:
    MessageBoxYesNo(std::string header = "Message", std::string message = {}, MessageBoxCallback onClose = {});
    int run();
    using MessageBoxBehavior::closed;
    using MessageBoxBehavior::result;
};

class MessageBoxYesNoCancel : public MessageBoxYesNoCancelUI, private MessageBoxBehavior {
public:
    MessageBoxYesNoCancel(std::string header = "Message", std::string message = {}, MessageBoxCallback onClose = {});
    int run();
    using MessageBoxBehavior::closed;
    using MessageBoxBehavior::result;
};

}  // namespace ui::dialogs
