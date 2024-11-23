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

    // Initialize circles
    for (int i = 0; i < 5; ++i) {
        float radius = 100 + i * 85;
        float angle = i;
        float speed = 50.0f / radius;
        float size = 25.0f + i * 2.0f;
        float mass = size * size;
        circles.emplace_back(radius, angle, speed, size, sf::Color::Green, mass);
    }

    // Add Sun
    circles.emplace_back(0, 0, 0.0f, 35, sf::Color::Yellow, 35 * 35);
}

void GameManager::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) { // Mouse button
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (event.mouseButton.button == sf::Mouse::Left) {  // Left click
                tracker.planetClick = false;

                // Check planet clicks
                for (auto& planet : circles) {
                    if (planet.isClicked(mousePos)) { // Clicked planet spawns rocket
                        tracker.planetClick = true;
                        sf::Vector2f linearVelocity = planet.getLinearVelocity(center);
                        sf::Vector2f spawnPosition = planet.shape.getPosition() +
                            sf::Vector2f(std::cos(planet.angle), std::sin(planet.angle)) * (planet.size * 2.5f);

                        rockets.emplace_back(spawnPosition.x, spawnPosition.y, 1.f, tracker.rockedID, linearVelocity);
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
                    for (auto& rocket : rockets) {
                        if (rocket.ID == tracker.rockedComand) {
                            rocket.setTiltAngle(tracker.lineStart, lineEnd);
                        }
                    }
                }
                tracker.dragging = !tracker.dragging;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {    // Keyboard input
            if (event.key.code == sf::Keyboard::Space) {    // Pause button (Space)
                tracker.pause = !tracker.pause;
            }
            if (event.key.code == sf::Keyboard::Escape) {   // Exit game (Esc)
                window.close();
            }
        }
    }
}

void GameManager::updateGame(float deltaTime) {
    if (!tracker.pause) {   // Only if unpaused
        for (auto& circle : circles) {  // Update planets and the sun
            circle.update(deltaTime, center);
        }

        for (size_t i = 0; i < rockets.size();) {   // Updade rockets
            std::vector<sf::Vector2f> forces;
            for (const auto& circle : circles) {    // Gravity force on applied rockets
                forces.push_back(circle.calculateGravityForce(rockets[i].position));
            }

            rockets[i].update(deltaTime, forces);   // Apply gravity

            bool collided = false;
            for (const auto& circle : circles) {    // Check for colision
                if (rockets[i].checkCollision(circle.shape)) {
                    collided = true;
                    break;
                }
            }

            if (collided) { // Erase if collided
                rockets.erase(rockets.begin() + i);
            }
            else {
                ++i;
            }
        }
    }
}

void GameManager::renderGame() {
    window.clear();

    for (const auto& circle : circles) { // Draw planets and the sun
        //window.draw(circle.shape);
        circle.draw(window);
    }

    for (const auto& rocket : rockets) { // Draw rockets
        rocket.draw(window);
    }

    if (tracker.dragging) {     // If dragging control line
        sf::Vector2f endPosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        float distance = std::hypot(endPosition.x - tracker.lineStart.x, endPosition.y - tracker.lineStart.y);
        sf::Vector2f direction = (endPosition - tracker.lineStart) / distance;

        for (float i = 0; i < distance; i += 10.f) {    // Draw dotted line
            sf::CircleShape dot(2.f);
            dot.setFillColor(sf::Color::White);
            dot.setPosition(tracker.lineStart + direction * i);
            window.draw(dot);
        }
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
