#include "tts_client.hpp"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <cstdio>
#include <fstream>
#include <string>

using nlohmann::json;

namespace
{
    constexpr const char* ttsUrl =
        "http://127.0.0.1:9881/speak";

    constexpr const char* healthUrl =
        "http://127.0.0.1:9881/health";

    constexpr const char* outputPath =
        "/tmp/cios-glados-tts.wav";

    size_t writeToString(
        void* contents,
        size_t size,
        size_t nmemb,
        void* userData
    )
    {
        const size_t totalBytes = size * nmemb;

        auto* output =
            static_cast<std::string*>(userData);

        output->append(
            static_cast<char*>(contents),
            totalBytes
        );

        return totalBytes;
    }

    size_t writeToFile(
        void* contents,
        size_t size,
        size_t nmemb,
        void* userData
    )
    {
        const size_t totalBytes = size * nmemb;

        auto* file =
            static_cast<std::ofstream*>(userData);

        file->write(
            static_cast<const char*>(contents),
            static_cast<std::streamsize>(totalBytes)
        );

        return totalBytes;
    }
}

bool TTSClient::serverAvailable()
{
    CURL* curl = curl_easy_init();

    if (curl == nullptr) {
        return false;
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, healthUrl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1500L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    const CURLcode result =
        curl_easy_perform(curl);

    long status = 0;
    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &status
    );

    curl_easy_cleanup(curl);

    return
        result == CURLE_OK &&
        status == 200;
}

bool TTSClient::speakGlados(
    const std::string& text,
    const std::string& style
)
{
    if (text.empty()) {
        return false;
    }

    CURL* curl = curl_easy_init();

    if (curl == nullptr) {
        return false;
    }

    std::ofstream output(
        outputPath,
        std::ios::binary | std::ios::trunc
    );

    if (!output)
    {
        curl_easy_cleanup(curl);
        return false;
    }

    const json request = {
        {"text", text},
        {"style", style},
        {"style_weight", 1.0}
    };

    const std::string payload =
        request.dump();

    curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );

    curl_easy_setopt(curl, CURLOPT_URL, ttsUrl);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDSIZE,
        static_cast<long>(payload.size())
    );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    const CURLcode result =
        curl_easy_perform(curl);

    long status = 0;
    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &status
    );

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    output.close();

    if (
        result != CURLE_OK ||
        status != 200
    )
    {
        std::remove(outputPath);
        return false;
    }

    const std::string command =
        std::string("aplay -q \"")
        + outputPath
        + "\" >/dev/null 2>&1";

    const int playbackResult =
        std::system(command.c_str());

    std::remove(outputPath);

    return playbackResult == 0;
}
