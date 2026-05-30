/**
 * @file test_roster.cpp
 * @brief Is mantigi ve roster optimizasyonu entegrasyon testleri.
 */

#include <iostream>
#include <cassert>
#include "core/CrewRosterManager.h"
#include "core/Models.h"

void test_roster_integration_normal() {
    core::CrewRosterManager manager;

    // Normal uçuş girdisi
    core::Flight f{"FL999", "IST", "ESB", "10:00", "11:15", "A320"};
    manager.addFlight(f);

    // Kriterlere uyan ekip girdisi
    core::Crew c;
    c.id = "CR_TEST"; 
    c.role = "captain"; 
    c.base = "IST"; 
    c.seniority = 10;
    c.qualifications.add("A320");
    manager.addCrew(c);

    // Algoritmayı çalıştır
    manager.generateRoster();

    // Çıktıyı doğrula
    const auto& schedule = manager.getSchedule();
    assert(schedule.getSize() == 1);
    assert(schedule.get(0).assigned_crew_id == "CR_TEST");

    std::cout << "  [PASSED] Integration: Normal senaryo basariyla eslestirildi.\n";
}

void run_integration_tests() {
    std::cout << "\n[RUNNING] --- ENTEGRASYON TESTLERI ---\n";
    test_roster_integration_normal();
}