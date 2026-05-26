/**
 * @file main.cpp
 * @brief Uygulama ana giris kapisi.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#include <iostream>
#include <string>
#include "core/CrewRosterManager.h"
#include "utils/FileParser.h"

int main(int argc, char* argv[]) {
    // Varsayilan girdi dosyasi adi
    std::string inputFile = "data/input_sample.json"; 

    // Komut satiri arguman yönetimi kurali
    if (argc > 1) {
        inputFile = argv[1];
    }

    try {
        std::cout << "==================================================\n";
        std::cout << "   CSE 211 - AIRLINE CREW ROSTERING APP START     \n";
        std::cout << "==================================================\n";

        core::CrewRosterManager manager;

        // 1. Verileri yukle
        if (!utils::FileParser::loadMockData(manager, inputFile)) {
            throw std::runtime_error("Veri dosyasi yuklenirken hata olustu!");
        }

        // 2. Optimizasyon ve Eslestirme Algoritmasini Calistir
        std::cout << "[PROCESS] Rostering planlamasi hesaplaniyor...\n";
        manager.generateRoster();

        // 3. Rapor ciktilarini uret
        manager.printReport();

        std::cout << "\n[SUCCESS] Uygulama sorunsuz tamamlandi.\n";
    } 
    catch (const std::exception& e) {
        std::cerr << "Kritik Hata: " << e.what() << "\n";
        return 1;
    }

    return 0;
}