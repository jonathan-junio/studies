//
// Created by Jonathan Matos Nunes on 2/6/26.
//

#pragma once
#include <SFML/Graphics.hpp>

#include "simulation/Grid.h"

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    sf::RenderWindow m_window;
    bool m_isPaused = false;
    Grid m_grid;
    Config::TerrainType m_currentTool = Config::ROAD;
    sf::Clock m_imGuiClock;
    // TODO: agentManager

private:
    void processEvents();
    void update(sf::Time dt);
    void render();
};
