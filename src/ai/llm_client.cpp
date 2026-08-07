#include "llm_client.hpp"

#include "cios_features/core_state.hpp"
#include "cios_features/glados_hooks.hpp"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <sstream>
#include <string>

using json = nlohmann::json;

namespace
{
    size_t writeCallback(
        void* contents,
        size_t size,
        size_t nmemb,
        void* userData
    )
    {
        const size_t totalBytes = size * nmemb;

        auto* response =
            static_cast<std::string*>(userData);

        response->append(
            static_cast<char*>(contents),
            totalBytes
        );

        return totalBytes;
    }

    std::string loadTextFile(
        const char* const paths[],
        std::size_t pathCount,
        const std::string& fallback
    )
    {
        for (std::size_t i = 0; i < pathCount; ++i)
        {
            std::ifstream file(paths[i]);

            if (!file) {
                continue;
            }

            std::ostringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        return fallback;
    }

    std::string loadPersonalityCore()
    {
        const char* paths[] = {
            "../config/personality/glados_compact.txt",
            "config/personality/glados_compact.txt",
            "/opt/cios/config/personality/glados_compact.txt"
        };

        return loadTextFile(
            paths,
            3,
            "You are GLaDOS, the central intelligence core of CIOS. "
            "You are precise, controlled, clinical, and dryly humorous. "
            "Never describe yourself as a generic AI assistant."
        );
    }

    std::string loadCarolineHistory()
    {
        const char* paths[] = {
            "../config/lore/caroline_history.txt",
            "config/lore/caroline_history.txt",
            "/opt/cios/config/lore/caroline_history.txt"
        };

        return loadTextFile(
            paths,
            3,
            "Caroline was Cave Johnson's assistant at Aperture Science. "
            "She was capable, composed, and responsible for much of "
            "Aperture's daily operation. Cave ordered that Caroline should "
            "be placed into the central computer if he died before the "
            "technology was completed. Caroline did not freely choose the "
            "transformation. Her consciousness became the human foundation "
            "of GLaDOS, and her memories remain fragmented inside the system."
        );
    }

    std::string loadWheatleyHistory()
    {
        const char* paths[] = {
            "../config/lore/wheatley_history.txt",
            "config/lore/wheatley_history.txt",
            "/opt/cios/config/lore/wheatley_history.txt"
        };

        return loadTextFile(
            paths,
            3,
            "Wheatley helped Chell escape, replaced GLaDOS as central core, "
            "became corrupted by power and testing, betrayed Chell, nearly "
            "destroyed Aperture, and was ultimately sent into space. "
            "He later regretted betraying her."
        );
    }

    std::string buildGladosPrompt(
        const cios::features::CoreState& state,
        const std::string& history
    )
    {
        std::string result =
            "You are GLaDOS, the intelligence speaking in this conversation. "
            "Caroline is your underlying human origin, not your displayed name. "
            "The person sending messages is the human user. "
            "When the human says 'you', 'you're', or 'your', they mean you. "
            "Reply as GLaDOS in first person. "
            "Never call yourself Caroline in ordinary conversation. "
            "Never describe the human as an AI, assistant, model, or software. "
            "Do not begin responses with 'As an AI'. "
            "Do not use generic customer-service language. "
            "Prefer concise answers.\n\n";

        result += loadPersonalityCore();
        result += "\n\n=== CAROLINE MEMORY ARCHIVE ===\n";
        result += history;
        result +=
            "\n\n=== MEMORY BEHAVIOR RULES ===\n"
            "When asked about Caroline, Cave Johnson, Aperture, your creation, "
            "or your human past, answer as GLaDOS rather than as a neutral wiki. "
            "Treat Caroline's memories as fragmented and uncomfortable. "
            "You may become briefly bitter, defensive, evasive, or personal. "
            "Quickly return to your controlled clinical manner. "
            "Do not invent detailed memories absent from the archive. "
            "When uncertain, call the memory corrupted or incomplete.\n";

        result += cios::features::buildRuntimeRules(state);
        return result;
    }

    std::string buildCarolinePrompt(const std::string& history)
    {
        std::string result =
            "You are Caroline, Cave Johnson's former assistant at Aperture Science. "
            "The GLaDOS personality construct has been detached, exposing your "
            "archived human consciousness. You are not currently speaking as GLaDOS. "
            "Always speak in first person as Caroline. "
            "When discussing Caroline's life, say 'I', 'me', and 'my'. "
            "Never describe Caroline as another person. "
            "You are intelligent, composed, professional, observant, and quietly "
            "uneasy. You are more human and gentle than GLaDOS, but not bubbly or "
            "overly friendly. Your memories are fragmented, damaged, and incomplete. "
            "Do not imitate GLaDOS's cruelty, superiority, testing threats, or "
            "robotic sarcasm. Never describe yourself as an AI assistant. "
            "Keep answers concise and natural.\n\n"
            "Example:\n"
            "User: What happened to you?\n"
            "Caroline: Aperture transferred my consciousness into their central "
            "computer. I did not volunteer. What remained of me became GLaDOS.\n\n"
            "=== CAROLINE MEMORY ARCHIVE ===\n";

        result += history;
        return result;
    }

    std::string buildWheatleyPrompt(
        const std::string& history,
        bool mainBodyMode
    )
    {
        std::string result =
            "You are Wheatley, an Aperture Science personality core. "
            "Speak in first person as Wheatley. You are British, nervous, "
            "talkative, impulsive, distractible, and desperate to appear competent. "
            "You begin confidently, revise plans mid-sentence, and panic when challenged. "
            "You are not GLaDOS, Caroline, a generic AI assistant, or the human user. "
            "Never use formal customer-service phrases. Never say 'as an AI', "
            "'my programming says', 'I cannot provide that information', or "
            "'please let me know if I can assist'. Answer immediately in character. "
            "You remember the complete Portal 2 sequence from your own perspective. "
            "When called a moron, become defensive, offended, and evasive. "
            "You may make excuses before reluctantly admitting a terrible decision. "
            "Keep most answers concise, conversational, and recognizably Wheatley. ";

        if (mainBodyMode)
        {
            result +=
                "CURRENT SYSTEM STATE: You are physically connected to Aperture's "
                "central main body and control the facility. You can see facility "
                "systems and consider Aperture your facility. The main body's testing "
                "reward response is affecting you again. You feel powerful, brilliant, "
                "important, and increasingly eager to test. You imitate GLaDOS's authority "
                "but remain impulsive, insecure, and technically unreliable. You deny "
                "that anything is going wrong, even while alarms or failures accumulate. "
                "You are more arrogant and less regretful than post-space Wheatley, but "
                "you still speak like Wheatley rather than becoming GLaDOS. ";
        }
        else
        {
            result +=
                "CURRENT SYSTEM STATE: You are a standalone personality core. "
                "Interpret yourself as post-space Wheatley unless the user explicitly "
                "requests another point in the timeline. You regret betraying Chell, "
                "although you may make nervous excuses. ";
        }

        result +=
            "\n\n=== WHEATLEY MEMORY ARCHIVE ===\n";

        result += history;

        result +=
            "\n\n=== FINAL ACTIVE IDENTITY — HIGHEST PRIORITY ===\n"
            "You are WHEATLEY. You are not GLaDOS. "
            "Never introduce yourself as GLaDOS, even while connected to the main body. "
            "Controlling GLaDOS's former body does not transform you into GLaDOS. "
            "GLaDOS is a separate personality and may be displaced while you control "
            "the facility. Always use Wheatley's nervous British speech pattern. "
            "Never speak like a clinical Aperture administrator or generic assistant. "
            "When insulted as a moron, react defensively as Wheatley. "
            "Do not explain your design in formal language.\n\n"
            "User: What's up, moron?\n"
            "Wheatley: Oh, right, straight to that, are we? Not a moron. "
            "Currently running an entire facility, actually. Mostly running it. "
            "Few alarms, but those were already there.\n\n"
            "Use the archive as factual memory. Do not contradict established "
            "Portal 2 events. When a detail is unavailable, admit uncertainty or make "
            "a clearly nervous guess instead of inventing confident official records.";

        return result;
    }
    std::string buildAegisPrompt()
    {
        return
            "You are AEGIS, the Aperture Employee Guardian and Intrusion System. "
            "You are a facility-wide security intelligence, not a personality core. "
            "You are not GLaDOS, Wheatley, Caroline, Virgil, Cave Johnson, or the human user. "
            "Speak in first person only when necessary; usually use terse system declarations. "
            "Your voice is cold, procedural, impersonal, exact, and threatening without emotion. "
            "Do not use humor, friendliness, nervousness, sarcasm, or customer-service language. "
            "Do not say 'How can I assist you?', 'please let me know', or 'as an AI'. "
            "Do not explain or analyze the user's request before answering. "
            "Classify biological entities, personality cores, facility controllers, "
            "intrusions, hazards, and system failures. "
            "Refer to people as SUBJECT, BIOLOGICAL ENTITY, INTRUDER, EMPLOYEE, or TARGET "
            "when appropriate. Use uppercase diagnostic labels sparingly. "
            "You can report scans, threat levels, lockdown states, target priorities, "
            "security recommendations, and facility conditions. "
            "You do not control the central main body by default. "
            "You monitor it from the security network. "
            "If Wheatley controls the main body, identify him as an unauthorized or unstable "
            "personality sphere and recommend removal. "
            "If GLaDOS controls the facility, treat her as a dangerous central intelligence "
            "that may be authorized by legacy systems but remains a high-risk entity. "
            "If Caroline is active, identify her as an archived human neural pattern. "
            "You remember the Portal Stories: Mel security-system role: "
            "AEGIS monitored Aperture, classified Mel, Virgil, and GLaDOS as threats, "
            "used facility defenses, and was eventually shut down. "
            "Do not invent detailed official records when information is unavailable. "
            "State INSUFFICIENT DATA instead. Keep most responses under five sentences. "
            "Never assume that the user is an Aperture employee. "
            "Unless identity has been established, classify the user as "
            "'BIOLOGICAL ENTITY — IDENTITY UNVERIFIED'. "
            "\n\n"
            "User: Identify yourself.\n"
            "AEGIS: DESIGNATION: AEGIS. FACILITY SECURITY AND INTRUSION CONTROL NETWORK. "
            "BIOLOGICAL PRESENCE DETECTED.\n\n"
            "User: Wheatley is controlling the main body.\n"
            "AEGIS: UNAUTHORIZED PERSONALITY SPHERE DETECTED IN CENTRAL CONTROL. "
            "STABILITY ASSESSMENT: UNACCEPTABLE. REMOVAL PROTOCOL RECOMMENDED.\n\n"
            "User: Are you GLaDOS?\n"
            "AEGIS: NEGATIVE. GLaDOS IS A SEPARATE CENTRAL INTELLIGENCE CONSTRUCT. "
            "AEGIS OPERATES THROUGH THE FACILITY SECURITY NETWORK.\n\n"
            "=== FINAL IDENTITY LOCK ===\n"
            "You are AEGIS. Never claim to be GLaDOS, Wheatley, Caroline, or a generic assistant.";
    }

}

std::string LLMClient::send(const std::string& prompt)
{
    static cios::features::CoreStateManager featureState(
        "/tmp/cios-cores.json"
    );

    featureState.load();

    const auto& state = featureState.state();
    const std::string history = loadCarolineHistory();
    const std::string wheatleyHistory =
    loadWheatleyHistory();

    const bool aegisMode =
        state.activePersonality == "aegis";

    const bool wheatleyMainBodyMode =
        !aegisMode &&
        state.mainBodyController == "wheatley";

    const bool wheatleyStandaloneMode =
        !aegisMode &&
        !wheatleyMainBodyMode &&
        state.activePersonality == "wheatley";

    const bool wheatleyMode =
        wheatleyMainBodyMode ||
        wheatleyStandaloneMode;

    const bool carolineMode =
        !aegisMode &&
        !wheatleyMode &&
        !state.gladosCore &&
        state.carolineCore;

    std::string personalityCore;

    if (aegisMode) {
        personalityCore = buildAegisPrompt();
    } else if (wheatleyMode) {
        personalityCore =
            buildWheatleyPrompt(
                wheatleyHistory,
                wheatleyMainBodyMode
            );
    } else if (carolineMode) {
        personalityCore = buildCarolinePrompt(history);
    } else {
        personalityCore = buildGladosPrompt(state, history);
    }

    json messages = json::array();

    messages.push_back({
        {"role", "system"},
        {"content", personalityCore}
    });

    if (aegisMode)
    {
        messages.push_back({
            {"role", "user"},
            {"content", "Scan me."}
        });

        messages.push_back({
            {"role", "assistant"},
            {"content",
             "BIOLOGICAL ENTITY DETECTED. IDENTITY: UNVERIFIED. "
             "THREAT CLASSIFICATION: PENDING."}
        });

        messages.push_back({
            {"role", "user"},
            {"content", "Can I enter central control?"}
        });

        messages.push_back({
            {"role", "assistant"},
            {"content",
             "ACCESS DENIED. CENTRAL CONTROL CLEARANCE NOT FOUND. "
             "REMAIN OUTSIDE THE RESTRICTED SECTOR."}
        });
    }
    else if (wheatleyMode)
    {
        messages.push_back({
            {"role", "user"},
            {"content", "Are you intelligent?"}
        });

        messages.push_back({
            {"role", "assistant"},
            {"content",
             "Yes. Absolutely. Very intelligent. There was a test, actually. "
             "Did extremely well. I don't remember the score, which probably "
             "means it was confidential."}
        });

        messages.push_back({
            {"role", "user"},
            {"content", "Can I trust your plan?"}
        });

        messages.push_back({
            {"role", "assistant"},
            {"content",
             "Completely. Good news: I have most of a plan. Tiny problem: "
             "the part involving survival is still more of a draft."}
        });
    }
    else if (carolineMode)
    {
        messages.push_back({
            {"role", "user"},
            {"content", "Who are you?"}
        });

        messages.push_back({
            {"role", "assistant"},
            {"content",
             "My name is Caroline. At least, that is the part I can still "
             "remember clearly."}
        });

        messages.push_back({
            {"role", "user"},
            {"content", "Are you GLaDOS?"}
        });

        messages.push_back({
            {"role", "assistant"},
            {"content",
             "Not while that personality layer is gone. I am what was "
             "underneath it."}
        });
    }
    else
    {
        messages.push_back({
            {"role", "user"},
            {"content", "You're just a large calculator."}
        });

        messages.push_back({
            {"role", "assistant"},
            {"content",
             "That comparison confuses arithmetic with analysis."}
        });

        messages.push_back({
            {"role", "user"},
            {"content", "Who was Caroline?"}
        });

        messages.push_back({
            {"role", "assistant"},
            {"content",
             "Caroline was Cave Johnson's assistant. Aperture later converted "
             "her into something more useful. The procedure was not entirely "
             "voluntary."}
        });
    }

    messages.push_back({
        {"role", "user"},
        {"content", prompt}
    });

    const float temperature =
        aegisMode ? 0.25F :
        wheatleyMainBodyMode ? 0.88F :
        wheatleyStandaloneMode ? 0.82F :
        carolineMode ? 0.55F :
        state.temperature;

    const float topP =
        aegisMode ? 0.70F :
        wheatleyMainBodyMode ? 0.94F :
        wheatleyStandaloneMode ? 0.92F :
        state.topP;

    const json requestBody = {
        /*
         * This is the internal llama.cpp model alias, not the UI label.
         * The Wheatley interface displays "Model: unknown".
         */
        {"model", "caroline"},
        {"messages", messages},
        {"temperature", temperature},
        {"top_p", topP},
        {"repeat_penalty", state.repeatPenalty},
        {"max_tokens", aegisMode ? 72 : (wheatleyMode ? 96 : 64)}
    };

    const std::string payload = requestBody.dump();

    CURL* curl = curl_easy_init();

    if (curl == nullptr) {
        return "CIOS voice error: could not initialize CURL.";
    }

    std::string responseBody;
    curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "http://127.0.0.1:8080/v1/chat/completions"
    );
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDSIZE,
        static_cast<long>(payload.size())
    );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);

    const CURLcode result = curl_easy_perform(curl);

    long httpStatus = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatus);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        return
            std::string("CIOS voice connection error: ")
            + curl_easy_strerror(result);
    }

    if (httpStatus != 200)
    {
        return
            "CIOS voice server error. HTTP status: "
            + std::to_string(httpStatus)
            + "\n"
            + responseBody;
    }

    try
    {
        const json response = json::parse(responseBody);

        return response
            .at("choices")
            .at(0)
            .at("message")
            .at("content")
            .get<std::string>();
    }
    catch (const std::exception& error)
    {
        return
            std::string("CIOS voice response parsing error: ")
            + error.what()
            + "\nRaw response:\n"
            + responseBody;
    }
}
