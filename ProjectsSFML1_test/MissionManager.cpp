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
    payload carP(0, 3, car, 1000, 2000, "Moon vehicle", "pixelarty/apolloCarThing.png");        // 0
    payload elonP(1, 15, elon, 3000, 8000, "Spaceship", "pixelarty/elonRocket.png");             // 1
    payload roverP(2, 2, rover, 750, 1250, "Rover", "pixelarty/rover.png");                    // 2
    payload sateliteP(3, 3, satelite, 500, 1000, "Satelite", "pixelarty/satelite.png");        // 3
    payload stationP(4, 10, station, 2000, 4000, "Space station", "pixelarty/spaceStation.png"); // 4
    payload sputnikP(5, 1, sputnik, 100, 400, "Sputnik", "pixelarty/sputnik.png");            // 5
    payload telescopeP(6, 5, telescope, 1500, 3000, "Telescope", "pixelarty/telescope.png");//TOOD: update masses and money for balance

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
