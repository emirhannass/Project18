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
    // 1. Aşama: Uçuşları birbirine bağlayarak yasal Pairing'leri üret
    data_structures::LinkedList<Pairing> validPairings = generator.generateValidPairings(all_flights);

    // Zaten atanmış olan ekip ID'lerini hafızada tutmak için kendi LinkedList yapımız
    data_structures::LinkedList<std::string> assignedCrewIds;

    // 2. Aşama: Üretilen bu hazır pairing zincirlerine en uygun ekipleri ata
    for (auto& pairing : validPairings) {
        if (pairing.flights.isEmpty()) continue;

        Crew* bestCandidate = nullptr;
        Flight firstFlight = pairing.flights.get(0); // Zincirin ilk uçuşu

        for (auto& crew : all_crew) {
            // KURAL 1: Üs Kontrolü (Ekibin üssü ile zincirin başlangıç meydanı uyuşmalı)
            if (crew.base != firstFlight.from_airport) {
                continue;
            }

            // KURAL 2: Kalifikasyon Kontrolü (Uçağa ehliyeti var mı?)
            if (!isCrewQualified(crew, firstFlight.aircraft_type)) {
                continue;
            }

            // KURAL 3: Çift Atama Engelleme (Bu ekip üyesi başka bir uçuş zincirine atanmış mı?)
            bool alreadyAssigned = false;
            for (int k = 0; k < assignedCrewIds.getSize(); ++k) {
                if (assignedCrewIds.get(k) == crew.id) {
                    alreadyAssigned = true;
                    break;
                }
            }
            if (alreadyAssigned) {
                continue; // Ekip meşgulse sonraki adaya geç
            }

            // KURAL 4: Kıdem (Seniority) Önceliği
            if (bestCandidate == nullptr || crew.seniority > bestCandidate->seniority) {
                bestCandidate = &crew;
            }
        }

        // Uygun ve boşta ekip bulunduysa ata ve nihai takvime ekle
        if (bestCandidate != nullptr) {
            pairing.assigned_crew_id = bestCandidate->id;
            assignedCrewIds.add(bestCandidate->id); // Bu ekibi artık meşgul listesine ekle
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

}// namespace core