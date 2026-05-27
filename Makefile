CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Klasör Yapısı
BUILD_DIR = build
BIN_DIR = bin

# Ana Uygulama Hedefleri (Nesne dosyaları artık build/ altına gidecek)
OBJS = $(BUILD_DIR)/CrewRosterManager.o \
       $(BUILD_DIR)/PairingGenerator.o \
       $(BUILD_DIR)/FileParser.o \
       $(BUILD_DIR)/main.o

TARGET = $(BIN_DIR)/app

# Test Uygulaması Hedefleri
# (Not: Yüklediğin test dosyasının adı 'tests_main.cpp' olduğu için burayı ona göre eşitledim)
TEST_OBJS = $(BUILD_DIR)/CrewRosterManager.o \
            $(BUILD_DIR)/PairingGenerator.o \
            $(BUILD_DIR)/FileParser.o \
            $(BUILD_DIR)/tests_main.o

TEST_TARGET = $(BIN_DIR)/run_tests

# Varsayılan Hedef
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

# Tekil .cpp Dosyalarının build/ Klasörüne Derlenmesi
$(BUILD_DIR)/CrewRosterManager.o: src/core/CrewRosterManager.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/PairingGenerator.o: src/core/PairingGenerator.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/FileParser.o: src/utils/FileParser.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests_main.o: tests/tests_main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Uygulamayı bin klasöründen çalıştırma kısa yolu
run: $(TARGET)
	./$(TARGET)

# Gelişmiş Özellik: Frontend köprüsü için paylaşımlı kütüphane oluşturma
libproject.so: src/core/CrewRosterManager.cpp src/core/PairingGenerator.cpp src/utils/FileParser.cpp src/frontend_bridge.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o libproject.so $^
	@echo "[SUCCESS] Frontend köprüsü için libproject.so üretildi."

# Gelişmiş Özellik: Valgrind ile Bellek Sızıntısı Analizi
memcheck: $(TEST_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_TARGET)

# Tam Temizlik Kuralı
clean:
	rm -rf $(BUILD_DIR)/* $(BIN_DIR)/* libproject.so
	@echo "[CLEAN] build/, bin/ klasorleri ve shared kütüphane tamamen temizlendi."

.PHONY: all test run memcheck clean create_dirs