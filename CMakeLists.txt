cmake_minimum_required(VERSION 3.20)

project(CIOS)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    cios
    commands/shell.cpp
    src/core_state.cpp
    src/terminal_fx.cpp
    src/resource_monitor.cpp
    src/glados_hooks.cpp
    src/lore_commands.cpp
    src/module_loader.cpp
    src/main.cpp
    src/cios.cpp
    src/terminal.cpp
    src/boot.cpp
    src/config.cpp
    src/module.cpp
    src/system_state.cpp
    src/command_manager.cpp
    src/command_registry.cpp
    src/module_manager.cpp
    src/caroline/caroline.cpp
    src/ai/ai_engine.cpp
    src/ai/ai_manager.cpp
    src/ai/llm_client.cpp
    src/tts/tts_client.cpp

    commands/start.cpp
    commands/caroline.cpp
    commands/wheatley.cpp
    commands/aegis.cpp
    commands/help.cpp
    commands/status.cpp
    commands/model.cpp
    commands/modules.cpp
    commands/hardware.cpp

    hardware/hardware.cpp
    hardware/gpu_parser.cpp
    hardware/cpu_parser.cpp
    hardware/profile.cpp
)

add_executable(
    cios-init
    src/init/cios_init.cpp
)

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(
    cios
    PRIVATE
    CURL::libcurl
    Threads::Threads
)

target_include_directories(
    cios-init
    PRIVATE
    include
)

target_include_directories(
    cios
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    include
    commands
    src
)
