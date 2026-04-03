@echo off
setlocal
echo Compiling for Windows...

gcc -c src/sqlite3.c -o sqlite3.o -Iinclude -Os
if errorlevel 1 goto :build_failed

echo Building CLI executable...
g++ src/*.cpp sqlite3.o -Iinclude -o LabReservation.exe -static -std=c++11 -lws2_32
if errorlevel 1 goto :build_failed

echo Building dashboard server executable...
g++ src/database.cpp src/linkedList.cpp src/queue.cpp src/stack.cpp src/validation.cpp server.cpp sqlite3.o -Iinclude -o LabReservationServer.exe -static -std=c++11 -lws2_32
if errorlevel 1 goto :build_failed

del /Q *.o >nul 2>nul
echo.
echo Build successful.
echo - CLI    : LabReservation.exe
echo - Server : LabReservationServer.exe
echo.
echo Run CLI with:    LabReservation.exe
echo Run Server with: LabReservationServer.exe
pause
exit /b 0

:build_failed
echo.
echo Build failed.
pause
exit /b 1