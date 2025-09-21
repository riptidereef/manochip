#include <cstdint>
#include <random>
#include <chrono>
#include <raylib.h>
using namespace std;

class CPU {
public:
    CPU();
    void LoadROM(const char* filename);
    static constexpr uint8_t SCREEN_WIDTH = 64;
    static constexpr uint8_t SCREEN_HEIGHT = 32;
    void Cycle();
    void SetKey(uint8_t key, uint8_t value);

    Color* GetColorBuffer() {
        return &video[0][0];
    }
    
    Color onColor = GREEN;
    Color offColor = BLACK;

private:
    uint8_t memory[4096] {};
    uint8_t registers[16] {};
    uint16_t index {};
    uint16_t pc {};
    uint16_t stack[16] {};
    uint8_t sp {};
    uint8_t delay_timer {};
    uint8_t sound_timer {};
    uint8_t keypad[16] {};
    Color video[SCREEN_HEIGHT][SCREEN_WIDTH] {BLACK};
    uint16_t opcode;
    static constexpr uint16_t START_ADDR = 0x200;

    static constexpr size_t FONTSET_SIZE = 80;
    static constexpr uint16_t FONTSET_ADDR = 0x50;
    uint8_t fontset[FONTSET_SIZE] =
    {
	    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	    0x20, 0x60, 0x20, 0x20, 0x70, // 1
	    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    default_random_engine randGen;
    uniform_int_distribution<uint8_t> randByte;

    void OP_00E0();
    void OP_00EE();
    void OP_0nnn();
    void OP_1nnn();
    void OP_2nnn();
    void OP_3xkk();
    void OP_4xkk();
    void OP_5xy0();
    void OP_6xkk();
    void OP_7xkk();
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();
    void OP_9xy0();
    void OP_Annn();
    void OP_Bnnn();
    void OP_Cxkk();
    void OP_Dxyn();
    void OP_Ex9E();
    void OP_ExA1();
    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();
    void OP_none();
};