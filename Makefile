CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Ana uygulama kaynak dosyaları
SRC = src/core/CrewRosterManager.cpp src/core/PairingGenerator.cpp src/utils/FileParser.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = app

# Test kaynak dosyaları
TEST_SRC = src/core/CrewRosterManager.cpp src/core/PairingGenerator.cpp src/utils/FileParser.cpp tests/test_main.cpp
TEST_OBJ = src/core/CrewRosterManager.o src/core/PairingGenerator.o src/utils/FileParser.o tests/test_main.o
TEST_TARGET = run_tests

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# make test hedefi (Şartnameye uygun derleme ve çalıştırma)
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

libproject.so: src/core/CrewRosterManager.cpp src/core/PairingGenerator.cpp src/utils/FileParser.cpp src/frontend_bridge.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o libproject.so $^

memcheck: $(TEST_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_TARGET)

clean:
	rm -f src/core/*.o src/utils/*.o src/*.o tests/*.o $(TARGET) $(TEST_TARGET) libproject.so