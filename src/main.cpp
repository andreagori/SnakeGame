#include "raylib.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <string.h>

// TYPEDEF PARA SUBMENUS Y MENUS.

typedef enum jugarscreen
{
    BASICO,
    LETRAS,
    COLORES,
} jugarscreen;

// ESTE TYPEDEF ME AYUDA CON EL ESTADO DEL JUEGO, SI ESTA JUGANDO O SI REGRESA AL MENU ANTERIOR. ------------------------------
typedef enum JuegoEstado
{
    JUEGO_JUGANDO,
    JUEGO_REGRESAR_MENU,
    MUSICA,
    FULLSCREEN,
} JuegoEstado;

// ESTE TYPEDEF ME AYUDA CON LOS ESTADOS DE LA PANTALLA, SI ESTA EN EL MENU, JUGANDO, CREDITOS, ETC. TAMBIEN CON JUGAR_... BASICAMENTE JUNTO EL PRIMER TYPEDEF CON ESTE.
typedef enum GameScreen
{
    INICIO = 0,
    JUGAR,
    CREDITOS,
    JUGAR_BASICO,
    JUGAR_LETRAS,
    JUGAR_COLORES,
} GameScreen;

// STRUCT PARA CARGAR LOS ARCHIVOS DE LAS PANTALLAS. ------------------------------
typedef struct cargas
{
    Texture2D backgroundTexture;
    Texture2D backgroundTexture_creditos;
    Texture2D backgroundTexture_basico;
    Texture2D backgroundTexture_letras;
    Texture2D backgroundTexture_colores;
    Sound buttonSound;
    Texture2D buttonTextureA;
    Texture2D buttonTextureB;
    Texture2D buttonTextureC;
    Texture2D buttonTextureD;
    Texture2D buttonTextureE;
    Texture2D buttonTexture1;
    Texture2D buttonTexture2;
    Texture2D buttonTexture3;
    Texture2D buttonTexture4;
    Music musica_fondo;
} cargas;

typedef struct cartas
{
    Texture2D carta_back;
    Texture2D carta1;
    Texture2D carta2;
    Texture2D carta3;
    Texture2D carta4;
    Texture2D carta5;
    Texture2D carta6;
    Texture2D carta7;
    Texture2D carta8;
    Texture2D carta9;
} cartas;

typedef struct memorama
{
    int state;
    int firstCardCol;
    int firstCardRow;
    bool cardSelected;
    int selectedCardIndex;
    int cardsMatched;
    int numRows = 3;
    int numCols = 6;
    int cardStates[3][6];
    int cards[3][6];
} memorama;

// AQUI IRA EL NUEVO STRUCT PARA CADA TEXTURA DE LAS CARTAS, HACER UNO PARA CADA CATEGORIA. ------------------------------

// SACAR LA RESOLUCION DEL MONITOR
int screenWidth = GetMonitorWidth(0);
int screenHeight = GetMonitorHeight(0);

// PROTOTIPOS DE FUNCIONES ------------------------------
// >> CARGAS ARCHIVOS Y MENUS.
cargas LoadContent(const char pantalla[], cargas archivos);
int drawcreditos(cargas archivos);
void menudraw(GameScreen currentScreen, cargas archivos, cartas todo, memorama &memo);
void ToggleFullscreenAndResize();
bool musica(bool musicToggle, bool &musicPaused, Music &musica_fondo);
int drawinicio(cargas archivos);
int drawjugar(GameScreen currentScreen, cargas archivos);
void UnloadContent(cargas archivos, GameScreen currentScreen);
// >> JUEGO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void inicializarMemorama(memorama *estruct);
void juego_basico(cartas todo, memorama *estruct);
cartas LoadCartas_b(const char categoria[], cartas todo);
cartas UnloadCartas_b(const char categoria[], cartas todo);
Texture2D GetCartaTexture(cartas todo, int numeroCarta);
JuegoEstado jugar_basico(GameScreen currentScreen, cargas archivos, cartas todo, memorama &memo);
void menudraw(GameScreen currentScreen, cargas archivos, cartas todo, memorama &memo);
JuegoEstado jugar_letras(GameScreen currentScreen, cargas archivos);
JuegoEstado jugar_colores(GameScreen currentScreen, cargas archivos);

// MAIN ------------------------------
int main(void)
{
    // CREAR LA VENTANA E INICIAR EL AUDIO, Y CARGAR LOS ARCHIVOS.
    InitWindow(screenWidth, screenHeight, "SIGN MATCH - GAME");
    InitAudioDevice();
    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    Image icon = LoadImage("resources/SG_ICONO.png");
    SetWindowIcon(icon);

    // ASIGNAR LAS PANTALLAS A LAS VARIABLES. Y TAMBIEN CARGAR EL SONIDO Y LA MUSICA.
    GameScreen currentScreen = INICIO;
    GameScreen nextScreen = INICIO;
    cargas archivos;
    archivos.buttonSound = LoadSound("audio/resources/buttonsound.wav");
    archivos.musica_fondo = LoadMusicStream("audio/resources/fondo.mp3");
    archivos = LoadContent("MENU", archivos);
    // BANDERAS DE LA FUNCION PAUSA MUSICA. Y REPRODUCIR LA MUSICA DE FONDO.
    bool musicPaused = false;
    bool musicToggle = false;

    // JUEGO
    cartas todo;
    cartas basico;
    memorama memo;
    basico = LoadCartas_b("BASICO", basico);
    PlayMusicStream(archivos.musica_fondo);
    //--------------------------------------------------------------------------------------

    // MIENTRAS QUE NO SE CIERRE LA VENTANA, SEGUIR EJECUTANDO EL PROGRAMA.
    while (!WindowShouldClose()) // TE VA A SACAR DEL PROGRAMA CON EL "ESC".
    {
        ToggleFullscreenAndResize(); // FUNCION PARA PANTALLA COMPLETA.

        // VARIABLES PARA LOS BOTONES. Y NAVEGAR ENTRE PANTALLAS.
        int buttonClicked = drawinicio(archivos);
        int buttonClicked2 = drawjugar(currentScreen, archivos);
        int buttonClicked3 = drawcreditos(archivos);

        // SWITCH PARA CAMBIAR DE PANTALLAS EN EL JUEGO, Y TAMBIEN PARA CAMBIAR DE PANTALLAS EN EL MENU.
        switch (currentScreen)
        {
        case INICIO: // AQUI EN ESTA OPCION ESTARA EL MENU.
        {
            if (buttonClicked == 1)
            {
                // SI SE PRESIONA EL BOTON 1, CAMBIAR A LA PANTALLA JUGAR. Y DESCARGAR LOS ARCHIVOS DE LA PANTALLA ANTERIOR. Y CARGAR LOS ARCHIVOS DE LA PANTALLA NUEVA.
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                UnloadContent(archivos, INICIO);
                archivos = LoadContent("JUGAR", archivos);
            }
            if (buttonClicked == 2)
            {
                // SI SE PRESIONA EL BOTON 2, CAMBIAR A LA PANTALLA CREDITOS. Y DESCARGAR LOS ARCHIVOS DE LA PANTALLA ANTERIOR. Y CARGAR LOS ARCHIVOS DE LA PANTALLA NUEVA.
                PlaySound(archivos.buttonSound);
                currentScreen = CREDITOS;
                UnloadContent(archivos, INICIO);
                archivos = LoadContent("CREDITOS", archivos);
            }
            if (IsKeyPressed(KEY_M) || buttonClicked == 4)
            {
                // SI SE PRESIONA LA TECLA "M", PAUSAR LA MUSICA.
                PlaySound(archivos.buttonSound);
                musicToggle = !musicToggle;
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (buttonClicked == 3)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
        }
        break;
        case JUGAR: // AQUI EN ESTA OPCION ESTARA EL SUBMENU DE JUGAR.
        {
            if (buttonClicked2 == 1)
            {
                // SI SE PRESIONA EL BOTON 1, CAMBIAR A LA PANTALLA JUGAR_BASICO. Y DESCARGAR LOS ARCHIVOS DE LA PANTALLA ANTERIOR. Y CARGAR LOS ARCHIVOS DE LA PANTALLA NUEVA.
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_BASICO;
                archivos = LoadContent("JUGAR_BASICO", archivos);
            }
            if (buttonClicked2 == 2)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_LETRAS;
                archivos = LoadContent("JUGAR_LETRAS", archivos);
            }
            if (buttonClicked2 == 3)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_COLORES;
                archivos = LoadContent("JUGAR_COLORES", archivos);
            }
            if (IsKeyPressed(KEY_DELETE) || buttonClicked2 == 6)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = nextScreen;
                archivos = LoadContent("MENU", archivos);
            }
            if (IsKeyPressed(KEY_M) || buttonClicked2 == 5)
            {
                // SI SE PRESIONA LA TECLA "M", PAUSAR LA MUSICA.
                PlaySound(archivos.buttonSound);
                musicToggle = !musicToggle;
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (buttonClicked2 == 4)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
        }
        break;
        case JUGAR_BASICO:
        {
            // Llama a la función jugar_basico y obtén el estado del juego
            // BOTONES DE CARTAS FINAL -------------------------------------------------
            inicializarMemorama(&memo);
            JuegoEstado estadoJuego = jugar_basico(currentScreen, archivos, basico, memo);

            // Verifica el estado del juego
            if (estadoJuego == JUEGO_REGRESAR_MENU)
            {
                // Regresa al menú
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                UnloadContent(archivos, JUGAR_BASICO);
                archivos = LoadContent("JUEGO", archivos);
            }
            if (IsKeyPressed(KEY_M) || estadoJuego == MUSICA)
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (IsKeyPressed(KEY_F) || estadoJuego == FULLSCREEN)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
            break;
        }
        case JUGAR_LETRAS:
        {
            JuegoEstado estadoJuego = jugar_letras(currentScreen, archivos);
            if (estadoJuego == JUEGO_REGRESAR_MENU)
            {
                // Regresa al menú
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                UnloadContent(archivos, JUGAR_LETRAS);
                archivos = LoadContent("JUEGO", archivos);
            }
            if (IsKeyPressed(KEY_M) || estadoJuego == MUSICA)
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (IsKeyPressed(KEY_F) || estadoJuego == FULLSCREEN)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
            break;
        }
        case JUGAR_COLORES:
        {
            JuegoEstado estadoJuego = jugar_colores(currentScreen, archivos);
            if (estadoJuego == JUEGO_REGRESAR_MENU)
            {
                // Regresa al menú
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                UnloadContent(archivos, JUGAR_COLORES);
                archivos = LoadContent("JUEGO", archivos);
            }
            if (IsKeyPressed(KEY_M) || estadoJuego == MUSICA)
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (IsKeyPressed(KEY_F) || estadoJuego == FULLSCREEN)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
            break;
        }
        case CREDITOS:
        {
            if (!musicPaused) // Only resume the music if it's not supposed to be paused
            {
                ResumeMusicStream(archivos.musica_fondo);
            }
            if (IsKeyPressed(KEY_DELETE) || buttonClicked3 == 2)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = nextScreen;
                UnloadContent(archivos, CREDITOS);
                archivos = LoadContent("MENU", archivos);
            }
            if (IsKeyPressed(KEY_M) || buttonClicked3 == 3)
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (buttonClicked3 == 1)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
            break;
        }
        default:
            break;
        }
        // Draw
        //----------------------------------------------------------------------------------
        menudraw(currentScreen, archivos, basico, memo);
        UpdateMusicStream(archivos.musica_fondo);
    }

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadContent(archivos, currentScreen);
    UnloadContent(archivos, nextScreen);
    UnloadCartas_b("BASICO", basico);
    UnloadSound(archivos.buttonSound);
    UnloadMusicStream(archivos.musica_fondo);
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
        archivos.backgroundTexture = LoadTexture("resources/SM_Pantalla.png");
        archivos.buttonTextureA = LoadTexture("resources/SM_BotonJugar_1.png");
        archivos.buttonTextureB = LoadTexture("resources/SM_BotonCreditos.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
        SetMusicVolume(archivos.musica_fondo, 1.0f);
    }
    if (strcmp(pantalla, "JUGAR") == 0)
    {
        archivos.backgroundTexture = LoadTexture("resources/SM_PantallaJugar.png");
        archivos.buttonTextureC = LoadTexture("resources/SM_BotonBasicos.png");
        archivos.buttonTextureD = LoadTexture("resources/SM_BotonLetras.png");
        archivos.buttonTextureE = LoadTexture("resources/SM_BotonColores.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
    }

    if (strcmp(pantalla, "CREDITOS") == 0)
    {
        archivos.backgroundTexture_creditos = LoadTexture("resources/SM_PantallaCreditos.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
    }

    if (strcmp(pantalla, "JUGAR_BASICO") == 0)
    {
        archivos.backgroundTexture_basico = LoadTexture("resources/SM_PantallaBasicos.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
        // archivos.buttonTexture1 = LoadTexture("resources/SM_CartaAtras.png");
    }

    if (strcmp(pantalla, "JUGAR_LETRAS") == 0)
    {
        archivos.backgroundTexture_letras = LoadTexture("resources/SM_PantallaLetras.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
        // archivos.buttonTexture1 = LoadTexture("resources/SM_CartaAtras.png");
    }

    if (strcmp(pantalla, "JUGAR_COLORES") == 0)
    {
        archivos.backgroundTexture_colores = LoadTexture("resources/SM_PantallaColores.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
        // archivos.buttonTexture1 = LoadTexture("resources/SM_CartaAtras.png");
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
        UnloadTexture(archivos.buttonTexture2);
        UnloadTexture(archivos.buttonTexture4);
    }
    if (currentScreen == JUGAR)
    {
        UnloadTexture(archivos.backgroundTexture);
        UnloadTexture(archivos.buttonTextureC);
        UnloadTexture(archivos.buttonTextureD);
        UnloadTexture(archivos.buttonTextureE);
        UnloadTexture(archivos.buttonTexture3);
    }
    if (currentScreen == CREDITOS)
    {
        UnloadTexture(archivos.backgroundTexture_creditos);
    }

    if (currentScreen == JUGAR_BASICO)
    {
        UnloadTexture(archivos.backgroundTexture_basico);
        // UnloadTexture(archivos.buttonTexture1);
    }

    if (currentScreen == JUGAR_LETRAS)
    {
        UnloadTexture(archivos.backgroundTexture_letras);
        // UnloadTexture(archivos.buttonTexture1);
    }

    if (currentScreen == JUGAR_COLORES)
    {
        UnloadTexture(archivos.backgroundTexture_colores);
        // UnloadTexture(archivos.buttonTexture1);
    }
}

cartas LoadCartas_b(const char categoria[], cartas todo)
{
    if (strcmp(categoria, "BASICO") == 0)
    {
        todo.carta_back = LoadTexture("resources/SM_CartaAtras.png");
        todo.carta1 = LoadTexture("resources/SM_CartaBien.png");
        todo.carta2 = LoadTexture("resources/SM_CartaEs.png");
        todo.carta3 = LoadTexture("resources/SM_CartaGracias.png");
        todo.carta4 = LoadTexture("resources/SM_CartaMal.png");
        todo.carta5 = LoadTexture("resources/SM_CartaNo.png");
        todo.carta6 = LoadTexture("resources/SM_CartaPerdon.png");
        todo.carta7 = LoadTexture("resources/SM_CartaPorFavor.png");
        todo.carta8 = LoadTexture("resources/SM_CartaQue.png");
        todo.carta9 = LoadTexture("resources/SM_CartaSi.png");
    }

    return todo;
}

cartas UnloadCartas_b(const char categoria[], cartas todo)
{
    if (strcmp(categoria, "BASICO") == 0)
    {
        UnloadTexture(todo.carta_back);
        UnloadTexture(todo.carta1);
        UnloadTexture(todo.carta2);
        UnloadTexture(todo.carta3);
        UnloadTexture(todo.carta4);
        UnloadTexture(todo.carta5);
        UnloadTexture(todo.carta6);
        UnloadTexture(todo.carta7);
        UnloadTexture(todo.carta8);
        UnloadTexture(todo.carta9);
    }

    return todo;
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

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    /* VERIFICAR SI EL MOUSE ESTÁ SOBRE LOS BOTONES */
    bool isMouseOverButtonA = CheckCollisionPointRec(GetMousePosition(), buttonRectA);
    bool isMouseOverButtonB = CheckCollisionPointRec(GetMousePosition(), buttonRectB);
    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

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
    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    /* AJUSTAR LA POSICIÓN DEL BOTÓN PARA QUE ESTÉ EN EL CENTRO */
    Vector2 buttonPosition = {buttonRectA.x + buttonRectA.width / 2, buttonRectA.y + buttonRectA.height / 2};
    Vector2 buttonPositionB = {buttonRectB.x + buttonRectB.width / 2, buttonRectB.y + buttonRectB.height / 2};
    DrawTexturePro(archivos.buttonTextureA, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureA.width), static_cast<float>(archivos.buttonTextureA.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectA.width, buttonRectA.height}, (Vector2){buttonRectA.width / 2, buttonRectA.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureB, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureB.width), static_cast<float>(archivos.buttonTextureB.height)}, (Rectangle){buttonPositionB.x, buttonPositionB.y, buttonRectB.width, buttonRectB.height}, (Vector2){buttonRectB.width / 2, buttonRectB.height / 2}, 0.0f, WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButtonA)
            return 1; // Botón A clickeado
        if (isMouseOverButtonB)
            return 2; // Botón B clickeado
        if (isMouseOverButton2)
            return 3; // Botón 2 clickeado
        if (isMouseOverButton4)
            return 4; // Botón 4 clickeado
    }

    return 0; // Ningún botón
}

Texture2D GetCartaTexture(cartas todo, int numeroCarta)
{
    // Dependiendo del número de la carta, devuelve la textura correspondiente
    switch (numeroCarta)
    {
    case 1:
        return todo.carta1;
    case 2:
        return todo.carta2;
    case 3:
        return todo.carta3;
    case 4:
        return todo.carta4;
    case 5:
        return todo.carta5;
    case 6:
        return todo.carta6;
    case 7:
        return todo.carta7;
    case 8:
        return todo.carta8;
    case 9:
        return todo.carta9;
    // Agrega más casos según la cantidad de cartas que tengas
    default:
        return todo.carta_back; // Textura por defecto (carta oculta)
    }
}

void inicializarMemorama(memorama *estruct)
{

    // Initialize the firstCardRow and firstCardCol to -1
    estruct->firstCardRow = -1;
    estruct->firstCardCol = -1;

    int cartaActual = 1;

    // Iterar sobre cada fila y columna
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 6; ++col)
        {
            // Asignar el valor de la carta actual y avanzar al siguiente
            estruct->cards[row][col] = cartaActual;
            estruct->cardStates[row][col] = 0;
            cartaActual++;
        }
    }
}

void juego_basico(cartas todo, memorama *estruct)
{
    // Obtener las dimensiones de la ventana
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Definir el número de filas y columnas
    int numRows = estruct->numRows;
    int numCols = estruct->numCols;

    // Calcular el espaciado entre las cartas
    float paddingX = 10.0f;
    float paddingY = 10.0f;

    // Definir la relación de aspecto deseada (altura / anchura)
    float aspectRatio = 179.0f / 130.0f; // Ajusta esto según la relación de aspecto de tus cartas

    // Ajustar el tamaño de las cartas
    float adjustedCardWidth = 130.0f; // Ajusta el valor según lo que desees
    float cardHeight = adjustedCardWidth * aspectRatio;

    // Calcular la posición inicial para centrar la cuadrícula en la pantalla
    float startX = (screenWidth - numCols * (adjustedCardWidth + paddingX) + paddingX) / 2.0f;
    float startY = (screenHeight - numRows * (cardHeight + paddingY) + paddingY) / 2.0f;

    // Obtener la posición del mouse
    Vector2 mousePosition = GetMousePosition();

    for (int row = 0; row < numRows; ++row)
    {
        for (int col = 0; col < numCols; ++col)
        {
            // Calcular la posición de la carta
            float xPos = startX + col * (adjustedCardWidth + paddingX);
            float yPos = startY + row * (cardHeight + paddingY);

            // Crear un rectángulo para la carta
            Rectangle cardRect = {xPos, yPos, adjustedCardWidth, cardHeight};

            // Verificar si el mouse está sobre la carta
            bool isMouseOverCard = CheckCollisionPointRec(mousePosition, cardRect);

            // Cambiar el estado de la carta al hacer clic izquierdo si el estado actual es 0
            if (isMouseOverCard && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && estruct->cardStates[row][col] == 0)
            {
                estruct->cardStates[row][col] = 1;

                // Si esta es la primera carta del par
                if (estruct->firstCardRow == -1 && estruct->firstCardCol == -1)
                {
                    // Guardar la posición de la primera carta
                    estruct->firstCardRow = row;
                    estruct->firstCardCol = col;
                }
                else
                {
                    // Esta es la segunda carta del par

                    // Si las cartas no coinciden
                    if (estruct->cards[row][col] != estruct->cards[estruct->firstCardRow][estruct->firstCardCol])
                    {
                        // Ocultar las cartas
                        estruct->cardStates[row][col] = 0;
                        estruct->cardStates[estruct->firstCardRow][estruct->firstCardCol] = 0;
                    }

                    // Restablecer la posición de la primera carta
                    estruct->firstCardRow = -1;
                    estruct->firstCardCol = -1;
                }
            }

            // Dibujar la carta
            if (estruct->cardStates[row][col] == 0)
            {
                // Si la carta está oculta, dibujar la textura de la carta oculta
                DrawTexturePro(
                    todo.carta_back,
                    (Rectangle){0, 0, (float)todo.carta_back.width, (float)todo.carta_back.height},
                    cardRect,
                    (Vector2){0, 0},
                    0.0f,
                    WHITE);
            }
            else
            {
                // Si la carta está revelada, dibujar la textura de la carta correspondiente
                int numeroCarta = (row * numCols + col) % (numRows * numCols / 2) + 1; // Asigna números del 1 al (numRows * numCols / 2) a las cartas
                DrawTexturePro(
                    GetCartaTexture(todo, numeroCarta),
                    (Rectangle){0, 0, (float)GetCartaTexture(todo, numeroCarta).width, (float)GetCartaTexture(todo, numeroCarta).height},
                    cardRect,
                    (Vector2){0, 0},
                    0.0f,
                    WHITE);
            }
        }
    }
}

JuegoEstado jugar_basico(GameScreen currentScreen, cargas archivos, cartas todo, memorama &memo)
{
    DrawTexturePro(
        archivos.backgroundTexture_basico,
        (Rectangle){0, 0, (float)archivos.backgroundTexture_basico.width, (float)archivos.backgroundTexture_basico.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    juego_basico(todo, &memo);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (3) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButton2)
            return FULLSCREEN; // Botón 2 clickeado
        if (isMouseOverButton3)
            return JUEGO_REGRESAR_MENU; // Botón 3 clickeado
        if (isMouseOverButton4)
            return MUSICA; // Botón 4 clickeado
    }

    if (IsKeyPressed(KEY_DELETE))
    {
        // Si se presiona DELETE, regresar al menú
        return JUEGO_REGRESAR_MENU;
    }

    // Si no hay cambio de pantalla, continúa jugando
    return JUEGO_JUGANDO;
}

JuegoEstado jugar_letras(GameScreen currentScreen, cargas archivos)
{
    DrawTexturePro(
        archivos.backgroundTexture_letras,
        (Rectangle){0, 0, (float)archivos.backgroundTexture_letras.width, (float)archivos.backgroundTexture_letras.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (3) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    if (IsKeyPressed(KEY_DELETE))
    {
        // Si se presiona DELETE, regresar al menú
        return JUEGO_REGRESAR_MENU;
    }

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButton2)
            return FULLSCREEN; // Botón 2 clickeado
        if (isMouseOverButton3)
            return JUEGO_REGRESAR_MENU; // Botón 3 clickeado
        if (isMouseOverButton4)
            return MUSICA; // Botón 4 clickeado
    }

    // Si no hay cambio de pantalla, continúa jugando
    return JUEGO_JUGANDO;
}

JuegoEstado jugar_colores(GameScreen currentScreen, cargas archivos)
{
    DrawTexturePro(
        archivos.backgroundTexture_colores,
        (Rectangle){0, 0, (float)archivos.backgroundTexture_colores.width, (float)archivos.backgroundTexture_colores.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (3) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    if (IsKeyPressed(KEY_DELETE))
    {
        // Si se presiona DELETE, regresar al menú
        return JUEGO_REGRESAR_MENU;
    }

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButton2)
            return FULLSCREEN; // Botón 2 clickeado
        if (isMouseOverButton3)
            return JUEGO_REGRESAR_MENU; // Botón 3 clickeado
        if (isMouseOverButton4)
            return MUSICA; // Botón 4 clickeado
    }

    // Si no hay cambio de pantalla, continúa jugando
    return JUEGO_JUGANDO;
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

    // Usar proporciones relativas para calcular las posiciones de los botones
    float buttonStartY = 0.35f * GetScreenHeight(); // Ajusta el valor 0.35 según tus necesidades
    float buttonSpacing = 0.2f * GetScreenHeight(); // Ajusta el valor 0.2 según tus necesidades

    Rectangle buttonRectC = {(GetScreenWidth() - archivos.buttonTextureC.width) / 2, buttonStartY, (float)archivos.buttonTextureC.width, (float)archivos.buttonTextureC.height};
    Rectangle buttonRectD = {(GetScreenWidth() - archivos.buttonTextureD.width) / 2, buttonStartY + buttonSpacing, (float)archivos.buttonTextureD.width, (float)archivos.buttonTextureD.height};
    Rectangle buttonRectE = {(GetScreenWidth() - archivos.buttonTextureE.width) / 2, buttonStartY + 2 * buttonSpacing, (float)archivos.buttonTextureE.width, (float)archivos.buttonTextureE.height};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    /* VERIFICAR SI EL MOUSE ESTA SOBRE LOS BOTONES */
    bool isMouseOverButtonC = CheckCollisionPointRec(GetMousePosition(), buttonRectC);
    bool isMouseOverButtonD = CheckCollisionPointRec(GetMousePosition(), buttonRectD);
    bool isMouseOverButtonE = CheckCollisionPointRec(GetMousePosition(), buttonRectE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

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

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    /* AJUSTAR LA POSICION DEL BOTON PARA QUE ESTE EN EL CENTRO */
    Vector2 buttonPosition = {buttonRectC.x + buttonRectC.width / 2, buttonRectC.y + buttonRectE.height / 2};
    Vector2 buttonPositionD = {buttonRectD.x + buttonRectD.width / 2, buttonRectD.y + buttonRectD.height / 2};
    Vector2 buttonPositionE = {buttonRectE.x + buttonRectE.width / 2, buttonRectE.y + buttonRectE.height / 2};
    DrawTexturePro(archivos.buttonTextureC, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureC.width), static_cast<float>(archivos.buttonTextureC.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectC.width, buttonRectC.height}, (Vector2){buttonRectC.width / 2, buttonRectC.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureD, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureD.width), static_cast<float>(archivos.buttonTextureD.height)}, (Rectangle){buttonPositionD.x, buttonPositionD.y, buttonRectD.width, buttonRectD.height}, (Vector2){buttonRectD.width / 2, buttonRectD.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureE, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureE.width), static_cast<float>(archivos.buttonTextureE.height)}, (Rectangle){buttonPositionE.x, buttonPositionE.y, buttonRectE.width, buttonRectE.height}, (Vector2){buttonRectE.width / 2, buttonRectE.height / 2}, 0.0f, WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLICK EN EL BOTON */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButtonC)
            return 1; // Botón C clickeado
        if (isMouseOverButtonD)
            return 2; // Botón D clickeado
        if (isMouseOverButtonE)
            return 3; // Botón D clickeado
        if (isMouseOverButton2)
            return 4; // Botón 2 clickeado
        if (isMouseOverButton3)
            return 6; // Botón 3 clickeado
        if (isMouseOverButton4)
            return 5; // Botón 4 clickeado
    }

    return 0; // Ningún botón
}

int drawcreditos(cargas archivos)
{
    DrawTexturePro(
        archivos.backgroundTexture_creditos,
        (Rectangle){0, 0, (float)archivos.backgroundTexture_creditos.width, (float)archivos.backgroundTexture_creditos.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLICK EN EL BOTON */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButton2)
            return 1; // Botón 2 clickeado
        if (isMouseOverButton3)
            return 2; // Botón 3 clickeado
        if (isMouseOverButton4)
            return 3; // Botón 4 clickeado
    }

    return 0; // Ningún botón
}

void menudraw(GameScreen currentScreen, cargas archivos, cartas todo, memorama &memo)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (currentScreen)
    {
    case INICIO:
    {
        drawinicio(archivos);
        break;
    }
    case JUGAR:
    {
        drawjugar(currentScreen, archivos);
        break;
    }
    case CREDITOS:
    {
        drawcreditos(archivos);
        break;
    }
    case JUGAR_BASICO:
    {
        jugar_basico(currentScreen, archivos, todo, memo);
        break;
    }
    case JUGAR_LETRAS:
    {
        jugar_letras(currentScreen, archivos);
        break;
    }
    case JUGAR_COLORES:
    {
        jugar_colores(currentScreen, archivos);
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