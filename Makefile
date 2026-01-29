CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC = src/main.cpp src/FileScanner.cpp
OUT = indexer

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)