#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObjects.h"
#include "MissionManager.h"

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
    std::vector<mission> missions;
    sf::Clock clock;    // For timekeeping
    StateTracker tracker;

    bool uiVisible = false; // UI visibility toggle

    // UI Elements
    sf::Font font;
    sf::RectangleShape uiPanel;
    std::vector<std::unique_ptr<sf::Text>> missionTexts; // Dynamic allocation
    std::vector<std::unique_ptr<sf::RectangleShape>> startButtons;

    void handleInput();         // Keyboard and mouse input
    void updateGame(float deltaTime);   // Updades everything
    void renderGame();          // Draws everything
    void renderUI(); // Render the mission UI

    void generateMissions(); // Generate missions
    void handleMissionStart(int missionIndex); // Start a mission

    int playerMoney = 1000; // Player starts with 1000 money
    void checkMissionCompletion(); // To handle mission completion logic

public:
    GameManager();
    void run();     // Runs all GameManager functions
};
