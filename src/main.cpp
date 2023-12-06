#include "raylib.h"
#include <cmath>
// #include "raymath.h"

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen
{
    INICIO = 0,
    JUGAR,
    CREDITOS
} GameScreen;

// SACAR LA RESOLUCION DEL MONITOR
int screenWidth = GetMonitorWidth(0);
int screenHeight = GetMonitorHeight(0);

Texture2D backgroundTexture;
Sound buttonSound;
Texture2D buttonTextureA;
Texture2D buttonTextureB;
Music musica_fondo;

void LoadContent();
void UnloadContent();
int drawinicio();
void drawjugar();
void drawopciones();
void drawcreditos();
void menudraw(GameScreen currentScreen);
void ToggleFullscreenAndResize();
bool musica(bool musicToggle, bool musicPaused);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "SIGN MATCH - GAME");
    InitAudioDevice();

    GameScreen currentScreen = INICIO;

    // TODO: Initialize all required variables and load all required data here!
    LoadContent();
    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    bool musicPaused = false;
    bool musicToggle = false;

    Image icon = LoadImage("resources/SG_ICONO.png");
    SetWindowIcon(icon);
    PlayMusicStream(musica_fondo);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        ToggleFullscreenAndResize(); // Manejar cambios de pantalla completa y tamaño de ventana

        int buttonClicked = drawinicio();
        switch (currentScreen)
        {
        case INICIO: // AQUI EN ESTA OPCION ESTARA EL MENU.
        {

            if (buttonClicked == 1)
            {
                PlaySound(buttonSound);
                currentScreen = JUGAR;
            }
            if (buttonClicked == 2)
            {
                PlaySound(buttonSound);
                currentScreen = CREDITOS;
            }
            if (IsKeyPressed(KEY_M))
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused);
            }
        }
        break;
        case JUGAR:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                PlaySound(buttonSound);
                currentScreen = INICIO;
            }
        }
        break;
        case CREDITOS:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                PlaySound(buttonSound);
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
        UpdateMusicStream(musica_fondo);
    }

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadContent();
    UnloadImage(icon);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void LoadContent()
{
    // Cargar la textura de fondo en el main
    backgroundTexture = LoadTexture("resources/fullback.png");
    // Ajustar el tamaño de las imágenes según la resolución de la pantalla
    buttonTextureA = LoadTexture("resources/SM_BOTON.png");
    buttonTextureB = LoadTexture("resources/SM_BOTONcred.png");
    buttonSound = LoadSound("audio/resources/buttonsound.wav");
    musica_fondo = LoadMusicStream("audio/resources/fondo.mp3");
    SetMusicVolume(musica_fondo, 1.0f);
}

bool musica(bool musicToggle, bool musicPaused)
{
    if (musicToggle)
    {
        // Si musicToggle es true, pausar la música si no está pausada
        if (!musicPaused)
        {
            PauseMusicStream(musica_fondo);
            musicPaused = true;
        }
    }
    else
    {
        // Si musicToggle es false, reanudar la música si está pausada
        if (musicPaused)
        {
            ResumeMusicStream(musica_fondo);
            musicPaused = false;
        }
    }
    return musicPaused;
}

void UnloadContent()
{
    // Liberar la textura de fondo al final del programa
    UnloadTexture(backgroundTexture);
    UnloadSound(buttonSound);
    UnloadTexture(buttonTextureA);
    UnloadTexture(buttonTextureB);
    UnloadMusicStream(musica_fondo);
}

int drawinicio()
{
    DrawTexturePro(
        backgroundTexture,
        (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // Dibujar los botones
    Rectangle buttonRectA = {585.0f, 320.0, (float)buttonTextureA.width, (float)buttonTextureA.height};
    Rectangle buttonRectB = {585.0f, 420.0f, (float)buttonTextureB.width, (float)buttonTextureB.height};

    // Verificar si el mouse está sobre los botones
    bool isMouseOverButtonA = CheckCollisionPointRec(GetMousePosition(), buttonRectA);
    bool isMouseOverButtonB = CheckCollisionPointRec(GetMousePosition(), buttonRectB);

    // Cambiar el tamaño del botón A si el mouse está sobre él
    if (isMouseOverButtonA)
    {
        // Agrandar y reducir el botón A en un bucle
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectA.width = buttonTextureA.width * scale;
        buttonRectA.height = buttonTextureA.height * scale;
    }

    if (isMouseOverButtonB)
    {
        // Agrandar y reducir el botón B en un bucle
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectB.width = buttonTextureB.width * scale;
        buttonRectB.height = buttonTextureB.height * scale;
    }

    // Ajustar la posición del botón para que sea el centro del botón
    Vector2 buttonPosition = {buttonRectA.x + buttonRectA.width / 2, buttonRectA.y + buttonRectA.height / 2};
    Vector2 buttonPositionB = {buttonRectB.x + buttonRectB.width / 2, buttonRectB.y + buttonRectB.height / 2};
    DrawTexturePro(buttonTextureA, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureA.width), static_cast<float>(buttonTextureA.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectA.width, buttonRectA.height}, (Vector2){buttonRectA.width / 2, buttonRectA.height / 2}, 0.0f, WHITE);
    DrawTexturePro(buttonTextureB, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureB.width), static_cast<float>(buttonTextureB.height)}, (Rectangle){buttonPositionB.x, buttonPositionB.y, buttonRectB.width, buttonRectB.height}, (Vector2){buttonRectB.width / 2, buttonRectB.height / 2}, 0.0f, WHITE);

    // Verificar si se hizo clic en algún botón
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButtonA)
            return 1; // Botón A clickeado
        if (isMouseOverButtonB)
            return 2; // Botón B clickeado
    }

    return 0; // Ningún botón
}

void drawjugar()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
    DrawText("JUGAR SCREEN", 20, 20, 40, DARKGREEN);
    DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, DARKGREEN);
}

void drawcreditos()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
    DrawText("CREDITOS SCREEN", 20, 20, 40, DARKBLUE);
    DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, DARKBLUE);
}

void menudraw(GameScreen currentScreen)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (currentScreen)
    {
    case INICIO:
        drawinicio();
        break;
    case JUGAR:
        drawjugar();
        break;
    case CREDITOS:
        drawcreditos();
        break;
    default:
        break;
    }

    EndDrawing();
}

void ToggleFullscreenAndResize()
{
    if (IsKeyPressed(KEY_F))
    {
        // Cambiar a pantalla completa al presionar F11
        ToggleFullscreen();

        // Ajustar el tamaño de la ventana al cambiar a pantalla completa
        if (IsWindowFullscreen())
        {
            screenWidth = GetMonitorWidth(0);
            screenHeight = GetMonitorHeight(0);
        }
        else
        {
            // Restaurar la resolución original al salir de pantalla completa
            screenWidth = GetMonitorWidth(0);
            screenHeight = GetMonitorHeight(0);
        }

        // Cambiar el tamaño de la ventana
        SetWindowSize(screenWidth, screenHeight);
    }
}
