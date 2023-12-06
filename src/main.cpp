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
bool musica( bool musicToggle, bool musicPaused);

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

            if (IsKeyPressed(KEY_A) || buttonClicked == 1)
            {
                PlaySound(buttonSound);
                currentScreen = JUGAR;
            }
            if (IsKeyPressed(KEY_S) || buttonClicked == 2)
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
        case OPCIONES:
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

bool musica( bool musicToggle, bool musicPaused)
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
    DrawTexture(buttonTextureA, buttonRectA.x, buttonRectA.y, WHITE);
    Rectangle buttonRectB = {585.0f, 420.0f, (float)buttonTextureB.width, (float)buttonTextureB.height};
    DrawTexture(buttonTextureB, buttonRectB.x, buttonRectB.y, WHITE);

    // Verificar si el mouse está sobre los botones
    bool isMouseOverButtonA = CheckCollisionPointRec(GetMousePosition(), buttonRectA);
    bool isMouseOverButtonB = CheckCollisionPointRec(GetMousePosition(), buttonRectB);
    // bool isMouseOverButtonC = CheckCollisionPointRec(GetMousePosition(), buttonRectC);

    // Cambiar el color del texto según la interacción
    DrawText("PRESS A to JUMP to JUGAR SCREEN", 120, 220, 20, isMouseOverButtonA ? RED : YELLOW);
    DrawText("PRESS S to JUMP to OPCIONES SCREEN", 120, 240, 20, isMouseOverButtonB ? BLUE : YELLOW);
    // DrawText("PRESS D to JUMP to CREDITOS SCREEN", 120, 260, 20, isMouseOverButtonC ? GREEN : YELLOW);

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

void drawopciones()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    DrawText("OPCIONES SCREEN", 20, 20, 40, MAROON);
    DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, MAROON);
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
    case OPCIONES:
        drawopciones();
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
