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
                // logic if has eaten the apple and regenerate it.
                if (has_eaten_food(&snake, &apple, 15, 15))
                {
                    grow_snake(&snake, 15, 15);
                    init_apple(&apple, 360, 360, 15, 15, start_x, start_y);
                }
                draw_game(&snake, &apple, start_x, start_y);
            }
            break;
        }
        }
        // Draw the current screen.
        if (current_screen == MENU)
        {
            draw_menu(assets, current_screen);
        }
    }
    // Unload the assets and memory.
    unload(assets);
    UnloadImage(icon);
    UnloadTexture(apple.appledrawing);
    free_snake(&snake);
    free_apple(&apple);
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// FUNCTIONS DEFINITIONS.
/* Function to load the assets
 * Parameters:
 * assets (files) - the assets to load.
 * Return:
 * assets (files) - the assets loaded.
 */
files load(files assets)
{
    assets.background = LoadTexture("resources/UABC_ANDREA_FONDO.png");
    return assets;
}

// Function to unload the assets
/* Parameters:
 * assets (files) - the assets to unload.
 */
void unload(files assets)
{
    UnloadTexture(assets.background);
}

// Function to animate the text
/* Parameters:
 * texto (const char *) - the text to animate.
 * fontSize (int) - the size of the font.
 * textWidth (int) - the width of the text.
 * textHeight (int) - the height of the text.
 * assets (files) - the assets to use.
 */
void animetext(const char *texto, int fontSize, int textWidth, int textHeight, files assets)
{
    static int timer = 0;
    // Add the timer to the text.
    timer++;

    // Get the mouse position.
    Vector2 mousePosition = GetMousePosition();

    // Create a rectangle for the text.
    Rectangle textRect = {
        (GetScreenWidth() - textWidth) / 2,   // x position of the text
        (GetScreenHeight() - textHeight) / 2, // y position of the text
        textWidth,                            // text width
        textHeight                            // text height
    };

    // Check if the mouse is over the text.
    bool isMouseOverText = CheckCollisionPointRec(mousePosition, textRect);

    // If the mouse is over the text, don't animate it.
    if (isMouseOverText)
    {
        DrawText(texto, (GetScreenWidth() - textWidth) / 2, (GetScreenHeight() - textHeight) / 2, fontSize, YELLOW);
    }
    else
    {
        // if the mouse is not over the text, animate it.
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());

        // aplicate the scale to the font size
        int newFontSize = fontSize * scale;

        // draw the text with the new font size
        DrawText(texto, (GetScreenWidth() - textWidth * scale) / 2, (GetScreenHeight() - textHeight * scale) / 2, newFontSize, YELLOW);
    }
}

// Function to draw the grid
/* Parameters:
 * height (int) - the height of the grid.
 * weight (int) - the weight of the grid.
 */
void grid(int height, int weight)
{
    // Draw the grid lines on the screen.
    for (int i = 0; i < weight; i += 20)
    {
        DrawLine(i, 0, i, height, Fade(LIGHTGRAY, 0.1f));
    }
    for (int i = 0; i < height; i += 20)
    {
        DrawLine(0, i, weight, i, Fade(LIGHTGRAY, 0.1f));
    }
}

// Function to draw the menu
/* Parameters:
 * assets (files) - the assets to use.
 * pantalla (GameScreen) - the screen to draw.
 */
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

    animetext("Press ENTER to start", 25, 250, 20, assets);
    DrawText("made by: @andreagori", 80, 380, 10, BLACK);

    // Draw the grid
    grid(450, 450);

    EndDrawing();
}

void gridSnake(int height_cellsnumber, int width_cellsnumber, int gridHeight, int gridWidth)
{
    // For customization in color,  use: CLITERAL(Color){ x, x, x, x }
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

// Function to draw the game
/* Parameters:
 * snake (Snake *) - the snake to draw.
 * apple (Apple *) - the apple to draw.
 * start_x (int) - the x position to start drawing.
 * start_y (int) - the y position to start drawing.
 */
void draw_game(Snake *snake, Apple *apple, int start_x, int start_y)
{
    BeginDrawing();
    ClearBackground(CLITERAL(Color){2, 43, 15, 255});
    DrawText("[TAB] to go back to the menu", 10, 10, 15, YELLOW);

    // Draw the grid background and the snake grid.
    grid(450, 450);
    gridSnake(15, 15, 360, 360);

    int cellWidth = 360 / 15; // Assuming a 15x15 grid
    int cellHeight = 360 / 15;

    // Draw the snake and the apple.
    direction_snake(snake, 15, 15, 360, 360);
    draw_apple(apple, snake->head, 15, 15, 360, 360, cellHeight, cellWidth, start_x, start_y);

    // Draw the score
    char scoreStr[50];
    sprintf(scoreStr, "Score: %i", snake->size);
    DrawText(scoreStr, 10, 430, 20, YELLOW);
    EndDrawing();
}