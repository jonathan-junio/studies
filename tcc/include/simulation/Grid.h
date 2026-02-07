//
// Created by Jonathan Matos Nunes on 2/6/26.
//

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "core/Constants.h"

class Grid : public sf::Drawable {
public:
    Grid(int tileWidth, int tileHeight);
    bool handleMouseClick(float mouseX, float mouseY, Config::TerrainType newType);
    void reset();
    void updateVisuals(float ts);

private:
    int m_tileWidth;
    int m_tileHeight;
    sf::VertexArray m_terrainData;
    std::vector<Config::TerrainType> m_tileType;
    std::vector<int> m_tileJitter;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    static sf::Color getColor(Config::TerrainType type) ;
    bool setTile(int x, int y, Config::TerrainType newType);

};