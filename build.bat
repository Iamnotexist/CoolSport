g++ lib\date.cpp -c -o lib\date.o
g++ lib\provod2.cpp -c -o lib\provod2.o
g++ Sport.cpp -c -o Sport.o
g++ -o CoolSport.exe Sport.o lib\date.o lib\provod2.o
del /Q lib\date.o
del /Q lib\provod2.o
del /Q Sport.o
