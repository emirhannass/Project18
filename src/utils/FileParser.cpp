/**
 * @file FileParser.cpp
 * @brief Girdi JSON verilerinin dinamik olarak işlenerek sisteme doldurulması.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#include "utils/FileParser.h"
#include <iostream>
#include <fstream>
#include <string>

namespace utils {

// Yardımcı Fonksiyon: Satır içerisindeki belirli bir JSON anahtarının (key) değerini ayıklar
std::string extractValue(const std::string& line, const std::string& key) {
    size_t keyPos = line.find("\"" + key + "\"");
    if (keyPos == std::string::npos) return "";

    size_t colonPos = line.find(":", keyPos);
    if (colonPos == std::string::npos) return "";

    // Değerin başlangıç noktasını bul (boşlukları atla)
    size_t valueStart = colonPos + 1;
    while (valueStart < line.size() && (line[valueStart] == ' ' || line[valueStart] == '\t')) {
        valueStart++;
    }

    if (valueStart < line.size() && line[valueStart] == '"') {
        // Metinsel (String) değerler için iki tırnak arasını al
        size_t valueEnd = line.find("\"", valueStart + 1);
        if (valueEnd != std::string::npos) {
            return line.substr(valueStart + 1, valueEnd - valueStart - 1);
        }
    } else {
        // Sayısal (Integer) değerler için (Örn: seniority) rakamları topla
        std::string numStr = "";
        while (valueStart < line.size() && (std::isdigit(line[valueStart]) || line[valueStart] == '-')) {
            numStr += line[valueStart];
            valueStart++;
        }
        return numStr;
    }
    return "";
}

bool FileParser::loadMockData(core::CrewRosterManager& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[HATA] JSON dosyasi acilamadi: " << filename << "\n";
        return false;
    }

    std::cout << "[INFO] Girdi verileri yukleniyor: " << filename << "\n";

    std::string line;
    bool inFlights = false;
    bool inCrew = false;

    while (std::getline(file, line)) {
        // Hangi JSON dizisinde (array) olduğumuzu tespit etme
        if (line.find("\"flights\"") != std::string::npos) {
            inFlights = true;
            inCrew = false;
            continue;
        }
        if (line.find("\"crew\"") != std::string::npos) {
            inFlights = false;
            inCrew = true;
            continue;
        }
        if (line.find("\"regulations\"") != std::string::npos) {
            inFlights = false;
            inCrew = false;
            continue;
        }

        // 1. UÇUŞ SATIRLARINI AYRIŞTIRMA
        if (inFlights && line.find("{\"id\"") != std::string::npos) {
            core::Flight f;
            f.id = extractValue(line, "id");
            f.from_airport = extractValue(line, "from");
            f.to_airport = extractValue(line, "to");
            f.depart_time = extractValue(line, "depart");
            f.arrive_time = extractValue(line, "arrive");
            f.aircraft_type = extractValue(line, "aircraft");

            if (!f.id.empty()) {
                manager.addFlight(f);
            }
        }

        // 2. EKİP SATIRLARINI AYRIŞTIRMA
        if (inCrew && line.find("{\"id\"") != std::string::npos) {
            core::Crew c;
            c.id = extractValue(line, "id");
            c.role = extractValue(line, "role");
            c.base = extractValue(line, "base");
            
            std::string senStr = extractValue(line, "seniority");
            if (!senStr.empty()) {
                c.seniority = std::stoi(senStr);
            }

            // Yetkinlikleri (qualifications: ["A320", "B737"]) ayıklama döngüsü
            size_t startBracket = line.find("[");
            size_t endBracket = line.find("]");
            if (startBracket != std::string::npos && endBracket != std::string::npos) {
                std::string qualsRaw = line.substr(startBracket + 1, endBracket - startBracket - 1);
                size_t qPos = 0;
                while ((qPos = qualsRaw.find("\"", qPos)) != std::string::npos) {
                    size_t nextQ = qualsRaw.find("\"", qPos + 1);
                    if (nextQ != std::string::npos) {
                        std::string qual = qualsRaw.substr(qPos + 1, nextQ - qPos - 1);
                        if (!qual.empty()) {
                            c.qualifications.add(qual);
                        }
                        qPos = nextQ + 1;
                    } else {
                        break;
                    }
                }
            }

            if (!c.id.empty()) {
                manager.addCrew(c);
            }
        }
    }

    file.close();
    return true;
}

} // namespace utils