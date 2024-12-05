#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class payload {
public:
    int id;
    int mass;
    sf::Image icon;
    int cost;
    int reward;
    std::string name;

    // Constructor
    payload(int id, int mass, const sf::Image& icon, int cost, int reward, const std::string& name)
        : id(id), mass(mass), icon(icon), cost(cost), reward(reward), name(name) {
    }

    // Default constructor
    payload() : id(0), mass(0), cost(0), reward(0), name("") {}
};

class mission {
public:
    int id;
    payload missionPayload; // Rename the member variable
    int destination;

    // Constructor
    mission(int id, const payload& missionPayload, int destination)
        : id(id), missionPayload(missionPayload), destination(destination) {
    }

    // Default constructor
    mission() : id(0), destination(0) {}
};

mission generateRandomMission(int);
