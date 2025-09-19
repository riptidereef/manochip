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

// CLS
void CPU::OP_00E0() {
    // Clear the display.
    memset(video, 0, sizeof(video));
}

// RET
void CPU::OP_00EE() {
    // Return from a subroutine.
    // The interpreter sets the program counter to the address at
    // the top of the stack, then subtracts 1 from the stack pointer.
    sp--;
    pc = stack[sp];
}

// SYS addr
void CPU::OP_0nnn() {
    // Jump to a machine code routine at nnn.
    // This instruction is only used on the old computers on which 
    // Chip-8 was originally implemented. It is ignored by modern 
    // interpreters.
}

// JP addr
void CPU::OP_1nnn() {
    // Jump to location nnn.
    // The interpreter sets the program counter to nnn.
    uint16_t address = opcode & 0x0FFFu;
    stack[sp] = pc;
    sp++;
    pc = address;
}

// CALL addr
void CPU::OP_2nnn() {
    // Call subroutine at nnn.
    // The interpreter increments the stack pointer, then puts the 
    // current PC on the top of the stack. The PC is then set to nnn.
    uint16_t address = opcode & 0x0FFFu;
    pc = address;
}

// 3xkk - SE Vx, byte
void CPU::OP_3xkk() {
    // Skip next instruction if Vx = kk.
    // The interpreter compares register Vx to kk, and if they are 
    // equal, increments the program counter by 2.
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    if (registers[Vx] == kk) {
        pc += 2;
    }
}

// SNE Vx, byte
void CPU::OP_4xkk() {
    // Skip next instruction if Vx != kk.
    // The interpreter compares register Vx to kk, and if they are
    // not equal, increments the program counter by 2.
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    if (registers[Vx] != kk) {
        pc += 2;
    }
}

// SE Vx, Vy
void CPU::OP_5xy0() {
    // Skip next instruction if Vx = Vy.
    // The interpreter compares register Vx to register Vy, and if
    // they are equal, increments the program counter by 2.
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] == registers[Vy]) {
        sp += 2;
    }
}

void CPU::OP_6xkk() {

}

void CPU::OP_7xkk() {

}

void CPU::OP_8xy0() {

}

void CPU::OP_8xy1() {

}

void CPU::OP_8xy2() {

}

void CPU::OP_8xy3() {

}

void CPU::OP_8xy4() {

}

void CPU::OP_8xy5() {

}

void CPU::OP_8xy6() {

}

void CPU::OP_8xy7() {

}

void CPU::OP_8xyE() {

}

void CPU::OP_9xy0() {

}

void CPU::OP_Annn() {

}

void CPU::OP_Bnnn() {

}

void CPU::OP_Cxkk() {

}

void CPU::OP_Dxyn() {

}

void CPU::OP_Ex9E() {

}

void CPU::OP_ExA1() {

}

void CPU::OP_Fx07() {

}

void CPU::OP_Fx0A() {

}

void CPU::OP_Fx15() {

}

void CPU::OP_Fx18() {

}

void CPU::OP_Fx1E() {

}

void CPU::OP_Fx29() {

}

void CPU::OP_Fx33() {

}

void CPU::OP_Fx55() {

}

void CPU::OP_Fx65() {

}
