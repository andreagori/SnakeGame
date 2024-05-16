/*******************************************************************************************
 *
 *   raylib game - snake.
 *   This game is a simple implementation of the classic snake game.
 *   School: Universidad Autonoma de Baja California. Facultad de Ingenieria, Arquitectura y Diseño.
 *   Subject: Paradigmas de Programacion. (Programming Paradigms). 2024-5.
 *   Author: Andrea.
 *
 *  NOTE. I really enjoyed this project, despite the headaches it gave me. I learned a lot about linked lists and pointers.
 *  I hope you enjoy it as much as I did. And I hope is well documented.
 *  I will be happy to receive feedback and suggestions for improvement.
 *  pS. Some function have uppercase letters because of the library used.
 ********************************************************************************************/

// LIBRARIES
#include "viborita.h"

// USING DEFINES
#define screenWidth 450
#define screenHeight 450

// STRUCTURES
// SCREENS
typedef enum GameScreen
{
    MENU,
    GAME,
} GameScreen;

// FILES STRUCTURE
typedef struct files
{
    Texture2D background;
} files;

// FUNCTIONS DECLARATIONS.

files load(files assets);
void unload(files assets);
void animetext(const char *texto, int fontsize, int textWidth, int textHeight, files assets);
void grid(int height, int weight);
void draw_menu(files assets, GameScreen pantalla);
void draw_game(Snake *snake, Apple *apple, int start_x, int start_y);

// Main function ------------------------
int main(void)
{
    // Initialization of the window ---------
    InitWindow(screenWidth, screenHeight, "Viboritaz - GAME");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    // Load the icon and assets as well as the snake and apple.
    Image icon = LoadImage("resources/UABC_ANDREA_VIBORITAZ.png");
    SetWindowIcon(icon);
    GameScreen current_screen = MENU;
    files assets;
    assets = load(assets);
    Snake snake;
    Apple apple;

    // Calculate the starting point to draw the grid, in the middle.
    int start_x = (screenWidth - 360) / 2;
    int start_y = (screenHeight - 360) / 2;

    // Main game loop ---------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update the game logic ---------
        switch (current_screen)
        {
        case MENU:
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                init_snake(&snake, 165, 213);                           // Initialize the snake
                init_apple(&apple, 360, 360, 15, 15, start_x, start_y); // Initialize the apple
                current_screen = GAME;
            }
        }
        break;
        case GAME:
        {
            if (IsKeyPressed(KEY_TAB))
            {
                current_screen = MENU;
            }
            if (collisions(&snake, 15, 15, 360, 360) != false)
            {
                current_screen = MENU;
                free_snake(&snake);
            }
            else
            {
                if (has_eaten_food(&snake, &apple, 15, 15))
                {
                    grow_snake(&snake, 15, 15);
                    init_apple(&apple, 360, 360, 15, 15, start_x, start_y);
                }
                draw_game(&snake, &apple, start_x, start_y);
            }
            break;
        }
            // Update
            //----------------------------------------------------------------------------------
        }
        // Dibujar la pantalla actual
        if (current_screen == MENU)
        {
            draw_menu(assets, current_screen);
        }
    }
    unload(assets);
    UnloadImage(icon);
    UnloadTexture(apple.appledrawing);
    free_snake(&snake);
    free_apple(&apple);
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

void draw_menu(files assets, GameScreen pantalla)
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
    int start_x = (screenWidth - gridWidth) / 2;
    int start_y = (screenHeight - gridHeight) / 2;

    // Draw the rectangle of the grid
    DrawRectangle(start_x, start_y, gridWidth, gridHeight, LIME);

    // Calculate the size of each cell.
    int cellWidth = gridWidth / width_cellsnumber;
    int cellHeight = gridHeight / height_cellsnumber;

    // Draw the grid lines
    for (int i = 0; i <= height_cellsnumber; ++i)
    {
        DrawLine(start_x, start_y + i * cellHeight, start_x + gridWidth, start_y + i * cellHeight, BLACK);
    }

    for (int j = 0; j <= width_cellsnumber; ++j)
    {
        DrawLine(start_x + j * cellWidth, start_y, start_x + j * cellWidth, start_y + gridHeight, BLACK);
    }
}

void draw_game(Snake *snake, Apple *apple, int start_x, int start_y)
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

    direction_snake(snake, 15, 15, 360, 360);
    draw_apple(apple, snake->head, 15, 15, 360, 360, cellHeight, cellWidth, start_x, start_y);

    // Draw the score
    char scoreStr[50];
    sprintf(scoreStr, "Score: %i", snake->size);
    // Dibuja el puntaje
    DrawText(scoreStr, 10, 430, 20, YELLOW);
    EndDrawing();
}

/*
Dudas de documentacion:
La palabra clave const se debe usar siempre que sea apropiado. En barr dice que es una gran alternativa al uso del define? (entonces no lo incluyo en el codigo?)
En donde deberia de poner el static?
La palabra clave static se debe usar para declarar todas las funciones y variables que no necesitan ser visibles fuera del módulo en el que
se declaran.
*/