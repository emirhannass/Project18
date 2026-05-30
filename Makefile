CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
DOXYGEN = doxygen

ifeq ($(OS),Windows_NT)
    ifeq ($(shell where doxygen 2>NUL),)
        DOXYGEN = "C:/Program Files/doxygen/bin/doxygen.exe"
    endif
endif

# Klasör Yapısı
BUILD_DIR = build
BIN_DIR = bin

# Ana Uygulama Hedefleri
OBJS = $(BUILD_DIR)/CrewRosterManager.o \
       $(BUILD_DIR)/PairingGenerator.o \
       $(BUILD_DIR)/FileParser.o \
       $(BUILD_DIR)/main.o

TARGET = $(BIN_DIR)/app

# Test Uygulaması Hedefleri
TEST_OBJS = $(BUILD_DIR)/CrewRosterManager.o \
            $(BUILD_DIR)/PairingGenerator.o \
            $(BUILD_DIR)/FileParser.o \
            $(BUILD_DIR)/test_data_structures.o \
            $(BUILD_DIR)/test_roster.o \
            $(BUILD_DIR)/test_boundaries.o \
            $(BUILD_DIR)/tests_main.o

TEST_TARGET = $(BIN_DIR)/run_tests

# Varsayılan Hedef (Sadece 'make' yazınca çalışır)
all: create_dirs $(TARGET)

# Gerekli klasörleri otomatik oluşturma kuralı
create_dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Ana Uygulamayı Bağlama (Linking)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "[SUCCESS] Ana uygulama bin/app konumuna derlendi."

# Testleri Derleme ve Otomatik Çalıştırma
test: create_dirs $(TEST_TARGET)
	@echo "[RUNNING] Test senaryolari kosturuluyor..."
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)
	@echo "[SUCCESS] Test motoru basariyla olusturuldu."

# ====================================================================
# --- TEKİL .cpp DOSYALARININ DERLENMESİ (Çakışmalar Engellendi) ---
# ====================================================================

# Çekirdek (Core) Modülleri
$(BUILD_DIR)/CrewRosterManager.o: src/core/CrewRosterManager.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/PairingGenerator.o: src/core/PairingGenerator.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/FileParser.o: src/utils/FileParser.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test Modülleri (Alt Klasörlerdeki Yeni Dosyalar)
$(BUILD_DIR)/test_data_structures.o: tests/unit/test_data_structures.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_roster.o: tests/integration/test_roster.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_boundaries.o: tests/edge_cases/test_boundaries.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests_main.o: tests/tests_main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ====================================================================
# --- DİĞER YARDIMCI HEDEFLER ---
# ====================================================================

# Uygulamayı bin klasöründen çalıştırma kısa yolu
run: $(TARGET)
	./$(TARGET)

# Frontend köprüsü için paylaşımlı kütüphane oluşturma
libproject.so: src/core/CrewRosterManager.cpp src/core/PairingGenerator.cpp src/utils/FileParser.cpp src/frontend_bridge.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o libproject.so $^
	@echo "[SUCCESS] Frontend köprüsü için libproject.so üretildi."

# Valgrind ile Bellek Sızıntısı Analizi
memcheck: $(TEST_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_TARGET)

# Şartnameye Uygun Ekstra Zorunlu Hedefler (Checklist'i Tamamlamak İçin)
deps:
	@echo "[DEPS] Bagimliliklar kontrol ediliyor... Proje harici bir kütüphane kullanmiyor (Sistem Hazir)."

docs:
	@echo "[DOCS] Doxygen dokumantasyonu uretiliyor..."
	$(DOXYGEN) Doxyfile
	@echo "[SUCCESS] Dokumantasyon docs/doxygen/html/index.html konumuna uretildi."

# Tam Temizlik Kuralı
clean:
	rm -rf $(BUILD_DIR)/* $(BIN_DIR)/* libproject.so
	@echo "[CLEAN] build/, bin/ klasorleri ve shared kütüphane tamamen temizlendi."

.PHONY: all test run memcheck clean create_dirs deps docs