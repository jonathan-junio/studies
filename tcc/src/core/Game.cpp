//
// Created by Jonathan Matos Nunes on 2/6/26.
//

#include "core/Game.h"
#include "core/Constants.h"
#include <iostream>

#include "imgui-SFML.h"
#include "imgui.h"
#include "core/Logger.h"

Game::Game()
    : m_window(sf::VideoMode({Config::Window::WIDTH, Config::Window::HEIGHT}), Config::Window::TITLE.data())
    , m_grid(Config::Simulation::GRID_WIDTH, Config::Simulation::GRID_HEIGHT)
{
    LOG_INFO(std::format("Window {}x{}@{}",
        Config::Window::WIDTH,
        Config::Window::HEIGHT,
        Config::Window::TARGET_FPS)
    );
    LOG_INFO(std::format("VSync {}", Config::Window::VSYNC_ENABLED? "ON": "OFF"));
    m_window.setFramerateLimit(Config::Window::TARGET_FPS);
    m_window.setVerticalSyncEnabled(Config::Window::VSYNC_ENABLED);

    if (!ImGui::SFML::Init(m_window)) {
        LOG_ERROR("Failed to initializa ImGUI");
    }

}

Game::~Game() {
    ImGui::SFML::Shutdown();
    LOG_INFO("Closing game");
}

void Game::run() {
    sf::Clock clock;
    sf::Clock animationClock;

    while (m_window.isOpen()) {
        sf::Time dt = clock.restart();

        processEvents();
        update(dt);
        m_grid.updateVisuals(animationClock.getElapsedTime().asSeconds());
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        ImGui::SFML::ProcessEvent(m_window, *event);
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
                case sf::Keyboard::Key::Escape:
                    m_window.close();
                    break;
                case sf::Keyboard::Key::P:
                    m_isPaused = !m_isPaused;
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::update(const sf::Time dt) {
    ImGui::SFML::Update(m_window, dt);
    ImGui::Begin("Terrain Tools");
    ImGui::Text("Select the terrain");

    const auto toolPtr = reinterpret_cast<int*>(&m_currentTool);

    ImGui::RadioButton("ROAD (COST 1)", toolPtr, Config::ROAD);
    ImGui::RadioButton("GRASS (COST 5)", toolPtr, Config::GRASS);
    ImGui::RadioButton("MUD (COST 20)", toolPtr, Config::MUD);
    ImGui::RadioButton("WATER (COST 100)", toolPtr, Config::WATER);
    ImGui::RadioButton("WALL (COST 255)", toolPtr, Config::WALL);
    ImGui::RadioButton("TELEPORT (COST 0)", toolPtr, Config::TELEPORT);

    if (!ImGui::GetIO().WantCaptureMouse && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        const sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
        sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos);

        m_grid.handleMouseClick(worldPos.x, worldPos.y, m_currentTool);
    }
    ImGui::End();

    if (m_isPaused) return;

    //TODO: update agentManager, update pathfinding
}

void Game::render() {
    m_window.clear(sf::Color::Black);
    m_window.draw(m_grid);

    //TODO: grid, agents, vectors
    ImGui::SFML::Render(m_window);

    m_window.display();
}
