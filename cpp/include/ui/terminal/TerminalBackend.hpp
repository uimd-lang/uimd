#pragma once

#include <cstddef>
#include <optional>
#include <string_view>
#include <vector>

#include "ui/core/Event.hpp"
#include "ui/terminal/Input.hpp"

namespace ui {

class TerminalModeGuard {
public:
    explicit TerminalModeGuard(int fd);
    TerminalModeGuard(int modeFd, int outputFd);
    ~TerminalModeGuard();

    TerminalModeGuard(const TerminalModeGuard&) = delete;
    TerminalModeGuard& operator=(const TerminalModeGuard&) = delete;
    TerminalModeGuard(TerminalModeGuard&&) = delete;
    TerminalModeGuard& operator=(TerminalModeGuard&&) = delete;

    [[nodiscard]] bool enabled() const {
        return enabled_;
    }

    void restore();

private:
    int fd_;
    int outputFd_;
    bool enabled_ = false;
    bool restored_ = false;
    alignas(std::max_align_t) unsigned char saved_[128] = {};
};

class TerminalBackend {
public:
    virtual ~TerminalBackend() = default;

    [[nodiscard]] virtual std::optional<Size> size() const = 0;
    [[nodiscard]] virtual std::optional<Size> cellPixelSize();
    [[nodiscard]] virtual std::vector<Event> readEvents() = 0;
    virtual void write(std::string_view bytes) = 0;
};

class PosixTerminalBackend : public TerminalBackend {
public:
    PosixTerminalBackend(int inputFd, int outputFd);

    [[nodiscard]] std::optional<Size> size() const override;
    [[nodiscard]] std::optional<Size> cellPixelSize() override;
    [[nodiscard]] std::vector<Event> readEvents() override;
    void write(std::string_view bytes) override;

private:
    static constexpr int kReadBufferSize = 256;
    static constexpr int kReadDrainMaxBytes = 16384;

    int inputFd_;
    int outputFd_;
    InputParser parser_;
};

}  // namespace ui
