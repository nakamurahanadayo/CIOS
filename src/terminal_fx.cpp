#include "cios_features/terminal_fx.hpp"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <iostream>
#include <thread>
#include <unistd.h>

namespace cios::features {

std::string normalizeCommand(std::string input) {
    const auto notSpace = [](unsigned char c) {
        return !std::isspace(c);
    };

    input.erase(input.begin(), std::find_if(input.begin(), input.end(), notSpace));
    input.erase(std::find_if(input.rbegin(), input.rend(), notSpace).base(), input.end());

    if (!input.empty() && input.front() == '/') {
        input.erase(input.begin());
    }

    std::transform(
        input.begin(),
        input.end(),
        input.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        }
    );

    return input;
}

std::string sanitizeForTerminal(std::string_view input) {
    std::string output;
    output.reserve(input.size());

    for (unsigned char c : input) {
        if (std::isprint(c)) {
            output.push_back(static_cast<char>(c));
        }
    }

    return output;
}

void setPalette(std::ostream& out, std::string_view palette) {
    if (palette == "amber") {
        out << "\033[38;2;255;170;0m";
    } else if (palette == "cyan") {
        out << "\033[38;2;0;200;255m";
    } else {
        out << "\033[38;2;0;255;70m";
    }
    out.flush();
}

void setBlinkingBlockCursor(std::ostream& out) {
    out << "\033[1 q";
    out.flush();
}

void resetTerminalStyle(std::ostream& out) {
    out << "\033[0m";
    out.flush();
}

void printTeletype(
    std::ostream& out,
    std::string_view text,
    int delayMs,
    bool enabled
) {
    const bool interactive = ::isatty(STDOUT_FILENO);

    if (!enabled || !interactive || delayMs <= 0) {
        out << text << '\n';
        return;
    }

    for (char c : text) {
        out.put(c);
        out.flush();

        if (c != '\n') {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
    }

    out << '\n';
}

void briefRedAlert(std::ostream& out, std::string_view message) {
    out << "\033[31m\033[5m" << message << "\033[0m\n";
    out.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(450));
}

}  // namespace cios::features
