#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObjects.h"
#include "MissionManager.h"
#include "GraphicalEffects.h"

class StateTracker {
public:
    bool pause = 1;         // Pause
    bool dragging = 0;      // Tells if a if control line is being dragged
    bool planetClick = 0;   // Checks if a planet got clicked
    bool uiVisible = false; // UI visibility toggle
    bool uiHelpVisible = 1; // HelpUI visibility toggle
    sf::Vector2f lineStart = sf::Vector2f(0.f, 0.f);    // Start of rocket control line()
    unsigned int rockedID = 1;      // Next ID to assign
    unsigned int rockedComand = 0;  // Id of currently tracked rocked
    int destinationTracker = 0;     // Rockets destination
    bool rocketAlive = 0; // Is controled rocket operational
    StateTracker() = default;
};

class GameManager {
private:
    sf::RenderWindow window;        // Graphical window
    sf::Vector2f Resolution = sf::Vector2f(1920, 1080);
    sf::Vector2f cameraPosition = sf::Vector2f(0.f, 0.f); // Initialize to (0,0)
    float Zoom = 1.f;
    float BaseCameraSpeed = 10.f;
    float CameraSpeed = BaseCameraSpeed;
    sf::View view;
    sf::View Guiview;


    sf::Vector2f center;            // Planets orbit this point
    std::vector<MovingCircle> circles;  // Contains planets and the sun
    std::vector<Rocket> rockets;        // Contains rockets
    std::vector<mission> missions;      // Contains missions
    std::vector<Explosion> explosions;  // Contains explosions
    sf::Clock clock;    // For timekeeping
    StateTracker tracker;
    // Minimal and maximal lenghts for rocket control
    float controlMin = 25.f;
    float controlMax = 75.f;

    int playerMoney = 2000; // Player starts with 2000 money
    int income = 0; // Player income

    float payduration = 10.f;    // Time between regular time transfers
    float paytracker = payduration;    // Time tracker

    // UI Elements
    sf::Font font;
    sf::RectangleShape uiPanel;
    std::vector<std::unique_ptr<sf::Text>> missionTexts; // Dynamic allocation
    std::vector<std::unique_ptr<sf::RectangleShape>> startButtons;
    std::vector<std::unique_ptr<sf::Sprite>> payloadIcons; // Add icons for mission payloads

    void handleInput();         // Keyboard and mouse input
    void updateGame(float deltaTime);   // Updades everything
    void renderGame();          // Draws everything
    void renderUI(); // Render the mission UI
    void renderUI_Help(); // Render the help UI
    sf::Vector2f GetCameraMousePosition(const sf::Vector2f mousePosition);


    void generateMissions(); // Generate missions
    void handleMissionStart(int missionIndex); // Start a mission

    // To handle mission completion logic
    //void checkMissionCompletion(); 
    void checkMissionCompletionSingular(Rocket& rocket, MovingCircle& circle);
    void checkIncome();

public:
    GameManager();
    void run();     // Runs all GameManager functions
};
