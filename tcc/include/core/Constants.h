#pragma once
#include <array>
#include <string_view>
#include <cstdint>

namespace Config {
    constexpr std::array TERRAIN_COSTS = {
        1.0f,   // road
        5.0f,   // grass
        20.0f,  // mud
        100.0f, // water
        255.0f, // wall
        0.0f,   // tp
    };

    enum TerrainType : uint8_t {
        ROAD = 0,
        GRASS = 1,
        MUD = 2,
        WATER = 3,
        WALL = 4,
        TELEPORT = 5
      };

    constexpr int JITTER_STRENGTH = 6;
    constexpr float TP_ANIMATION_SPEED = 1.0f;
    constexpr int AMP = 50;
    constexpr int BASE = 10;
    constexpr float PI = 3.14159265f;

    namespace Window {
        constexpr std::string_view TITLE = "Pathfinding - GPU vs CPU";
        constexpr int WIDTH = 1280;
        constexpr int HEIGHT = 720;
        constexpr int TARGET_FPS = 60;
        constexpr bool VSYNC_ENABLED = false; // sync FPS with moinitor refresh rate,
                                            //can affect performance (OFF for benchmark)
    } // namespace Window

    namespace Simulation {
        constexpr int TILE_SIZE = 32;
        constexpr int GRID_WIDTH = Window::WIDTH /TILE_SIZE;
        constexpr int GRID_HEIGHT = Window::HEIGHT / TILE_SIZE;
    } // namespace Simulation
} // namespace Config
