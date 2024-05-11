// LIBRARIES
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

// DEFINES
#define VELOCITY 1

// STRUCTURES
typedef struct NodeSnake
{
    int axisX, axisY;
    int prevX, prevY;
    struct NodeSnake *next;
} NodeSnake;

typedef struct Snake
{
    NodeSnake *head = NULL;
    NodeSnake *tail = NULL;
    int size;
    int direction;
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
    int axisX;
    int axisY;
    Texture2D appledrawing = LoadTexture("resources/apple.png");
} Apple;

SnakeDirection currentDirection = RIGHT; // Inicializa la dirección a la derecha.

void initApple(Apple *apple, int gridHeight, int gridWidth, int cellHeight, int cellWidth, int startX, int startY)
{
    // Generate random position for the apple within the grid
    apple->axisX = (GetRandomValue(0, (gridWidth / cellWidth) - 1) * cellWidth) + startX;
    apple->axisY = (GetRandomValue(0, (gridHeight / cellHeight) - 1) * cellHeight) + startY;
}

void drawApple(Apple *apple, NodeSnake *snake, int height_cellsnumber, int width_cellsnumber, int gridHeight, int gridWidth, int cellHeight, int cellWidth, int startX, int startY)
{
    bool overlap = false;

    do
    {
        // Check if apple overlaps with any part of the snake
        NodeSnake *current = snake;
        while (current != NULL)
        {
            if (current->axisX == apple->axisX && current->axisY == apple->axisY)
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

        initApple(apple, gridHeight, gridWidth, cellHeight, cellWidth, startX, startY); // Regenerate the apple position
        overlap = false;

    } while (true);

    // Calculate the size of the apple
    float scale = 1.9f; // Aumenta este valor para hacer la manzana más grande

    // Draw the apple with the adjusted size
    DrawTexturePro(apple->appledrawing,
                   (Rectangle){0, 0, apple->appledrawing.width, apple->appledrawing.height},
                   (Rectangle){apple->axisX, apple->axisY, cellWidth * scale, cellHeight * scale},
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
}

bool hasEatenFood(Snake *snake, Apple *apple, int cellWidth, int cellHeight)
{
    NodeSnake *current = snake->head;

    // Check if any part of the head of the snake is within the area occupied by the apple
    if (current->axisX >= apple->axisX - cellWidth && current->axisX < apple->axisX + cellWidth &&
        current->axisY >= apple->axisY - cellHeight && current->axisY < apple->axisY + cellHeight)
    {
        // printf("Apple eaten!\n");
        return true;
    }

    return false;
}

void initSnake(Snake *snake, int x, int y)
{
    snake->head = (NodeSnake *)malloc(sizeof(NodeSnake));
    snake->head->axisX = x;
    snake->head->axisY = y;
    snake->head->next = NULL;
    snake->tail = snake->head;
    snake->size = 1;
    snake->direction = 1; // Dirección inicial: derecha
}

void growSnake(Snake *snake, int cellWidth, int cellHeight)
{
    NodeSnake *newNode = (NodeSnake *)malloc(sizeof(NodeSnake));
    switch (snake->direction)
    {
    case RIGHT:
        newNode->axisX = snake->tail->axisX - cellWidth;
        newNode->axisY = snake->tail->axisY;
        break;
    case LEFT:
        newNode->axisX = snake->tail->axisX + cellWidth;
        newNode->axisY = snake->tail->axisY;
        break;
    case UP:
        newNode->axisX = snake->tail->axisX;
        newNode->axisY = snake->tail->axisY + cellHeight;
        break;
    case DOWN:
        newNode->axisX = snake->tail->axisX;
        newNode->axisY = snake->tail->axisY - cellHeight;
        break;
    }
    newNode->prevX = newNode->axisX;
    newNode->prevY = newNode->axisY;

    newNode->next = NULL;
    snake->tail->next = newNode;
    snake->tail = newNode;
    snake->size++;
}

void snakemovement(Snake *snake, int height_cellsnumber, int width_cellsnumber, int gridHeight, int gridWidth)
{
    NodeSnake *current = snake->head;
    int cellWidth = gridWidth / width_cellsnumber;
    int cellHeight = gridHeight / height_cellsnumber;
    // Calcula el punto de inicio para dibujar la cuadrícula, en el medio.
    int startX = (GetScreenWidth() - gridWidth) / 2;
    int startY = (GetScreenHeight() - gridHeight) / 2;

    int prevX = current->axisX;
    int prevY = current->axisY;

    // Move the head of the snake
    switch (snake->direction)
    {
    case RIGHT:
        if (current->axisX < startX + gridWidth - cellWidth)
        {
            current->axisX += VELOCITY;
        }
        break;
    case LEFT:
        if (current->axisX > startX)
        {
            current->axisX -= VELOCITY;
        }
        break;
    case UP:
        if (current->axisY > startY)
        {
            current->axisY -= VELOCITY;
        }
        break;
    case DOWN:
        if (current->axisY < startY + gridHeight - cellHeight)
        {
            current->axisY += VELOCITY;
        }
        break;
    }

    // Move the rest of the snake
    current = current->next;
    while (current != NULL)
    {
        int tempX = current->axisX;
        int tempY = current->axisY;
        current->axisX = prevX;
        current->axisY = prevY;
        prevX = tempX;
        prevY = tempY;
        current = current->next;
    }
}

void drawSnake(Snake *snake, int height_cellsnumber, int width_cellsnumber, int gridHeight, int gridWidth)
{
    NodeSnake *current = snake->head;
    int cellWidth = gridWidth / width_cellsnumber;
    int cellHeight = gridHeight / height_cellsnumber;

    // Calcula el punto de inicio para dibujar la cuadrícula, en el medio.
    int startX = (GetScreenWidth() - gridWidth) / 2;
    int startY = (GetScreenHeight() - gridHeight) / 2;

    // Verifica las pulsaciones de teclas fuera del bucle
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

    // Mueve la serpiente en la dirección actual
    while (current != NULL)
    {
        switch (snake->direction)
        {
        case RIGHT:
            if (current->axisX < startX + gridWidth - cellWidth)
            {
                current->axisX += VELOCITY;
            }
            break;
        case LEFT:
            if (current->axisX > startX)
            {
                current->axisX -= VELOCITY; // Utiliza VELOCITY definido previamente
            }
            break;
        case UP:
            if (current->axisY > startY)
            {
                current->axisY -= VELOCITY; // Utiliza VELOCITY definido previamente
            }
            break;
        case DOWN:
            if (current->axisY < startY + gridHeight - cellHeight)
            {
                current->axisY += VELOCITY; // Utiliza VELOCITY definido previamente
            }
            break;
        }
        current = current->next;
    }

    // Vuelve a establecer current al inicio de la serpiente
    current = snake->head;
    while (current != NULL)
    {
        DrawRectangle(current->axisX, current->axisY, cellWidth, cellHeight, RED);
        current = current->next;
    }

    // Snake Movement
    snakemovement(snake, height_cellsnumber, width_cellsnumber, gridHeight, gridWidth);
}

bool collisions(Snake *snake, int height_cellsnumber, int width_cellsnumber, int gridHeight, int gridWidth)
{
    // Check if the snake has collided with the walls
    NodeSnake *current = snake->head;
    int cellWidth = gridWidth / width_cellsnumber;
    int cellHeight = gridHeight / height_cellsnumber;

    // Calculate the starting point to draw the grid, in the middle.
    int startX = (GetScreenWidth() - gridWidth) / 2;
    int startY = (GetScreenHeight() - gridHeight) / 2;

    if (/*superior wall*/ current->axisX <= startX || /*right wall */ current->axisX >= startX + gridWidth - cellWidth ||
        /*inferior wall */ current->axisY <= startY || /*bottom wall*/ current->axisY >= startY + gridHeight - cellHeight)
    {
        return true;
    }
    return false;
}

void freeSnake(Snake *snake)
{
    NodeSnake *current = snake->head;
    while (current != NULL)
    {
        NodeSnake *temp = current;
        current = current->next;
        delete temp;
    }
}
