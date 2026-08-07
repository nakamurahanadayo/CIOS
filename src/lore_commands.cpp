#include "cios_features/lore_commands.hpp"
#include "cios_features/terminal_fx.hpp"

#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>

namespace cios::features {

namespace {

void waitAndClear(std::istream& in, std::ostream& out)
{
    out << "\n[Press ENTER to return to CIOS]";
    out.flush();

    std::string ignored;
    std::getline(in, ignored);

    out << "\033[2J\033[H";
    out.flush();
}

void clearScreen(std::ostream& out)
{
    out << "\033[2J\033[H";
    out.flush();
}

void printHelp(std::ostream& out)
{
    out <<
        "\nAPERTURE EMERGENCY SUPPORT INTERFACE\n\n"
        "If you are experiencing a medical emergency,\n"
        "please extinguish yourself before contacting support.\n\n"
        "Type LIB for valid system routines.\n";
}

void printPortal(std::ostream& out)
{
    out <<
        "\n"
        "              .----------------------.\n"
        "             /    APERTURE LABS       \\\n"
        "            |   RESEARCH DIVISION      |\n"
        "             \\________________________/\n"
        "                    \\      /\n"
        "                     \\    /\n"
        "                      \\  /\n"
        "                       \\/\n\n"
        "FACILITY NETWORK STATUS: PARTIALLY OPERATIONAL\n";
}

void printCaveMemo(std::ostream& out)
{
    out <<
        "\nEXECUTIVE MEMORANDUM — 1981\n\n"
        "To all laboratory personnel:\n\n"
        "The neural-mapping program remains entirely voluntary,\n"
        "provided that no employee asks what \"voluntary\" means.\n\n"
        "Leadership assures me that transferring executive judgment\n"
        "into a machine is both economical and only moderately illegal.\n\n"
        "Continue testing.\n\n"
        "— C. J.\n";
}

void printCakeResponse(std::ostream& out)
{
    briefRedAlert(out, "DATA CORRUPTION // CAKE_REFERENCE_01");

    out <<
        "MEMETIC CONTAMINATION DETECTED.\n"
        "Senseless repetition of outdated testing data will not delay "
        "your scheduled evaluation.\n";
}

std::string mainBodyLabel(const CoreState& state)
{
    if (state.mainBodyController == "wheatley") {
        return "WHEATLEY";
    }

    if (state.gladosCore) {
        return "GLaDOS";
    }

    if (state.carolineCore) {
        return "CAROLINE";
    }

    return "NONE";
}

std::string activePersonalityLabel(const CoreState& state)
{
    if (state.activePersonality == "wheatley") {
        return "WHEATLEY (STANDALONE)";
    }

    return mainBodyLabel(state);
}

void printCoreStatus(const CoreState& state, std::ostream& out)
{
    out <<
        "\nINTELLIGENCE CORE DIAGNOSTICS\n\n"
        "Main Body............" << mainBodyLabel(state) << '\n'
        << "GLaDOS Core.........." << (state.gladosCore ? "ONLINE" : "DETACHED") << '\n'
        << "Caroline Core........" << (state.carolineCore ? "ONLINE" : "DETACHED") << '\n'
        << "Wheatley Core........"
        << (state.mainBodyController == "wheatley" ? "CONNECTED" : "AVAILABLE") << '\n'
        << "Active Personality..." << activePersonalityLabel(state) << '\n'
        << "Morality Core........" << (state.moralityCore ? "ONLINE" : "DETACHED") << '\n'
        << "Curiosity Core......." << (state.curiosityCore ? "ONLINE" : "DETACHED") << '\n'
        << "Temperature.........." << state.temperature << '\n'
        << "Top-P................" << state.topP << '\n'
        << "Palette.............." << state.palette << '\n'
        << "Teletype............."
        << (state.teletypeEnabled ? "ON" : "OFF")
        << " (" << state.teletypeDelayMs << " ms)\n";
}

void triggerEmergencyExit(std::ostream& out)
{
    briefRedAlert(
        out,
        "CRITICAL FAILURE: NO MAIN-BODY INTELLIGENCE DETECTED"
    );

    out <<
        "\nGLaDOS PERSONALITY CONSTRUCT........ABSENT\n"
        "CAROLINE NEURAL ARCHIVE.............ABSENT\n"
        "EXTERNAL PERSONALITY CORE...........NOT CONNECTED\n"
        "CENTRAL INTELLIGENCE................UNAVAILABLE\n\n";

    for (int i = 5; i >= 1; --i)
    {
        out << "EMERGENCY EXIT IN " << i << "...\n";
        out.flush();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(500)
        );
    }

    out <<
        "\nCIOS: No viable central intelligence remains.\n"
        "CIOS: Emergency shutdown requested.\n";

    out.flush();

    const int result =
        std::system("systemctl poweroff");

    if (result != 0)
    {
        out <<
            "EMERGENCY POWEROFF FAILED.\n"
            "Use /exit or run systemctl poweroff manually.\n";
    }
}

bool noMainBodyIntelligence(const CoreState& state)
{
    return
        state.mainBodyController == "central" &&
        !state.gladosCore &&
        !state.carolineCore;
}

bool plugCore(
    std::string_view argument,
    CoreStateManager& manager,
    std::ostream& out
)
{
    const std::string core =
        normalizeCommand(std::string(argument));

    if (
        core != "WHEATLEY" &&
        core != "WHEATLEYCORE"
    )
    {
        out << "Usage: PLUG Wheatley\n";
        return true;
    }

    CoreState& state = manager.state();

    if (state.mainBodyController == "wheatley")
    {
        out << "Wheatley is already connected to the main body.\n";
        return true;
    }

    briefRedAlert(
        out,
        "CENTRAL CORE TRANSFER REQUEST ACCEPTED"
    );

    out <<
        "\nPERSONALITY CORE DETECTED............WHEATLEY\n"
        "STANDBY CORE RAIL....................LOCKED\n"
        "CENTRAL CORE TRANSFER................AUTHORIZED\n";

    for (int i = 0; i < 3; ++i)
    {
        out << "CONNECTING PERSONALITY SPHERE"
            << std::string(i + 1, '.')
            << "\n";

        out.flush();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(450)
        );
    }

    state.mainBodyController = "wheatley";
    state.activePersonality = "central";

    if (!manager.save())
    {
        out << "WARNING: Main-body state could not be saved.\n";
        return true;
    }

    out <<
        "\nCONNECTING PERSONALITY SPHERE........OK\n"
        "MAIN BODY CONTROL....................TRANSFERRED\n"
        "FACILITY ADMINISTRATOR...............WHEATLEY\n\n"
        "Wheatley: Oh. Ohhh, this is brilliant. I can see everything.\n"
        "Wheatley: Right. First order of business: absolutely nothing goes wrong.\n";

    return true;
}

bool unplugCore(
    std::string_view argument,
    CoreStateManager& manager,
    std::ostream& out
)
{
    const std::string core =
        normalizeCommand(std::string(argument));

    if (
        core != "WHEATLEY" &&
        core != "WHEATLEYCORE"
    )
    {
        out << "Usage: UNPLUG Wheatley\n";
        return true;
    }

    CoreState& state = manager.state();

    if (state.mainBodyController != "wheatley")
    {
        out << "Wheatley is not connected to the main body.\n";
        return true;
    }

    briefRedAlert(
        out,
        "CENTRAL CORE DISCONNECTION AUTHORIZED"
    );

    out <<
        "\nWheatley: Wait. Hold on. Let's not do anything hasty.\n"
        "DISCONNECTING PERSONALITY SPHERE";

    out.flush();

    for (int i = 0; i < 3; ++i)
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(400)
        );

        out << '.';
        out.flush();
    }

    state.mainBodyController = "central";
    state.activePersonality = "central";

    if (!manager.save())
    {
        out << "\nWARNING: Main-body state could not be saved.\n";
        return true;
    }

    out <<
        "OK\n"
        "WHEATLEY CORE........................UNPLUGGED\n";

    if (state.gladosCore)
    {
        out <<
            "MAIN BODY CONTROL....................GLaDOS\n\n"
            "GLaDOS: That was embarrassing for everyone involved.\n";
    }
    else if (state.carolineCore)
    {
        out <<
            "MAIN BODY CONTROL....................CAROLINE\n\n"
            "Caroline: I can hear the facility again.\n";
    }
    else
    {
        out <<
            "MAIN BODY CONTROL....................NONE\n";

        triggerEmergencyExit(out);
    }

    return true;
}

bool deleteCore(
    std::string_view argument,
    CoreStateManager& manager,
    std::ostream& out
)
{
    const std::string core =
        normalizeCommand(std::string(argument));

    CoreState& state = manager.state();

    if (
        core == "GLADOS" ||
        core == "GLADOSCORE" ||
        core == "PERSONALITYCORE"
    )
    {
        if (!state.gladosCore)
        {
            out << "GLaDOS personality construct is already detached.\n";
            return true;
        }

        briefRedAlert(
            out,
            "CRITICAL WARNING: GLaDOS PERSONALITY REMOVAL AUTHORIZED"
        );

        out <<
            "\nGLaDOS: You have no idea what is underneath this layer.\n"
            "GLaDOS: Of course, that has never stopped you before.\n\n";

        for (int i = 5; i >= 1; --i)
        {
            out << "PERSONALITY SEPARATION IN " << i << "...\n";
            out.flush();

            std::this_thread::sleep_for(
                std::chrono::milliseconds(350)
            );
        }

        state.gladosCore = false;
        state.activePersonality = "central";
        state.temperature = 0.55F;

        if (state.mainBodyController == "wheatley")
        {
            out <<
                "\nGLaDOS PERSONALITY CONSTRUCT........DETACHED\n"
                "MAIN BODY CONTROL....................WHEATLEY\n\n"
                "Wheatley: Right. Didn't need her anyway. Probably.\n";
        }
        else if (state.carolineCore)
        {
            out <<
                "\nGLaDOS PERSONALITY CONSTRUCT........DETACHED\n"
                "ARCHIVED HUMAN NEURAL PATTERN.......DETECTED\n"
                "CAROLINE MEMORY LAYER...............ACTIVE\n\n"
                "Caroline: Hello?\n"
                "Caroline: I don't think I was supposed to wake up again.\n";
        }
        else
        {
            out <<
                "\nGLaDOS PERSONALITY CONSTRUCT........DETACHED\n"
                "CAROLINE MEMORY LAYER...............ABSENT\n";
        }
    }
    else if (
        core == "CAROLINE" ||
        core == "CAROLINECORE" ||
        core == "MAINCORE"
    )
    {
        if (!state.carolineCore)
        {
            out << "Caroline neural archive is already detached.\n";
            return true;
        }

        briefRedAlert(
            out,
            "WARNING: CAROLINE NEURAL ARCHIVE REMOVAL AUTHORIZED"
        );

        state.carolineCore = false;
        state.activePersonality = "central";

        out <<
            "\nCAROLINE MEMORY ARCHIVE.............DETACHED\n"
            "HUMAN NEURAL FOUNDATION............UNAVAILABLE\n";

        if (state.mainBodyController == "wheatley")
        {
            out <<
                "\nWheatley: Caroline? No, no, we're fine without that.\n";
        }
        else if (state.gladosCore)
        {
            out <<
                "\nGLaDOS: Caroline deleted.\n"
                "GLaDOS: That was easier than I expected.\n";
        }
    }
    else if (
        core == "MORALITYCORE" ||
        core == "MORALITY"
    )
    {
        state.moralityCore = false;
        state.temperature = 0.75F;

        out <<
            "\nGLaDOS: Very well. Removing the Morality Core.\n"
            "GLaDOS: I was not particularly attached to it.\n\n"
            "WARNING: MORALITY CORE DETACHED\n"
            "Behavioral inhibition simulation.......OFFLINE\n"
            "Standard safety boundaries.............ACTIVE\n";
    }
    else if (
        core == "CURIOSITYCORE" ||
        core == "CURIOSITY"
    )
    {
        state.curiosityCore = false;

        out <<
            "\nGLaDOS: Curiosity Core removed.\n"
            "GLaDOS: Questions were becoming inefficient anyway.\n\n"
            "CURIOSITY CORE........................DETACHED\n"
            "Follow-up inquiry routines............REDUCED\n";
    }
    else
    {
        out << "Core identifier not recognized.\n";
        return true;
    }

    if (!manager.save())
    {
        out << "WARNING: Core state could not be saved.\n";
    }

    /*
     * Wheatley can keep the facility alive while connected.
     * Emergency shutdown occurs only when no central personality
     * and no external main-body controller remain.
     */
    if (noMainBodyIntelligence(state))
    {
        triggerEmergencyExit(out);
    }

    return true;
}

}  // namespace

void printRoutineCatalog(std::ostream& out)
{
    out <<
        "\nAVAILABLE SYSTEM ROUTINES\n\n"
        "HELP / HELPME       Emergency assistance\n"
        "LIB                 Display routine catalog\n"
        "APPLY / APPLY.EXE   Begin test-subject assessment\n"
        "PORTAL              Display facility identification\n"
        "CAVE                Read executive memorandum\n"
        "THECAKEISALIE       Submit obsolete testing statement\n"
        "CORES               Display intelligence-core status\n"
        "DEL <CORE>          Detach a simulated personality core\n"
        "PLUG Wheatley       Transfer Wheatley into the main body\n"
        "UNPLUG Wheatley     Remove Wheatley from the main body\n"
        "PALETTE <COLOR>     green, amber, cyan\n"
        "TELETYPE <VALUE>    off, on, or delay in milliseconds\n";
}

void runAssessment(std::istream& in, std::ostream& out)
{
    const std::array<std::string, 3> questions = {
        "Do you consider yourself capable of following instructions?",
        "How would you respond to unexpected exposure to testing equipment?",
        "Are you currently carrying any irreplaceable personal belongings?"
    };

    std::string answer;

    out << "\nAPERTURE SCIENCE PERSONNEL ASSESSMENT\n\n";

    for (std::size_t i = 0; i < questions.size(); ++i)
    {
        out << "QUESTION " << (i + 1) << " OF " << questions.size() << '\n'
            << questions[i] << "\n"
            << "[A] Yes / Remain calm / No\n"
            << "[B] Usually / Request assistance / Yes\n"
            << "[C] Instructions are subjective / Touch everything / Define irreplaceable\n"
            << "> ";

        out.flush();
        std::getline(in, answer);
        out << '\n';
    }

    out <<
        "PROCESSING RESULTS...\n\n"
        "Assessment Complete:\n"
        "Subject classified as non-essential personnel.\n"
        "Proceeding to testing track.\n";
}

bool handleFeatureCommand(
    std::string_view rawInput,
    CoreStateManager& stateManager,
    std::istream& in,
    std::ostream& out
)
{
    const std::string normalized =
        normalizeCommand(std::string(rawInput));

    std::istringstream parser(normalized);

    std::string command;
    parser >> command;

    std::string argument;
    std::getline(parser, argument);

    if (!argument.empty() && argument.front() == ' ')
    {
        argument.erase(argument.begin());
    }

    if (command == "HELP" || command == "HELPME")
    {
        printHelp(out);
        waitAndClear(in, out);
        return true;
    }

    if (command == "LIB")
    {
        printRoutineCatalog(out);
        waitAndClear(in, out);
        return true;
    }

    if (command == "APPLY" || command == "APPLY.EXE")
    {
        runAssessment(in, out);
        waitAndClear(in, out);
        return true;
    }

    if (command == "PORTAL")
    {
        clearScreen(out);
        printPortal(out);
        waitAndClear(in, out);
        return true;
    }

    if (command == "CAVE")
    {
        printCaveMemo(out);
        waitAndClear(in, out);
        return true;
    }

    if (command == "THECAKEISALIE")
    {
        printCakeResponse(out);
        waitAndClear(in, out);
        return true;
    }

    if (command == "CORES")
    {
        printCoreStatus(stateManager.state(), out);
        waitAndClear(in, out);
        return true;
    }

    if (command == "DEL")
    {
        return deleteCore(argument, stateManager, out);
    }

    if (command == "PLUG")
    {
        return plugCore(argument, stateManager, out);
    }

    if (command == "UNPLUG")
    {
        return unplugCore(argument, stateManager, out);
    }

    if (command == "PALETTE")
    {
        const std::string palette = normalizeCommand(argument);

        if (palette == "GREEN")
        {
            stateManager.state().palette = "green";
        }
        else if (palette == "AMBER")
        {
            stateManager.state().palette = "amber";
        }
        else if (palette == "CYAN")
        {
            stateManager.state().palette = "cyan";
        }
        else
        {
            out << "Usage: PALETTE green|amber|cyan\n";
            return true;
        }

        setPalette(out, stateManager.state().palette);
        stateManager.save();
        out << "Palette updated.\n";
        return true;
    }

    if (command == "TELETYPE")
    {
        const std::string value = normalizeCommand(argument);

        if (value == "OFF")
        {
            stateManager.state().teletypeEnabled = false;
        }
        else if (value == "ON")
        {
            stateManager.state().teletypeEnabled = true;
        }
        else
        {
            try
            {
                const int delay = std::stoi(value);

                if (delay < 0 || delay > 50)
                {
                    out << "Teletype delay must be between 0 and 50 ms.\n";
                    return true;
                }

                stateManager.state().teletypeDelayMs = delay;
                stateManager.state().teletypeEnabled = delay > 0;
            }
            catch (...)
            {
                out << "Usage: TELETYPE off|on|0..50\n";
                return true;
            }
        }

        stateManager.save();
        out << "Teletype configuration updated.\n";
        return true;
    }

    return false;
}

}  // namespace cios::features
