#include "ball.hpp"
#include "intro.hpp"
#include "obstacle.hpp"
#include <algorithm>
#include <raylib.h>
#include <string>

using namespace std;

int
main ()
{
  // === INICIALIZACE ===

  // --- Raylib Flags ---

  SetConfigFlags (FLAG_VSYNC_HINT);
  SetConfigFlags (FLAG_MSAA_4X_HINT);
  SetConfigFlags (FLAG_WINDOW_HIGHDPI);

  // --- Konstanty & Proměnné ---

  constexpr bool DEBUG = false; // DEBUG mód
  bool playIntro = !DEBUG;      // Přehraj raylib intro
  bool playMusic = !DEBUG;      // Hraj muzikuc na pozadí
  bool IsStatsVisible = DEBUG;  // Zobrazit statistiky pohybu
  int windowRatio = 1;          // koeficient velikosti okna
  if (DEBUG)
    windowRatio = 2;             // V případě debugu poloviční okno
  InitWindow (800, 600, "Init"); // otevřené okno pro získání info o rozlišení
  const int CURRENT_MONITOR = GetCurrentMonitor (); // index aktuální obrazovky
  const int SCREEN_WIDTH
      = GetMonitorWidth (CURRENT_MONITOR) / windowRatio; // šířka obrazovky
  const int SCREEN_HEIGHT
      = GetMonitorHeight (CURRENT_MONITOR) / windowRatio; // výška obrazovky
  int fps = GetMonitorRefreshRate (CURRENT_MONITOR);      // Refresh rate FPS
  CloseWindow ();                                         // Zavření init okna
  const float GROUND_LEVEL = SCREEN_HEIGHT;               // hladina země
  constexpr float GRAVITY = 9.8f * 150;               // gravitační konstanta
  const string GAME_NAME = "Mikesh's Geometry Trash"; // název hry
  const char *WINDOW_TITLE = GAME_NAME.c_str ();      // nadpis okna
  const Color COL_BACK = { 205, 245, 245, 255 };      // hlavní barva pozadí
  int playerX = SCREEN_WIDTH / 2;            // horizontální pozice hráče
  int playerY = SCREEN_HEIGHT / 2;           // vertikální pozice hráče
  float playerRadius = 30.0f;                // rádius míče
  float jumpVel = -600.0f;                   // síla skoku
  float bounceFactor = 0.3f;                 // faktor odrazu míče
  float squashFactor = 0.5f;                 // faktor sploštění míče
  float moveSpeed = 800.0f;                  // rychlost pohybu
  float acceleration = 1500.0f;              // faktor akcelerace pohybu x
  float deceleration = 1500.0f;              // faktor decelerace pohybu x
  int gamepad = 0;                           // index gamepad
  float leftStickX = 0.0f;                   // síla pohybu levé páčky v X
  float leftStickY = 0.0f;                   // síla pohybu levé páčky v Y
  constexpr float leftStickDeadzoneX = 0.1f; // mrtvá zóna levé páčky v X
  constexpr float leftStickDeadzoneY = 0.1f; // mrtvá zóna levé páčky v Y
  float horizontalInput = 0.0f; // vstup pro horizontální pohyb hráče
  bool isCollision = false;     // je kolize hráče s překážkou
  int lifes = 5;                // počet životů
  bool inTheHitBox = false;     // je v hitboxu

  // --- Příprava před spuštěním ---

  // Hlavní okno
  InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  if (!DEBUG)
    ToggleFullscreen ();
  SetTargetFPS (fps);
  HideCursor ();
  SetExitKey (KEY_F10);

  // Inicializace hudby na pozadí
  InitAudioDevice ();
  Music music = LoadMusicStream ("audio/music.ogg");
  SetMusicVolume (music, 1.0f);
  if (playMusic)
    PlayMusicStream (music);

  // Hlavní postava hráče
  Ball player (DEBUG, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, playerRadius,
               GRAVITY, jumpVel, moveSpeed, acceleration, deceleration,
               bounceFactor, squashFactor);

  // Překážka
  Obstacle prekazka (
      DEBUG,
      (Vector2){ (float)SCREEN_WIDTH / 2.0f - 15.0f, (float)SCREEN_HEIGHT },
      (Vector2){ (float)SCREEN_WIDTH / 2.0f + 15.0f, (float)SCREEN_HEIGHT },
      (Vector2){ (float)(SCREEN_WIDTH / 2.0f), (float)SCREEN_HEIGHT - 60.0f },
      DARKGRAY);

  // Kamera
  Camera2D camera = { 0 };
  camera.target = player.GetCurrentPosition ();
  camera.offset
      = (Vector2){ (float)SCREEN_WIDTH / 2.0f, (float)SCREEN_HEIGHT / 2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  // === INTRO ===

  Intro intro (WINDOW_TITLE); // Raylib Intro

  if (playIntro)
    {
      SetTargetFPS (60); // Nastavení FPS na 60, aby byla synchronizovaná hudba

      while (WindowShouldClose () == false && playIntro)
        {
          if (playMusic)
            {
              UpdateMusicStream (music);
            }

          if (!intro.IsIntroFininsh ())
            {
              BeginDrawing ();
              ClearBackground (RAYWHITE);
              intro.UpdateLogoScreen ();
              intro.DrawLogoScreen ();
              EndDrawing ();
            }
          else // Konec intra
            {
              SetTargetFPS (fps); // nastavení FPS zpět
              break;
            }
        }
    }

  // === HLAVNÍ SMYČKA ===

  while (WindowShouldClose () == false)
    {
      // --- Hudba ---

      if (playMusic)
        {
          UpdateMusicStream (music);
        }

      // --- 1. Zpracování událostí ---

      if (IsGamepadAvailable (gamepad)) // načtení hodnot z gamepadu
        {
          // získat pohyby levé páčky
          leftStickX = GetGamepadAxisMovement (gamepad, GAMEPAD_AXIS_LEFT_X);
          leftStickY = GetGamepadAxisMovement (gamepad, GAMEPAD_AXIS_LEFT_Y);

          // výpočet deadzone
          if (leftStickX > -leftStickDeadzoneX
              and leftStickX < leftStickDeadzoneX)
            leftStickX = 0.0f;
          if (leftStickY > -leftStickDeadzoneY
              and leftStickY < leftStickDeadzoneY)
            leftStickY = 0.0f;
        }

      // Výskok
      if (IsKeyDown (KEY_SPACE)
          or IsGamepadButtonDown (gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        {
          player.Jump (GROUND_LEVEL);
        }

      // Pohyb hráče v X
      horizontalInput = 0.0f; // vynulování vstupu pohybu hráče v X

      if (leftStickX != 0) // pohyb gamepadem po ose X
        {
          horizontalInput = leftStickX;
        }

      if (IsKeyDown (KEY_A)) // Pohyb doleva
        {
          horizontalInput = -1.0f;
        }

      if (IsKeyDown (KEY_D)) // Pohyb doprava
        {
          horizontalInput = 1.0f;
        }

      // Funkční klávesy
      if (IsKeyPressed (KEY_F5)) // Přepnout Fullscreen
        {
          ToggleFullscreen ();
        }

      if (IsKeyPressed (KEY_F8)) // Zobrazit statistiky
        {
          IsStatsVisible = !IsStatsVisible;
        }

      if (IsKeyPressed (KEY_F7)) // Reset kamera zoom
        {
          camera.zoom = 1.0f;
          camera.rotation = 0.0f;
        }

      // Camera zoom
      camera.zoom = clamp (camera.zoom + (float)GetMouseWheelMove () * 0.05f,
                           0.1f, 3.0f);

      // --- 2. Aktualizace stavu ---

      float deltaTime = GetFrameTime ();
      player.Update (horizontalInput, deltaTime, GROUND_LEVEL);

      // kontrola kolize
      isCollision = CheckCollisionCircleRec (
          player.GetCurrentPosition (), playerRadius, prekazka.GetHitbox ());
      if (isCollision and !inTheHitBox)
        {
          lifes -= 1;
          inTheHitBox = true;
        }
      else if (!isCollision)
        {
          inTheHitBox = false;
        }
      // Camera target follows player
      camera.target = (Vector2){ player.GetCurrentPosition () };

      // --- 3. Vykreslení ---

      BeginDrawing ();            // Start drawing
      ClearBackground (COL_BACK); // Vykreslení pozadí
      BeginMode2D (camera);       // Kamera
      prekazka.Draw ();           // Vykreslí překážku
      player.Draw (deltaTime);    // Vykreslí hráče

      EndMode2D (); // Konec kamery

      if (IsStatsVisible)
        {
          DrawFPS (SCREEN_WIDTH - 90, 5); // Vypíše aktuální FPS

          // Vypsání statistik
          DrawText (TextFormat ("Target FPS: %i", fps), 10, 10, 10, GRAY);
          DrawText (TextFormat ("Position X: %.2f px",
                                player.GetCurrentPosition ().x),
                    10, 20, 10, GRAY);
          DrawText (TextFormat ("Position Y: %.2f px",
                                player.GetCurrentPosition ().y),
                    10, 30, 10, GRAY);
          DrawText (TextFormat ("Speed X: %.2f px/s",
                                player.GetCurrentSpeed (deltaTime).x),
                    10, 40, 10, GRAY);
          DrawText (TextFormat ("Speed Y: %.2f px/s",
                                player.GetCurrentSpeed (deltaTime).y),
                    10, 50, 10, GRAY);
          DrawText (TextFormat ("GP%d: %s", gamepad, GetGamepadName (gamepad)),
                    10, 60, 10, GRAY);
          DrawText (
              TextFormat ("Left Axis X: %f, Y: %f", leftStickX, leftStickY),
              10, 70, 10, GRAY);
          DrawText (TextFormat ("Lifes: %i", lifes), 10, 80, 10, GRAY);
        }

      EndDrawing (); // Konec drawing
    }

  // === UKONČOVACÍ PROCEDŮRA ===

  UnloadMusicStream (music);
  CloseAudioDevice ();
  ShowCursor ();
  CloseWindow ();
}