#include "cios_features/core_state.hpp"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <utility>

namespace fs = std::filesystem;
using nlohmann::json;

namespace cios::features {

namespace {

void fromJson(const json& j, CoreState& state)
{
    state.gladosCore = j.value("glados_core", true);
    state.moralityCore = j.value("morality_core", true);
    state.curiosityCore = j.value("curiosity_core", true);
    state.carolineCore = j.value("caroline_core", true);

    state.activePersonality =
        j.value("active_personality", std::string{"central"});

    state.mainBodyController =
        j.value("main_body_controller", std::string{"central"});

    state.temperature = j.value("temperature", 0.45F);
    state.topP = j.value("top_p", 0.85F);
    state.repeatPenalty = j.value("repeat_penalty", 1.10F);
    state.teletypeEnabled = j.value("teletype_enabled", true);
    state.teletypeDelayMs = j.value("teletype_delay_ms", 4);
    state.palette = j.value("palette", std::string{"green"});
}

json toJson(const CoreState& state)
{
    return {
        {"glados_core", state.gladosCore},
        {"morality_core", state.moralityCore},
        {"curiosity_core", state.curiosityCore},
        {"caroline_core", state.carolineCore},
        {"active_personality", state.activePersonality},
        {"main_body_controller", state.mainBodyController},
        {"temperature", state.temperature},
        {"top_p", state.topP},
        {"repeat_penalty", state.repeatPenalty},
        {"teletype_enabled", state.teletypeEnabled},
        {"teletype_delay_ms", state.teletypeDelayMs},
        {"palette", state.palette}
    };
}

bool readStateFile(const std::string& path, CoreState& state)
{
    std::ifstream input(path);

    if (!input) {
        return false;
    }

    try {
        json j;
        input >> j;
        fromJson(j, state);
        return true;
    } catch (...) {
        return false;
    }
}

}  // namespace

CoreStateManager::CoreStateManager(
    std::string runtimePath,
    std::string persistentPath,
    std::string defaultPath
)
    : runtimePath_(std::move(runtimePath)),
      persistentPath_(std::move(persistentPath)),
      defaultPath_(std::move(defaultPath))
{
}

bool CoreStateManager::load()
{
    if (readStateFile(runtimePath_, state_)) {
        return true;
    }

    if (readStateFile(persistentPath_, state_)) {
        return save();
    }

    if (readStateFile(defaultPath_, state_)) {
        return save();
    }

    reset();
    return save();
}

bool CoreStateManager::save() const
{
    try {
        const fs::path path(runtimePath_);

        if (path.has_parent_path()) {
            fs::create_directories(path.parent_path());
        }

        std::ofstream output(runtimePath_);

        if (!output) {
            return false;
        }

        output << toJson(state_).dump(2) << '\n';
        return true;
    } catch (...) {
        return false;
    }
}

void CoreStateManager::reset()
{
    state_ = CoreState{};
}

CoreState& CoreStateManager::state()
{
    return state_;
}

const CoreState& CoreStateManager::state() const
{
    return state_;
}

}  // namespace cios::features
