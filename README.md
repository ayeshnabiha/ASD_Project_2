# Lab Reservation System (CLI + Dashboard)

A C++ laboratory reservation system with two interfaces:

- **CLI application** for terminal-based reservation management.
- **HTTP server + web dashboard** for browser-based access.

Both interfaces share the same core logic and SQLite database, so reservation behavior stays consistent.

## Description

This project manages computer lab reservations using custom data structures and SQLite persistence.

Main capabilities:

- User login with 6-digit NIU.
- Create new reservation with validation (date, time, duration).
- Detect time-slot conflicts.
- View active reservation queue (FIFO behavior).
- View reservation history (LIFO behavior via stack).
- Dashboard API for queue/history/statistics and reservation creation.

## Requirements

### Runtime / Build tools

- `g++` (C++11 or newer)
- `gcc`
- `make`
- `curl` (optional, to download `httplib.h`)

### External header dependency

If `httplib.h` is not present in the project root, download it (it is already included in this repo):

```bash
curl -o httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
```

### Platform notes

- macOS / Linux: build uses `-lpthread -ldl`
- Windows (via Makefile OS detection): builds `.exe` targets
- For Windows, use **MSYS2/MinGW** (recommended) so `make`, `gcc`, and `g++` are available.

## Project Structure

```text
.
├── makefile                  # Main build (CLI + Server)
├── server.cpp                # HTTP API server entrypoint
├── httplib.h                 # cpp-httplib header
├── reservations.db           # SQLite database file
├── src/
│   ├── main.cpp              # CLI entrypoint
│   ├── database.cpp
│   ├── linkedList.cpp
│   ├── queue.cpp
│   ├── stack.cpp
│   ├── validation.cpp
│   └── sqlite3.c
├── include/
│   ├── database.h
│   ├── linkedList.h
│   ├── queue.h
│   ├── stack.h
│   ├── validation.h
│   └── sqlite3.h
└── dashboard/
		└── index.html            # Dashboard UI
```

## How to Use

### 1) Build

From project root:

```bash
make all
```

Build specific target only:

```bash
make cli      # builds LabReservation
make server   # builds LabReservationServer
```

Clean build artifacts:

```bash
make clean
```

### Windows build (option A: recommended, via make)

In an MSYS2 MinGW terminal, from project root:

```bash
make clean
make all
```

Output on Windows:

- `LabReservation.exe`
- `LabReservationServer.exe`

### Windows build (option B: via batch file)

From Command Prompt / PowerShell in project root:

```bat
compile_win.bat
```

This also builds both executables:

- `LabReservation.exe`
- `LabReservationServer.exe`

### 2) Run CLI app

```bash
./LabReservation
```

Windows:

```bat
LabReservation.exe
```

Or via make:

```bash
make run-terminal
```

### 3) Run dashboard server

```bash
./LabReservationServer
```

Windows:

```bat
LabReservationServer.exe
```

Or via make:

```bash
make run-server
```

Server listens on:

- `http://localhost:8080`

### 4) Open dashboard UI

Open in browser:

- `http://localhost:8080`

Fallback (if you open file directly):

- `dashboard/index.html`

Then sign in with a 6-digit NIU.

## API Overview

Base URL: `http://localhost:8080`

- `GET /api/reservations`
	- Returns active queue (upcoming reservations).
- `GET /api/reservations/all`
	- Returns all reservations (all statuses).
- `GET /api/reservations/history?niu=XXXXXX`
	- Returns reservation history for one NIU.
- `POST /api/reservations`
	- Creates new reservation after validation + conflict check.
- `GET /api/conflict?date=YYYY-MM-DD&time_start=HH:MM&duration=N`
	- Checks whether a slot conflicts without creating reservation.

## Validation Rules

Applied consistently across CLI and server:

- NIU must be 6 digits.
- Date must be valid calendar date (range 2000–2100).
- Date in the past is rejected.
- Start time must be valid `HH:MM`.
- For today, start time must be in the future.
- Duration must be `1..1440` minutes.
- Conflicting time slots are rejected.

## Architecture

### High-level flow

1. **User interface layer**
	 - CLI (`src/main.cpp`) or Dashboard (`dashboard/index.html`)
2. **Service/API layer**
	 - HTTP server (`server.cpp`) for dashboard calls
3. **Core domain/data-structure layer**
	 - Linked List as base structure
	 - Queue for active reservation ordering
	 - Stack for history ordering
4. **Persistence layer**
	 - SQLite via `database.cpp` + `sqlite3.c`

### Data structures

- **Linked List**: base node storage and traversal operations
- **Queue (FIFO)**: reservation queue visualization and active scheduling flow
- **Stack (LIFO)**: history behavior (latest reservation first)

### Consistency model

- CLI and dashboard server use shared validation/database modules.
- Both interfaces read/write the same `reservations.db`.
- Business rules (conflict checking, date/time validation) are centralized in core modules.

## Troubleshooting

- **Server offline in dashboard**
	- Make sure `./LabReservationServer` is running.
- **Port 8080 already in use**
	- Stop old server process, then restart.
- **`httplib.h` not found at compile time**
	- Download it into project root using the `curl` command above.

## License

This repository currently has no explicit license file.