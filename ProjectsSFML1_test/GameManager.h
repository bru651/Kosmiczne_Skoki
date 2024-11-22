#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObjects.h"

class StateTracker {
public:
    bool pause = 1;
    bool dragging = 0;
    bool planetClick = 0;
    sf::Vector2f lineStart = sf::Vector2f(0.f, 0.f);
    unsigned int rockedID = 1;
    unsigned int rockedComand = 0;
    StateTracker() = default;
};

class GameManager {
private:
    sf::RenderWindow window;
    sf::Vector2f center;
    std::vector<MovingCircle> circles;
    std::vector<Rocket> rockets;
    StateTracker tracker;
    sf::Clock clock;

    void handleInput();
    void updateGame(float deltaTime);
    void renderGame();

public:
    GameManager();
    void run();
};
