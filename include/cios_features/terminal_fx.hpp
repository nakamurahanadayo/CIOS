#pragma once

#include <iosfwd>
#include <string>
#include <string_view>

namespace cios::features {

std::string normalizeCommand(std::string input);
std::string sanitizeForTerminal(std::string_view input);

void setPalette(std::ostream& out, std::string_view palette);
void setBlinkingBlockCursor(std::ostream& out);
void resetTerminalStyle(std::ostream& out);

void printTeletype(
    std::ostream& out,
    std::string_view text,
    int delayMs,
    bool enabled
);

void briefRedAlert(std::ostream& out, std::string_view message);

}  // namespace cios::features
