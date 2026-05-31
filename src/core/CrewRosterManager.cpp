/**
 * @file CrewRosterManager.cpp
 * @brief Rostering kuralları ve eşleştirme algoritmasının uygulaması.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#include "core/CrewRosterManager.h"
#include "core/PairingGenerator.h"
#include <iostream>

namespace core {

void CrewRosterManager::addFlight(const Flight& flight) {
    all_flights.add(flight);
}

void CrewRosterManager::addCrew(const Crew& crew) {
    all_crew.add(crew);
}

bool CrewRosterManager::isCrewQualified(const Crew& crew, const std::string& aircraftType) {
    for (const auto& q : crew.qualifications) {
        if (q == aircraftType) {
            return true;
        }
    }
    return false;
}

void CrewRosterManager::generateRoster() {
    PairingGenerator generator;
    data_structures::LinkedList<Pairing> validPairings = generator.generateValidPairings(all_flights);
    data_structures::LinkedList<std::string> assignedCrewIds;

    for (auto& pairing : validPairings) {
        if (pairing.flights.isEmpty()) continue;

        Crew* bestCandidate = nullptr;
        Flight firstFlight = pairing.flights.get(0);

        for (auto& crew : all_crew) {
            if (crew.base != firstFlight.from_airport) {
                continue;
            }

            if (!isCrewQualified(crew, firstFlight.aircraft_type)) {
                continue;
            }

            bool alreadyAssigned = false;
            for (int k = 0; k < assignedCrewIds.getSize(); ++k) {
                if (assignedCrewIds.get(k) == crew.id) {
                    alreadyAssigned = true;
                    break;
                }
            }
            if (alreadyAssigned) {
                continue;
            }

            if (bestCandidate == nullptr || crew.seniority > bestCandidate->seniority) {
                bestCandidate = &crew;
            }
        }

        if (bestCandidate != nullptr) {
            pairing.assigned_crew_id = bestCandidate->id;
            assignedCrewIds.add(bestCandidate->id);
            final_schedule.add(pairing);
        } else {
            std::cout << "[UYARI] " << pairing.id << " rotasi icin uygun bosta ekip bulunamadi!\n";
        }
    }
}

void CrewRosterManager::printReport() const {
    std::cout << "\n==================================================\n";
    std::cout << "          EKİP ATAMA RAPORU (ROSTER REPORT)        \n";
    std::cout << "==================================================\n";
    
    if (final_schedule.isEmpty()) {
        std::cout << "Eslesen aktif gorev bulunmamaktadir.\n";
        return;
    }

    for (const auto& pairing : final_schedule) {
        std::cout << "Gorev No: " << pairing.id << " | Atanan Ekip: " << pairing.assigned_crew_id << "\n";
        for (const auto& flight : pairing.flights) {
            std::cout << "  -> Ucus: " << flight.id 
                      << " [" << flight.from_airport << " -> " << flight.to_airport << "] "
                      << "Ucak: " << flight.aircraft_type << "\n";
        }
        std::cout << "--------------------------------------------------\n";
    }
}

std::string CrewRosterManager::getRosterAsJson() const {
    std::string json = "[";
    bool first = true;

    for (const auto& pairing : final_schedule) {
        if (!first) {
            json += ", ";
        }
        
        json += "{";
        json += "\"pairingId\": \"📦 " + pairing.id + "\", ";
        json += "\"crew\": \"" + pairing.assigned_crew_id + "\", ";
        json += "\"flights\": [";
        
        bool firstFlight = true;
        for (const auto& flight : pairing.flights) {
            if (!firstFlight) {
                json += ", ";
            }
            json += "{";
            json += "\"flightId\": \"" + flight.id + "\", ";
            json += "\"from\": \"" + flight.from_airport + "\", ";
            json += "\"to\": \"" + flight.to_airport + "\", ";
            json += "\"aircraft\": \"" + flight.aircraft_type + "\"";
            json += "}";
            firstFlight = false;
        }
        json += "]";
        json += "}";
        first = false;
    }
    json += "]";
    return json;
} 

std::string CrewRosterManager::getAllFlightsAsJson() const {
    std::string json = "[";
    bool first = true;
    for (const auto& flight : all_flights) {
        if (!first) {
            json += ", ";
        }
        json += "{";
        json += "\"flightId\": \"" + flight.id + "\", ";
        json += "\"from\": \"" + flight.from_airport + "\", ";
        json += "\"to\": \"" + flight.to_airport + "\", ";
        json += "\"aircraft\": \"" + flight.aircraft_type + "\"";
        json += "}";
        first = false;
    }
    json += "]";
    return json;
} 

std::string CrewRosterManager::getAllCrewAsJson() const {
    std::string json = "[";
    bool first = true;
    for (const auto& crew : all_crew) {
        if (!first) {
            json += ", ";
        }
        json += "{";
        json += "\"crewId\": \"" + crew.id + "\", ";
        json += "\"base\": \"" + crew.base + "\", ";
        json += "\"qualifications\": [";
        
        bool firstQ = true;
        for (const auto& q : crew.qualifications) {
            if (!firstQ) {
                json += ", ";
            }
            json += "\"" + q + "\"";
            firstQ = false;
        }
        json += "]";
        json += "}";
        first = false;
    }
    json += "]";
    return json;
} 

} // namespace core