#include "cpu.h"
#include <fstream>
#include <cstring>
#include <iostream>


using namespace std;

CPU::CPU() {
    cout << "Making a CPU." << endl;
    
    randGen.seed(chrono::system_clock::now().time_since_epoch().count());

    pc = START_ADDR;

    memcpy(&memory[FONTSET_ADDR], fontset, FONTSET_SIZE);

    randByte = uniform_int_distribution<uint8_t>(0, 255U);
}

void CPU::LoadROM(const char* filename) {
    ifstream file(filename, ios::binary | ios::ate);

    if (file.is_open()) {
        streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, ios::beg);
        file.read(buffer, size);
        file.close();

        memcpy(&memory[START_ADDR], buffer, size);

        delete[] buffer;

        cout << "Loaded file!" << endl;
    }
}