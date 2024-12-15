//#include <SFML/Graphics.hpp>
//#include <cmath>
//#include <vector>
#include "GameManager.h"
#include <iostream>
#include <cmath>

GameManager::GameManager()
    : window(sf::VideoMode(1920, 1080), "Skoki prototyp", sf::Style::Fullscreen),
    center(1920 / 2, 1080 / 2) {
    window.setFramerateLimit(60);
    int circleID = 1;

    // Initialize circles
    for (int i = 0; i < 5; ++i) {
        float radius = 100 + i * 85;
        float angle = i;
        float speed = 50.0f / radius;
        float size = 25.0f + i * 2.0f;
        float mass = size * size;
        circles.emplace_back(radius, angle, speed, size, sf::Color::Green, mass, circleID++, 1);
    }

    // Add Sun
    circles.emplace_back(0, 0, 0.0f, 35, sf::Color::Yellow, 35 * 35, 0, 0);

    // Load font
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    // Initialize UI panel
    uiPanel.setSize(sf::Vector2f(800, 600));
    uiPanel.setFillColor(sf::Color(50, 50, 50, 200));
    uiPanel.setPosition(560, 240); // Centered position

    // Generate missions
    generateMissions();
}

void GameManager::generateMissions() {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::cout << "Func: generateMissions" << std::endl;
    missionTexts.clear();
    startButtons.clear();
    for (int i = 0; i < 4; ++i) {
        missions.push_back(generateRandomMission(i + 1));
    }

    // Create UI elements for missions
    missionTexts.clear();
    startButtons.clear();

    for (size_t i = 0; i < missions.size(); ++i) {
        // Mission text
        auto text = std::make_unique<sf::Text>();
        text->setFont(font);
        text->setCharacterSize(20);
        text->setFillColor(sf::Color::White);
        text->setString(
            "Mission " + std::to_string(missions[i].id) + ":\n" +
            "Name: " + missions[i].missionPayload.name + "\n" +
            "Mass: " + std::to_string(missions[i].missionPayload.mass) + "\n" +
            "Cost: " + std::to_string(missions[i].missionPayload.cost) + "\n" +
            "Reward: " + std::to_string(missions[i].missionPayload.reward) + "\n" +
            "Destination: " + std::to_string(missions[i].destination)
        );
        text->setPosition(580, 260 + i * 140); // Adjust position for each mission
        missionTexts.push_back(std::move(text));

        // Start button
        auto button = std::make_unique<sf::RectangleShape>(sf::Vector2f(100, 40));
        button->setFillColor(sf::Color::Green);
        button->setPosition(1260, 260 + i * 140);
        startButtons.push_back(std::move(button));
   }
}


void GameManager::handleInput() {
    //std::cout << "Func: handleInput" << std::endl;
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) { // Mouse button
            std::cout << "Input: click" << std::endl;
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (event.mouseButton.button == sf::Mouse::Left) {  // Left click
                tracker.planetClick = false;

                // Check if a button is clicked
                for (size_t i = 0; i < startButtons.size(); ++i) {
                    if (startButtons[i]->getGlobalBounds().contains(mousePos)) {
                        handleMissionStart(i);
                        break;
                    }
                }

                // Check planet clicks
                for (auto& planet : circles) {
                    if (planet.isClicked(mousePos)) { // Clicked planet spawns rocket
                        tracker.planetClick = true;
                        sf::Vector2f linearVelocity = planet.getLinearVelocity(center);
                        sf::Vector2f spawnPosition = planet.shape.getPosition() +
                            sf::Vector2f(std::cos(planet.angle), std::sin(planet.angle)) * (planet.size * 2.5f);

                        // Create a default mission for the rocket
                        mission defaultMission(tracker.rockedID,
                            payload(0, 1, sf::Image(), 0, 0, "Default Payload"),
                            0); // No destination set

                        // Spawn the rocket with the default mission
                        rockets.emplace_back(spawnPosition.x, spawnPosition.y, 1.f, tracker.rockedID, linearVelocity, defaultMission);
                        tracker.rockedID++; // Iterate ID
                        break;
                    }
                }


                // Check rocket clicks
                if (!tracker.planetClick) {
                    tracker.rockedComand = 0;
                    float minDistance = 15;
                    for (const auto& rocket : rockets) {
                        float distance = std::hypot(
                            rocket.shape.getPosition().x - mousePos.x,
                            rocket.shape.getPosition().y - mousePos.y
                        );
                        if (distance < minDistance) {   //Closest rocked within minimal distance gets tracked
                            tracker.rockedComand = rocket.ID;
                            break;
                        }
                    }
                }
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {    // Right click
                if (!tracker.dragging) {    // If not dragging then start dragging
                    tracker.lineStart = mousePos;
                }
                else {  // Line orientation determines direction of a tracked rocket
                    sf::Vector2f lineEnd = sf::Vector2f(sf::Mouse::getPosition(window));
                    //sf::Vector2f lineEnd = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    for (auto& rocket : rockets) {
                        if (rocket.ID == tracker.rockedComand) {
                            float distance = std::hypot(lineEnd.x - tracker.lineStart.x, lineEnd.y - tracker.lineStart.y);
                            rocket.thrustMultiplier = (distance - controlMin)/controlMax;
                            rocket.thrustMultiplier = std::min(std::max(rocket.thrustMultiplier, 0.f), 1.f);
                            rocket.setTiltAngle(tracker.lineStart, lineEnd);
                        }
                    }
                }
                tracker.dragging = !tracker.dragging;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {    // Keyboard input
            if (event.key.code == sf::Keyboard::Space) {    // Pause button (Space)
                std::cout << "Input: Space" << std::endl;
                tracker.pause = !tracker.pause;
            }
            if (event.key.code == sf::Keyboard::Escape) {   // Exit game (Esc)
                std::cout << "Input: Esc" << std::endl;
                window.close();
            }
            if (event.key.code == sf::Keyboard::M) {        // Toggle UI visibility
                std::cout << "Input: M" << std::endl;
                tracker.uiVisible = !tracker.uiVisible;
            } 
            if (event.key.code == sf::Keyboard::H) {        // Toggle help UI visibility
                std::cout << "Input: H" << std::endl;
                tracker.uiHelpVisible = !tracker.uiHelpVisible;
            }
        }
    }
}

void GameManager::updateGame(float deltaTime) {
    //std::cout << "Func: updateGame" << std::endl;
    if (!tracker.pause) {   // Only if unpaused

        paytracker -= deltaTime;
        if (paytracker < 0) {
            playerMoney += income;
            paytracker = payduration;
        }

        for (auto& circle : circles) {  // Update planets and the sun
            circle.update(deltaTime, center);
        }

        for (auto it = explosions.begin(); it != explosions.end();) {   // updatd explosions
            if (it->update(deltaTime)) {  // If explosion is finished
                it = explosions.erase(it);
            }
            else {
                it->draw(window);
                ++it;
            }
        }

        for (size_t i = 0; i < rockets.size();) {   // Updade rockets
            std::vector<sf::Vector2f> forces;
            for (const auto& circle : circles) {    // Gravity force on applied rockets
                forces.push_back(circle.calculateGravityForce(rockets[i].position));
            }

            

            rockets[i].update(deltaTime, forces);   // Apply gravity

            //checkMissionCompletion(); // Check for completed or failed missions

            bool collided = false;
            for (auto& circle : circles) {    // Check for colision
                if (rockets[i].checkCollision(circle.shape)) {
                    collided = true;
                    // Calculate the angle of collision
                    sf::Vector2f collisionPoint = rockets[i].shape.getPosition();
                    sf::Vector2f toCollision = collisionPoint - circle.shape.getPosition();
                    float angle = std::atan2(toCollision.y, toCollision.x);

                    // Add a building to the planet at the collision angle
                    //circle.addBuilding(rockets[i].associatedMission.missionPayload.name, angle);
                    if(!circle.checkAndDestroyBuilding(angle, circle.size)) {
                        // If no building destroyed, add a new one
                        checkMissionCompletionSingular(rockets[i], circle); // Check for completed or failed missions
                        if (circle.walkable) { circle.addBuilding(rockets[i].associatedMission.missionPayload.name, angle); }
                    }
                    else{ explosions.emplace_back(rockets[i].position, 25.f, 1.f, sf::Color::Red); }    // Explosion
                    checkIncome();
                    break;
                }
            }

            if (collided) { // Erase if collided

                //explosions.emplace_back(rockets[i].position, 25.f, 1.f, sf::Color::Red);
                rockets.erase(rockets.begin() + i); // Remove rocket
            }
            else {
                ++i;
            }
        }
    }
}

void GameManager::renderGame() {
    //std::cout << "Func: renderGame" << std::endl;
    window.clear();

    tracker.rocketAlive = 0;
    
    for (const auto& rocket : rockets) { // Draw rockets
        if (rocket.ID == tracker.rockedComand) { 
            GraphicalEffects::drawDottedCircle(window, rocket.position, 15, 4.f, 1.f, sf::Color::Magenta);
            tracker.rocketAlive = 1;
            tracker.destinationTracker = rocket.associatedMission.destination;
        }
        rocket.draw(window);
    }

    for (const auto& circle : circles) { // Draw planets and the sun
       if ((tracker.rocketAlive == 1) && (circle.id == tracker.destinationTracker)) {
           GraphicalEffects::drawDottedCircle(window, circle.position, circle.size + 20, 12.f, 3.f, sf::Color::Magenta); }
        circle.draw(window);
    }


    for (const auto& explosion : explosions) { // Draw rockets
        explosion.draw(window);
    }

    if (tracker.dragging) {     // If dragging control line
        sf::Vector2f endPosition = sf::Vector2f(sf::Mouse::getPosition(window));
        //sf::Vector2f endPosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        float distance = std::hypot(endPosition.x - tracker.lineStart.x, endPosition.y - tracker.lineStart.y);
        sf::Vector2f direction = (endPosition - tracker.lineStart) / distance;

        GraphicalEffects::drawDottedCircle(window, tracker.lineStart, controlMin, 10.f, 2.f);

        GraphicalEffects::drawDottedCircle(window, tracker.lineStart, controlMax, 10.f, 2.f);

        for (float i = 0; i < distance; i += 10.f) {    // Draw dotted line
            sf::CircleShape dot(2.f);
            dot.setFillColor(sf::Color::White);
            dot.setPosition(tracker.lineStart + direction * i);
            window.draw(dot);
        }

        
    }

    // Render UI if visible
    if (tracker.uiVisible) {
        renderUI();
    }
    // Render help UI if visible
    if (tracker.uiHelpVisible) {
        renderUI_Help();
    }

    window.display();
}

void GameManager::run() {   // Runs the game
    while (window.isOpen()) {
        handleInput();
        float deltaTime = clock.restart().asSeconds();
        updateGame(deltaTime);
        renderGame();
    }
}

void GameManager::handleMissionStart(int missionIndex) {
    std::cout << "Func: handleMissionStart" << std::endl;
    if (missionIndex < 0 || missionIndex >= missions.size()) {
        std::cerr << "Invalid mission index\n";
        return;
    }

    mission& selectedMission = missions[missionIndex];

    // Check if the player has enough money
    if (playerMoney < selectedMission.missionPayload.cost) {
        std::cout << "Not enough money to start this mission.\n";
        return;
    }

    // Find the planet with ID=3
    auto it = std::find_if(circles.begin(), circles.end(), [](const MovingCircle& circle) { return circle.id == 3; });
    if (it == circles.end()) {
        std::cerr << "Planet with ID 3 not found.\n";
        return;
    }
    MovingCircle& targetPlanet = *it;

    // Spawn a rocket near the planet
    sf::Vector2f spawnPosition = targetPlanet.shape.getPosition() +
        sf::Vector2f(std::cos(targetPlanet.angle), std::sin(targetPlanet.angle)) * (targetPlanet.size * 2.5f);
    sf::Vector2f linearVelocity = targetPlanet.getLinearVelocity(center);

    // Ensure mission is valid
    if (!selectedMission.missionPayload.name.empty()) {
        rockets.emplace_back(spawnPosition.x, spawnPosition.y, 1.f + selectedMission.missionPayload.mass, tracker.rockedID++, linearVelocity, selectedMission);
    }
    else {
        std::cerr << "Mission payload is not valid!\n";
        return;
    }

    // Deduct money from the player
    playerMoney -= selectedMission.missionPayload.cost;
    std::cout << "Mission started. Remaining money: " << playerMoney << "\n";

    // Remove the mission from available missions
    missions.erase(missions.begin() + missionIndex);
}


/*void GameManager::checkMissionCompletion() {
    //std::cout << "Func: checkMissionCompletion" << std::endl;
    for (size_t i = 0; i < rockets.size();) {
        Rocket& rocket = rockets[i];
        bool completed = false;
        bool collided = false;

        // Check for collision with planets
        for (const auto& circle : circles) {
            if (rocket.checkCollision(circle.shape)) {
                std::cout << "PlanetID:" << circle.id << std::endl;
                std::cout << "MissionID:" << rocket.associatedMission.destination << std::endl;
                if (circle.id == rocket.associatedMission.destination) {
                    // Mission completed
                    playerMoney += rocket.associatedMission.missionPayload.reward;
                    std::cout << "Mission completed! Reward: " << rocket.associatedMission.missionPayload.reward
                        << " Remaining money: " << playerMoney << "\n";
                    completed = true;
                }
                collided = true;
                break;
            }
        }

        i++;
    }
}*/

void GameManager::checkMissionCompletionSingular(Rocket& rocket, MovingCircle& circle) {

    if (circle.id == rocket.associatedMission.destination) {
        // Mission completed
        playerMoney += rocket.associatedMission.missionPayload.reward;
        std::cout << "Mission completed! Reward: " << rocket.associatedMission.missionPayload.reward
            << " Remaining money: " << playerMoney << "\n";
    }
}


void GameManager::renderUI() {
    //std::cout << "Func: renderUI" << std::endl;
    window.draw(uiPanel);

    sf::Text moneyText;
    moneyText.setFont(font);
    moneyText.setString("Money: " + std::to_string(playerMoney) + " + " + std::to_string(income));
    moneyText.setCharacterSize(20);
    moneyText.setFillColor(sf::Color::White);
    moneyText.setPosition(580, 200);
    window.draw(moneyText);

    // Draw mission texts
    for (const auto& text : missionTexts) {
        if (text) {
            window.draw(*text); // Safely dereference
        }
    }

    // Draw buttons
    for (const auto& button : startButtons) {
        if (button) {
            window.draw(*button); // Safely dereference
        }
    }
}

void GameManager::renderUI_Help() {
    //std::cout << "Func: renderUI" << std::endl;
    window.draw(uiPanel);

    sf::Text text;
    text.setFont(font);
    text.setString(
        "Press 'H' to toggle this panel\n"
        "Press 'M' to toggle mission panel\n"
        "Press 'Space' to toggle pause\n"
        "Press 'Escape' to exit game\n"
        "Deliver rocket to assigned planet to recive funds\n"
        "Left click to select rocket to control\n"
        "Right click to control rocket");
    text.setCharacterSize(35);
    text.setFillColor(sf::Color::White);
    text.setPosition(580, 250);
    window.draw(text);

}

void  GameManager::checkIncome() {
    std::cout << "Counting Income" << std::endl;
    int bval = 25;
    int inc = bval;
    for (const auto& circle : circles) {
        inc += circle.buildings.size() * bval;
    }
    income = inc;
}