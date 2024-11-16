#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

class MovingCircle {
public:
    sf::CircleShape shape;
    float angle;        // Angle in radians for the circle's position
    float radius;       // Radius from the center point
    float speed;        // Speed of rotation around the center
    float mass;             //masa
    float size;

    MovingCircle(float radius, float angle, float speed, float size, sf::Color color, float mass)
        : radius(radius), angle(angle), speed(speed), mass(mass), size(size) {
        shape.setRadius(size);
        shape.setFillColor(color);
        shape.setOrigin(size, size);  // Set origin to center
    }

    // Update position around the central point
    void update(float deltaTime, const sf::Vector2f& center) {
        angle += speed * deltaTime; // Move the angle by speed
        shape.setPosition(
            center.x + radius * std::cos(angle),
            center.y + radius * std::sin(angle)
        );
    }
    sf::Vector2f calculateGravityForce(const sf::Vector2f& point) const {
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
    sf::Vector2f getLinearVelocity(sf::Vector2f center) {
        float orbitRadius = std::hypot(center.x - shape.getPosition().x, center.y - shape.getPosition().y);;
        float linearSpeed = speed * orbitRadius;
        sf::Vector2f velocity(
            -linearSpeed * std::sin(angle), // Perpendicular to the orbit (cos(angle + π/2))
            linearSpeed * std::cos(angle)
        );
        return velocity;
    }

    bool isClicked(sf::Vector2f mousePosition) {
        float dist = std::hypot(mousePosition.x - shape.getPosition().x, mousePosition.y - shape.getPosition().y);
        return dist <= shape.getRadius();
    }
};

class Rocket {
public:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float tiltAngle;
    float mass;
    sf::CircleShape flame;
    float thrustPower = 4.f; // Thrust
    unsigned int ID;              // IDs

    Rocket(float startX, float startY, float mass, unsigned int ID, sf::Vector2f initVelocity)
        : position(startX, startY), velocity(initVelocity), acceleration(0.f, 0.f), tiltAngle(0.0f), mass(mass), ID(ID) {
        shape.setRadius(5.f);
        shape.setFillColor(sf::Color::Blue);
        shape.setOrigin(5.f, 5.f); // Center the shape
        shape.setPosition(position);

        flame.setRadius(2.5f);
        flame.setFillColor(sf::Color::Red);
        flame.setOrigin(flame.getRadius(), flame.getRadius());
    }

    void update(float deltaTime, const std::vector<sf::Vector2f>& forces) {
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
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(flame);
    }
    void setTiltAngle(const sf::Vector2f& start, const sf::Vector2f& target) {
        sf::Vector2f direction = target - start;
        tiltAngle = std::atan2(direction.y, direction.x); // Calculate angle in radians
    }

    // Check collision with a circle
    bool checkCollision(const sf::CircleShape& circle) {
        float distance = std::hypot(circle.getPosition().x - position.x, circle.getPosition().y - position.y);
        return distance < (shape.getRadius() + circle.getRadius());
    }
};

class StateTracker {
public:
    bool pause = 1;     // Pause?
    bool dragging = 0;  // Drag mode?
    bool planetClick = 0;
    sf::Vector2f lineStart = sf::Vector2f(0.f, 0.f);    // Start of a line
    unsigned int rockedID = 1;              // For giving rockets IDs
    unsigned int rockedComand = 0;          // Tracked rocked
    StateTracker() = default;
};

int main() {
    auto tracker = new StateTracker;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Skoki prototyp", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Center point around which circles will move
    sf::Vector2f center(1920/2, 1080/2);

    // Create a vector of moving circles
    std::vector<MovingCircle> circles;
    std::vector<Rocket> rockets;
    for (int i = 0; i < 5; ++i) {
        std::string name = "test";
        float radius = 100 + i * 85;               // Radius from the center
        float angle = i;//i * 126;          // Initial angle
        float speed = 50.0f/radius;           // Speed of rotation
        float size = 25.0f + i * 2.0f;              // Circle size
        float mass = size*size;              // Circle size
        circles.emplace_back(radius, angle, speed, size, sf::Color::Green, mass);
    }
    // Sun
    {
    std::string name = "Sol";
    float radius = 0;               // Radius from the center
    float angle = 0;              // Initial angle
    float speed = 0.0f;           // Speed of rotation
    float size = 35;              // Circle size
    float mass = size * size;              // Circle size
    circles.emplace_back(radius, angle, speed, size, sf::Color::Yellow, mass); }

    sf::Clock clock;
    while (window.isOpen()) {           // Main game loop
        sf::Event event;
        while (window.pollEvent(event)) {// Buttons and stuff
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    tracker->planetClick = 0; // Will check if planet got clicked
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    for (auto& planet : circles) {
                        if (planet.isClicked(mousePos)) {
                            tracker->planetClick = 1; // Got clicked
                            // Get linear velocity of the clicked planet
                            sf::Vector2f linearVelocity = planet.getLinearVelocity(center);
                            sf::Vector2f rocketSpawnOffset(planet.size * std::cos(planet.angle) * 2.5, planet.size * std::sin(planet.angle) * 2.5);
                            // Calculate spawn position a bit away from the planet
                            sf::Vector2f spawnPosition = planet.shape.getPosition() + rocketSpawnOffset;

                            // Create and add a new rocket with the planet's linear velocity
                            rockets.emplace_back(spawnPosition.x, spawnPosition.y, 1.f, tracker->rockedID, linearVelocity);// Make rocket
                            tracker->rockedID += 1;     // Iterate next ID

                            std::cout << "Rocket Spawn" << std::endl;
                            break;
                        }
                    }
                    if (!tracker->planetClick) {    // Clicked a Rocket?
                        tracker->rockedComand = 0;
                        float minDistance = 15;
                        for (auto& rocket : rockets) {
                            sf::Vector2f rocketPos = rocket.shape.getPosition();
                            float distance = std::hypot(rocketPos.x - mousePos.x, rocketPos.y - mousePos.y);
                            if (distance < minDistance) {
                                minDistance = distance;
                                tracker->rockedComand = rocket.ID;      // Will comand this rocked
                            }
                        }
                    }  
                }
                if (event.mouseButton.button == sf::Mouse::Right) {

                    //rockets.emplace_back(event.mouseButton.x, event.mouseButton.y, 1.f);
                    if (!tracker->dragging) {
                        tracker->lineStart = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                    }
                    else {
                        // Update tilt for all rockets based on the end position of the line
                        sf::Vector2f lineEnd = sf::Vector2f(sf::Mouse::getPosition(window));
                        for (auto& rocket : rockets) {
                            if (rocket.ID == tracker->rockedComand) {
                                rocket.setTiltAngle(tracker->lineStart, lineEnd);
                                std::cout << "Rocket Comand" << std::endl;
                            }
                        }
                    }
                    tracker->dragging = !tracker->dragging;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {//klawiatura
                if (event.key.code == sf::Keyboard::Space)
                {
                    tracker->pause = !tracker->pause;
                    std::cout << "Spacja" << std::endl;
                }
                if (event.key.code == sf::Keyboard::Escape)
                {
                    tracker->pause = !tracker->pause;
                    std::cout << "Escape" << std::endl;
                    window.close();
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        // Update all
        if (tracker->pause == 0) {
            for (auto& circle : circles) {
                circle.update(deltaTime, center);
            }
            for (size_t i = 0; i < rockets.size();) {
                // Calculate gravitational forces from each circle
                std::vector<sf::Vector2f> forces;
                for (const auto& circle : circles) {
                    forces.push_back(circle.calculateGravityForce(rockets[i].position));
                }

                // Update rocket's position based on gravitational forces
                rockets[i].update(deltaTime, forces);

                // Check collision with each circle
                bool collided = false;
                for (const auto& circle : circles) {
                    if (rockets[i].checkCollision(circle.shape)) {
                        collided = true;
                        break;
                    }
                }

                // Remove rocket if it collided, otherwise continue
                if (collided) {
                    rockets.erase(rockets.begin() + i);
                }
                else {
                    ++i;
                }
            }
        }

        // Renderowanie
        window.clear();
        for (const auto& circle : circles) {
            window.draw(circle.shape);
        }
        for (auto& rocket : rockets) {
            rocket.draw(window);
        }
        if (tracker->dragging) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f endPosition = static_cast<sf::Vector2f>(mousePosition);

            // Draw a dotted line between `lineStart` and `endPosition`
            float distance = std::hypot(endPosition.x - tracker->lineStart.x, endPosition.y - tracker->lineStart.y);
            sf::Vector2f direction = (endPosition - tracker->lineStart) / distance;
            float dotSpacing = 10.f;

            for (float i = 0; i < distance; i += dotSpacing) {
                sf::CircleShape dot(2.f);  // Dot radius
                dot.setFillColor(sf::Color::White);
                dot.setPosition(tracker->lineStart + direction * i);
                window.draw(dot);
            }
        }
        window.display();
    }

    return 0;
}
