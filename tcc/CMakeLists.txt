cmake_minimum_required(VERSION 3.25)

# ----------------------------
# Projeto
# ----------------------------
project(TCC_Pathfinding VERSION 0.1 LANGUAGES CXX)

# C++20 (mais "moderno CMake": preferível usar features)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Útil pra clangd, VSCode, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ajuda em FetchContent com URLs/timestamps (CMake 3.24+)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

# ----------------------------
# SFML 3.0 (Fetch)
# ----------------------------
# Otimização: usa deps do sistema (Homebrew) se disponível
set(SFML_USE_SYSTEM_DEPS ON CACHE BOOL "Use system dependencies" FORCE)

# Deixa o SFML mais rápido de compilar (opcional)
set(SFML_BUILD_AUDIO    OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK  OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC      OFF CACHE BOOL "" FORCE)

FetchContent_Declare(sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.0
)
FetchContent_MakeAvailable(sfml)

# ----------------------------
# Dear ImGui (Fetch - PIN FIXO compatível com imgui-sfml)
# ----------------------------
# NÃO use "docking" sem tag (isso é head -> quebra do nada).
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.91.1-docking
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

# ----------------------------
# ImGui-SFML (Fetch)
# ----------------------------
# Usando nome com underscore pra evitar dor de cabeça (melhor prática)
FetchContent_Declare(imgui_sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        # Ideal seria fixar numa tag/commit. Se a tag v3.0 existir no seu momento, troque.
        # GIT_TAG        v3.0
        GIT_TAG        master
)
FetchContent_GetProperties(imgui_sfml)
if(NOT imgui_sfml_POPULATED)
    FetchContent_Populate(imgui_sfml)
endif()

# ----------------------------
# Fontes do projeto + ImGui embutido no executável
# ----------------------------
set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp

        # binding do imgui-sfml (arquivo principal)
        ${imgui_sfml_SOURCE_DIR}/imgui-SFML.cpp
)

set(PROJECT_SOURCES
        src/main.cpp
        src/core/Game.cpp
        src/simulation/Grid.cpp
)

add_executable(PathfindingBenchmark
        ${PROJECT_SOURCES}
        ${IMGUI_SOURCES}
)

# ----------------------------
# Includes
# ----------------------------
target_include_directories(PathfindingBenchmark
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}
        ${imgui_sfml_SOURCE_DIR}
)

# ----------------------------
# Configuração "mágica" do imgui-sfml
# ----------------------------
# Ponto importante: usar caminho absoluto e com escape "CMake style"
# (Em macOS/linux isso é tranquilo; em Windows também ajuda.)
set(IMGUI_USER_CONFIG_PATH "${imgui_sfml_SOURCE_DIR}/imconfig-SFML.h")
target_compile_definitions(PathfindingBenchmark
        PRIVATE
        IMGUI_USER_CONFIG="${IMGUI_USER_CONFIG_PATH}"
)

# ----------------------------
# Linkagem
# ----------------------------
# Em SFML 3, os targets normalmente são SFML::Graphics etc.
target_link_libraries(PathfindingBenchmark
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

# ----------------------------
# Warnings / flags
# ----------------------------
if(MSVC)
    target_compile_options(PathfindingBenchmark PRIVATE /W4 /permissive-)
else()
    target_compile_options(PathfindingBenchmark PRIVATE
            -Wall -Wextra -Wpedantic
            # Suprime warnings do ImGui para não poluir
            -Wno-unused-function
            -Wno-macro-redefined
    )

    # Otimizações no Release
    target_compile_options(PathfindingBenchmark PRIVATE
            $<$<CONFIG:Release>:-O3>
            $<$<CONFIG:Release>:-march=native>
            $<$<CONFIG:Release>:-ffast-math>
    )
endif()

# ----------------------------
# Windows: copiar DLLs após build
# ----------------------------
if(WIN32)
    add_custom_command(TARGET PathfindingBenchmark POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:PathfindingBenchmark>
            $<TARGET_FILE_DIR:PathfindingBenchmark>
            COMMAND_EXPAND_LISTS
    )
endif()
