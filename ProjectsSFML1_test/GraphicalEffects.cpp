#include "GraphicalEffects.h"


// Function to draw a dotted circle
void GraphicalEffects::drawDottedCircle(sf::RenderWindow& window, const sf::Vector2f& center, float radius, float dotSpacing, float dotRadius, sf::Color color) {
    const float PI = 3.14159265359f;
    float circumference = 2 * PI * radius;  // Total circumference of the circle
    int numDots = static_cast<int>(circumference / dotSpacing);  // Number of dots to draw

    for (int i = 0; i < numDots; ++i) {
        // Calculate angle for the current dot
        float angle = (2 * PI / numDots) * i;

        // Calculate position of the dot
        float x = center.x + radius * std::cos(angle);
        float y = center.y + radius * std::sin(angle);

        // Create and position the dot
        sf::CircleShape dot(dotRadius);
        color.a = 155;  // Add transparency
        dot.setFillColor(color);
        dot.setPosition(x - dotRadius, y - dotRadius);  // Center the dot on the calculated position

        // Draw the dot
        window.draw(dot);
    }
}

// Explosion class implementation
Explosion::Explosion(const sf::Vector2f& position, float maxRadius, float duration, sf::Color color)
    : center(position), maxRadius(maxRadius), duration(duration), elapsedTime(0.f), color(color), currentRadius(0.f) {
    // Initialize the shape
    shape.setFillColor(sf::Color::Transparent);  // No fill
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(color);
}

bool Explosion::update(float deltaTime) {
    elapsedTime += deltaTime;

    // Calculate the current radius and transparency
    currentRadius = (elapsedTime / duration) * maxRadius;
    int alpha = static_cast<int>(255 * (1.f - (elapsedTime / duration)));  // Fades out over time
    if (alpha < 0) alpha = 0;

    // Update shape properties
    color.a = alpha;
    shape.setOutlineColor(color);
    shape.setRadius(currentRadius);
    shape.setOrigin(currentRadius, currentRadius);  // Center the circle
    shape.setPosition(center);

    // Return true if the explosion is finished
    return elapsedTime >= duration;
}

void Explosion::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}
