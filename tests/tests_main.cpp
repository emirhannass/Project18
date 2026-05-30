/**
 * @file tests_main.cpp
 * @brief Tum alt test modüllerini tek çati altinda calistiran ana test motoru.
 */

#include <iostream>

// Dışarıdaki test fonksiyonlarının prototipleri
void run_unit_tests();
void run_integration_tests();
void run_edge_case_tests();

int main() {
    std::string separator = "==================================================";
    std::cout << separator << "\n";
    std::cout << "          CSE 211 - TESTING SUITE RUNNING         \n";
    std::cout << separator << "\n";

    // Test suitlerini sırayla çalıştır
    run_unit_tests();
    run_integration_tests();
    run_edge_case_tests();

    std::cout << "\n" << separator << "\n";
    std::cout << "   [SUCCESS] ALL PROJECT TESTS PASSED SUCCESSFULLY! \n";
    std::cout << separator << "\n";

    return 0;
}