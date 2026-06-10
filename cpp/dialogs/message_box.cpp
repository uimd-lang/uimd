#include "message_box.hpp"

#include <utility>

namespace ui::dialogs {

void MessageBoxBehavior::configure(std::string header, std::string message, MessageBoxCallback onClose,
                                   std::unordered_map<std::string, std::string> resultByButton,
                                   std::string defaultResult) {
    header_ = std::move(header);
    message_ = std::move(message);
    onClose_ = std::move(onClose);
    resultByButton_ = std::move(resultByButton);
    defaultResult_ = std::move(defaultResult);
    result_.clear();
    closed_ = false;
}

void MessageBoxBehavior::applyText(Label* header, Label* message) {
    if (header != nullptr) {
        header->setText(header_);
    }
    if (message != nullptr) {
        message->setText(message_);
    }
}

bool MessageBoxBehavior::handleButton(std::string_view name) {
    const auto it = resultByButton_.find(std::string(name));
    if (it == resultByButton_.end()) {
        return false;
    }
    close(it->second);
    return true;
}

bool MessageBoxBehavior::handleKey(std::string_view key) {
    if (key == "Escape" && !defaultResult_.empty()) {
        close(defaultResult_);
        return true;
    }
    return false;
}

GeneratedWindowRuntimeOptions MessageBoxBehavior::runtimeOptions() {
    GeneratedWindowRuntimeOptions options;
    options.initialFocusName = buttonNameForResult(defaultResult_);
    options.onButton = [this](std::string_view name) {
        (void)handleButton(name);
    };
    options.onKey = [this](std::string_view key) {
        return handleKey(key);
    };
    options.shouldClose = [this] {
        return closed_;
    };
    return options;
}

std::string MessageBoxBehavior::buttonNameForResult(std::string_view result) const {
    for (const auto& [buttonName, buttonResult] : resultByButton_) {
        if (buttonResult == result) {
            return buttonName;
        }
    }
    return {};
}

void MessageBoxBehavior::close(std::string result) {
    result_ = std::move(result);
    closed_ = true;
    if (onClose_) {
        onClose_(result_);
    }
}

MessageBoxOk::MessageBoxOk(std::string header, std::string message, MessageBoxCallback onClose) {
    configure(std::move(header), std::move(message), std::move(onClose), {{"ok_btn", "ok"}}, "ok");
    applyText(dialog_header, this->message);
}

int MessageBoxOk::run() {
    return runGeneratedWindow(*this, MessageBoxBehavior::runtimeOptions());
}

MessageBoxOkCancel::MessageBoxOkCancel(std::string header, std::string message, MessageBoxCallback onClose) {
    configure(std::move(header), std::move(message), std::move(onClose),
              {{"ok_btn", "ok"}, {"cancel_btn", "cancel"}}, "cancel");
    applyText(dialog_header, this->message);
}

int MessageBoxOkCancel::run() {
    return runGeneratedWindow(*this, MessageBoxBehavior::runtimeOptions());
}

MessageBoxYesNo::MessageBoxYesNo(std::string header, std::string message, MessageBoxCallback onClose) {
    configure(std::move(header), std::move(message), std::move(onClose),
              {{"yes_btn", "yes"}, {"no_btn", "no"}}, "no");
    applyText(dialog_header, this->message);
}

int MessageBoxYesNo::run() {
    return runGeneratedWindow(*this, MessageBoxBehavior::runtimeOptions());
}

MessageBoxYesNoCancel::MessageBoxYesNoCancel(std::string header, std::string message, MessageBoxCallback onClose) {
    configure(std::move(header), std::move(message), std::move(onClose),
              {{"yes_btn", "yes"}, {"no_btn", "no"}, {"cancel_btn", "cancel"}}, "cancel");
    applyText(dialog_header, this->message);
}

int MessageBoxYesNoCancel::run() {
    return runGeneratedWindow(*this, MessageBoxBehavior::runtimeOptions());
}

}  // namespace ui::dialogs
