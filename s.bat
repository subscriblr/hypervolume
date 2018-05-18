g++.exe 1.cpp -O2 -std=c++11 -Dhome -Wall -static -Wl,--stack=500000000 -Wsign-compare -std=c++11 -o 1.exe
start cmd /k 1.exe
