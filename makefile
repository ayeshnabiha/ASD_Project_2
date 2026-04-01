# Variables
CC = gcc
CXX = g++
CFLAGS = -Iinclude -O2 -c
CXXFLAGS = -std=c++11 -Iinclude -O2
LDFLAGS = -lpthread -ldl
TARGET = LabReservation

# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    LDFLAGS = -static
endif

all: $(TARGET)

# Step 1: Compile SQLite Engine
sqlite3.o: src/sqlite3.c
	$(CC) $(CFLAGS) src/sqlite3.c -o sqlite3.o

# Step 2: Compile logic and Link (Added -Iinclude here)
$(TARGET): sqlite3.o
	$(CXX) $(CXXFLAGS) src/*.cpp sqlite3.o -o $(TARGET) $(LDFLAGS)
	@rm -f sqlite3.o
	@echo "Build Successful: ./$(TARGET)"

clean:
	rm -f $(TARGET) $(TARGET).exe *.o