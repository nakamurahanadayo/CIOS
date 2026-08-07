#pragma once

#include <optional>
#include <string>

namespace cios::features {

struct ResourceState {
    double cpuUsagePercent = 0.0;
    double memoryUsagePercent = 0.0;
};

enum class ResourcePressure {
    Normal,
    Elevated,
    Critical
};

ResourceState readResourceState();
ResourcePressure classifyPressure(const ResourceState& state);
std::optional<std::string> resourceRemark(
    ResourcePressure pressure,
    unsigned responseCounter
);

}  // namespace cios::features
