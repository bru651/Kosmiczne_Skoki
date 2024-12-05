#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "GameObjects.h"

MovingCircle::MovingCircle(float radius, float angle, float speed, float size, sf::Color color, float mass, int id)
        : radius(radius), angle(angle), speed(speed), mass(mass), size(size), id(id) {
        shape.setRadius(size);
        shape.setFillColor(color);
        shape.setOrigin(size, size);  // Set origin to center
    }

    // Update position around the central point
    void MovingCircle::update(float deltaTime, const sf::Vector2f& center) {
        angle += speed * deltaTime; // Move the angle by speed
        shape.setPosition(
            center.x + radius * std::cos(angle),
            center.y + radius * std::sin(angle)
        );
    }
    // Draw the rocket to the window
    void MovingCircle::draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }
    // Calculate G force from planet
    sf::Vector2f MovingCircle::calculateGravityForce(const sf::Vector2f& point) const {
        sf::Vector2f position = shape.getPosition();
        sf::Vector2f direction = point - position; // Direction vector from circle to point
        float distanceSquared = direction.x * direction.x + direction.y * direction.y;

        if (distanceSquared == 0) return sf::Vector2f(0.f, 0.f); // Avoid division by zero

        float distance = std::sqrt(distanceSquared);
        float forceMagnitude = (50 * mass) / distanceSquared; // Gravitational force magnitude

        // Normalize direction and scale by force magnitude
        sf::Vector2f force = (direction / distance) * forceMagnitude;
        return force;
    }

    // Calculate the linear velocity of the planet
    sf::Vector2f MovingCircle::getLinearVelocity(sf::Vector2f center) {
        float orbitRadius = std::hypot(center.x - shape.getPosition().x, center.y - shape.getPosition().y);;
        float linearSpeed = speed * orbitRadius;
        sf::Vector2f velocity(
            -linearSpeed * std::sin(angle), // Perpendicular to the orbit (cos(angle + π/2))
            linearSpeed * std::cos(angle)
        );
        return velocity;
    }

    // Clicked?
    bool MovingCircle::isClicked(sf::Vector2f mousePosition) {
        float dist = std::hypot(mousePosition.x - shape.getPosition().x, mousePosition.y - shape.getPosition().y);
        return dist <= shape.getRadius();
    }


    Rocket::Rocket(float startX, float startY, float mass, unsigned int ID, sf::Vector2f initVelocity, const mission& associatedMission)
        : position(startX, startY), velocity(initVelocity), acceleration(0.f, 0.f), tiltAngle(0.0f), mass(mass), ID(ID), thrustPower(4.f), associatedMission(associatedMission) {
        shape.setRadius(5.f);
        shape.setFillColor(sf::Color::Blue);
        shape.setOrigin(5.f, 5.f); // Center the shape
        shape.setPosition(position);

        // Flame graphics
        flame.setRadius(2.5f);
        flame.setFillColor(sf::Color::Red);
        flame.setOrigin(flame.getRadius(), flame.getRadius());
    }

    void Rocket::update(float deltaTime, const std::vector<sf::Vector2f>& forces) {
        // Compute net gravitational force
        sf::Vector2f netForce(0.f, 0.f);
        for (const auto& force : forces) {
            netForce += force;
        }
        // Thrust
        sf::Vector2f thrust(std::cos(tiltAngle) * thrustPower, std::sin(tiltAngle) * thrustPower);
        netForce += thrust;
        // Calculate acceleration (F = ma -> a = F / m)
        sf::Vector2f newAcceleration = -(netForce / mass);

        // Apply velocity Verlet integration
        position += velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;
        sf::Vector2f nextVelocity = velocity + 0.5f * (acceleration + newAcceleration) * deltaTime;

        // Update velocity and acceleration
        velocity = nextVelocity;
        acceleration = newAcceleration;

        // Update rocket's position
        shape.setPosition(position);
        flame.setPosition(position + sf::Vector2f(std::cos(tiltAngle), std::sin(tiltAngle)) * 10.f);
    }

    // Draw the rocket to the window
    void Rocket::draw(sf::RenderWindow& window) const {
        window.draw(shape);
        window.draw(flame);
    }
    void Rocket::setTiltAngle(const sf::Vector2f& start, const sf::Vector2f& target) {
        sf::Vector2f direction = target - start;
        tiltAngle = std::atan2(direction.y, direction.x); // Calculate angle in radians
    }

    // Check collision with a circle
    bool Rocket::checkCollision(const sf::CircleShape& circle) {
        float distance = std::hypot(circle.getPosition().x - position.x, circle.getPosition().y - position.y);
        return distance < (shape.getRadius() + circle.getRadius());
    }
