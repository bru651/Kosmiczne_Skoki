#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObjects.h"

class StateTracker {
public:
    bool pause = 1;         // Pause
    bool dragging = 0;      // Tells if a if control line is being dragged
    bool planetClick = 0;   // Checks if a planet got clicked
    sf::Vector2f lineStart = sf::Vector2f(0.f, 0.f);    // Start of rocket control line()
    unsigned int rockedID = 1;      // Next ID to assign
    unsigned int rockedComand = 0;  // Id of currently tracked rocked
    StateTracker() = default;
};

class GameManager {
private:
    sf::RenderWindow window;        // Graphical window
    sf::Vector2f center;            // Planets orbit this point
    std::vector<MovingCircle> circles;  // Contains planets and the sun
    std::vector<Rocket> rockets;        // Contains rockets
    StateTracker tracker;
    sf::Clock clock;    // For timekeeping

    void handleInput();         // Keyboard and mouse input
    void updateGame(float deltaTime);   // Updades everything
    void renderGame();          // Draws everything

public:
    GameManager();
    void run();     // Runs all GameManager functions
};
