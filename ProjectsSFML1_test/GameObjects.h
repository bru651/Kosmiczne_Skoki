//#ifndef GAMEOBJECTS_H
//#define GAMEOBJECTS_H
#pragma once

#include <SFML/Graphics.hpp>
#include "MissionManager.h"
#include <cmath>
#include <vector>

class Building {
public:
    std::string name;       // Name of the building (same as rocket payload)
    float angle;            // Relative angle (in radians) on the planet's surface
    float size;             // Size
    sf::CircleShape shape;  // Graphical representation of the building

    // Constructor
    Building(const std::string& name, float angle);

    // Update position of the building based on the planet's center and radius
    void updatePosition(const sf::Vector2f& center, float radius);

    // Draw the building
    void draw(sf::RenderWindow& window) const;

    // Calculate the angular width of the building on the planet
    float getAngularWidth(float planetRadius) const;
};

// MovingCircle class
class MovingCircle {
public:
    sf::CircleShape shape;
    std::vector<Building> buildings;  // Buildings on the planet
    float angle;    // Angle in radians for the circle's position
    float radius;   // Radius from the orbited center point
    float speed;    // Speed(angular) of rotation around the center
    float mass;     // Mass
    float size;     // Size of the planet
    int id;         // Id of the planet
    bool walkable;  // Can it have buildings
    sf::Vector2f position = sf::Vector2f(0, 0);

    MovingCircle(float radius, float angle, float speed, float size, sf::Color color, float mass, int id, bool walkable);  //Constructor
    void update(float deltaTime, const sf::Vector2f& center); // Updates planets position based on time and point its orbiting
    void draw(sf::RenderWindow& window) const;  // Draws planets graphic
    sf::Vector2f calculateGravityForce(const sf::Vector2f& point) const; // Calculate planets gravity on specified point 
    sf::Vector2f getLinearVelocity(sf::Vector2f center);    // Converts angular velocity to linear
    bool isClicked(sf::Vector2f mousePosition); // Determines if planet got clicked
    void addBuilding(const std::string name, float angle); // Add building
    bool checkAndDestroyBuilding(float collisionAngle, float planetRadius); // Remove building

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
    float thrustMultiplier; // Thrust
    unsigned int ID;   // Rockets ID
    mission associatedMission;

    Rocket(float startX, float startY, float mass, unsigned int ID, sf::Vector2f initVelocity, const mission& mission);
    void update(float deltaTime, const std::vector<sf::Vector2f>& forces);  // Updates position and velocity
    void draw(sf::RenderWindow& window) const;              // Drawing both rocked and flame
    void setTiltAngle(const sf::Vector2f& start, const sf::Vector2f& target);   // For rotating the rocked
    bool checkCollision(const sf::CircleShape& circle);     // For colisions with planets
};



//#endif // GAMEOBJECTS_H
