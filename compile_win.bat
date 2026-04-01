@echo off
echo Compiling for Windows...
gcc -c src/sqlite3.c -o sqlite3.o -Os
g++ -std=c++11 src/*.cpp sqlite3.o -o LabReservation.exe -static
del sqlite3.o
echo Done! Run LabReservation.exe
pause