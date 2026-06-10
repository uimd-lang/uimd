#include "ui/terminal/TerminalBackend.hpp"

#include <algorithm>
#include <array>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <system_error>

#ifndef _WIN32
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace ui {

namespace {

#ifndef _WIN32

constexpr std::size_t kSavedTermiosStorageSize = 128;
constexpr std::string_view kTerminalCellPixelQuery = "\x1b[16t";
constexpr std::string_view kTerminalTextAreaPixelQuery = "\x1b[14t";
constexpr std::string_view kTerminalCellPixelResponsePrefix = "\x1b[6;";
constexpr std::string_view kTerminalTextAreaPixelResponsePrefix = "\x1b[4;";
constexpr char kTerminalCellPixelResponseSeparator = ';';
constexpr char kTerminalCellPixelResponseTerminator = 't';
constexpr std::size_t kTerminalCellPixelResponseMaxBytes = 64;
constexpr int kTerminalCellPixelQueryTimeoutMicroseconds = 50000;

static_assert(sizeof(termios) <= kSavedTermiosStorageSize, "TerminalModeGuard saved_ is too small for termios");

[[nodiscard]] termios& savedTermios(unsigned char* storage) {
    return *reinterpret_cast<termios*>(storage);
}

void forceNormalOutputFlags(int fd) {
    termios current{};
    if (tcgetattr(fd, &current) != 0) {
        return;
    }
    current.c_oflag |= static_cast<tcflag_t>(OPOST | ONLCR);
    (void)tcsetattr(fd, TCSANOW, &current);
}

[[nodiscard]] std::optional<Size> terminalCellPixelSizeFromIoctl(int fd) {
    winsize size{};
    if (ioctl(fd, TIOCGWINSZ, &size) != 0 ||
        size.ws_col == 0 || size.ws_row == 0 ||
        size.ws_xpixel == 0 || size.ws_ypixel == 0) {
        return std::nullopt;
    }
    const int width = std::max(1, (static_cast<int>(size.ws_xpixel) + static_cast<int>(size.ws_col) / 2) /
                                  static_cast<int>(size.ws_col));
    const int height = std::max(1, (static_cast<int>(size.ws_ypixel) + static_cast<int>(size.ws_row) / 2) /
                                   static_cast<int>(size.ws_row));
    return Size{width, height};
}

[[nodiscard]] std::optional<Size> parseTerminalPixelResponse(std::string_view response, std::string_view responsePrefix) {
    const std::size_t prefix = response.find(responsePrefix);
    if (prefix == std::string_view::npos) {
        return std::nullopt;
    }
    const std::size_t heightStart = prefix + responsePrefix.size();
    const std::size_t separator = response.find(kTerminalCellPixelResponseSeparator, heightStart);
    if (separator == std::string_view::npos) {
        return std::nullopt;
    }
    const std::size_t terminator = response.find(kTerminalCellPixelResponseTerminator, separator + 1);
    if (terminator == std::string_view::npos) {
        return std::nullopt;
    }
    try {
        const int height = std::stoi(std::string(response.substr(heightStart, separator - heightStart)));
        const int width = std::stoi(std::string(response.substr(separator + 1, terminator - separator - 1)));
        if (width <= 0 || height <= 0) {
            return std::nullopt;
        }
        return Size{width, height};
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

[[nodiscard]] std::optional<Size> queryTerminalPixelReport(
    PosixTerminalBackend& backend,
    int inputFd,
    std::string_view query,
    std::string_view responsePrefix
) {
    try {
        backend.write(query);
    } catch (const std::exception&) {
        return std::nullopt;
    }

    std::string response;
    response.reserve(kTerminalCellPixelResponseMaxBytes);
    while (response.size() < kTerminalCellPixelResponseMaxBytes) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(inputFd, &readSet);
        timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = kTerminalCellPixelQueryTimeoutMicroseconds;
        const int ready = select(inputFd + 1, &readSet, nullptr, nullptr, &timeout);
        if (ready <= 0) {
            break;
        }
        std::array<char, kTerminalCellPixelResponseMaxBytes> bytes{};
        const std::size_t remaining = kTerminalCellPixelResponseMaxBytes - response.size();
        const ssize_t count = ::read(inputFd, bytes.data(), std::min(bytes.size(), remaining));
        if (count <= 0) {
            break;
        }
        response.append(bytes.data(), static_cast<std::size_t>(count));
        if (std::optional<Size> parsed = parseTerminalPixelResponse(response, responsePrefix)) {
            return parsed;
        }
    }
    return std::nullopt;
}

[[nodiscard]] bool inputReady(int fd)
{
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(fd, &readSet);
    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(fd + 1, &readSet, nullptr, nullptr, &timeout) > 0;
}

#endif

}  // namespace

std::optional<Size> TerminalBackend::cellPixelSize() {
    return std::nullopt;
}

TerminalModeGuard::TerminalModeGuard(int fd) : TerminalModeGuard(fd, fd) {}

TerminalModeGuard::TerminalModeGuard(int modeFd, int outputFd) : fd_(modeFd), outputFd_(outputFd) {
#ifndef _WIN32
    forceNormalOutputFlags(fd_);
    if (outputFd_ != fd_) {
        forceNormalOutputFlags(outputFd_);
    }
    termios current{};
    if (tcgetattr(fd_, &current) != 0) {
        return;
    }
    savedTermios(saved_) = current;

    termios raw = current;
    raw.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON | IEXTEN | ISIG));
    raw.c_iflag &= static_cast<tcflag_t>(~(BRKINT | ICRNL | INPCK | ISTRIP | IXON));
    raw.c_cflag |= CS8;
    raw.c_oflag &= static_cast<tcflag_t>(~OPOST);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(fd_, TCSAFLUSH, &raw) == 0) {
        enabled_ = true;
    }
#else
    (void)fd_;
#endif
}

TerminalModeGuard::~TerminalModeGuard() {
    restore();
}

void TerminalModeGuard::restore() {
#ifndef _WIN32
    if (enabled_ && !restored_) {
        termios restored = savedTermios(saved_);
        restored.c_oflag |= static_cast<tcflag_t>(OPOST | ONLCR);
        (void)tcsetattr(fd_, TCSAFLUSH, &restored);
        if (outputFd_ != fd_) {
            forceNormalOutputFlags(outputFd_);
        }
        restored_ = true;
    }
#endif
}

PosixTerminalBackend::PosixTerminalBackend(int inputFd, int outputFd)
    : inputFd_(inputFd), outputFd_(outputFd) {}

std::optional<Size> PosixTerminalBackend::size() const {
#ifndef _WIN32
    winsize size{};
    if (ioctl(outputFd_, TIOCGWINSZ, &size) != 0 || size.ws_col == 0 || size.ws_row == 0) {
        return std::nullopt;
    }
    return Size{static_cast<int>(size.ws_col), static_cast<int>(size.ws_row)};
#else
    return std::nullopt;
#endif
}

std::optional<Size> PosixTerminalBackend::cellPixelSize() {
#ifndef _WIN32
    if (std::optional<Size> ioctlSize = terminalCellPixelSizeFromIoctl(outputFd_)) {
        return ioctlSize;
    }
    if (std::optional<Size> direct = queryTerminalPixelReport(
        *this,
        inputFd_,
        kTerminalCellPixelQuery,
        kTerminalCellPixelResponsePrefix
    )) {
        return direct;
    }
    const std::optional<Size> terminalSize = size();
    if (!terminalSize.has_value() || terminalSize->width <= 0 || terminalSize->height <= 0) {
        return std::nullopt;
    }
    if (std::optional<Size> textArea = queryTerminalPixelReport(
        *this,
        inputFd_,
        kTerminalTextAreaPixelQuery,
        kTerminalTextAreaPixelResponsePrefix
    )) {
        const int width = std::max(1, (textArea->width + terminalSize->width / 2) / terminalSize->width);
        const int height = std::max(1, (textArea->height + terminalSize->height / 2) / terminalSize->height);
        return Size{width, height};
    }
#endif
    return std::nullopt;
}

std::vector<Event> PosixTerminalBackend::readEvents() {
#ifndef _WIN32
    std::array<char, kReadBufferSize> bytes{};
    const ssize_t count = ::read(inputFd_, bytes.data(), bytes.size());
    if (count < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            return {};
        }
        throw std::system_error(errno, std::generic_category(), "terminal read failed");
    }
    if (count == 0) {
        return {};
    }
    std::string input;
    input.append(bytes.data(), static_cast<std::size_t>(count));
    while (input.size() < kReadDrainMaxBytes && inputReady(inputFd_)) {
        const std::size_t remaining = kReadDrainMaxBytes - input.size();
        const ssize_t next = ::read(inputFd_, bytes.data(), std::min(bytes.size(), remaining));
        if (next <= 0) {
            break;
        }
        input.append(bytes.data(), static_cast<std::size_t>(next));
    }
    return parser_.feed(input);
#else
    return {};
#endif
}

void PosixTerminalBackend::write(std::string_view bytes) {
#ifndef _WIN32
    const char* cursor = bytes.data();
    std::size_t remaining = bytes.size();
    while (remaining > 0) {
        const ssize_t written = ::write(outputFd_, cursor, remaining);
        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw std::system_error(errno, std::generic_category(), "terminal write failed");
        }
        cursor += written;
        remaining -= static_cast<std::size_t>(written);
    }
#else
    (void)bytes;
#endif
}

}  // namespace ui
