#include "ball.hpp"
#include "intro.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <string>

using namespace std;

int
main ()
{
  // === INICIALIZATION ===

  SetConfigFlags (FLAG_VSYNC_HINT);
  SetConfigFlags (FLAG_MSAA_4X_HINT);
  SetConfigFlags (FLAG_WINDOW_HIGHDPI);
  SetConfigFlags (FLAG_WINDOW_TRANSPARENT);

  // --- Constants & Variables ---
  const bool DEBUG = false;    // DEBUG toggle
  bool playIntro = !DEBUG;     // Play raylib intro
  bool playMusic = !DEBUG;     // Play background music
  bool IsStatsVisible = DEBUG; // Zobrazit statistiky pohybu

  // Musi byt inicializované okno pro zjištění rozlišení monitoru
  int windowRatio = 1;
  if (DEBUG) // V případě debugu poloviční okno
    windowRatio = 2;
  InitWindow (800, 600, "Init");
  const int CURRENT_MONITOR = GetCurrentMonitor ();
  const int WINDOW_WIDTH = GetMonitorWidth (CURRENT_MONITOR) / windowRatio;
  const int WINDOW_HEIGHT = GetMonitorHeight (CURRENT_MONITOR) / windowRatio;
  int fps = GetMonitorRefreshRate (CURRENT_MONITOR);  // Target FPS
  CloseWindow ();

  const float GROUND_LEVEL = WINDOW_HEIGHT;           // hladina země
  const float GRAVITY = 9.8f * 150;                   // gravitační konstanta
  const string GAME_NAME = "Mikesh's Geometry Trash"; // Název hry
  const char *WINDOW_TITLE = GAME_NAME.c_str ();      // Nadpis okna
  const Color COL_BACK = { 245, 245, 245, 255 };      // barva pozadí
  int playerX = WINDOW_WIDTH / 2;  // horizontální pozice hráče
  int playerY = WINDOW_HEIGHT / 2; // vertikální pozice hráče
  float playerRadius = 40.0f;      // rádius míče
  float jumpVel = -800.0f;         // síla skoku
  float bounceFactor = 0.3f;       // faktor odrazu míče
  float squashFactor = 0.5f;       // faktor sploštění míče
  float moveSpeed = 800.0f;        // rychlost pohybu
  float acceleration = 1500.0f;    // decelerace pohybu x
  float deceleration = 1500.0f;    // decelerace pohybu x

  // Gamepad
  int gamepad = 0;
  float leftStickX = 0.0f;
  float leftStickY = 0.0f;
  const float leftStickDeadzoneX = 0.1f;
  const float leftStickDeadzoneY = 0.1f;

  // --- Init Main window ---

  InitWindow (WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
  if (!DEBUG)
    ToggleFullscreen ();
  SetTargetFPS (fps);
  HideCursor ();
  SetExitKey (KEY_F10);

  // Inicializace hudby na pozadí
  InitAudioDevice ();
  Music music = LoadMusicStream ("audio/music.ogg");
  SetMusicVolume (music, 1.0f);
  PlayMusicStream (music);

  // === INTRO ===

  Intro intro (WINDOW_TITLE); // Raylib Intro screen

  if (playIntro)
    {
      SetTargetFPS (60); // Set FPS to 60 to have same lenght of the video

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
          else // Intro is finish
            {
              SetTargetFPS (fps); // set back the FPS to config value
              break;
            }
        }
    }

  // === MAIN LOOP ===

  // Hlavní postava hráče
  Ball player (DEBUG, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, playerRadius,
               GRAVITY, jumpVel, moveSpeed, acceleration, deceleration,
               bounceFactor, squashFactor);

  while (WindowShouldClose () == false)
    {
      // --- Music ---

      if (playMusic)
        {
          UpdateMusicStream (music);
        }

      // --- 1. Event handling ---
      if (IsGamepadAvailable (gamepad))
        {
          // get axis movements
          leftStickX = GetGamepadAxisMovement (gamepad, GAMEPAD_AXIS_LEFT_X);
          leftStickY = GetGamepadAxisMovement (gamepad, GAMEPAD_AXIS_LEFT_Y);

          // calculate deadzone
          if (leftStickX > -leftStickDeadzoneX
              and leftStickX < leftStickDeadzoneX)
            leftStickX = 0.0f;
          if (leftStickY > -leftStickDeadzoneY
              and leftStickY < leftStickDeadzoneY)
            leftStickY = 0.0f;
        }

      float horizontalInput = 0.0f;

      if (IsKeyDown (KEY_SPACE)
          or IsGamepadButtonDown (gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        { // Výskok
          player.Jump (GROUND_LEVEL);
        }

      if (leftStickX != 0)
        { // pohyb gamepadem po ose X
          horizontalInput = leftStickX;
        }

      if (IsKeyDown (KEY_A))
        { // Pohyb doleva
          horizontalInput = -1.0f;
        }

      if (IsKeyDown (KEY_D))
        { // Pohyb doprava
          horizontalInput = 1.0f;
        }

      if (IsKeyDown (KEY_LEFT_ALT) and IsKeyDown (KEY_ENTER))
        { // Toggle Fullscreen
          ToggleFullscreen ();
        }

      if (IsKeyPressed (KEY_F8))
        { // Zobrazit statistiky
          IsStatsVisible = !IsStatsVisible;
        }

      // --- 2. Updating state ---

      float deltaTime = GetFrameTime ();
      player.Update (horizontalInput, deltaTime, GROUND_LEVEL);

      // --- 3. Drawing ---

      BeginDrawing ();
      ClearBackground (COL_BACK);     // Clear the background
      DrawFPS (WINDOW_WIDTH - 90, 5); // Write FPS to right corner
      player.Draw (deltaTime);        // Draw player
      if (IsStatsVisible)
        {
          // Zobrazení pozice

          DrawText (TextFormat ("Target FPS: %i", fps), 10, 10, 10, GRAY);
          DrawText (TextFormat ("Position X: %.2f px",
                                player.GetCurrentPosition ().x),
                    10, 20, 10, GRAY);
          DrawText (TextFormat ("Position Y: %.2f px",
                                player.GetCurrentPosition ().y),
                    10, 30, 10, GRAY);
          // Zobrazení rychlosti
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
        }
      EndDrawing ();
    }

  // === CLOSING ROUTINE ===

  UnloadMusicStream (music);
  CloseAudioDevice ();
  ShowCursor ();
  CloseWindow ();
}