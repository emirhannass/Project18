/**
 * @file CrewRosterManager.cpp
 * @brief Rostering kuralları ve eşleştirme algoritmasının uygulaması.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#include "core/CrewRosterManager.h"
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
    // Kıdem bazlı eşleştirme için basit bir sıralama mantığıyla (Seniority öncelikli)
    // her bir uçuş serisi (Pairing) oluşturulur ve uygun ekibe atanır.
    int pairingCounter = 1;

    for (const auto& flight : all_flights) {
        Crew* bestCandidate = nullptr;

        for (auto& crew : all_crew) {
            // Kural 1: Uçağa kalifikasyonu var mı? [cite: 2124, 2133]
            if (!isCrewQualified(crew, flight.aircraft_type)) {
                continue;
            }

            // Kural 2: Üs eşleşmesi var mı? [cite: 2125]
            if (crew.base != flight.from_airport) {
                continue;
            }

            // Kural 3: Kıdem önceliği kontrolü (Seniority-based bidding) [cite: 2126, 2146]
            if (bestCandidate == nullptr || crew.seniority > bestCandidate->seniority) {
                bestCandidate = &crew;
            }
        }

        // Eğer uygun ekip bulunduysa Pairing nesnesi oluşturulup takvime eklenir
        if (bestCandidate != nullptr) {
            Pairing newPairing;
            newPairing.id = "PA" + std::to_string(pairingCounter++);
            newPairing.assigned_crew_id = bestCandidate->id;
            newPairing.flights.add(flight);
            
            final_schedule.add(newPairing);
        } else {
            std::cout << "[UYARI] " << flight.id << " ucusu icin uygun yetkinlikte bos ekip bulunamadi!\n";
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

} // namespace core