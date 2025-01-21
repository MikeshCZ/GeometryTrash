#include "grid.hpp"
#include "intro.hpp"
#include "obstacle.hpp"
#include "player.hpp"
#include <algorithm>
#include <raylib.h>
#include <string>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

using namespace std;

int
main ()
{
  // === INICIALIZACE ===

  // --- Raylib Flags ---

  SetConfigFlags (FLAG_VSYNC_HINT);
  SetConfigFlags (FLAG_MSAA_4X_HINT);
  SetConfigFlags (FLAG_FULLSCREEN_MODE);
  SetConfigFlags (FLAG_WINDOW_HIGHDPI);

  // --- Konstanty & Proměnné ---
  const string GAME_NAME = "Mikesh's Geometry Trash"; // název hry
  InitWindow (0, 0, GAME_NAME.c_str ());              // inicializace okna
  constexpr bool DEBUG = true;                        // DEBUG mód
  bool playIntro = false;                             // Přehraj raylib intro
  bool playMusic = false;                             // Hraj muzikuc na pozadí
  bool IsStatsVisible = DEBUG; // Zobrazit statistiky pohybu
  const int CURRENT_MONITOR = GetCurrentMonitor (); // index aktuální obrazovky
  const int SCREEN_WIDTH = GetMonitorWidth (CURRENT_MONITOR);   // šířka obr.
  const int SCREEN_HEIGHT = GetMonitorHeight (CURRENT_MONITOR); // výška obr.
  int fps = GetMonitorRefreshRate (CURRENT_MONITOR); // Refresh rate FPS
  const float GROUND_LEVEL = SCREEN_HEIGHT;          // hladina země
  constexpr float GRAVITY = 9.8f * 150;              // gravitační konstanta
  const Color COL_BACK = { 205, 245, 245, 255 };     // hlavní barva pozadí
  int playerX = SCREEN_WIDTH / 2;            // horizontální pozice hráče
  int playerY = SCREEN_HEIGHT / 2;           // vertikální pozice hráče
  int gamepad = 0;                           // index gamepad
  float leftStickX = 0.0f;                   // síla pohybu levé páčky v X
  float leftStickY = 0.0f;                   // síla pohybu levé páčky v Y
  constexpr float leftStickDeadzoneX = 0.1f; // mrtvá zóna levé páčky v X
  constexpr float leftStickDeadzoneY = 0.1f; // mrtvá zóna levé páčky v Y
  float horizontalInput = 0.0f; // vstup pro horizontální pohyb hráče
  vector<Obstacle> obstacles;   // vektor překážek
  float deltaTime;              // čas mezi jdnotlivými snímky
  int lives = 5;                // počet životů

  // --- Příprava před spuštěním ---

  // Hlavní okno
  if (!IsWindowFullscreen ())
    ToggleFullscreen ();
  SetTargetFPS (fps);
  HideCursor ();
  SetExitKey (KEY_F10);

  // bílá obrazovka
  BeginDrawing ();
  ClearBackground (RAYWHITE);
  EndDrawing ();

  // Inicializace hudby na pozadí
  InitAudioDevice ();
  Music music = LoadMusicStream ("assets/music.ogg");
  SetMusicVolume (music, 1.0f);
  if (playMusic)
    PlayMusicStream (music);

  // === INTRO ===

  if (playIntro)
    {
      Intro intro (GAME_NAME.c_str ()); // Raylib Intro
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

  // Hlavní postava hráče
  Player *player
      = new Player (DEBUG, 60, SCREEN_HEIGHT / 2.0f, GRAVITY, lives,SCREEN_WIDTH, SCREEN_HEIGHT);

  // Překážky
  obstacles.emplace_back (DEBUG,
                          (Vector2){
                              800.0f,
                              (float)SCREEN_HEIGHT,
                          },
                          true, DARKGRAY);
  obstacles.emplace_back (DEBUG,
                          (Vector2){
                              820.0f,
                              (float)SCREEN_HEIGHT,
                          },
                          true, DARKGRAY);
  obstacles.emplace_back (DEBUG,
                          (Vector2){
                              840.0f,
                              (float)SCREEN_HEIGHT,
                          },
                          false, DARKGRAY);
  obstacles.emplace_back (DEBUG,
                          (Vector2){
                              860.0f,
                              (float)SCREEN_HEIGHT,
                          },
                          false, DARKGRAY);
  obstacles.emplace_back (DEBUG,
                          (Vector2){
                              880.0f,
                              (float)SCREEN_HEIGHT,
                          },
                          false, DARKGRAY);
  obstacles.emplace_back (DEBUG,
                          (Vector2){
                              900.0f,
                              (float)SCREEN_HEIGHT,
                          },
                          true, DARKGRAY);

  // Grid
  Grid grid (SCREEN_WIDTH, SCREEN_HEIGHT, 20, COL_BACK);

  // Kamera
  Camera2D camera = { 0 };
  camera.target = player->GetCurrentPosition ();
  camera.offset
      = (Vector2){ (float)SCREEN_WIDTH / 2.0f, (float)SCREEN_HEIGHT / 2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

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
          player->Jump (GROUND_LEVEL);
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

      // --- 2. Aktualizace stavu ---

      deltaTime = GetFrameTime ();
      player->Update (horizontalInput, deltaTime, GROUND_LEVEL);

      // Camera zoom
      float targetZoom = std::clamp (
          3.0f - std::abs (player->GetCurrentSpeed (deltaTime).z) / 300.0f,
          0.5f, 2.0f);
      float smoothFactor = 0.01f; // Míra hladkosti
      camera.zoom += (targetZoom - camera.zoom) * smoothFactor;

      // camera target follows player
      camera.target = (Vector2){ player->GetCurrentPosition () };

      // Kontrola kolize s každou překážkou
      for (auto &obstacle : obstacles)
        {
          player->CheckCollision (obstacle.GetHitbox ());
        }

      if (player->GetDoRestart ())
        {
          lives--;
          delete player;
          player
              = new Player (DEBUG, 60, SCREEN_HEIGHT / 2.0f, GRAVITY, lives,SCREEN_WIDTH, SCREEN_HEIGHT);
        }

      // --- 3. Vykreslení ---

      BeginDrawing ();        // start drawing
      ClearBackground (GRAY); // vykreslení pozadí
      BeginMode2D (camera);   // kamera
      grid.Draw ();           // show Grid

      // Vykreslení překážek
      for (auto &obstacle : obstacles)
        {
          obstacle.Draw ();
        }

      player->Draw (deltaTime); // vykreslí hráče
      EndMode2D ();             // Konec kamery

      if (IsStatsVisible)
        {
          DrawFPS (SCREEN_WIDTH - 90, 5); // Vypíše aktuální FPS

          // Vypsání statistik
          int statsSize = 20;
          int statsPosition = statsSize;
          DrawText (TextFormat ("Lifes: %i", player->GetLives ()), statsSize,
                    statsPosition = +statsSize, statsSize, GREEN);
          DrawText (TextFormat ("Resolution: %i x %i, FPS: %i", SCREEN_WIDTH,
                                SCREEN_HEIGHT, fps),
                    statsSize, statsPosition += statsSize, statsSize, GREEN);
          DrawText (TextFormat ("Position X: %.0f px, Y: %.0f px",
                                player->GetCurrentPosition ().x,
                                player->GetCurrentPosition ().y),
                    statsSize, statsPosition += statsSize, statsSize, GREEN);
          DrawText (TextFormat ("Speed X: %4.0f px/s, Y: %4.0f px/s, V: %4.0f",
                                player->GetCurrentSpeed (deltaTime).x,
                                player->GetCurrentSpeed (deltaTime).y,
                                player->GetCurrentSpeed (deltaTime).z),
                    statsSize, statsPosition += statsSize, statsSize, GREEN);
          DrawText (TextFormat ("GP%d: %s | LAxis X: %1.3f, Y: %1.3f", gamepad,
                                GetGamepadName (gamepad), leftStickX,
                                leftStickY),
                    statsSize, statsPosition += statsSize, statsSize, GREEN);
        }

      EndDrawing (); // Konec drawing
    }

  // === UKONČOVACÍ PROCEDŮRA ===

  UnloadMusicStream (music);
  CloseAudioDevice ();
  ShowCursor ();
  CloseWindow ();
  delete player;
}