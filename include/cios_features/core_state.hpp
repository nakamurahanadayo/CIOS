#pragma once

#include <string>

namespace cios::features {

struct CoreState {
    bool gladosCore = true;
    bool moralityCore = true;
    bool curiosityCore = true;
    bool carolineCore = true;

    /*
     * Temporary chat selection.
     * "central" uses the current main-body controller.
     * "/wheatley" temporarily changes this to "wheatley".
     */
    std::string activePersonality = "central";

    /*
     * Permanent facility-control selection.
     * Supported values currently:
     *   "central"  -> GLaDOS or Caroline
     *   "wheatley" -> Wheatley controls the main body
     */
    std::string mainBodyController = "central";

    float temperature = 0.45F;
    float topP = 0.85F;
    float repeatPenalty = 1.10F;

    bool teletypeEnabled = true;
    int teletypeDelayMs = 4;
    std::string palette = "green";
};

class CoreStateManager {
public:
    explicit CoreStateManager(
        std::string runtimePath = "/run/cios/cores.json",
        std::string persistentPath = "/var/lib/cios/cores.json",
        std::string defaultPath = "/opt/cios/config/default_cores.json"
    );

    bool load();
    bool save() const;
    void reset();

    CoreState& state();
    const CoreState& state() const;

private:
    std::string runtimePath_;
    std::string persistentPath_;
    std::string defaultPath_;
    CoreState state_;
};

}  // namespace cios::features
