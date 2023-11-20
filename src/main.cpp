/*******************************************************************************************
 *
 *   raylib [textures] example - sprite button
 *
 *   Example originally created with raylib 2.5, last time updated with raylib 2.5
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2019-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"

#define NUM_FRAMES 3 // Number of frames (rectangles) for the button sprite texture
#define screenWidth 800
#define screenHeight 450

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "SIGN MATCH GAME");

    InitAudioDevice(); // Initialize audio device

    Sound fxButton = LoadSound("audio/resources/buttonsound.wav"); // Load button sound
    Texture2D button = LoadTexture("resources/startbutton.png");   // Load button texture

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height / NUM_FRAMES;
    Rectangle sourceRec = {0, 0, (float)button.width, frameHeight};

    // Define button bounds on screen
    Rectangle btnBounds = {screenWidth / 2.0f - button.width / 2.0f, screenHeight / 2.0f - button.height / NUM_FRAMES / 2.0f, (float)button.width, frameHeight};

    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false; // Button action should be activated

    Vector2 mousePoint = {0.0f, 0.0f};

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();
        btnAction = false;

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                btnState = 2;
            else
                btnState = 1;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                btnAction = true;
        }
        else
            btnState = 0;

        if (btnAction)
        {
            PlaySound(fxButton);

            // TODO: Any desired action
        }

        // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState * frameHeight;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureRec(button, sourceRec, (Vector2){btnBounds.x, btnBounds.y}, RAYWHITE); // Draw button frame

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(button); // Unload button texture
    UnloadSound(fxButton); // Unload sound

    CloseAudioDevice(); // Close audio device

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}