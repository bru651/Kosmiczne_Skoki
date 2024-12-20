#include <SFML/Graphics.hpp>
#include "MissionManager.h"
#include <cstdlib>  
#include <ctime>   
#include <vector>
#include <iostream>

sf::Image car;
sf::Image elon;
sf::Image rover;
sf::Image satelite;
sf::Image station;
sf::Image sputnik;
sf::Image telescope;
sf::Image money;

// Variable to track the number of completed missions
int completedMissions = 0;

// Function to check if a payload is unlocked based on completed missions
bool isPayloadUnlocked(const payload& p) {
    switch (p.id) {
    case 5:
        return true;  // Always available
    case 3:
        return true;  // Always available
    case 6:
        return completedMissions >= 3;  // Unlocked after 3 missions
    case 2:
        return completedMissions >= 3;  // Unlocked after 3 missions
    case 4:
        return completedMissions >= 5;  // Unlocked after 5 missions
    case 0:
        return completedMissions >= 5;  // Unlocked after 5 missions
    case 1:
        return completedMissions >= 7;  // Unlocked after 7 missions
    default:
        return false;  // If unknown payload, not unlocked
    }
}


// Load images from files
void loadImages() {
    std::cout << "Loading images" << std::endl;
    if (!car.loadFromFile("pixelarty/apolloCarThing.png")) {
        throw std::runtime_error("Failed to load car image");
    }
    if (!elon.loadFromFile("pixelarty/elonRocket.png")) {
        throw std::runtime_error("Failed to load elon image");
    }
    if (!rover.loadFromFile("pixelarty/rover.png")) {
        throw std::runtime_error("Failed to load rover image");
    }
    if (!satelite.loadFromFile("pixelarty/satelite.png")) {
        throw std::runtime_error("Failed to load satelite image");
    }
    if (!station.loadFromFile("pixelarty/spaceStation.png")) {
        throw std::runtime_error("Failed to load station image");
    }
    if (!sputnik.loadFromFile("pixelarty/sputnik.png")) {
        throw std::runtime_error("Failed to load sputnik image");
    }
    if (!telescope.loadFromFile("pixelarty/telescope.png")) {
        throw std::runtime_error("Failed to load telescope image");
    }
    if (!money.loadFromFile("pixelarty/stonk.png")) {
        throw std::runtime_error("Failed to load money image");
    }
    std::cout << "Loaded images" << std::endl;
}

// Function to generate a random mission
mission generateRandomMission(int uniqueId) {
    loadImages();
    // Define the payloads
    std::cout << "Func: generateRandomMission" << std::endl;
    payload carP(0, 1, car, 1000, 2000, "Moon vehicle");
    payload elonP(1, 1, elon, 1000, 2000, "Spaceship");
    payload roverP(2, 1, rover, 1000, 2000, "Rover");
    payload sateliteP(3, 1, satelite, 1000, 2000, "Satelite");
    payload stationP(4, 1, station, 1000, 2000, "Space station");
    payload sputnikP(5, 1, sputnik, 1000, 2000, "Sputnik");
    payload telescopeP(6, 1, telescope, 1000, 2000, "Telescope");//TOOD: update masses and money for balance

    // Store payloads in a vector
    std::vector<payload> payloads = { carP, elonP, roverP, sateliteP, stationP, sputnikP, telescopeP };

    // Create a vector to store unlocked payloads
    std::vector<payload> unlockedPayloads;

    // Check which payloads are unlocked
    for (const auto& p : payloads) {
        if (isPayloadUnlocked(p)) {
            unlockedPayloads.push_back(p);
        }
    }

    // Generate a random index for payload
    int payloadIndex = std::rand() % unlockedPayloads.size();

    // Generate a random destination between 1 and 5
    int randomDestination = 1 + std::rand() % 5;

    // Select random payload and destination
    payload randomPayload = unlockedPayloads[payloadIndex];

    // Create and return a new mission
    return mission(uniqueId, randomPayload, randomDestination);
}
