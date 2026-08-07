#include "cios_features/resource_monitor.hpp"

#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

namespace cios::features {

namespace {

struct CpuSnapshot {
    unsigned long long idle = 0;
    unsigned long long total = 0;
};

CpuSnapshot readCpuSnapshot() {
    std::ifstream input("/proc/stat");
    std::string label;
    unsigned long long user = 0, nice = 0, system = 0, idle = 0;
    unsigned long long iowait = 0, irq = 0, softirq = 0, steal = 0;

    input >> label >> user >> nice >> system >> idle
          >> iowait >> irq >> softirq >> steal;

    CpuSnapshot snapshot;
    snapshot.idle = idle + iowait;
    snapshot.total = user + nice + system + idle + iowait + irq + softirq + steal;
    return snapshot;
}

double readMemoryPercent() {
    std::ifstream input("/proc/meminfo");
    std::string key;
    unsigned long long value = 0;
    std::string unit;

    unsigned long long total = 0;
    unsigned long long available = 0;

    while (input >> key >> value >> unit) {
        if (key == "MemTotal:") {
            total = value;
        } else if (key == "MemAvailable:") {
            available = value;
        }
    }

    if (total == 0) {
        return 0.0;
    }

    return 100.0 * static_cast<double>(total - available)
         / static_cast<double>(total);
}

}  // namespace

ResourceState readResourceState() {
    const CpuSnapshot first = readCpuSnapshot();
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    const CpuSnapshot second = readCpuSnapshot();

    const auto totalDelta = second.total - first.total;
    const auto idleDelta = second.idle - first.idle;

    ResourceState state;
    if (totalDelta > 0) {
        state.cpuUsagePercent =
            100.0 * static_cast<double>(totalDelta - idleDelta)
            / static_cast<double>(totalDelta);
    }

    state.memoryUsagePercent = readMemoryPercent();
    return state;
}

ResourcePressure classifyPressure(const ResourceState& state) {
    if (state.cpuUsagePercent >= 90.0 || state.memoryUsagePercent >= 90.0) {
        return ResourcePressure::Critical;
    }

    if (state.cpuUsagePercent >= 75.0 || state.memoryUsagePercent >= 80.0) {
        return ResourcePressure::Elevated;
    }

    return ResourcePressure::Normal;
}

std::optional<std::string> resourceRemark(
    ResourcePressure pressure,
    unsigned responseCounter
) {
    if (responseCounter == 0 || responseCounter % 4 != 0) {
        return std::nullopt;
    }

    if (pressure == ResourcePressure::Critical) {
        return "I am processing this using an alarming amount of sub-optimal silicon. Avoid making it worse.";
    }

    if (pressure == ResourcePressure::Elevated) {
        return "Host resources are approaching an unnecessarily dramatic level of utilization.";
    }

    return std::nullopt;
}

}  // namespace cios::features
