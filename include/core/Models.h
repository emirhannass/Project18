/**
 * @file Models.h
 * @brief Havayolu veri modellerinin tanımları.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#pragma once
#include <string>
#include "data_structures/LinkedList.h"

namespace core {

struct Flight {
    std::string id;
    std::string from_airport;
    std::string to_airport;
    std::string depart_time;
    std::string arrive_time;
    std::string aircraft_type;
};

struct Crew {
    std::string id;
    std::string role;
    std::string base;
    int seniority;
    data_structures::LinkedList<std::string> qualifications;

    // c1, c2, c3 hatalarını çözen kurucu metot (Default Constructor)
    Crew() : id(""), role(""), base(""), seniority(0), qualifications() {}
};

struct Pairing {
    std::string id;
    std::string assigned_crew_id;
    data_structures::LinkedList<Flight> flights;
    
    Pairing() : id(""), assigned_crew_id(""), flights() {}
};

} // namespace core