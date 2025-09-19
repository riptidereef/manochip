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

// DONE //

// LD Vx, byte
void CPU::OP_6xkk() {
    // Set Vx = kk.
    // The interpreter puts the value kk into register Vx.

}

// ADD Vx, byte
void CPU::OP_7xkk() {
    // Set Vx = Vx + kk.
    // Adds the value kk to the value of register Vx, then stores 
    // the result in Vx.

}

// LD Vx, Vy
void CPU::OP_8xy0() {
    // Set Vx = Vy.
    // Stores the value of register Vy in register Vx.
}

// OR Vx, Vy
void CPU::OP_8xy1() {
    // Set Vx = Vx OR Vy.
    // Performs a bitwise OR on the values of Vx and Vy, then stores the
    // result in Vx. A bitwise OR compares the corrseponding bits from 
    // two values, and if either bit is 1, then the same bit in the result 
    // is also 1. Otherwise, it is 0.

}

// AND Vx, Vy
void CPU::OP_8xy2() {
    // Set Vx = Vx AND Vy.
    // Performs a bitwise AND on the values of Vx and Vy, then stores the 
    // result in Vx. A bitwise AND compares the corrseponding bits from 
    // two values, and if both bits are 1, then the same bit in the result
    // is also 1. Otherwise, it is 0. 

}

// XOR Vx, Vy
void CPU::OP_8xy3() {
    // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores
    // the result in Vx. An exclusive OR compares the corrseponding bits from 
    // two values, and if the bits are not both the same, then the corresponding
    // bit in the result is set to 1. Otherwise, it is 0. 
}

// ADD Vx, Vy
void CPU::OP_8xy4() {
    // Set Vx = Vx + Vy, set VF = carry.
    // The values of Vx and Vy are added together. If the result is greater
    // than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 
    // 8 bits of the result are kept, and stored in Vx.
}

// SUB Vx, Vy
void CPU::OP_8xy5() {
    // Set Vx = Vx - Vy, set VF = NOT borrow.
    // If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, 
    // and the results stored in Vx.
}

// SHR Vx {, Vy}
void CPU::OP_8xy6() {
    // Set Vx = Vx SHR 1.
    // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. 
    // Then Vx is divided by 2.
}

// SUBN Vx, Vy
void CPU::OP_8xy7() {
    // Set Vx = Vy - Vx, set VF = NOT borrow.
    // If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
    // and the results stored in Vx.
}

// SHL Vx {, Vy}
void CPU::OP_8xyE() {
    // Set Vx = Vx SHL 1.
    // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. 
    // Then Vx is multiplied by 2.
}

// SNE Vx, Vy
void CPU::OP_9xy0() {
    // Skip next instruction if Vx != Vy.
    // The values of Vx and Vy are compared, and if they are not equal, 
    // the program counter is increased by 2.
}

// LD I, addr
void CPU::OP_Annn() {
    // Set I = nnn.
    // The value of register I is set to nnn.
}

// JP V0, addr
void CPU::OP_Bnnn() {
    // Jump to location nnn + V0.
    // The program counter is set to nnn plus the value of V0.
}

// RND Vx, byte
void CPU::OP_Cxkk() {
    // Set Vx = random byte AND kk.
    // The interpreter generates a random number from 0 to 255, 
    // which is then ANDed with the value kk. The results are stored in Vx. 
    // See instruction 8xy2 for more information on AND.
}

// DRW Vx, Vy, nibble
void CPU::OP_Dxyn() {
    // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    // The interpreter reads n bytes from memory, starting at the address stored in I. 
    // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
    // Sprites are XORed onto the existing screen. If this causes any pixels to be erased, 
    // VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it 
    // is outside the coordinates of the display, it wraps around to the opposite side of the 
    // screen. See instruction 8xy3 for more information on XOR, and section 2.4, 
    // Display, for more information on the Chip-8 screen and sprites.
}

// SKP Vx
void CPU::OP_Ex9E() {
    // Skip next instruction if key with the value of Vx is pressed.
    // Checks the keyboard, and if the key corresponding to the value of Vx 
    // is currently in the down position, PC is increased by 2.
}

// SKNP Vx
void CPU::OP_ExA1() {
    // Skip next instruction if key with the value of Vx is not pressed.
    // Checks the keyboard, and if the key corresponding to the value of Vx 
    // is currently in the up position, PC is increased by 2.
}

// LD Vx, DT
void CPU::OP_Fx07() {
    // Set Vx = delay timer value.
    // The value of DT is placed into Vx.
}

// LD Vx, K
void CPU::OP_Fx0A() {
    // Wait for a key press, store the value of the key in Vx.
    // All execution stops until a key is pressed, then the value of that key is stored in Vx.
}

// LD DT, Vx
void CPU::OP_Fx15() {
    // Set delay timer = Vx.
    // DT is set equal to the value of Vx.
}

// LD ST, Vx
void CPU::OP_Fx18() {
    // Set sound timer = Vx.
    // ST is set equal to the value of Vx.
}

// ADD I, Vx
void CPU::OP_Fx1E() {
    // Set I = I + Vx.
    // The values of I and Vx are added, and the results are stored in I.
}

// LD F, Vx
void CPU::OP_Fx29() {
    // Set I = location of sprite for digit Vx.
    // The value of I is set to the location for the hexadecimal sprite 
    // corresponding to the value of Vx. See section 2.4, Display, for more
    // information on the Chip-8 hexadecimal font.
}

// LD B, Vx
void CPU::OP_Fx33() {
    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
    // The interpreter takes the decimal value of Vx, and places the hundreds
    // digit in memory at location in I, the tens digit at location I+1, 
    // and the ones digit at location I+2.
}

// LD [I], Vx
void CPU::OP_Fx55() {
    // Store registers V0 through Vx in memory starting at location I.
    // The interpreter copies the values of registers V0 through Vx into memory,
    // starting at the address in I.
}

// LD Vx, [I]
void CPU::OP_Fx65() {
    // Read registers V0 through Vx from memory starting at location I.
    // The interpreter reads values from memory starting at location I
    // into registers V0 through Vx.
}
