#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>

struct Flight {
    std::string id;         // Örn: "FL100" [cite: 38]
    std::string from;       // Kalkış havaalanı [cite: 38]
    std::string to;         // Varış havaalanı [cite: 38]
    std::string depart;     // Kalkış zamanı (ISO 8601 formatında) [cite: 38]
    std::string arrive;     // Varış zamanı (ISO 8601 formatında) [cite: 38]
    std::string aircraft;   // Uçak tipi (A320, B737 vb.) [cite: 38, 42]
};

#endif