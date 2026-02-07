//
// Created by Jonathan Matos Nunes on 2/6/26.
//
#include <cmath>
#include "simulation/Grid.h"

#include "core/Logger.h"

static uint32_t hash2D(const uint32_t x, const uint32_t y) {
    uint32_t h = x * 0x9E3779B1u ^ y * 0x85EBCA6Bu;
    h ^= h >> 16;
    h *= 0x7FEB352Du;
    h ^= h >> 15;
    h *= 0x846CA68Bu;
    h ^= h >> 16;
    return h;
}

static uint8_t clampByte(const int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return static_cast<uint8_t>(v);
}

static sf::Color applyJitter(const sf::Color& c, const int j) {
    return {
        clampByte(static_cast<int>(c.r) + j),
        clampByte(static_cast<int>(c.g) + j),
        clampByte(static_cast<int>(c.b) + j),
        c.a
    };
}


Grid::Grid(const int tileWidth, const int tileHeight)
    : m_tileWidth(tileWidth), m_tileHeight(tileHeight)
{
    const int size = m_tileWidth * m_tileHeight;
    m_terrainData.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_terrainData.resize(size * 6); // two triangles, six vertex
    m_tileType.resize(size, Config::ROAD);
    m_tileJitter.resize(size, 0);
    LOG_INFO(std::format("Grid initialized"));
    reset();
}

void Grid::reset() {
    const auto size = static_cast<float>(Config::Simulation::TILE_SIZE);
    constexpr float padding = 1.0f;

    for (int y = 0; y < m_tileHeight; ++y) {
        for (int x = 0; x < m_tileWidth; ++x) {
            // position (x, y)
            float px = x * size;
            float py = y * size;

            float x1 = px + padding;
            float y1 = py + padding;
            float x2 = px + size - padding;
            float y2 = py + size - padding;

            // i vertex
            const size_t i = (x + y * m_tileWidth) * 6;

            // positions
            // t1
            m_terrainData[i + 0].position = {x1, y1};
            m_terrainData[i + 1].position = {x2, y1};
            m_terrainData[i + 2].position = {x1, y2};

            // t2
            m_terrainData[i + 3].position = {x2, y1};
            m_terrainData[i + 4].position = {x2, y2};
            m_terrainData[i + 5].position = {x1, y2};

            Config::TerrainType type = Config::GRASS;
            const int tileIndex = x + y * m_tileWidth;
            const uint32_t h = hash2D(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
            m_tileType[tileIndex] = type;
            m_tileJitter[tileIndex] =
                static_cast<int>(h % (2 * Config::JITTER_STRENGTH + 1)) - Config::JITTER_STRENGTH;
            const sf::Color color = applyJitter(getColor(type), m_tileJitter[tileIndex]);
            for (int k = 0; k < 6; ++k)
                m_terrainData[i + k].color = color;
        }
    }
}

bool Grid::setTile(const int x, const int y, const Config::TerrainType newType) {
    // memory safeness
    if (x < 0 || x >= m_tileWidth || y < 0 || y >= m_tileHeight) {
        LOG_ERROR("Terrain not changed - Tile out of bounds");
        return  false;
    }
    const int tileIndex = x + y * m_tileWidth;
    m_tileType[tileIndex] = newType;
    sf::Color color = getColor(newType);
    color = applyJitter(color, m_tileJitter[tileIndex]);
    const std::size_t base = static_cast<std::size_t>(x + y * m_tileWidth) * 6;

    if (m_terrainData[base].color == color) {
        return false;
    }
    for (int k = 0; k < 6; ++k) {
        m_terrainData[base + k].color = color;
    }
    return true;
}

sf::Color Grid::getColor(const Config::TerrainType type) {
    switch (type) {
        case Config::ROAD:     return {0xC2, 0xC3, 0xC7};
        case Config::GRASS:    return {0x00, 0x87, 0x51};
        case Config::MUD:      return {0xAB, 0x52, 0x36};
        case Config::WATER:    return {0x1D, 0x2B, 0x53};
        case Config::WALL:     return {0x29, 0x2A, 0x2D};
        case Config::TELEPORT: return {0x83, 0x76, 0x9C};
        default:               return sf::Color::Magenta; // debug
    }
}

void Grid::draw(sf::RenderTarget &target, const sf::RenderStates states) const {
    target.draw(m_terrainData, states);
}

bool Grid::handleMouseClick(const float mouseX, const float mouseY, Config::TerrainType newType) {
    const int x = static_cast<int>(mouseX) / Config::Simulation::TILE_SIZE;
    const int y = static_cast<int>(mouseY) / Config::Simulation::TILE_SIZE;
    setTile(x, y, newType);
    return true;
}

void Grid::updateVisuals(const float ts) {
    LOG_DEBUG(std::format("TS: {}", ts));
    const float fSinusoidal = 0.5f + 0.5f * std::sin(ts * Config::TP_ANIMATION_SPEED * 2.0f * Config::PI);
    for (int y = 0; y < m_tileHeight; ++y) {
        for (int x = 0; x < m_tileWidth; ++x) {
            const int tileIndex = x + y * m_tileWidth;
            if (m_tileType[tileIndex] != Config::TELEPORT) continue;

            const std::size_t base = static_cast<std::size_t>(tileIndex) * 6;
            sf::Color c = applyJitter(getColor(Config::TELEPORT), m_tileJitter[tileIndex]);
            const int glow = Config::BASE + static_cast<int>(Config::AMP * fSinusoidal);
            LOG_DEBUG(std::format("Aplling glow: {}", glow));

            c.r = clampByte(static_cast<int>(c.r) + glow);
            c.g = clampByte(static_cast<int>(c.g) + glow);
            c.b = clampByte(static_cast<int>(c.b) + glow);

            for (int k = 0; k < 6; ++k) {
                m_terrainData[base + k].color = c;
            }
        }
    }
}
