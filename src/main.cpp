#include <raylib.h>
#include "cpu.h"

void HandleInput(CPU& cpu);

int main(void)
{
    const int screenWidth = 64 * 10;
    const int screenHeight = 32 * 10;
    InitWindow(screenWidth, screenHeight, "ManoChip");

    SetTargetFPS(120);

    CPU cpu;
    cpu.LoadROM("../data/Worm V4 [RB-Revival Studios, 2007].ch8");

    int ms = 0;

    Texture2D screenTex = LoadTextureFromImage(GenImageColor(CPU::SCREEN_WIDTH, CPU::SCREEN_HEIGHT, BLACK));

    while (!WindowShouldClose())
    {
        
        ms += GetFrameTime() * 1000.0f;
        if (ms > 5.0f) {
            ms = 0.0f;
            HandleInput(cpu);
            cpu.Cycle();
        }

        UpdateTexture(screenTex, cpu.GetColorBuffer());

        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(
                screenTex,
                {0, 0, (float)CPU::SCREEN_WIDTH, (float)CPU::SCREEN_HEIGHT},
                {0, 0, (float)screenWidth, (float)screenHeight},
                {0, 0},
                0,
                WHITE
            );
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void HandleInput(CPU& cpu) {
    cpu.SetKey(0x1, IsKeyDown(KEY_ONE));
    cpu.SetKey(0x2, IsKeyDown(KEY_TWO));
    cpu.SetKey(0x3, IsKeyDown(KEY_THREE));
    cpu.SetKey(0xC, IsKeyDown(KEY_FOUR));

    cpu.SetKey(0x4, IsKeyDown(KEY_Q));
    cpu.SetKey(0x5, IsKeyDown(KEY_W));
    cpu.SetKey(0x6, IsKeyDown(KEY_E));
    cpu.SetKey(0xD, IsKeyDown(KEY_R));

    cpu.SetKey(0x7, IsKeyDown(KEY_A));
    cpu.SetKey(0x8, IsKeyDown(KEY_S));
    cpu.SetKey(0x9, IsKeyDown(KEY_D));
    cpu.SetKey(0xE, IsKeyDown(KEY_F));

    cpu.SetKey(0xA, IsKeyDown(KEY_Z));
    cpu.SetKey(0x0, IsKeyDown(KEY_X));
    cpu.SetKey(0xB, IsKeyDown(KEY_C));
    cpu.SetKey(0xF, IsKeyDown(KEY_V));
}