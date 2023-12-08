#include "raylib.h"
#include <cmath>
// #include "raymath.h"

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

/* --------------------------------------------------------------------------------------------------- */
/* PROGRAMA PRINCIPAL */
/* --------------------------------------------------------------------------------------------------- */
int main(void)
{
    /* INICIALIZACION */
    InitWindow(screenWidth, screenHeight, "SIGN MATCH - GAME");
    InitAudioDevice();

    GameScreen currentScreen = INICIO;

    /* INICIALIZAR TODAS LAS VARIABLES REQUERIDAS PARA CARGAR LOS DATOS AQUI */
    LoadContent();
    SetTargetFPS(60); // FRAMES POR SEGUNDO
    bool musicPaused = false;
    bool musicToggle = false;

    Image icon = LoadImage("resources/SG_ICONO.png");
    SetWindowIcon(icon);
    PlayMusicStream(musica_fondo);

    /* ----------------------------------------------------------------------------------------------- */
    /* LOOP PRINCIPAL DEL JUEGO */
    /* ----------------------------------------------------------------------------------------------- */
    while (!WindowShouldClose()) // DETECTAR QUE SE CIERRA EL PROGRAMA CON LA TECLA (ESC)
    {
        ToggleFullscreenAndResize(); // MANEJAR CAMBIOS DE PANTALLA COMPLETA Y TAMAÑO DE VENTANA

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
                musicToggle = !musicToggle; // CAMBIAR EL ESTADO DE LA MUSICA CON LA TECLA (M)
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
        /* ------------------------------------------------------------------------------------------- */
        /* DIBUJAR */
        /* ------------------------------------------------------------------------------------------- */
        menudraw(currentScreen);
        UpdateMusicStream(musica_fondo);
    }

    /* ----------------------------------------------------------------------------------------------- */
    /* CARGAR TODOS LOS DATOS AQUI: Texturas, Fuentes, Audio, etc... */
    /* ----------------------------------------------------------------------------------------------- */
    UnloadContent();
    UnloadImage(icon);

    CloseWindow(); // CERRAR VENTANA

    return 0;
}

/* --------------------------------------------------------------------------------------------------- */

void LoadContent()
{
    /* CARGAR LA TEXTURA DE FONDO EN EL MAIN */
    backgroundTexture = LoadTexture("resources/fullback.png");
    /* AJUSTAR EL TAMAÑO DE LAS IMAGENES SEGUN LA RESOLUCION DE LA PANTALLA */
    buttonTextureA = LoadTexture("resources/SM_BOTON.png");
    buttonTextureB = LoadTexture("resources/SM_BOTONcred.png");
    buttonSound = LoadSound("audio/resources/buttonsound.wav");
    musica_fondo = LoadMusicStream("audio/resources/fondo.mp3");
    SetMusicVolume(musica_fondo, 1.0f);
}

/* --------------------------------------------------------------------------------------------------- */

bool musica(bool musicToggle, bool musicPaused)
{
    if (musicToggle)
    {
        /* SI SE CUMPLE musicToggle, PAUSAR LA MUSICA SI ES QUE NO ESTA YA PAUSADA */
        if (!musicPaused)
        {
            PauseMusicStream(musica_fondo);
            musicPaused = true;
        }
    }
    else
    {
        /* REANUDAR LA MUSICA SI musicToggle ESTA PAUSADA */
        if (musicPaused)
        {
            ResumeMusicStream(musica_fondo);
            musicPaused = false;
        }
    }
    return musicPaused;
}

/* --------------------------------------------------------------------------------------------------- */

void UnloadContent()
{
    /* LIBERAR LA TEXTURA DE FONDO AL FINAL DEL PROGRAMA */
    UnloadTexture(backgroundTexture);
    UnloadSound(buttonSound);
    UnloadTexture(buttonTextureA);
    UnloadTexture(buttonTextureB);
    UnloadMusicStream(musica_fondo);
}

/* --------------------------------------------------------------------------------------------------- */

int drawinicio()
{
    DrawTexturePro(
        backgroundTexture,
        (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    /* DIBUJAR LOS BOTONES */
    Rectangle buttonRectA = {585.0f, 320.0, (float)buttonTextureA.width, (float)buttonTextureA.height};
    Rectangle buttonRectB = {585.0f, 420.0f, (float)buttonTextureB.width, (float)buttonTextureB.height};

    /* VERIFICAR SI EL MOUSE ESTA SOBRE LOS BOTONES */
    bool isMouseOverButtonA = CheckCollisionPointRec(GetMousePosition(), buttonRectA);
    bool isMouseOverButtonB = CheckCollisionPointRec(GetMousePosition(), buttonRectB);

    /* CAMBIAR EL TAMAÑO DEL BOTON (A) SI EL MOUSE ESTA SOBRE DE EL */
    if (isMouseOverButtonA)
    {
        /* AGRANDAR Y REDUCIR (A) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectA.width = buttonTextureA.width * scale;
        buttonRectA.height = buttonTextureA.height * scale;
    }

    if (isMouseOverButtonB)
    {
        /* AGRANDAR Y REDUCIR (B) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectB.width = buttonTextureB.width * scale;
        buttonRectB.height = buttonTextureB.height * scale;
    }

    /* AJUSTAR LA POSICION DEL BOTON PARA QUE ESTE EN EL CENTRO */
    Vector2 buttonPosition = {buttonRectA.x + buttonRectA.width / 2, buttonRectA.y + buttonRectA.height / 2};
    Vector2 buttonPositionB = {buttonRectB.x + buttonRectB.width / 2, buttonRectB.y + buttonRectB.height / 2};
    DrawTexturePro(buttonTextureA, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureA.width), static_cast<float>(buttonTextureA.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectA.width, buttonRectA.height}, (Vector2){buttonRectA.width / 2, buttonRectA.height / 2}, 0.0f, WHITE);
    DrawTexturePro(buttonTextureB, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureB.width), static_cast<float>(buttonTextureB.height)}, (Rectangle){buttonPositionB.x, buttonPositionB.y, buttonRectB.width, buttonRectB.height}, (Vector2){buttonRectB.width / 2, buttonRectB.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLICK EN EL BOTON */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButtonA)
            return 1; // Botón A clickeado
        if (isMouseOverButtonB)
            return 2; // Botón B clickeado
    }

    return 0; // Ningún botón
}

/* --------------------------------------------------------------------------------------------------- */

void drawjugar()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
    DrawText("JUGAR SCREEN", 20, 20, 40, DARKGREEN);
    DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, DARKGREEN);
}

/* --------------------------------------------------------------------------------------------------- */

void drawcreditos()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
    DrawText("CREDITOS SCREEN", 20, 20, 40, DARKBLUE);
    DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, DARKBLUE);
}

/* --------------------------------------------------------------------------------------------------- */

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

/* --------------------------------------------------------------------------------------------------- */

void ToggleFullscreenAndResize()
{
    if (IsKeyPressed(KEY_F))
    {
        /* CAMBIAR LA PANTALLA COMPLETA AL PRESIONAR (F11) */
        ToggleFullscreen();

        /* AJUSTAR EL TAMAÑO DE LA PANTALLA AL CAMBIARLO A FULLSCREEN */
        if (IsWindowFullscreen())
        {
            screenWidth = GetMonitorWidth(0);
            screenHeight = GetMonitorHeight(0);
        }
        else
        {
            /* RESTAURAR LA RESOLUCION AL SALIR DE LA PANTALLA COMPLETA */
            screenWidth = GetMonitorWidth(0);
            screenHeight = GetMonitorHeight(0);
        }

        /* CAMBIAR EL TAMAÑO DE LA VENTANA */
        SetWindowSize(screenWidth, screenHeight);
    }
}
