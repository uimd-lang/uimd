#include "ui/terminal/Clipboard.hpp"

#include <array>
#include <cerrno>
#include <string>

#ifndef _WIN32
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace ui {

namespace {

struct ClipboardCommand {
    std::array<const char*, 4> argv;
};

[[nodiscard]] std::string& clipboardStorage() {
    static std::string text;
    return text;
}

#ifndef _WIN32

constexpr std::array<ClipboardCommand, 4> kClipboardCommands{{
    ClipboardCommand{{"pbcopy", nullptr, nullptr, nullptr}},
    ClipboardCommand{{"wl-copy", nullptr, nullptr, nullptr}},
    ClipboardCommand{{"xclip", "-selection", "clipboard", nullptr}},
    ClipboardCommand{{"xsel", "--clipboard", "--input", nullptr}},
}};

void redirectToDevNull(int fd) {
    const int devNull = open("/dev/null", O_WRONLY);
    if (devNull < 0) {
        return;
    }
    (void)dup2(devNull, fd);
    close(devNull);
}

[[nodiscard]] bool writeAll(int fd, std::string_view text) {
    const char* cursor = text.data();
    std::size_t remaining = text.size();
    while (remaining > 0) {
        const ssize_t written = write(fd, cursor, remaining);
        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            return errno == EPIPE;
        }
        cursor += written;
        remaining -= static_cast<std::size_t>(written);
    }
    return true;
}

[[nodiscard]] bool runClipboardCommand(const ClipboardCommand& command, std::string_view text) {
    int inputPipe[2] = {-1, -1};
    if (pipe(inputPipe) != 0) {
        return false;
    }

    const pid_t child = fork();
    if (child < 0) {
        close(inputPipe[0]);
        close(inputPipe[1]);
        return false;
    }

    if (child == 0) {
        close(inputPipe[1]);
        (void)dup2(inputPipe[0], STDIN_FILENO);
        close(inputPipe[0]);
        redirectToDevNull(STDOUT_FILENO);
        redirectToDevNull(STDERR_FILENO);
        execvp(command.argv[0], const_cast<char* const*>(command.argv.data()));
        _exit(127);
    }

    close(inputPipe[0]);
    const bool wroteInput = writeAll(inputPipe[1], text);
    close(inputPipe[1]);

    int status = 0;
    while (waitpid(child, &status, 0) < 0) {
        if (errno != EINTR) {
            return false;
        }
    }
    return wroteInput && WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

#endif

}  // namespace

bool copyTextToClipboard(std::string_view text) {
    clipboardStorage() = std::string(text);
    if (text.empty()) {
        return false;
    }

#ifndef _WIN32
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        return false;
    }
    for (const ClipboardCommand& command : kClipboardCommands) {
        if (runClipboardCommand(command, text)) {
            return true;
        }
    }
#endif

    return false;
}

std::string runtimeClipboardText() {
    return clipboardStorage();
}

}  // namespace ui
