/**
 * @file FileParser.cpp
 * @brief Örnek girdi verilerinin el ile işlenerek sisteme doldurulması.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#include "utils/FileParser.h"
#include <iostream>

namespace utils {

bool FileParser::loadMockData(core::CrewRosterManager& manager, const std::string& filename) {
    std::cout << "[INFO] Girdi verileri yukleniyor: " << filename << "\n";

    // PROJ-18 ucus verileri ekleniyor
    core::Flight f1;
    f1.id = "FL100"; f1.from_airport = "IST"; f1.to_airport = "ESB"; 
    f1.depart_time = "2026-04-01T08:00"; f1.arrive_time = "2026-04-01T09:15"; f1.aircraft_type = "A320";

    core::Flight f2;
    f2.id = "FL101"; f2.from_airport = "ESB"; f2.to_airport = "ADB"; 
    f2.depart_time = "2026-04-01T10:30"; f2.arrive_time = "2026-04-01T11:45"; f2.aircraft_type = "A320";

    core::Flight f3;
    f3.id = "FL102"; f3.from_airport = "ADB"; f3.to_airport = "IST"; 
    f3.depart_time = "2026-04-01T14:00"; f3.arrive_time = "2026-04-01T15:20"; f3.aircraft_type = "B737";

    core::Flight f4;
    f4.id = "FL103"; f4.from_airport = "IST"; f4.to_airport = "ADB"; 
    f4.depart_time = "2026-04-02T07:00"; f4.arrive_time = "2026-04-02T08:20"; f4.aircraft_type = "A320";
    
    manager.addFlight(f1);
    manager.addFlight(f2);
    manager.addFlight(f3);
    manager.addFlight(f4);

    // Ekip nesneleri olusturuluyor (Constructor hatasi giderildi)
    core::Crew c1;
    c1.id = "CR01"; 
    c1.role = "captain"; 
    c1.base = "IST"; 
    c1.seniority = 15;
    c1.qualifications.add("A320");
    c1.qualifications.add("B737");

    core::Crew c2;
    c2.id = "CR02"; 
    c2.role = "captain"; 
    c2.base = "IST"; 
    c2.seniority = 8;
    c2.qualifications.add("A320");

    core::Crew c3;
    c3.id = "CR03"; 
    c3.role = "first_officer"; 
    c3.base = "ESB"; 
    c3.seniority = 5;
    c3.qualifications.add("A320");
    c3.qualifications.add("B737");

    manager.addCrew(c1);
    manager.addCrew(c2);
    manager.addCrew(c3);

    return true;
}

} // namespace utils