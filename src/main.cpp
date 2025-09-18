#include <raylib.h>
#include "cpu.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "ManoChip");

    SetTargetFPS(60);

    CPU cpu;
    cpu.LoadROM("../data/Breakout (Brix hack) [David Winter, 1997].ch8");

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}