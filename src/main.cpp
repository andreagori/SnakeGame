/*******************************************************************************************
 *
 *   raylib game - snake.
 *
 *   Example originally created with raylib 1.5, last time updated with raylib 3.0
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2016-2024 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/
// LIBRERIAS
#include "viborita.h"

// ESTRUCTURAS Y DEFINES.
#define screenWidth 450
#define screenHeight 450

// PANTALLAS
typedef enum GameScreen
{
    MENU,
    GAME,
} GameScreen;

typedef struct files
{
    Texture2D background;
} files;

// DECLARACION DE FUNCIONES

files load(files assets);
void unload(files assets);
void animetext(const char *texto, int fontsize, int textWidth, int textHeight, files assets);
void grid(int height, int weight);
void DrawMenu(files assets, GameScreen pantalla);
void Drawgame(Snake *snake, Apple *apple, int startX, int startY);

//------------------------------------------------------------------------------------
// FUNCION INT MAIN.
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Viboritaz - GAME");
    SetTargetFPS(60);
    InitAudioDevice();

    Image icon = LoadImage("resources/UABC_ANDREA_VIBORITAZ.png");
    SetWindowIcon(icon);
    GameScreen currentScreen = MENU;
    files assets;
    assets = load(assets);
    Snake snake;
    Apple apple;
    int startX = (screenWidth - 360) / 2;
    int startY = (screenHeight - 360) / 2;

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch (currentScreen)
        {
        case MENU:
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                initSnake(&snake, 165, 213);
                initApple(&apple, 360, 360, 15, 15, startX, startY); // Initialize the apple
                // initApple(&apple, 150, 200);
                // randomApple(&apple.axisX, &apple.axisY, 15, 15, 360, 360);
                currentScreen = GAME;
            }
        }
        break;
        case GAME:
        {
            if (IsKeyPressed(KEY_TAB))
            {
                currentScreen = MENU;
            }
            if (collisions(&snake, 15, 15, 360, 360) != false)
            {
                currentScreen = MENU;
                freeSnake(&snake);
            }
            else
            {
                if (hasEatenFood(&snake, &apple, 15, 15))
                {
                    growSnake(&snake, 15, 15);
                    initApple(&apple, 360, 360, 15, 15, startX, startY); // Reinitialize the apple
                }
                Drawgame(&snake, &apple, startX, startY);
            }
            break;
        }
            // Update
            //----------------------------------------------------------------------------------
        }
        // Dibujar la pantalla actual
        if (currentScreen == MENU)
        {
            DrawMenu(assets, currentScreen);
        }

        // if (currentScreen == GAME)
        // {
        //     Drawgame(&snake, &apple, startX, startY);
        // }
    }
    unload(assets);
    UnloadImage(icon);
    UnloadTexture(apple.appledrawing);
    freeSnake(&snake);
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

files load(files assets)
{
    assets.background = LoadTexture("resources/UABC_ANDREA_FONDO.png");
    return assets;
}

void unload(files assets)
{
    UnloadTexture(assets.background);
}

void animetext(const char *texto, int fontSize, int textWidth, int textHeight, files assets)
{
    static int originalSize = fontSize;
    static int timer = 0;

    // Incrementar el temporizador
    timer++;

    // Obtener la posición del cursor
    Vector2 mousePosition = GetMousePosition();

    // Crear un rectángulo del área ocupada por el texto
    Rectangle textRect = {
        (GetScreenWidth() - textWidth) / 2,   // posición x del texto
        (GetScreenHeight() - textHeight) / 2, // posición y del texto
        textWidth,                            // ancho del texto
        textHeight                            // height del texto
    };

    // Verificar si el cursor está sobre el área ocupada por el texto
    bool isMouseOverText = CheckCollisionPointRec(mousePosition, textRect);

    // Si el cursor está sobre el texto, no aplicar animación
    if (isMouseOverText)
    {
        DrawText(texto, (GetScreenWidth() - textWidth) / 2, (GetScreenHeight() - textHeight) / 2, fontSize, YELLOW);
    }
    else
    {
        // Si el cursor no está sobre el texto, aplicar animación
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());

        // Aplicar la escala al tamaño del texto
        int newFontSize = fontSize * scale;

        // Dibujar el texto con el nuevo tamaño
        DrawText(texto, (GetScreenWidth() - textWidth * scale) / 2, (GetScreenHeight() - textHeight * scale) / 2, newFontSize, YELLOW);
    }
}

void grid(int height, int weight)
{
    // Dibuja el grid de fondo gris.
    for (int i = 0; i < weight; i += 20)
    {
        DrawLine(i, 0, i, height, Fade(LIGHTGRAY, 0.1f));
    }
    for (int i = 0; i < height; i += 20)
    {
        DrawLine(0, i, weight, i, Fade(LIGHTGRAY, 0.1f));
    }
}

void DrawMenu(files assets, GameScreen pantalla)
{
    BeginDrawing();
    DrawTexturePro(
        assets.background,
        (Rectangle){0, 0, (float)assets.background.width, (float)assets.background.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // DrawText("Press ENTER to start", 50, 250, 30, YELLOW);
    animetext("Press ENTER to start", 25, 250, 20, assets);
    DrawText("made by: @andreagori", 80, 380, 10, BLACK);

    // Dibuja el grid de fondo gris.
    grid(450, 450);

    EndDrawing();
}

void gridSnake(int height_cellsnumber, int width_cellsnumber, int gridHeight, int gridWidth)
{
    // Implement the logic for gridSnake function.
    // For customization in color, check raylib library or use: CLITERAL(Color){ x, x, x, x }
    // Before everthing, we need to make sure the grid and rectangles will be in the middle.
    // Calculate the starting point to draw the grid, in the middle.
    int startX = (screenWidth - gridWidth) / 2;
    int startY = (screenHeight - gridHeight) / 2;

    // Draw the rectangle of the grid
    DrawRectangle(startX, startY, gridWidth, gridHeight, LIME);

    // Calculate the size of each cell.
    int cellWidth = gridWidth / width_cellsnumber;
    int cellHeight = gridHeight / height_cellsnumber;

    // Draw the grid lines
    for (int i = 0; i <= height_cellsnumber; ++i)
    {
        DrawLine(startX, startY + i * cellHeight, startX + gridWidth, startY + i * cellHeight, BLACK);
    }

    for (int j = 0; j <= width_cellsnumber; ++j)
    {
        DrawLine(startX + j * cellWidth, startY, startX + j * cellWidth, startY + gridHeight, BLACK);
    }
}

void Drawgame(Snake *snake, Apple *apple, int startX, int startY)
{
    BeginDrawing();
    ClearBackground(CLITERAL(Color){2, 43, 15, 255});
    DrawText("[TAB] para volver al menu", 10, 10, 15, YELLOW);

    // Dibuja el grid de fondo gris.
    grid(450, 450);
    gridSnake(15, 15, 360, 360);

    // Dibuja la serpiente y la manzana
    int cellWidth = 360 / 15; // Assuming a 15x15 grid
    int cellHeight = 360 / 15;

    if (hasEatenFood(snake, apple, 15, 15))
    {
        growSnake(snake, cellWidth, cellHeight);
        initApple(apple, 360, 360, 15, 15, startX, startY);
    }

    drawSnake(snake, 15, 15, 360, 360);
    drawApple(apple, snake->head, 15, 15, 360, 360, cellHeight, cellWidth, startX, startY);
    EndDrawing();
}