# Derleyici ve Bayraklar
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
# Eğer nlohmann/json gibi kütüphaneler ekleyeceksen -Iinclude kısmı onları görmeni sağlar

# Klasör Yapısı
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

# Hedef Uygulama Adı
TARGET = $(BIN_DIR)/crew_roster

# Kaynak ve Nesne Dosyaları
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Varsayılan Hedef (make)
all: $(TARGET)

# Bağlama (Linking)
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Derleme (Compiling)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Temizlik (make clean)
clean:
	@echo "Temizleniyor..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Test (make test)
test: $(TARGET)
	@echo "Testler calistiriliyor..."
	./$(TARGET) --test # Veya kendi test script'iniz