/*******************************************************************************************
 *
 *   raylib [core] examples - basic screen manager
 *
 *   NOTE: This example illustrates a very simple screen manager based on a states machines
 *
 *   Example originally created with raylib 4.0, last time updated with raylib 4.0
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2021-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen
{
    INICIO = 0,
    JUGAR,
    OPCIONES,
    CREDITOS
} GameScreen;

#define screenWidth 800
#define screenHeight 450

Texture2D backgroundTexture;
void LoadContent();
void UnloadContent();
void menudraw(GameScreen currentScreen);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "SIGN MATCH - GAME");

    GameScreen currentScreen = INICIO;

    // TODO: Initialize all required variables and load all required data here!
    LoadContent();
    SetTargetFPS(60); // Set desired framerate (frames-per-second)

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch (currentScreen)
        {
        case INICIO: // AQUI EN ESTA OPCION ESTARA EL MENU.
        {
            if (IsKeyPressed(KEY_A))
            {
                currentScreen = JUGAR;
            }
            if (IsKeyPressed(KEY_S))
            {
                currentScreen = OPCIONES;
            }
            if (IsKeyPressed(KEY_D))
            {
                currentScreen = CREDITOS;
            }
        }
        break;
        case JUGAR:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                currentScreen = INICIO;
            }
        }
        break;
        case OPCIONES:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                currentScreen = INICIO;
            }
        }
        break;
        case CREDITOS:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                currentScreen = INICIO;
            }
        }
        break;
        default:
            break;
        }
        // Draw
        //----------------------------------------------------------------------------------
        menudraw(currentScreen);
    }

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadContent();

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void LoadContent()
{
    // Cargar la textura de fondo en el main
    backgroundTexture = LoadTexture("resources/signmatch-MENU.png");
}

void UnloadContent()
{
    // Liberar la textura de fondo al final del programa
    UnloadTexture(backgroundTexture);
}

void menudraw(GameScreen currentScreen)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (currentScreen)
    {
    case INICIO:
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        DrawText("INICIO SCREEN", 20, 20, 40, LIGHTGRAY);
        DrawText("PRESS A to JUMP to JUGAR SCREEN", 120, 220, 20, GRAY);
        DrawText("PRESS S to JUMP to OPCIONES SCREEN", 120, 240, 20, GRAY);
        DrawText("PRESS D to JUMP to CREDITOS SCREEN", 120, 260, 20, GRAY);
        DrawText("PRESS ESCAPE to EXIT", 120, 280, 20, GRAY);
        break;
    case JUGAR:
        DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
        DrawText("JUGAR SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, DARKGREEN);
        break;
    case OPCIONES:
        DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
        DrawText("OPCIONES SCREEN", 20, 20, 40, MAROON);
        DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, MAROON);
        break;
    case CREDITOS:
        DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
        DrawText("CREDITOS SCREEN", 20, 20, 40, DARKBLUE);
        DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, DARKBLUE);
        break;
    default:
        break;
    }

    EndDrawing();
    // ---- CERRAR ARCHIVOS ABIERTOS
}
