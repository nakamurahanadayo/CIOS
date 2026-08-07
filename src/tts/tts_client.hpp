#pragma once

#include <string>

class TTSClient
{
public:
    static bool speakGlados(
        const std::string& text,
        const std::string& style = "Neutral"
    );

    static bool serverAvailable();
};
