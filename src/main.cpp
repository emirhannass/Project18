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
    // 1. Yardim / Kullanim Klavuzu Kontrolü (Projenin kalitesini gosterir)
    if (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
        std::cout << "Kullanim: ./build/bin/main [dosya_yolu]\n";
        std::cout << "Varsayilan dosya yolu: data/input_default.json\n";
        return 0;
    }

    // Hocanin standardina (data/input_default.json) tam uyum sagliyoruz
    std::string inputFile = "data/input_default.json"; 

    // Komut satiri arguman yönetimi kurali
    if (argc > 1) {
        inputFile = argv[1];
    }

    // Core optimizasyon nesnemiz
    core::CrewRosterManager manager;

    try {
        std::cout << "==================================================\n";
        std::cout << "   CSE 211 - AIRLINE CREW ROSTERING APP START     \n";
        std::cout << "==================================================\n";
        std::cout << "[INFO] Yuklenen girdi dosyasi: " << inputFile << "\n\n";

        // 1. Verileri el yapimi pointer tabanli yapilarimiza yukle
        if (!utils::FileParser::loadMockData(manager, inputFile)) {
            throw std::runtime_error("Veri dosyasi yuklenirken hata olustu!");
        }

        // 2. Optimizasyon ve Eslestirme Algoritmasini Calistir
        std::cout << "[PROCESS] Rostering planlamasi hesaplaniyor...\n";
        manager.generateRoster();

        // 3. Rapor ciktilarini terminale ozet olarak bas
        manager.printReport();
        std::cout << "\n[SUCCESS] Core optimizasyon motoru basariyla calisti.\n\n";
    } 
    catch (const std::exception& e) {
        std::cerr << "Kritik Core Hatasi: " << e.what() << "\n";
        return 1;
    }

    // ============================================================================
    // 4. WEB GUI SUNUCUSU BAŞLATMA (Hocanın Görsel Frontend Şartı İçin)
    // ============================================================================
    httplib::Server svr;
    
    std::cout << "==================================================\n";
    std::cout << "🌐 CSE 211 Web GUI Sunucusu aktif ediliyor...\n";
    std::cout << "[URL] http://localhost:8080 adresinden cozumunuzu gorebilirsiniz.\n";
    std::cout << "[INFO] Sunucuyu kapatmak icin terminalde Ctrl+C yapiniz.\n";
    std::cout << "==================================================\n\n";

    // Tarayici kok dizine (/) istek attiginda frontend/index.html sayfasini gonderir
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

    // Web arayuzundeki buton tıklandiginda el yapimi veri yapilarinin sonuclarini donen API endpoint'i
    svr.Get("/api/roster", [&manager](const httplib::Request&, httplib::Response& res) {
        // İLERİDE YAPILACAK NOT: Burada manager nesnesinin icindeki el yapimi bagli liste 
        // veya Graph dugumlerini dolasarak dinamik bir JSON string'i uretebilirsin.
        // Simdilik arayuzun baglantisini dogrulamak adina mock veri donuyoruz:
        std::string jsonResponse = R"([
            {"flightId": "FL-101", "origin": "IST", "dest": "CDG", "crew": "Kaptan Emirhan + Ekip A"},
            {"flightId": "FL-102", "origin": "CDG", "dest": "JFK", "crew": "Kaptan Emirhan + Ekip A"},
            {"flightId": "FL-203", "origin": "IST", "dest": "LHR", "crew": "Yedek Ekip B"}
        ])";
        
        res.set_content(jsonResponse, "application/json; charset=utf-8");
    });

    // Lokal sunucuyu 8080 portunda dinlemeye aliyoruz
    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "[ERROR] Sunucu 8080 portunu mesgul oldugu icin baslatilamadi!\n";
        return 1;
    }

    return 0;
}