#include "raylib.h"
#include <cmath>
#include <string.h>

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------

typedef enum jugarscreen
{
    BASICO,
    LETRAS,
    COLORES,
} jugarscreen;

typedef enum GameScreen
{
    INICIO = 0,
    JUGAR,
    CREDITOS,
    JUGAR_BASICO,
    JUGAR_LETRAS,
    JUGAR_COLORES,
} GameScreen;

typedef struct cargas
{
    Texture2D backgroundTexture;
    Sound buttonSound;
    Texture2D buttonTextureA;
    Texture2D buttonTextureB;
    Texture2D buttonTextureC;
    Texture2D buttonTextureD;
    Texture2D buttonTextureE;
    Texture2D buttonTexture1;
    Music musica_fondo;
} cargas;

// SACAR LA RESOLUCION DEL MONITOR
int screenWidth = GetMonitorWidth(0);
int screenHeight = GetMonitorHeight(0);

cargas LoadContent(const char pantalla[], cargas archivos);
void UnloadContent(cargas archivos, GameScreen currentScreen);
int drawinicio(cargas archivos);
int drawjugar(GameScreen currentScreen, cargas archivos);
void drawcreditos();
void menudraw(GameScreen currentScreen, cargas archivos);
void ToggleFullscreenAndResize();
bool musica(bool musicToggle, bool &musicPaused, Music &musica_fondo);

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
    GameScreen nextScreen = INICIO;
    cargas archivos;
    archivos.buttonSound = LoadSound("audio/resources/buttonsound.wav");
    archivos.musica_fondo = LoadMusicStream("audio/resources/fondo.mp3");
    archivos = LoadContent("MENU", archivos);

    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    bool musicPaused = false;
    bool musicToggle = false;

    Image icon = LoadImage("resources/SG_ICONO.png");
    SetWindowIcon(icon);
    PlayMusicStream(archivos.musica_fondo);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        ToggleFullscreenAndResize(); // Manejar cambios de pantalla completa y tamaño de ventana

        int buttonClicked = drawinicio(archivos);
        switch (currentScreen)
        {
        case INICIO: // AQUI EN ESTA OPCION ESTARA EL MENU.
        {
            if (!musicPaused) // Only resume the music if it's not supposed to be paused
            {
                ResumeMusicStream(archivos.musica_fondo);
            }
            if (buttonClicked == 1)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                PauseMusicStream(archivos.musica_fondo);
                UnloadContent(archivos, INICIO);
                archivos = LoadContent("JUGAR", archivos);
            }
            if (buttonClicked == 2)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = CREDITOS;
            }
            if (IsKeyPressed(KEY_M))
            {
                musicToggle = !musicToggle;
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
        }
        break;
        case JUGAR:
        {
            if (!musicPaused) // Only resume the music if it's not supposed to be paused
            {
                ResumeMusicStream(archivos.musica_fondo);
            }

            if (buttonClicked == 1)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_BASICO;
            }
            if (buttonClicked == 2)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_LETRAS;
            }
            if (buttonClicked == 3)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_COLORES;
            }
            if (IsKeyPressed(KEY_DELETE))
            {
                PlaySound(archivos.buttonSound);
                currentScreen = nextScreen;
                PauseMusicStream(archivos.musica_fondo);
                UnloadContent(archivos, JUGAR);
                archivos = LoadContent("MENU", archivos);
            }
            if (IsKeyPressed(KEY_M))
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
        }
        break;
        case CREDITOS:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                PlaySound(archivos.buttonSound);
                currentScreen = INICIO;
            }
        }
        break;
        default:
            break;
        }
        // Draw
        //----------------------------------------------------------------------------------
        menudraw(currentScreen, archivos);
        UpdateMusicStream(archivos.musica_fondo);
    }

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadContent(archivos, currentScreen);
    UnloadImage(icon);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

bool musica(bool musicToggle, bool &musicPaused, Music &musica_fondo)
{
    if (musicToggle)
    {
        if (IsMusicStreamPlaying(musica_fondo))
        {
            PauseMusicStream(musica_fondo);
            musicPaused = true;
        }
        else
        {
            ResumeMusicStream(musica_fondo);
            musicPaused = false;
        }
    }

    return musicPaused;
}

cargas LoadContent(const char pantalla[], cargas archivos)
{
    // Cargar texturas según el nombre de la pantalla
    if (strcmp(pantalla, "MENU") == 0)
    {
        archivos.backgroundTexture = LoadTexture("resources/fullback.png");
        archivos.buttonTextureA = LoadTexture("resources/SM_BotonJugar_1.png");
        archivos.buttonTextureB = LoadTexture("resources/SM_BotonCreditos.png");
        SetMusicVolume(archivos.musica_fondo, 1.0f);
    }
    if (strcmp(pantalla, "JUGAR") == 0)
    {
        archivos.backgroundTexture = LoadTexture("resources/SM_PantallaJugar.png");
        archivos.buttonTextureC = LoadTexture("resources/SM_BotonBasicos.png");
        archivos.buttonTextureD = LoadTexture("resources/SM_BotonLetras.png");
        archivos.buttonTextureE = LoadTexture("resources/SM_BotonColores.png");
    }
    // Agrega más condiciones para otras pantallas

    return archivos;
}

void UnloadContent(cargas archivos, GameScreen currentScreen)
{
    // Descargar texturas
    if (currentScreen == INICIO)
    {
        UnloadTexture(archivos.backgroundTexture);
        UnloadTexture(archivos.buttonTextureA);
        UnloadTexture(archivos.buttonTextureB);
    }
    if (currentScreen == JUGAR)
    {
        UnloadTexture(archivos.backgroundTexture);
        UnloadTexture(archivos.buttonTextureC);
        UnloadTexture(archivos.buttonTextureD);
        UnloadTexture(archivos.buttonTextureE);
    }
}

int drawinicio(cargas archivos)
{
    DrawTexturePro(
        archivos.backgroundTexture,
        (Rectangle){0, 0, (float)archivos.backgroundTexture.width, (float)archivos.backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    /* DIBUJAR LOS BOTONES */
    // Centrar los botones horizontalmente y ajustar la distancia entre ellos
    float startX = (GetScreenWidth() - archivos.buttonTextureA.width) / 2;
    float startY = (GetScreenHeight() - archivos.buttonTextureA.height) / 2;
    float bstartX = (GetScreenWidth() - archivos.buttonTextureB.width) / 2;

    Rectangle buttonRectA = {startX, startY, (float)archivos.buttonTextureA.width, (float)archivos.buttonTextureA.height};
    Rectangle buttonRectB = {bstartX, GetScreenHeight() - archivos.buttonTextureB.height, (float)archivos.buttonTextureB.width, (float)archivos.buttonTextureB.height};
    /* VERIFICAR SI EL MOUSE ESTÁ SOBRE LOS BOTONES */
    bool isMouseOverButtonA = CheckCollisionPointRec(GetMousePosition(), buttonRectA);
    bool isMouseOverButtonB = CheckCollisionPointRec(GetMousePosition(), buttonRectB);

    /* CAMBIAR EL TAMAÑO DEL BOTÓN (A, B) SI EL MOUSE ESTÁ SOBRE DE EL */
    if (isMouseOverButtonA)
    {
        /* AGRANDAR Y REDUCIR (A) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectA.width = archivos.buttonTextureA.width * scale;
        buttonRectA.height = archivos.buttonTextureA.height * scale;
    }

    if (isMouseOverButtonB)
    {
        /* AGRANDAR Y REDUCIR (B) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectB.width = archivos.buttonTextureB.width * scale;
        buttonRectB.height = archivos.buttonTextureB.height * scale;
    }

    /* AJUSTAR LA POSICIÓN DEL BOTÓN PARA QUE ESTÉ EN EL CENTRO */
    Vector2 buttonPosition = {buttonRectA.x + buttonRectA.width / 2, buttonRectA.y + buttonRectA.height / 2};
    Vector2 buttonPositionB = {buttonRectB.x + buttonRectB.width / 2, buttonRectB.y + buttonRectB.height / 2};
    DrawTexturePro(archivos.buttonTextureA, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureA.width), static_cast<float>(archivos.buttonTextureA.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectA.width, buttonRectA.height}, (Vector2){buttonRectA.width / 2, buttonRectA.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureB, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureB.width), static_cast<float>(archivos.buttonTextureB.height)}, (Rectangle){buttonPositionB.x, buttonPositionB.y, buttonRectB.width, buttonRectB.height}, (Vector2){buttonRectB.width / 2, buttonRectB.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButtonA)
            return 1; // Botón A clickeado
        if (isMouseOverButtonB)
            return 2; // Botón B clickeado
    }

    return 0; // Ningún botón
}

int jugar_basico(cargas archivos)
{
    return 0;
}

int jugar_letras(cargas archivos)
{
    return 0;
}

int jugar_colores(cargas archivos)
{
    return 0;
}

int drawjugar(GameScreen currentScreen, cargas archivos)
{
    // Resto del código de la función drawjugar...

    DrawTexturePro(
        archivos.backgroundTexture,
        (Rectangle){0, 0, (float)archivos.backgroundTexture.width, (float)archivos.backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    Rectangle buttonRectC = {(GetScreenWidth() - archivos.buttonTextureC.width) / 2, 400.0f, (float)archivos.buttonTextureC.width, (float)archivos.buttonTextureC.height};
    Rectangle buttonRectD = {(GetScreenWidth() - archivos.buttonTextureD.width) / 2, 580.0f, (float)archivos.buttonTextureD.width, (float)archivos.buttonTextureD.height};
    Rectangle buttonRectE = {(GetScreenWidth() - archivos.buttonTextureE.width) / 2, 760.0f, (float)archivos.buttonTextureE.width, (float)archivos.buttonTextureE.height};

    /* VERIFICAR SI EL MOUSE ESTA SOBRE LOS BOTONES */
    bool isMouseOverButtonC = CheckCollisionPointRec(GetMousePosition(), buttonRectC);
    bool isMouseOverButtonD = CheckCollisionPointRec(GetMousePosition(), buttonRectD);
    bool isMouseOverButtonE = CheckCollisionPointRec(GetMousePosition(), buttonRectE);

    /* CAMBIAR EL TAMAÑO DEL BOTON (C,D,E) SI EL MOUSE ESTA SOBRE DE EL */
    if (isMouseOverButtonC)
    {
        /* AGRANDAR Y REDUCIR (C) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectC.width = archivos.buttonTextureC.width * scale;
        buttonRectC.height = archivos.buttonTextureC.height * scale;
    }

    if (isMouseOverButtonD)
    {
        /* AGRANDAR Y REDUCIR (D) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectD.width = archivos.buttonTextureD.width * scale;
        buttonRectD.height = archivos.buttonTextureD.height * scale;
    }

    if (isMouseOverButtonE)
    {
        /* AGRANDAR Y REDUCIR (E) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectE.width = archivos.buttonTextureE.width * scale;
        buttonRectE.height = archivos.buttonTextureE.height * scale;
    }

    /* AJUSTAR LA POSICION DEL BOTON PARA QUE ESTE EN EL CENTRO */
    Vector2 buttonPosition = {buttonRectC.x + buttonRectC.width / 2, buttonRectC.y + buttonRectE.height / 2};
    Vector2 buttonPositionD = {buttonRectD.x + buttonRectD.width / 2, buttonRectD.y + buttonRectD.height / 2};
    Vector2 buttonPositionE = {buttonRectE.x + buttonRectE.width / 2, buttonRectE.y + buttonRectE.height / 2};
    DrawTexturePro(archivos.buttonTextureC, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureC.width), static_cast<float>(archivos.buttonTextureC.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectC.width, buttonRectC.height}, (Vector2){buttonRectC.width / 2, buttonRectC.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureD, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureD.width), static_cast<float>(archivos.buttonTextureD.height)}, (Rectangle){buttonPositionD.x, buttonPositionD.y, buttonRectD.width, buttonRectD.height}, (Vector2){buttonRectD.width / 2, buttonRectD.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureE, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureE.width), static_cast<float>(archivos.buttonTextureE.height)}, (Rectangle){buttonPositionE.x, buttonPositionE.y, buttonRectE.width, buttonRectE.height}, (Vector2){buttonRectE.width / 2, buttonRectE.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLICK EN EL BOTON */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButtonC)
            return 1; // Botón C clickeado
        if (isMouseOverButtonD)
            return 2; // Botón D clickeado
        if (isMouseOverButtonE)
            return 3; // Botón D clickeado
    }

    return 0; // Ningún botón
}

void drawcreditos()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
    DrawText("CREDITOS SCREEN", 20, 20, 40, DARKBLUE);
    DrawText("PRESS DELETE to RETURN to INICIO SCREEN", 120, 280, 20, DARKBLUE);
}

void menudraw(GameScreen currentScreen, cargas archivos)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (currentScreen)
    {
    case INICIO:
        drawinicio(archivos);
        break;
    case JUGAR:
        drawjugar(currentScreen, archivos);
        break;
    case CREDITOS:
        drawcreditos();
        break;
    case JUGAR_BASICO:
    {
        jugar_basico(archivos);
        break;
    }
    case JUGAR_LETRAS:
    {
        jugar_letras(archivos);
        break;
    }
    case JUGAR_COLORES:
    {
        jugar_colores(archivos);
        break;
    }
    default:
        break;
    }

    EndDrawing();
}

void ToggleFullscreenAndResize()
{
    // Resto del código de la función ToggleFullscreenAndResize...
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

// CODIGO DE TERE PARA REFERENCIA:
// #include "raylib.h"
// #include <cmath>
// // #include "raymath.h"

// typedef enum GameScreen
// {
//     INICIO = 0,
//     JUGAR,
//     CREDITOS,
//     BASICOS,
//     LETRAS,
//     COLORES
// } GameScreen;

// // SACAR LA RESOLUCION DEL MONITOR
// int screenWidth = GetMonitorWidth(0);
// int screenHeight = GetMonitorHeight(0);

// // FONDOS
// Texture2D backgroundTexture;
// // 07/12/23 Añadi imagenes para los fondos
// Texture2D backgroundTextureA;
// Texture2D backgroundTextureB;
// Texture2D backgroundTextureC;
// Texture2D backgroundTextureD;
// Texture2D backgroundTextureE;
// // SONIDO DEL BOTON
// Sound buttonSound;
// // BOTONES
// // Pantalla Inicio
// Texture2D buttonTextureA;
// Texture2D buttonTextureB;
// // PantallaJugar
// Texture2D buttonTextureC;
// Texture2D buttonTextureD;
// Texture2D buttonTextureE;
// // Pantalla Basicos
// Texture2D buttonTexture1;
// // MUSICA
// Music musica_fondo;

// // CARGAR CONTENIDO
// void LoadContent();
// void UnloadContent();
// // PANTALLAS
// int drawinicio();
// int drawjugar();
// void drawopciones();
// void drawbasicos();
// void drawletras();
// void drawcolores();
// // EXTRAS
// void menudraw(GameScreen currentScreen);
// void ToggleFullscreenAndResize();
// bool musica(bool musicToggle, bool musicPaused);

// /* --------------------------------------------------------------------------------------------------- */
// /* PROGRAMA PRINCIPAL */
// /* --------------------------------------------------------------------------------------------------- */
// int main(void)
// {
//     /* INICIALIZACION */
//     InitWindow(screenWidth, screenHeight, "SIGN MATCH - GAME");
//     InitAudioDevice();

//     GameScreen currentScreen = INICIO;

//     /* INICIALIZAR TODAS LAS VARIABLES REQUERIDAS PARA CARGAR LOS DATOS AQUI */
//     LoadContent();
//     SetTargetFPS(60); // FRAMES POR SEGUNDO
//     bool musicPaused = false;
//     bool musicToggle = false;

//     Image icon = LoadImage("resources/SG_ICONO.png");
//     SetWindowIcon(icon);
//     PlayMusicStream(musica_fondo);

//     /* ----------------------------------------------------------------------------------------------- */
//     /* LOOP PRINCIPAL DEL JUEGO */
//     /* ----------------------------------------------------------------------------------------------- */
//     while (!WindowShouldClose()) // DETECTAR QUE SE CIERRA EL PROGRAMA CON LA TECLA (ESC)
//     {
//         ToggleFullscreenAndResize(); // MANEJAR CAMBIOS DE PANTALLA COMPLETA Y TAMAÑO DE VENTANA

//         int buttonClicked = drawinicio();
//         int buttonClicked2 = drawjugar();

//         switch (currentScreen)
//         {
//         case INICIO: // AQUI EN ESTA OPCION ESTARA EL MENU.
//         {
//             if (buttonClicked == 1)
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = JUGAR;
//             }
//             if (buttonClicked == 2)
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = CREDITOS;
//             }
//             if (IsKeyPressed(KEY_M))
//             {
//                 musicToggle = !musicToggle; // CAMBIAR EL ESTADO DE LA MUSICA CON LA TECLA (M)
//                 musicPaused = musica(musicToggle, musicPaused);
//             }
//         }
//         break;
//         case JUGAR:
//         {
//             if (buttonClicked2 == 1)
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = BASICOS;
//             }
//             if (buttonClicked2 == 2)
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = LETRAS;
//             }
//             if (buttonClicked2 == 3)
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = COLORES;
//             }
//             if (IsKeyPressed(KEY_DELETE))
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = INICIO;
//             }
//         }
//         break;
//         case CREDITOS:
//         {
//             if (IsKeyPressed(KEY_DELETE))
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = INICIO;
//             }
//         }
//         case BASICOS:
//         {
//             if (IsKeyPressed(KEY_DELETE))
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = JUGAR;
//             }
//         }
//         case LETRAS:
//         {
//             if (IsKeyPressed(KEY_DELETE))
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = JUGAR;
//             }
//         }
//         case COLORES:
//         {
//             if (IsKeyPressed(KEY_DELETE))
//             {
//                 PlaySound(buttonSound);
//                 currentScreen = JUGAR;
//             }
//         }
//         break;
//         default:
//             break;
//         }
//         /* ------------------------------------------------------------------------------------------- */
//         /* DIBUJAR */
//         /* ------------------------------------------------------------------------------------------- */
//         menudraw(currentScreen);
//         UpdateMusicStream(musica_fondo);
//     }

//     /* ----------------------------------------------------------------------------------------------- */
//     /* CARGAR TODOS LOS DATOS AQUI: Texturas, Fuentes, Audio, etc... */
//     /* ----------------------------------------------------------------------------------------------- */
//     UnloadContent();
//     UnloadImage(icon);

//     CloseWindow(); // CERRAR VENTANA

//     return 0;
// }

// /* --------------------------------------------------------------------------------------------------- */

// void LoadContent()
// {
//     /* CARGAR LA TEXTURA DE FONDO EN EL MAIN */
//     // 07/12/23 Cambio de imagen de fondo con doble de resolucion (3960 x 2160)
//     // Pantalla Inicio
//     backgroundTexture = LoadTexture("resources/SM_Pantalla.png");
//     backgroundTextureA = LoadTexture("resources/SM_PantallaCreditos.png");
//     backgroundTextureB = LoadTexture("resources/SM_PantallaJugar.png");
//     // Pantalla Jugar
//     backgroundTextureC = LoadTexture("resources/SM_PantallaBasicos.png");
//     backgroundTextureD = LoadTexture("resources/SM_PantallaLetras.png");
//     backgroundTextureE = LoadTexture("resources/SM_PantallaColores.png");
//     /* AJUSTAR EL TAMAÑO DE LAS IMAGENES SEGUN LA RESOLUCION DE LA PANTALLA */
//     // Pantalla Inicio
//     buttonTextureA = LoadTexture("resources/SM_BotonJugar_1.png");
//     buttonTextureB = LoadTexture("resources/SM_BotonCreditos.png");
//     // Pantalla Jugar
//     buttonTextureC = LoadTexture("resources/SM_BotonBasicos.png");
//     buttonTextureD = LoadTexture("resources/SM_BotonLetras.png");
//     buttonTextureE = LoadTexture("resources/SM_BotonColores.png");
//     // Pantalla Basicos
//     buttonTexture1 = LoadTexture("resources/SM_CartaAtras.png");
//     // Sonido/Musica
//     buttonSound = LoadSound("audio/resources/buttonsound.wav");
//     musica_fondo = LoadMusicStream("audio/resources/fondo.mp3");
//     SetMusicVolume(musica_fondo, 1.0f);
// }

// /* --------------------------------------------------------------------------------------------------- */

// bool musica(bool musicToggle, bool musicPaused)
// {
//     if (musicToggle)
//     {
//         /* SI SE CUMPLE musicToggle, PAUSAR LA MUSICA SI ES QUE NO ESTA YA PAUSADA */
//         if (!musicPaused)
//         {
//             PauseMusicStream(musica_fondo);
//             musicPaused = true;
//         }
//     }
//     else
//     {
//         /* REANUDAR LA MUSICA SI musicToggle ESTA PAUSADA */
//         if (musicPaused)
//         {
//             ResumeMusicStream(musica_fondo);
//             musicPaused = false;
//         }
//     }
//     return musicPaused;
// }

// /* --------------------------------------------------------------------------------------------------- */

// void UnloadContent()
// {
//     /* LIBERAR LA TEXTURA DE FONDO AL FINAL DEL PROGRAMA */
//     // FONDOS
//     UnloadTexture(backgroundTexture);
//     UnloadTexture(backgroundTextureA);
//     UnloadTexture(backgroundTextureB);
//     // BOTONES
//     UnloadSound(buttonSound);
//     // Pantalla Inicio
//     UnloadTexture(buttonTextureA);
//     UnloadTexture(buttonTextureB);
//     // Pantalla Jugar
//     UnloadTexture(buttonTextureC);
//     UnloadTexture(buttonTextureD);
//     UnloadTexture(buttonTextureE);
//     // Pantalla Basicos
//     UnloadTexture(buttonTexture1);
//     // MUSICA
//     UnloadMusicStream(musica_fondo);
// }

// /* --------------------------------------------------------------------------------------------------- */

// int drawinicio()
// {
//     DrawTexturePro(
//         backgroundTexture,
//         (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
//         (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
//         (Vector2){0, 0},
//         0.0f,
//         WHITE);

//     /* DIBUJAR LOS BOTONES */
//     // 07/12/23 Cambios en esta parte para ajustar que esten centrados horizontalmente a pesar del tamaño de la pantalla
//     // Tambien modifique la distancia entre ambos botones
//     Rectangle buttonRectA = {(GetScreenWidth() - buttonTextureA.width) / 2, 420.0f, (float)buttonTextureA.width, (float)buttonTextureA.height};
//     Rectangle buttonRectB = {(GetScreenWidth() - buttonTextureB.width) / 2, 970.0f, (float)buttonTextureB.width, (float)buttonTextureB.height};

//     /* VERIFICAR SI EL MOUSE ESTA SOBRE LOS BOTONES */
//     bool isMouseOverButtonA = CheckCollisionPointRec(GetMousePosition(), buttonRectA);
//     bool isMouseOverButtonB = CheckCollisionPointRec(GetMousePosition(), buttonRectB);

//     /* CAMBIAR EL TAMAÑO DEL BOTON (A,B) SI EL MOUSE ESTA SOBRE DE EL */
//     if (isMouseOverButtonA)
//     {
//         /* AGRANDAR Y REDUCIR (A) */
//         float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
//         buttonRectA.width = buttonTextureA.width * scale;
//         buttonRectA.height = buttonTextureA.height * scale;
//     }

//     if (isMouseOverButtonB)
//     {
//         /* AGRANDAR Y REDUCIR (B) */
//         float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
//         buttonRectB.width = buttonTextureB.width * scale;
//         buttonRectB.height = buttonTextureB.height * scale;
//     }

//     /* AJUSTAR LA POSICION DEL BOTON PARA QUE ESTE EN EL CENTRO */
//     Vector2 buttonPosition = {buttonRectA.x + buttonRectA.width / 2, buttonRectA.y + buttonRectA.height / 2};
//     Vector2 buttonPositionB = {buttonRectB.x + buttonRectB.width / 2, buttonRectB.y + buttonRectB.height / 2};
//     DrawTexturePro(buttonTextureA, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureA.width), static_cast<float>(buttonTextureA.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectA.width, buttonRectA.height}, (Vector2){buttonRectA.width / 2, buttonRectA.height / 2}, 0.0f, WHITE);
//     DrawTexturePro(buttonTextureB, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureB.width), static_cast<float>(buttonTextureB.height)}, (Rectangle){buttonPositionB.x, buttonPositionB.y, buttonRectB.width, buttonRectB.height}, (Vector2){buttonRectB.width / 2, buttonRectB.height / 2}, 0.0f, WHITE);

//     /* VERIFICAR SI SE HIZO CLICK EN EL BOTON */
//     if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
//     {
//         if (isMouseOverButtonA)
//             return 1; // Botón A clickeado
//         if (isMouseOverButtonB)
//             return 2; // Botón B clickeado
//     }

//     return 0; // Ningún botón
// }

// /* --------------------------------------------------------------------------------------------------- */

// int drawjugar()
// {
//     DrawTexturePro(
//         backgroundTextureB,
//         (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
//         (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
//         (Vector2){0, 0},
//         0.0f,
//         WHITE);

//     /* DIBUJAR LOS BOTONES */
//     // 07/12/23 Cambios en esta parte para ajustar que esten centrados horizontalmente a pesar del tamaño de la pantalla
//     // Tambien modifique la distancia entre ambos botones
//     Rectangle buttonRectC = {(GetScreenWidth() - buttonTextureC.width) / 2, 400.0f, (float)buttonTextureC.width, (float)buttonTextureC.height};
//     Rectangle buttonRectD = {(GetScreenWidth() - buttonTextureD.width) / 2, 580.0f, (float)buttonTextureD.width, (float)buttonTextureD.height};
//     Rectangle buttonRectE = {(GetScreenWidth() - buttonTextureE.width) / 2, 760.0f, (float)buttonTextureE.width, (float)buttonTextureE.height};

//     /* VERIFICAR SI EL MOUSE ESTA SOBRE LOS BOTONES */
//     bool isMouseOverButtonC = CheckCollisionPointRec(GetMousePosition(), buttonRectC);
//     bool isMouseOverButtonD = CheckCollisionPointRec(GetMousePosition(), buttonRectD);
//     bool isMouseOverButtonE = CheckCollisionPointRec(GetMousePosition(), buttonRectE);

//     /* CAMBIAR EL TAMAÑO DEL BOTON (C,D,E) SI EL MOUSE ESTA SOBRE DE EL */
//     if (isMouseOverButtonC)
//     {
//         /* AGRANDAR Y REDUCIR (C) */
//         float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
//         buttonRectC.width = buttonTextureC.width * scale;
//         buttonRectC.height = buttonTextureC.height * scale;
//     }

//     if (isMouseOverButtonD)
//     {
//         /* AGRANDAR Y REDUCIR (D) */
//         float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
//         buttonRectD.width = buttonTextureD.width * scale;
//         buttonRectD.height = buttonTextureD.height * scale;
//     }

//     if (isMouseOverButtonE)
//     {
//         /* AGRANDAR Y REDUCIR (E) */
//         float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
//         buttonRectE.width = buttonTextureE.width * scale;
//         buttonRectE.height = buttonTextureE.height * scale;
//     }

//     /* AJUSTAR LA POSICION DEL BOTON PARA QUE ESTE EN EL CENTRO */
//     Vector2 buttonPosition = {buttonRectC.x + buttonRectC.width / 2, buttonRectC.y + buttonRectC.height / 2};
//     Vector2 buttonPositionB = {buttonRectD.x + buttonRectD.width / 2, buttonRectD.y + buttonRectD.height / 2};
//     Vector2 buttonPositionC = {buttonRectE.x + buttonRectE.width / 2, buttonRectE.y + buttonRectE.height / 2};
//     DrawTexturePro(buttonTextureC, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureC.width), static_cast<float>(buttonTextureC.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectC.width, buttonRectC.height}, (Vector2){buttonRectC.width / 2, buttonRectC.height / 2}, 0.0f, WHITE);
//     DrawTexturePro(buttonTextureD, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureD.width), static_cast<float>(buttonTextureD.height)}, (Rectangle){buttonPositionB.x, buttonPositionB.y, buttonRectD.width, buttonRectD.height}, (Vector2){buttonRectD.width / 2, buttonRectD.height / 2}, 0.0f, WHITE);
//     DrawTexturePro(buttonTextureE, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureE.width), static_cast<float>(buttonTextureE.height)}, (Rectangle){buttonPositionC.x, buttonPositionC.y, buttonRectE.width, buttonRectE.height}, (Vector2){buttonRectE.width / 2, buttonRectE.height / 2}, 0.0f, WHITE);

//     /* VERIFICAR SI SE HIZO CLICK EN EL BOTON */
//     if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
//     {
//         if (isMouseOverButtonC)
//             return 1; // Botón C clickeado
//         if (isMouseOverButtonD)
//             return 2; // Botón D clickeado
//         if (isMouseOverButtonE)
//             return 3; // Botón D clickeado
//     }

//     return 0; // Ningún botón
// }

// /* --------------------------------------------------------------------------------------------------- */

// void drawcreditos()
// {
//     DrawTexturePro(
//         backgroundTextureA,
//         (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
//         (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
//         (Vector2){0, 0},
//         0.0f,
//         WHITE);
// }

// /* --------------------------------------------------------------------------------------------------- */

// void drawbasicos()
// {
//     DrawTexturePro(
//         backgroundTextureC,
//         (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
//         (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
//         (Vector2){0, 0},
//         0.0f,
//         WHITE);

//     float initialButtonWidth = 180.0f;
//     float initialButtonHeight = 239.0f;

//     Rectangle buttonRect1 = {320.0f, 200.0f, initialButtonWidth, initialButtonHeight};

//     Vector2 buttonPosition = {buttonRect1.x + buttonRect1.width / 2, buttonRect1.y + buttonRect1.height / 2};

//     DrawTexturePro(buttonTexture1, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTexture1.width), static_cast<float>(buttonTexture1.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRect1.width, buttonRect1.height}, (Vector2){buttonRect1.width / 2, buttonRect1.height / 2}, 0.0f, WHITE);
// }

// /* --------------------------------------------------------------------------------------------------- */

// void drawletras()
// {
//     DrawTexturePro(
//         backgroundTextureD,
//         (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
//         (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
//         (Vector2){0, 0},
//         0.0f,
//         WHITE);
// }

// /* --------------------------------------------------------------------------------------------------- */

// void drawcolores()
// {
//     DrawTexturePro(
//         backgroundTextureE,
//         (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
//         (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
//         (Vector2){0, 0},
//         0.0f,
//         WHITE);
// }

// /* --------------------------------------------------------------------------------------------------- */

// void menudraw(GameScreen currentScreen)
// {
//     BeginDrawing();
//     ClearBackground(RAYWHITE);

//     switch (currentScreen)
//     {
//     case INICIO:
//         drawinicio();
//         break;
//     case JUGAR:
//         drawjugar();
//         break;
//     case CREDITOS:
//         drawcreditos();
//         break;
//     case BASICOS:
//         drawbasicos();
//         break;
//     case LETRAS:
//         drawletras();
//         break;
//     case COLORES:
//         drawcolores();
//         break;
//     default:
//         break;
//     }

//     EndDrawing();
// }

// /* --------------------------------------------------------------------------------------------------- */

// void ToggleFullscreenAndResize()
// {
//     if (IsKeyPressed(KEY_F))
//     {
//         /* CAMBIAR LA PANTALLA COMPLETA AL PRESIONAR (F11) */
//         ToggleFullscreen();

//         /* AJUSTAR EL TAMAÑO DE LA PANTALLA AL CAMBIARLO A FULLSCREEN */
//         if (IsWindowFullscreen())
//         {
//             screenWidth = GetMonitorWidth(0);
//             screenHeight = GetMonitorHeight(0);
//         }
//         else
//         {
//             /* RESTAURAR LA RESOLUCION AL SALIR DE LA PANTALLA COMPLETA */
//             screenWidth = GetMonitorWidth(0);
//             screenHeight = GetMonitorHeight(0);
//         }

//         /* CAMBIAR EL TAMAÑO DE LA VENTANA */
//         SetWindowSize(screenWidth, screenHeight);
//     }
// }
