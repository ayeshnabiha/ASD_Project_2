# Variables
CC       = gcc
CXX      = g++
CFLAGS   = -Iinclude -O2 -c
CXXFLAGS = -std=c++11 -Iinclude -O2

TARGET     = LabReservation
SERVER     = LabReservationServer

# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    SERVER := $(SERVER).exe
    LDFLAGS        = -static -lws2_32
    LDFLAGS_SERVER = -lws2_32
else
    LDFLAGS        = -lpthread -ldl
    LDFLAGS_SERVER = -lpthread -ldl
endif

# ── Default: build everything ──────────────────────────────────────────────────
all: $(TARGET) $(SERVER)

# Convenience aliases
cli: $(TARGET)
server: $(SERVER)

# ── Step 1: compile SQLite C engine ───────────────────────────────────────────
sqlite3.o: src/sqlite3.c
	$(CC) $(CFLAGS) src/sqlite3.c -o sqlite3.o

# ── Step 2: original terminal app ─────────────────────────────────────────────
$(TARGET): sqlite3.o
	$(CXX) $(CXXFLAGS) src/*.cpp sqlite3.o -o $(TARGET) $(LDFLAGS)
	@echo "Built terminal app → ./$(TARGET)"

# ── Step 3: HTTP dashboard server ─────────────────────────────────────────────
# server.cpp lives at the repo root and links the same src/*.cpp except main.cpp
$(SERVER): sqlite3.o
	$(CXX) $(CXXFLAGS) \
	    $(filter-out src/main.cpp, $(wildcard src/*.cpp)) \
	    server.cpp sqlite3.o \
	    -o $(SERVER) $(LDFLAGS_SERVER)
	@echo "Built dashboard server → ./$(SERVER)"
	@echo "Open dashboard/index.html in your browser, then run ./$(SERVER)"

# ── Helpers ───────────────────────────────────────────────────────────────────
clean:
	rm -f $(TARGET) $(TARGET).exe $(SERVER) $(SERVER).exe *.o

run-terminal:
	./$(TARGET)

run-server:
	./$(SERVER)

.PHONY: all cli server clean run-terminal run-server
