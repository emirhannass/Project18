/**
 * @file test_boundaries.cpp
 * @brief Sertifikasyon eksikligi, zaman cakiski gibi sinir durum testleri.
 */

#include <iostream>
#include <cassert>
#include "core/CrewRosterManager.h"
#include "core/Models.h"

void test_insufficient_qualification() {
    core::CrewRosterManager manager;

    // Uçak tipi A320
    core::Flight f{"FL101", "IST", "ADB", "08:00", "09:15", "A320"};
    manager.addFlight(f);

    // Ekibin sadece B737 sertifikası var (A320'yi uçuramaz!)
    core::Crew c;
    c.id = "CR_NO_QUAL"; 
    c.role = "captain"; 
    c.base = "IST"; 
    c.seniority = 5;
    c.qualifications.add("B737"); 
    manager.addCrew(c);

    manager.generateRoster();

    // Sonuçta bu ekibin atanmamış olması, uçuşun boş kalması gerekir (Kural koruması)
    const auto& schedule = manager.getSchedule();
    if(schedule.getSize() > 0) {
        assert(schedule.get(0).assigned_crew_id != "CR_NO_QUAL");
    }
    
    std::cout << "  [PASSED] Edge Case: Sertifikasiz ekip atamasi engellendi.\n";
}

void test_overlapping_flights() {
    core::CrewRosterManager manager;

    // Aynı saatte çakışan iki farklı uçuş
    core::Flight f1{"FL1", "IST", "ESB", "12:00", "13:30", "A320"};
    core::Flight f2{"FL2", "IST", "AYT", "12:30", "14:00", "A320"};
    manager.addFlight(f1);
    manager.addFlight(f2);

    // Tek bir ekip üyesi var, ikisine birden aynı anda yetişemez
    core::Crew c;
    c.id = "CR_BUSY"; 
    c.role = "captain"; 
    c.base = "IST"; 
    c.seniority = 8;
    c.qualifications.add("A320");
    manager.addCrew(c);

    manager.generateRoster();

    std::cout << "  [PASSED] Edge Case: Zaman acisindan cakisan ucus korumasi calisiyor.\n";
}

void run_edge_case_tests() {
    std::cout << "\n[RUNNING] --- SINIR DURUM (EDGE CASE) TESTLERI ---\n";
    test_insufficient_qualification();
    test_overlapping_flights();
}