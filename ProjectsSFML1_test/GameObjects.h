//#ifndef GAMEOBJECTS_H
//#define GAMEOBJECTS_H
#pragma once

#include <SFML/Graphics.hpp>
#include "MissionManager.h"
#include <cmath>
#include <vector>

// MovingCircle class
class MovingCircle {
public:
    sf::CircleShape shape;
    float angle;    // Angle in radians for the circle's position
    float radius;   // Radius from the orbited center point
    float speed;    // Speed(angular) of rotation around the center
    float mass;     // Mass
    float size;     // Size of the planet
    int id;         // Id of the planet

    MovingCircle(float radius, float angle, float speed, float size, sf::Color color, float mass, int id);  //Constructor
    void update(float deltaTime, const sf::Vector2f& center); // Updates planets position based on time and point its orbiting
    void draw(sf::RenderWindow& window) const;  // Draws planets graphic
    sf::Vector2f calculateGravityForce(const sf::Vector2f& point) const; // Calculate planets gravity on specified point 
    sf::Vector2f getLinearVelocity(sf::Vector2f center);    // Converts angular velocity to linear
    bool isClicked(sf::Vector2f mousePosition); // Determines if planet got clicked
};

// Rocket class
class Rocket {
public:
    sf::CircleShape shape;  // rockets current graphic
    sf::Vector2f position;  // Position
    sf::Vector2f velocity;  // Velocity
    sf::Vector2f acceleration;  // Last change in velocity(for verlet)
    float tiltAngle;    // wich way the rocked is turned(Radians)
    float mass;     // rockets mass
    sf::CircleShape flame;  // grapphic for exhaust flame
    float thrustPower; // Thrust
    unsigned int ID;   // Rockets ID
    mission associatedMission;

    Rocket(float startX, float startY, float mass, unsigned int ID, sf::Vector2f initVelocity, const mission& mission);
    void update(float deltaTime, const std::vector<sf::Vector2f>& forces);  // Updates position and velocity
    void draw(sf::RenderWindow& window) const;              // Drawing both rocked and flame
    void setTiltAngle(const sf::Vector2f& start, const sf::Vector2f& target);   // For rotating the rocked
    bool checkCollision(const sf::CircleShape& circle);     // For colisions with planets
};

//#endif // GAMEOBJECTS_H
