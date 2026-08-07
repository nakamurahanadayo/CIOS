#pragma once

#include <string>
#include <sys/types.h>

class AIEngine
{
public:
    AIEngine() = default;
    ~AIEngine();

    AIEngine(const AIEngine&) = delete;
    AIEngine& operator=(const AIEngine&) = delete;

    bool start();
    void stop();

    [[nodiscard]] bool isRunning() const;
    [[nodiscard]] std::string status() const;

private:
    bool waitUntilReady(int timeoutSeconds);
    std::string findServerPath() const;
    std::string findModelPath() const;

    bool running_{false};
    bool ownsServerProcess_{false};

    pid_t serverPid_{-1};

    std::string model_;
    std::string server_;
};
