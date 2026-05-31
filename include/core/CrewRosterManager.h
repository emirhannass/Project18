/**
 * @file CrewRosterManager.h
 * @brief Ekip planlama mantığını yöneten sınıf.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#pragma once
#include <string>
#include "data_structures/LinkedList.h"
#include "core/Models.h"

namespace core {

class CrewRosterManager {
private:
    data_structures::LinkedList<Flight> all_flights;
    data_structures::LinkedList<Crew> all_crew;
    data_structures::LinkedList<Pairing> final_schedule;

    bool isCrewQualified(const Crew& crew, const std::string& aircraftType);

public:
    CrewRosterManager() = default;

    void addFlight(const Flight& flight);
    void addCrew(const Crew& crew);
    void generateRoster();
    void printReport() const;
    std::string getRosterAsJson() const;
    std::string getAllFlightsAsJson() const;
    std::string getAllCrewAsJson() const;

    const data_structures::LinkedList<Pairing>& getSchedule() const {
        return final_schedule;
    }
};

} // namespace core