/**
 * @file FileParser.h
 * @brief Örnek statik veri yükleyici yardımcı sınıf tanımı.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#pragma once
#include <string>
#include "core/CrewRosterManager.h"

namespace utils {

class FileParser {
public:
    // PROJ-18.pdf içerisindeki örnek verileri sisteme doğrudan simüle ederek yükler [cite: 2150]
    static bool loadMockData(core::CrewRosterManager& manager, const std::string& filename);
};

} // namespace utils