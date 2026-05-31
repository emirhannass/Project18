/**
 * @file main.cpp
 * @brief Uygulama ana giris kapisi ve Web GUI Sunucusu.
 * @author Proje Grubu
 * @date 2026-05-30
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "httplib.h"
#include "core/CrewRosterManager.h"
#include "utils/FileParser.h"

int main(int argc, char* argv[]) {
    if (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
        std::cout << "Kullanim: ./build/bin/main [dosya_yolu]\n";
        std::cout << "Varsayilan dosya yolu: data/input_default.json\n";
        return 0;
    }

    std::string inputFile = "data/input_default.json"; 

    if (argc > 1) {
        inputFile = argv[1];
    }

    core::CrewRosterManager manager;

    try {
        std::cout << "==================================================\n";
        std::cout << "   CSE 211 - AIRLINE CREW ROSTERING APP START     \n";
        std::cout << "==================================================\n";
        std::cout << "[INFO] Yuklenen girdi dosyasi: " << inputFile << "\n\n";

        if (!utils::FileParser::loadMockData(manager, inputFile)) {
            throw std::runtime_error("Veri dosyasi yuklenirken hata olustu!");
        }

        std::cout << "[PROCESS] Rostering planlamasi hesaplaniyor...\n";
        manager.generateRoster();

        manager.printReport();
        std::cout << "\n[SUCCESS] Core optimizasyon motoru basariyla calisti.\n\n";
    } 
    catch (const std::exception& e) {
        std::cerr << "Kritik Core Hatasi: " << e.what() << "\n";
        return 1;
    }

    httplib::Server svr;
    
    std::cout << "==================================================\n";
    std::cout << "🌐 CSE 211 Web GUI Sunucusu aktif ediliyor...\n";
    std::cout << "[URL] http://localhost:8080 adresinden cozumunuzu gorebilirsiniz.\n";
    std::cout << "[INFO] Sunucuyu kapatmak icin terminalde Ctrl+C yapiniz.\n";
    std::cout << "==================================================\n\n";

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        std::ifstream file("frontend/index.html");
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            res.set_content(buffer.str(), "text/html; charset=utf-8");
        } else {
            res.status = 404;
            res.set_content("Hata: 'frontend/index.html' dosyasi bulunamadi!", "text/plain; charset=utf-8");
        }
    });
    
    svr.Get("/api/flights", [&manager](const httplib::Request&, httplib::Response& res) {
        res.set_content(manager.getAllFlightsAsJson(), "application/json; charset=utf-8");
    });

    svr.Get("/api/crew", [&manager](const httplib::Request&, httplib::Response& res) {
        res.set_content(manager.getAllCrewAsJson(), "application/json; charset=utf-8");
    });

    // ARTIK TAMAMEN DİNAMİK OLAN API ENDPOINT
    svr.Get("/api/roster", [&manager](const httplib::Request&, httplib::Response& res) {
        std::string jsonResponse = manager.getRosterAsJson();
        res.set_content(jsonResponse, "application/json; charset=utf-8");
    });

    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "[ERROR] Sunucu 8080 portunu mesgul oldugu icin baslatilamadi!\n";
        return 1;
    }

    return 0;
}