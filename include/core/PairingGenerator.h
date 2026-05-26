/**
 * @file PairingGenerator.h
 * @brief Uçuşları kurallara uygun olarak Pairing (Görev Çifti) dizilerine dönüştüren sınıf.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#pragma once
#include "data_structures/LinkedList.h"
#include "core/Models.h"

namespace core {

class PairingGenerator {
private:
    // Havacılık kuralları limitleri (PROJ-18.pdf ve base.pdf kriterleri)
    int max_duty_hours;
    int min_rest_hours;

    bool checkRestRequirement(const Flight& f1, const Flight& f2);

public:
    // Varsayılan kuralları constructor ile set ediyoruz (PROJ-18 Sayfa 2)
    PairingGenerator(int maxDuty = 12, int minRest = 10) 
        : max_duty_hours(maxDuty), min_rest_hours(minRest) {}

    /**
     * @brief Ham uçuş listesini alır ve birbiri ardına bağlanabilecek uçuşları Pairing haline getirir.
     * @param flights Ham uçuş listesi
     * @return data_structures::LinkedList<Pairing> Oluşturulan yasal pairing listesi
     */
    data_structures::LinkedList<Pairing> generateValidPairings(const data_structures::LinkedList<Flight>& flights);
};

} // namespace core