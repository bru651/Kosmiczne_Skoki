#include <SFML/Graphics.hpp>
#include "MissionManager.h"
#include <cstdlib>  
#include <ctime>   
#include <vector>

sf::Image car;
sf::Image elon;
sf::Image rover;
sf::Image satelite;
sf::Image station;
sf::Image sputnik;
sf::Image telescope;
sf::Image money;

// Load images from files
void loadImages() {
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
}

// Function to generate a random mission
mission generateRandomMission(int uniqueId) {
    // Define the payloads
    payload carP(0, 500, car, 1000, 2000, "Moon vehicle");
    payload elonP(1, 500, elon, 1000, 2000, "Spaceship");
    payload roverP(2, 500, rover, 1000, 2000, "Rover");
    payload sateliteP(3, 500, satelite, 1000, 2000, "Satelite");
    payload stationP(4, 500, station, 1000, 2000, "Space station");
    payload sputnikP(5, 500, sputnik, 1000, 2000, "Sputnik");
    payload telescopeP(6, 500, telescope, 1000, 2000, "Telescope");//TOOD: update masses and money for balance

    // Store payloads in a vector
    std::vector<payload> payloads = { carP, elonP, roverP, sateliteP, stationP, sputnikP, telescopeP };

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate a random index for payload
    int payloadIndex = std::rand() % payloads.size();

    // Generate a random destination between 1 and 5
    int randomDestination = 1 + std::rand() % 5;

    // Select random payload and destination
    payload randomPayload = payloads[payloadIndex];

    // Create and return a new mission
    return mission(uniqueId, randomPayload, randomDestination);
}


