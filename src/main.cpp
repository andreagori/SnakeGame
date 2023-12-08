#include "raylib.h"
#include <cmath>
// #include "raymath.h"

typedef enum GameScreen
{
    INICIO = 0,
    JUGAR,
<<<<<<< HEAD
    CREDITOS,
    BASICOS,
    LETRAS,
    COLORES
=======
    CREDITOS
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
} GameScreen;

// SACAR LA RESOLUCION DEL MONITOR
int screenWidth = GetMonitorWidth(0);
int screenHeight = GetMonitorHeight(0);

// FONDOS
Texture2D backgroundTexture;
// 07/12/23 Añadi imagenes para los fondos
Texture2D backgroundTextureA;
Texture2D backgroundTextureB;
Texture2D backgroundTextureC;
Texture2D backgroundTextureD;
Texture2D backgroundTextureE;
// SONIDO DEL BOTON
Sound buttonSound;
<<<<<<< HEAD
// BOTONES
// Pantalla Inicio
Texture2D buttonTextureA;
Texture2D buttonTextureB;
// PantallaJugar
Texture2D buttonTextureC;
Texture2D buttonTextureD;
Texture2D buttonTextureE;
// MUSICA
Music musica_fondo;

// CARGAR CONTENIDO
void LoadContent();
void UnloadContent();
// PANTALLAS
int drawinicio();
int drawjugar();
void drawopciones();
void drawbasicos();
void drawletras();
void drawcolores();
// EXTRAS
=======
Texture2D buttonTextureA;
Texture2D buttonTextureB;
Music musica_fondo;

void LoadContent();
void UnloadContent();
int drawinicio();
void drawjugar();
void drawopciones();
void drawcreditos();
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
void menudraw(GameScreen currentScreen);
void ToggleFullscreenAndResize();
bool musica(bool musicToggle, bool musicPaused);

/* --------------------------------------------------------------------------------------------------- */
/* PROGRAMA PRINCIPAL */
/* --------------------------------------------------------------------------------------------------- */
int main(void)
{
<<<<<<< HEAD
    /* INICIALIZACION */
=======
    // Initialization
    //--------------------------------------------------------------------------------------

>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
    InitWindow(screenWidth, screenHeight, "SIGN MATCH - GAME");
    InitAudioDevice();

    GameScreen currentScreen = INICIO;

    /* INICIALIZAR TODAS LAS VARIABLES REQUERIDAS PARA CARGAR LOS DATOS AQUI */
    LoadContent();
<<<<<<< HEAD
    SetTargetFPS(60); // FRAMES POR SEGUNDO
=======
    SetTargetFPS(60); // Set desired framerate (frames-per-second)
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
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
<<<<<<< HEAD
        ToggleFullscreenAndResize(); // MANEJAR CAMBIOS DE PANTALLA COMPLETA Y TAMAÑO DE VENTANA

        int buttonClicked = drawinicio();
        int buttonClicked2 = drawjugar();

=======
        // Update
        //----------------------------------------------------------------------------------
        ToggleFullscreenAndResize(); // Manejar cambios de pantalla completa y tamaño de ventana

        int buttonClicked = drawinicio();
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
        switch (currentScreen)
        {
        case INICIO: // AQUI EN ESTA OPCION ESTARA EL MENU.
        {
<<<<<<< HEAD
=======

>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
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
<<<<<<< HEAD
                musicToggle = !musicToggle; // CAMBIAR EL ESTADO DE LA MUSICA CON LA TECLA (M)
=======
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
                musicPaused = musica(musicToggle, musicPaused);
            }
        }
        break;
        case JUGAR:
<<<<<<< HEAD
        {   
            if (buttonClicked2 == 1)
            {
                PlaySound(buttonSound);
                currentScreen = BASICOS;
            }
            if (buttonClicked2 == 2)
            {
                PlaySound(buttonSound);
                currentScreen = LETRAS;
            }
            if (buttonClicked2 == 3)
            {
                PlaySound(buttonSound);
                currentScreen = COLORES;
            }
=======
        {
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
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
        case BASICOS:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                PlaySound(buttonSound);
                currentScreen = JUGAR;
            }
        }
        case LETRAS:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                PlaySound(buttonSound);
                currentScreen = JUGAR;
            }
        }
        case COLORES:
        {
            if (IsKeyPressed(KEY_DELETE))
            {
                PlaySound(buttonSound);
                currentScreen = JUGAR;
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
<<<<<<< HEAD
    /* CARGAR LA TEXTURA DE FONDO EN EL MAIN */
    // 07/12/23 Cambio de imagen de fondo con doble de resolucion (3960 x 2160)
    // Pantalla Inicio
    backgroundTexture = LoadTexture("resources/SM_Pantalla.png");
    backgroundTextureA = LoadTexture("resources/SM_PantallaCreditos.png");
    backgroundTextureB = LoadTexture("resources/SM_PantallaJugar.png");
    // Pantalla Jugar
    backgroundTextureC = LoadTexture("resources/SM_PantallaBasicos.png");
    backgroundTextureD = LoadTexture("resources/SM_PantallaLetras.png");
    backgroundTextureE = LoadTexture("resources/SM_PantallaColores.png");
    /* AJUSTAR EL TAMAÑO DE LAS IMAGENES SEGUN LA RESOLUCION DE LA PANTALLA */
    // Pantalla Inicio
    buttonTextureA = LoadTexture("resources/SM_BotonJugar_1.png");
    buttonTextureB = LoadTexture("resources/SM_BotonCreditos.png");
    // Pantalla Jugar
    buttonTextureC = LoadTexture("resources/SM_BotonBasicos.png");
    buttonTextureD = LoadTexture("resources/SM_BotonLetras.png");
    buttonTextureE = LoadTexture("resources/SM_BotonColores.png");
    // Sonido/Musica
    buttonSound = LoadSound("audio/resources/buttonsound.wav");
    musica_fondo = LoadMusicStream("audio/resources/fondo.mp3");
    SetMusicVolume(musica_fondo, 1.0f);
=======
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
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
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
    // FONDOS
    UnloadTexture(backgroundTexture);
    UnloadTexture(backgroundTextureA);
    UnloadTexture(backgroundTextureB);
    // BOTONES
    UnloadSound(buttonSound);
<<<<<<< HEAD
    // Pantalla Inicio
    UnloadTexture(buttonTextureA);
    UnloadTexture(buttonTextureB);
    // Pantalla Jugar
    UnloadTexture(buttonTextureC);
    UnloadTexture(buttonTextureD);
    UnloadTexture(buttonTextureE);
    // MUSICA
    UnloadMusicStream(musica_fondo);
=======
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
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
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
    // 07/12/23 Cambios en esta parte para ajustar que esten centrados horizontalmente a pesar del tamaño de la pantalla
    // Tambien modifique la distancia entre ambos botones
    Rectangle buttonRectA = {(GetScreenWidth() - buttonTextureA.width) / 2, 420.0f, (float)buttonTextureA.width, (float)buttonTextureA.height};
    Rectangle buttonRectB = {(GetScreenWidth() - buttonTextureB.width) / 2, 970.0f, (float)buttonTextureB.width, (float)buttonTextureB.height};

    /* VERIFICAR SI EL MOUSE ESTA SOBRE LOS BOTONES */
    bool isMouseOverButtonA = CheckCollisionPointRec(GetMousePosition(), buttonRectA);
    bool isMouseOverButtonB = CheckCollisionPointRec(GetMousePosition(), buttonRectB);

    /* CAMBIAR EL TAMAÑO DEL BOTON (A,B) SI EL MOUSE ESTA SOBRE DE EL */
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

int drawjugar()
{
    DrawTexturePro(
        backgroundTextureB,
        (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

        /* DIBUJAR LOS BOTONES */
    // 07/12/23 Cambios en esta parte para ajustar que esten centrados horizontalmente a pesar del tamaño de la pantalla
    // Tambien modifique la distancia entre ambos botones
    Rectangle buttonRectC = {(GetScreenWidth() - buttonTextureC.width) / 2, 400.0f, (float)buttonTextureC.width, (float)buttonTextureC.height};
    Rectangle buttonRectD = {(GetScreenWidth() - buttonTextureD.width) / 2, 580.0f, (float)buttonTextureD.width, (float)buttonTextureD.height};
    Rectangle buttonRectE = {(GetScreenWidth() - buttonTextureE.width) / 2, 760.0f, (float)buttonTextureE.width, (float)buttonTextureE.height};

    /* VERIFICAR SI EL MOUSE ESTA SOBRE LOS BOTONES */
    bool isMouseOverButtonC = CheckCollisionPointRec(GetMousePosition(), buttonRectC);
    bool isMouseOverButtonD = CheckCollisionPointRec(GetMousePosition(), buttonRectD);
    bool isMouseOverButtonE = CheckCollisionPointRec(GetMousePosition(), buttonRectE);

    /* CAMBIAR EL TAMAÑO DEL BOTON (C,D,E) SI EL MOUSE ESTA SOBRE DE EL */
    if (isMouseOverButtonC)
    {
        /* AGRANDAR Y REDUCIR (C) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectC.width = buttonTextureC.width * scale;
        buttonRectC.height = buttonTextureC.height * scale;
    }

    if (isMouseOverButtonD)
    {
        /* AGRANDAR Y REDUCIR (D) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectD.width = buttonTextureD.width * scale;
        buttonRectD.height = buttonTextureD.height * scale;
    }

    if (isMouseOverButtonE)
    {
        /* AGRANDAR Y REDUCIR (E) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectE.width = buttonTextureE.width * scale;
        buttonRectE.height = buttonTextureE.height * scale;
    }

    /* AJUSTAR LA POSICION DEL BOTON PARA QUE ESTE EN EL CENTRO */
    Vector2 buttonPosition = {buttonRectC.x + buttonRectC.width / 2, buttonRectC.y + buttonRectC.height / 2};
    Vector2 buttonPositionB = {buttonRectD.x + buttonRectD.width / 2, buttonRectD.y + buttonRectD.height / 2};
    Vector2 buttonPositionC = {buttonRectE.x + buttonRectE.width / 2, buttonRectE.y + buttonRectE.height / 2};
    DrawTexturePro(buttonTextureC, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureC.width), static_cast<float>(buttonTextureC.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectC.width, buttonRectC.height}, (Vector2){buttonRectC.width / 2, buttonRectC.height / 2}, 0.0f, WHITE);
    DrawTexturePro(buttonTextureD, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureD.width), static_cast<float>(buttonTextureD.height)}, (Rectangle){buttonPositionB.x, buttonPositionB.y, buttonRectD.width, buttonRectD.height}, (Vector2){buttonRectD.width / 2, buttonRectD.height / 2}, 0.0f, WHITE);
    DrawTexturePro(buttonTextureE, (Rectangle){0.0f, 0.0f, static_cast<float>(buttonTextureE.width), static_cast<float>(buttonTextureE.height)}, (Rectangle){buttonPositionC.x, buttonPositionC.y, buttonRectE.width, buttonRectE.height}, (Vector2){buttonRectE.width / 2, buttonRectE.height / 2}, 0.0f, WHITE);

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

/* --------------------------------------------------------------------------------------------------- */

void drawcreditos()
{
    DrawTexturePro(
        backgroundTextureA,
        (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);
}

/* --------------------------------------------------------------------------------------------------- */

void drawbasicos()
{
    DrawTexturePro(
        backgroundTextureC,
        (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);
}

/* --------------------------------------------------------------------------------------------------- */

void drawletras()
{
    DrawTexturePro(
        backgroundTextureD,
        (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);
}

/* --------------------------------------------------------------------------------------------------- */

void drawcolores()
{
    DrawTexturePro(
        backgroundTextureE,
        (Rectangle){0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);
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
<<<<<<< HEAD
        break;
    case BASICOS:
        drawbasicos();
        break;
    case LETRAS:
        drawletras();
        break;
    case COLORES:
        drawcolores();
=======
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
        break;
    default:
        break;
    }

    EndDrawing();
}

<<<<<<< HEAD
/* --------------------------------------------------------------------------------------------------- */

=======
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
void ToggleFullscreenAndResize()
{
    if (IsKeyPressed(KEY_F))
    {
<<<<<<< HEAD
        /* CAMBIAR LA PANTALLA COMPLETA AL PRESIONAR (F11) */
        ToggleFullscreen();

        /* AJUSTAR EL TAMAÑO DE LA PANTALLA AL CAMBIARLO A FULLSCREEN */
=======
        // Cambiar a pantalla completa al presionar F11
        ToggleFullscreen();

        // Ajustar el tamaño de la ventana al cambiar a pantalla completa
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
        if (IsWindowFullscreen())
        {
            screenWidth = GetMonitorWidth(0);
            screenHeight = GetMonitorHeight(0);
        }
        else
        {
<<<<<<< HEAD
            /* RESTAURAR LA RESOLUCION AL SALIR DE LA PANTALLA COMPLETA */
=======
            // Restaurar la resolución original al salir de pantalla completa
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
            screenWidth = GetMonitorWidth(0);
            screenHeight = GetMonitorHeight(0);
        }

<<<<<<< HEAD
        /* CAMBIAR EL TAMAÑO DE LA VENTANA */
=======
        // Cambiar el tamaño de la ventana
>>>>>>> 81d80953873cea8fc1dfa8a805c5e1c403f7c793
        SetWindowSize(screenWidth, screenHeight);
    }
}
