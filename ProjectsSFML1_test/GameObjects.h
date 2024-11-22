//#ifndef GAMEOBJECTS_H
//#define GAMEOBJECTS_H
#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

// MovingCircle class
class MovingCircle {
public:
    sf::CircleShape shape;
    float angle;    // Angle in radians for the circle's position
    float radius;   // Radius from the center point
    float speed;    // Speed of rotation around the center
    float mass;     // Mass
    float size;

    MovingCircle(float radius, float angle, float speed, float size, sf::Color color, float mass);
    void update(float deltaTime, const sf::Vector2f& center);
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f calculateGravityForce(const sf::Vector2f& point) const;
    sf::Vector2f getLinearVelocity(sf::Vector2f center);
    bool isClicked(sf::Vector2f mousePosition);
};

// Rocket class
class Rocket {
public:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float tiltAngle;
    float mass;
    sf::CircleShape flame;
    float thrustPower; // Thrust
    unsigned int ID;   // IDs

    Rocket(float startX, float startY, float mass, unsigned int ID, sf::Vector2f initVelocity);
    void update(float deltaTime, const std::vector<sf::Vector2f>& forces);
    void draw(sf::RenderWindow& window) const;
    void setTiltAngle(const sf::Vector2f& start, const sf::Vector2f& target);
    bool checkCollision(const sf::CircleShape& circle);
};

//#endif // GAMEOBJECTS_H
