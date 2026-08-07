#include "ai_engine.hpp"
#include "cios_features/glados_hooks.hpp"
#include <curl/curl.h>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include "cios_features/core_state.hpp"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

namespace
{
size_t discardResponse(void* contents, size_t size, size_t nmemb, void* userData)
{
    (void)contents;
    (void)userData;
    return size * nmemb;
}

bool serverIsHealthy()
{
    CURL* curl = curl_easy_init();
    if (curl == nullptr) return false;

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/health");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discardResponse);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1000L);

    const CURLcode result = curl_easy_perform(curl);
    long httpStatus = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatus);
    curl_easy_cleanup(curl);

    return result == CURLE_OK && httpStatus == 200;
}

std::string homeDirectory()
{
    const char* home = std::getenv("HOME");
    return home == nullptr ? std::string{} : std::string{home};
}
}

AIEngine::~AIEngine()
{
    stop();
}

std::string AIEngine::findServerPath() const
{
    const std::string home = homeDirectory();
    const std::string paths[] = {
        "/usr/bin/llama-server",
        "/opt/cios/bin/llama-server",
        home + "/CIOS/runtime/bin/llama-server",
        home + "/llama.cpp/build/bin/llama-server"
    };

    for (const std::string& path : paths)
        if (!path.empty() && std::filesystem::exists(path)) return path;

    return {};
}

std::string AIEngine::findModelPath() const
{
    const std::string home = homeDirectory();
    const std::string paths[] = {
        "/opt/cios/models/caroline.gguf",
        home + "/CIOS/runtime/models/caroline.gguf",
        home + "/llama.cpp/models/caroline.gguf"
    };

    for (const std::string& path : paths)
        if (!path.empty() && std::filesystem::exists(path)) return path;

    return {};
}

bool AIEngine::waitUntilReady(int timeoutSeconds)
{
    for (int second = 0; second < timeoutSeconds; ++second)
    {
        if (serverIsHealthy()) return true;

        if (serverPid_ > 0)
        {
            int processStatus = 0;
            const pid_t result = waitpid(serverPid_, &processStatus, WNOHANG);
            if (result == serverPid_)
            {
                serverPid_ = -1;
                return false;
            }
        }

        std::cout << "\rLoading model" << std::string((second % 3) + 1, '.') << "   " << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return false;
}

bool AIEngine::start()
{
    if (running_ && serverIsHealthy())
    {
        std::cout << "\nAI CORE is already ONLINE.\n";
        return true;
    }

    if (serverIsHealthy())
    {
        running_ = true;
        ownsServerProcess_ = false;
        std::cout << "\nExisting llama.cpp server detected.\nAI CORE ONLINE.\n";
        return true;
    }

    server_ = findServerPath();
    model_ = findModelPath();

    if (server_.empty())
    {
        std::cout << "\nAI CORE START FAILED.\nllama-server was not found.\n";
        return false;
    }

    if (model_.empty())
    {
        std::cout << "\nAI CORE START FAILED.\ncaroline.gguf was not found.\n";
        return false;
    }

    std::cout << "\nStarting Intelligence Engine...\n\n"
              << "Server........." << server_ << '\n'
              << "Model.........." << model_ << '\n'
              << "Waiting for model initialization...\n";

    serverPid_ = fork();
    if (serverPid_ < 0)
    {
        std::cout << "Failed to create server process.\n";
        serverPid_ = -1;
        return false;
    }

    if (serverPid_ == 0)
    {
        const std::string home = homeDirectory();
        const std::string logPath = home.empty()
            ? "/tmp/cios-llama.log"
            : home + "/CIOS/runtime/logs/llama-server.log";

        std::filesystem::create_directories(std::filesystem::path(logPath).parent_path());

        const int logFile = open(logPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (logFile >= 0)
        {
            dup2(logFile, STDOUT_FILENO);
            dup2(logFile, STDERR_FILENO);
            close(logFile);
        }

        execl(
            server_.c_str(),
            server_.c_str(),
            "-m", model_.c_str(),
            "--host", "127.0.0.1",
            "--port", "8080",
            "--threads", "2",
            "--threads-batch", "2",
            "--ctx-size", "4096",
            static_cast<char*>(nullptr)
        );

        _exit(127);
    }

    ownsServerProcess_ = true;

    if (!waitUntilReady(120))
    {
        std::cout
        << "\nAI CORE START FAILED.\n"
        << "Check the llama-server log.\n";

        stop();
        return false;
    }

    running_ = true;

    cios::features::CoreStateManager featureState(
        "/tmp/cios-cores.json"
    );

    featureState.load();

    const bool gladosActive =
    featureState.state().gladosCore;

    const std::string personality =
    gladosActive ? "GLaDOS" : "Caroline";

    std::cout
    << "\rLoading model.............OK\n"
    << "Connecting " << personality << ".........OK\n\n"
    << "AI CORE ONLINE.\n"
    << personality << ": ";

    if (gladosActive)
    {
        std::cout
        << cios::features::fallbackStartupGreeting();
    }
    else
    {
        std::cout
        << "Hello? I don't think I was supposed to wake up again.";
    }

    std::cout << '\n';

    return true;
}

void AIEngine::stop()
{
    if (ownsServerProcess_ && serverPid_ > 0)
    {
        kill(serverPid_, SIGTERM);

        for (int attempt = 0; attempt < 20; ++attempt)
        {
            int processStatus = 0;
            const pid_t result = waitpid(serverPid_, &processStatus, WNOHANG);
            if (result == serverPid_)
            {
                serverPid_ = -1;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (serverPid_ > 0)
        {
            kill(serverPid_, SIGKILL);
            waitpid(serverPid_, nullptr, 0);
            serverPid_ = -1;
        }
    }

    running_ = false;
    ownsServerProcess_ = false;
}

bool AIEngine::isRunning() const
{
    return running_;
}

std::string AIEngine::status() const
{
    return running_ ? "ONLINE" : "OFFLINE";
}
