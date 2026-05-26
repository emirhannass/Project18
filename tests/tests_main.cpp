/**
 * @file test_main.cpp
 * @brief Custom assert-based unit ve entegrasyon testleri.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#include <iostream>
#include <cassert>
#include "data_structures/LinkedList.h"
#include "core/Models.h"
#include "core/CrewRosterManager.h"

// 1. Veri Yapısı Unit Testi
void test_linked_list_basic() {
    data_structures::LinkedList<int> list;
    
    // Durum: Yeni kurulan liste boş olmalı
    assert(list.isEmpty() == true);
    assert(list.getSize() == 0);

    // Durum: Eleman ekleme testi (Beginning, Middle, End)
    list.add(10);
    list.add(20);
    list.add(30);

    assert(list.isEmpty() == false);
    assert(list.getSize() == 3);

    // Durum: Elemanlara erişim doğruluğu
    assert(list.get(0) == 10);
    assert(list.get(1) == 20);
    assert(list.get(2) == 30);

    // Durum: Sınır dışı indeks koruması kontrolü (Edge Case)
    try {
        list.get(5);
        assert(false); // Eğer hata fırlatmazsa test başarısız demektir
    } catch (const std::out_of_range& e) {
        assert(true);  // Beklenen hata yakalandı, test başarılı
    }

    std::cout << "  [PASSED] test_linked_list_basic\n";
}

// 2. İş Mantığı Entegrasyon Testi
void test_roster_integration() {
    core::CrewRosterManager manager;

    // Test girdisi hazırlama (Simüle edilmiş uçuş ve ekip)
    core::Flight f{"FL999", "IST", "ESB", "10:00", "11:15", "A320"};
    manager.addFlight(f);

    core::Crew c;
    c.id = "CR_TEST"; c.role = "captain"; c.base = "IST"; c.seniority = 10;
    c.qualifications.add("A320");
    manager.addCrew(c);

    // Algoritmayı tetikle
    manager.generateRoster();

    // Sonucu doğrula: Atama başarıyla yapıldı mı?
    const auto& schedule = manager.getSchedule();
    assert(schedule.getSize() == 1);
    assert(schedule.get(0).assigned_crew_id == "CR_TEST");

    std::cout << "  [PASSED] test_roster_integration\n";
}

int main() {
    std::cout << "[UNIT] Data Structures Testing...\n";
    test_linked_list_basic();

    std::cout << "[INTEGRATION] Workflow Testing...\n";
    test_roster_integration();

    std::cout << "\n===================================\n";
    std::cout << " SUMMARY: All tests PASSED successfully!\n";
    std::cout << "===================================\n";
    
    return 0;
}