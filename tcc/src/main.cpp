#include <SFML/Graphics.hpp>
#include <format>
#include "core/Game.h"
#include "core/Logger.h"

int main() {

    Logger::setLevel(LogLevel::DEBUG);
    LOG_INFO("System started");

    LOG_DEBUG("Initializing simulation");
    try {
        Game app;
        LOG_DEBUG("Game created");
        app.run();
    } catch (const std::exception& e) {
        LOG_ERROR(std::format("CRITICAL: {}", e.what()));
    }

    return 0;
}
