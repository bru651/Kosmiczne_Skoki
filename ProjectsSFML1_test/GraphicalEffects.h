#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>  // For trigonometric functions

namespace GraphicalEffects {
    void drawDottedCircle(sf::RenderWindow& window, const sf::Vector2f& center, float radius, float dotSpacing, float dotRadius, sf::Color color = sf::Color::White);
}

class Explosion {
private:
    sf::Vector2f center;        // Center of the explosion
    float maxRadius;            // Maximum radius of the explosion
    float duration;             // Total duration of the explosion
    float elapsedTime;          // Elapsed time since the explosion started
    sf::Color color;            // Color of the explosion
    float currentRadius;        // Current radius of the explosion
    sf::CircleShape shape;      // SFML circle shape to represent the explosion

public:
    Explosion(const sf::Vector2f& position, float maxRadius, float duration, sf::Color color = sf::Color::Red);

    // Update the explosion. Returns true if the explosion has finished
    bool update(float deltaTime);

    // Draw the explosion
    void draw(sf::RenderWindow& window) const;
};
