@echo off
echo Compiling for Windows...

gcc -c src/sqlite3.c -o sqlite3.o -Iinclude -Os

g++ src/*.cpp sqlite3.o -Iinclude -o LabReservation.exe -static -std=c++11 -pthread

del *.o
echo Done! Run LabReservation.exe
pause