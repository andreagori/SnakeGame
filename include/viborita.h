// LIBRARIES
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

// DEFINES
#define VELOCITY 2

// STRUCTURES
typedef struct NodeSnake
{
    int axis_x_t = 0;       // coordenate x of the node
    int axis_y_t = 0;       // coordenate y of the node
    int prev_x_t = 0;       // coordenate x of the previous node
    int prev_y_t = 0;       // coordenate y of the previous node
    struct NodeSnake *next; // pointer to the next node
} NodeSnake;

typedef struct Snake
{
    NodeSnake *head = NULL; // pointer to the head of the snake
    NodeSnake *tail = NULL; // pointer to the tail of the snake
    int size;               // size of the snake
    int direction;          // direction of the snake
} Snake;

enum SnakeDirection
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct Apple
{
    int axis_x_t;                                                // coordenate x of the apple
    int axis_y_t;                                                // coordenate y of the apple
    Texture2D appledrawing = LoadTexture("resources/apple.png"); // texture of the apple
} Apple;

// FUNCTIONS DECLARATIONS
void init_apple(Apple *apple, int grid_height, int grid_width, int cell_height, int cell_width, int start_x, int start_y);
void draw_apple(Apple *apple, NodeSnake *snake, int height_cellsnumber, int width_cellsnumber, int grid_height, int grid_width,
                int cell_height, int cell_width, int start_x, int start_y);
bool has_eaten_food(Snake *snake, Apple *apple, int cell_width, int cell_height);
void init_snake(Snake *snake, int x, int y);
void grow_snake(Snake *snake, int cell_width, int cell_height);
void snake_movement(Snake *snake, int height_cellsnumber, int width_cellsnumber, int grid_height, int grid_width);
void direction_snake(Snake *snake, int height_cellsnumber, int width_cellsnumber, int grid_height, int grid_width);
bool collisions(Snake *snake, int height_cellsnumber, int width_cellsnumber, int grid_height, int grid_width);
void free_snake(Snake *snake);
void free_apple(Apple *apple);

// FUNCTIONS ------------------------------------------------

// Function to initialize the apple inside of the grid.
/* Parameters:
 * apple: pointer to the apple structure
 * grid_height: height of the grid
 * grid_width: width of the grid
 * cell_height: height of each cell
 * cell_width: width of each cell
 * start_x: starting point of the grid in the x-axis
 * start_y: starting point of the grid in the y-axis
 */
void init_apple(Apple *apple, int grid_height, int grid_width, int cell_height, int cell_width, int start_x, int start_y)
{
    // Generate random position for the apple within the grid
    apple->axis_x_t = (GetRandomValue(0, (grid_width / cell_width) - 1) * cell_width) + start_x;
    apple->axis_y_t = (GetRandomValue(0, (grid_height / cell_height) - 1) * cell_height) + start_y;
}

// Function to draw the apple on the screen. -----------
/* Parameters:
 * apple: pointer to the apple structure
 * snake: pointer to the snake structure
 * height_cellsnumber: number of cells in the height of the grid
 * width_cellsnumber: number of cells in the width of the grid
 * grid_height: height of the grid
 * grid_width: width of the grid
 * cell_height: height of each cell
 * cell_width: width of each cell
 * start_x: starting point of the grid in the x-axis
 * start_y: starting point of the grid in the y-axis
 */
void draw_apple(Apple *apple, NodeSnake *snake, int height_cellsnumber, int width_cellsnumber, int grid_height,
                int grid_width, int cell_height, int cell_width, int start_x, int start_y)
{
    bool overlap = false;

    do
    {
        // Check if apple overlaps with any part of the snake
        NodeSnake *current = snake;
        while (current != NULL)
        {
            if ((current->axis_x_t == apple->axis_x_t) && (current->axis_y_t == apple->axis_y_t))
            {
                overlap = true;
                break;
            }
            current = current->next;
        }

        if (!overlap)
        {
            break;
        }

        init_apple(apple, grid_height, grid_width, cell_height, cell_width, start_x, start_y); // Regenerate the apple position
        overlap = false;

    } while (true);

    // Calculate the size of the apple
    float scale = 1.9f; // Aumenta este valor para hacer la manzana más grande

    // Draw the apple with the adjusted size
    DrawTexturePro(apple->appledrawing,
                   (Rectangle){0, 0, apple->appledrawing.width, apple->appledrawing.height},
                   (Rectangle){apple->axis_x_t, apple->axis_y_t, cell_width * scale, cell_height * scale},
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
}

// Function to check if the snake has eaten the apple. -----------
/* Parameters:
 * snake: pointer to the snake structure
 * apple: pointer to the apple structure
 * cell_width: width of each cell
 * cell_height: height of each cell
 * Returns:
 * true if the snake has eaten the apple, false otherwise.
 */
bool has_eaten_food(Snake *snake, Apple *apple, int cell_width, int cell_height)
{
    NodeSnake *current = snake->head;

    // Check if any part of the head of the snake is within the area occupied by the apple
    if ((current->axis_x_t >= apple->axis_x_t - cell_width) && (current->axis_x_t < apple->axis_x_t + cell_width) &&
        (current->axis_y_t >= apple->axis_y_t - cell_height) && (current->axis_y_t < apple->axis_y_t + cell_height))
    {
        return true;
    }

    return false;
}

// Function to initialize the snake on the screen. -----------
/* Parameters:
 * snake: pointer to the snake structure
 * x: initial x position of the snake
 * y: initial y position of the snake
 */
void init_snake(Snake *snake, int x, int y)
{
    snake->head = (NodeSnake *)malloc(sizeof(NodeSnake));
    snake->head->axis_x_t = x;
    snake->head->axis_y_t = y;
    snake->head->next = NULL;
    snake->tail = snake->head;
    snake->size = 1;
    snake->direction = 1; // initial direction of the snake
}

// Function to grow the snake. -----------
/* Parameters:
 * snake: pointer to the snake structure
 * cell_width: width of each cell
 * cell_height: height of each cell
 */
void grow_snake(Snake *snake, int cell_width, int cell_height)
{
    NodeSnake *newNode = (NodeSnake *)malloc(sizeof(NodeSnake));
    switch (snake->direction)
    {
    case RIGHT:
        newNode->axis_x_t = snake->tail->axis_x_t - cell_width;
        newNode->axis_y_t = snake->tail->axis_y_t;
        break;
    case LEFT:
        newNode->axis_x_t = snake->tail->axis_x_t + cell_width;
        newNode->axis_y_t = snake->tail->axis_y_t;
        break;
    case UP:
        newNode->axis_x_t = snake->tail->axis_x_t;
        newNode->axis_y_t = snake->tail->axis_y_t + cell_height;
        break;
    case DOWN:
        newNode->axis_x_t = snake->tail->axis_x_t;
        newNode->axis_y_t = snake->tail->axis_y_t - cell_height;
        break;
    }

    newNode->next = NULL;
    snake->tail->next = newNode;
    snake->tail = newNode;
    snake->size++;
}

// Function to move the snake. -----------
/* Parameters:
 * snake: pointer to the snake structure
 * height_cellsnumber: number of cells in the height of the grid
 * width_cellsnumber: number of cells in the width of the grid
 * grid_height: height of the grid
 * grid_width: width of the grid
 */
void snake_movement(Snake *snake, int height_cellsnumber, int width_cellsnumber, int grid_height, int grid_width)
{
    NodeSnake *current = snake->head;
    int cell_width = grid_width / width_cellsnumber;
    int cell_height = grid_height / height_cellsnumber;
    // calculate the starting point to draw the grid, in the middle.
    int start_x = (GetScreenWidth() - grid_width) / 2;
    int start_y = (GetScreenHeight() - grid_height) / 2;

    int prev_x_t = current->axis_x_t;
    int prev_y_t = current->axis_y_t;

    // Move the head of the snake
    switch (snake->direction)
    {
    case RIGHT:
        if (current->axis_x_t < start_x + grid_width - cell_width)
        {
            current->axis_x_t += VELOCITY;
        }
        break;
    case LEFT:
        if (current->axis_x_t > start_x)
        {
            current->axis_x_t -= VELOCITY;
        }
        break;
    case UP:
        if (current->axis_y_t > start_y)
        {
            current->axis_y_t -= VELOCITY;
        }
        break;
    case DOWN:
        if (current->axis_y_t < start_y + grid_height - cell_height)
        {
            current->axis_y_t += VELOCITY;
        }
        break;
    }

    // Move the rest of the snake
    current = current->next;
    while (current != NULL)
    {
        int tempX = current->axis_x_t;
        int tempY = current->axis_y_t;
        current->axis_x_t = prev_x_t;
        current->axis_y_t = prev_y_t;
        prev_x_t = tempX;
        prev_y_t = tempY;
        current = current->next;
    }

    // Reset current to the head of the snake
    current = snake->head;

    while (current != NULL)
    {
        DrawRectangle(current->axis_x_t, current->axis_y_t, cell_width, cell_height, RED);
        current = current->next;
    }
}

// Function to draw the snake on the screen. -----------
/* Parameters:
 * snake: pointer to the snake structure
 * height_cellsnumber: number of cells in the height of the grid
 * width_cellsnumber: number of cells in the width of the grid
 * grid_height: height of the grid
 * grid_width: width of the grid
 */
void direction_snake(Snake *snake, int height_cellsnumber, int width_cellsnumber, int grid_height, int grid_width)
{
    // Check key inputs to change the direction of the snake.
    if (IsKeyPressed(KEY_RIGHT))
    {
        snake->direction = RIGHT;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        snake->direction = LEFT;
    }
    if (IsKeyPressed(KEY_UP))
    {
        snake->direction = UP;
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        snake->direction = DOWN;
    }
    // Snake Movement
    snake_movement(snake, height_cellsnumber, width_cellsnumber, grid_height, grid_width);
}

// Function to check if the snake has collided with the walls. -----------
/* Parameters:
 * snake: pointer to the snake structure
 * height_cellsnumber: number of cells in the height of the grid
 * width_cellsnumber: number of cells in the width of the grid
 * grid_height: height of the grid
 * grid_width: width of the grid
 * Returns:
 * true if the snake has collided with the walls, false otherwise.
 */
bool collisions(Snake *snake, int height_cellsnumber, int width_cellsnumber, int grid_height, int grid_width)
{
    // Check if the snake has collided with the walls
    NodeSnake *current = snake->head;
    int cell_width = grid_width / width_cellsnumber;
    int cell_height = grid_height / height_cellsnumber;

    // Calculate the starting point to draw the grid, in the middle.
    int start_x = (GetScreenWidth() - grid_width) / 2;
    int start_y = (GetScreenHeight() - grid_height) / 2;

    /*superior wall*/ /*right wall */
    if ((current->axis_x_t <= start_x) || (current->axis_x_t >= start_x + grid_width - cell_width) ||
        (current->axis_y_t <= start_y) || (current->axis_y_t >= start_y + grid_height - cell_height))
    { /*inferior wall */ /*bottom wall*/
        return true;
    }

    // Check if the snake has collided with itself
    NodeSnake *temp = current->next;
    while (temp != NULL)
    {
        if (current->axis_x_t == temp->axis_x_t && current->axis_y_t == temp->axis_y_t)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// Function to free the memory allocated for the snake. -----------
/* Parameters:
 * snake: pointer to the snake structure
 */
void free_snake(Snake *snake)
{
    NodeSnake *current = snake->head;
    while (current != NULL)
    {
        NodeSnake *temp = current;
        current = current->next;
        delete temp;
    }
}

// Function to free the memory allocated for the apple. -----------
/* Parameters:
 * apple: pointer to the apple structure
 */
void free_apple(Apple *apple)
{
    UnloadTexture(apple->appledrawing);
}
