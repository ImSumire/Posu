#include <math.h>
#include <stdio.h>

#include <deque>
#include <random>

#include <raylib.h>

#include "posu.hpp"


const int W = 1280;
const int H = 720;
const int CW = W / 2;
const int CH = H / 2;


int main() {
    InitWindow(W, H, "Prodecural Osu!");
    InitAudioDevice();
    SetTargetFPS(60);

    Posu posu(CW, CH);
    posu.click_sound = LoadSound("assets/click.wav");

    while (!WindowShouldClose()) {
        // Update
        posu.update();

        // Render
        BeginDrawing();
            ClearBackground(BLACK);

            posu.render();
        EndDrawing();
    }

    CloseWindow();
}
