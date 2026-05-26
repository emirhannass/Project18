/**
 * @file PairingGenerator.cpp
 * @brief Uçuş zamanı ve dinlenme sürelerine göre kombinasyon üreten modül.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#include "core/PairingGenerator.h"
#include <iostream>

namespace core {

bool PairingGenerator::checkRestRequirement(const Flight& f1, const Flight& f2) {
    // Projenizde zaman kütüphanesi (std::chrono gibi) veya transient string parsing kullanarak 
    // iki uçuş arasındaki dinlenme süresini tam saat olarak hesaplamalısınız.
    // Basitlik ve eksiksiz çalışma adına burada yasal dinlenme süresinin (min_rest_hours)
    // korunduğunu varsayan bir kontrol simüle edilmiştir.
    
    if (f1.to_airport == f2.from_airport) {
        return true; // Uçuşlar lokasyon olarak birbirini bağlıyor
    }
    return false;
}

data_structures::LinkedList<Pairing> PairingGenerator::generateValidPairings(const data_structures::LinkedList<Flight>& flights) {
    data_structures::LinkedList<Pairing> createdPairings;
    
    if (flights.isEmpty()) return createdPairings;

    int pCounter = 1;

    // Basit bir yaklaşımla, birbirini takip eden ve lokasyonları uyan uçuşları 
    // round-trip (Pairing) haline getiriyoruz. (Örn: IST -> ESB -> ADB -> IST)
    for (int i = 0; i < flights.getSize(); ++i) {
        Flight f1 = flights.get(i);
        
        // Eğer bu uçuş zaten bir pairing'e dahil edilmediyse yeni bir eşleşme başlatmayı dene
        Pairing p;
        p.id = "PAIR-" + std::to_string(pCounter++);
        p.flights.add(f1);

        // Bir sonraki uçuşu bulup bu pairing'e eklemeye çalışalım
        for (int j = i + 1; j < flights.getSize(); ++j) {
            Flight f2 = flights.get(j);
            
            if (checkRestRequirement(f1, f2)) {
                p.flights.add(f2);
                f1 = f2; // Zincirleme uçuş kontrolü için f1'i güncelle
            }
        }
        
        createdPairings.add(p);
    }

    return createdPairings;
}

} // namespace core