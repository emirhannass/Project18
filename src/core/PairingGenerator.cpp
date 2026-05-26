/**
 * @file PairingGenerator.cpp
 * @brief Uçuş zamanı ve dinlenme sürelerine göre kombinasyon üreten modül.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#include "core/PairingGenerator.h"
#include <iostream>
#include <cstdio> // sscanf kullanımı için

namespace core {

bool PairingGenerator::checkRestRequirement(const Flight& f1, const Flight& f2) {
    if (f1.to_airport != f2.from_airport) {
        return false;
    }

    int year1, month1, day1, hour1, min1;
    int year2, month2, day2, hour2, min2;

    std::sscanf(f1.arrive_time.c_str(), "%d-%d-%dT%d:%d", &year1, &month1, &day1, &hour1, &min1);
    std::sscanf(f2.depart_time.c_str(), "%d-%d-%dT%d:%d", &year2, &month2, &day2, &hour2, &min2);

    int total_minutes_f1 = (day1 * 24 * 60) + (hour1 * 60) + min1;
    int total_minutes_f2 = (day2 * 24 * 60) + (hour2 * 60) + min2;

    int diff_hours = (total_minutes_f2 - total_minutes_f1) / 60;

    return (diff_hours >= min_rest_hours);
}

data_structures::LinkedList<Pairing> PairingGenerator::generateValidPairings(const data_structures::LinkedList<Flight>& flights) {
    data_structures::LinkedList<Pairing> createdPairings;
    if (flights.isEmpty()) return createdPairings;

    // Hangi uçuşların kullanıldığını takip etmek için basit bir yerel dizi (STL Vector yasak olduğu için)
    bool* usedFlights = new bool[flights.getSize()];
    for (int i = 0; i < flights.getSize(); ++i) usedFlights[i] = false;

    int pCounter = 1;

    for (int i = 0; i < flights.getSize(); ++i) {
        if (usedFlights[i]) continue; // Bu uçuş zaten bir pairing'e aitse atla

        Flight f1 = flights.get(i);
        Pairing p;
        p.id = "PAIR-" + std::to_string(pCounter++);
        p.flights.add(f1);
        usedFlights[i] = true;

        std::string startAirport = f1.from_airport; // Başlangıç üssünü kaydet

        for (int j = i + 1; j < flights.getSize(); ++j) {
            if (usedFlights[j]) continue;

            Flight f2 = flights.get(j);
            if (checkRestRequirement(f1, f2)) {
                p.flights.add(f2);
                usedFlights[j] = true;
                f1 = f2; // Zinciri ilerlet
                
                // Eğer başladığı meydana geri döndüyse zinciri tamamla (Round-Trip)
                if (f2.to_airport == startAirport) {
                    break;
                }
            }
        }
        createdPairings.add(p);
    }

    delete[] usedFlights;
    return createdPairings;
}

}// namespace core