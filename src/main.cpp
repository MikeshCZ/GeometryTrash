#include "ball.hpp"
#include "intro.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <string>

// === DEBUG TOGGLE ===

const bool DEBUG = false;

using namespace std;

int
main ()
{
  // === INICIALIZATION ===

  // --- Flags

  if (!DEBUG)
    SetConfigFlags (FLAG_FULLSCREEN_MODE);

  // --- Constants & Variables ---

  // Musi byt inicializované okno pro zjištění rozlišení monitoru
  InitWindow (800, 600, "");
  int windowRatio = 1;
  const int CURRENT_MONITOR = GetCurrentMonitor ();

  if (DEBUG)
    windowRatio = 2;

  const int WINDOW_WIDTH = GetMonitorWidth (CURRENT_MONITOR) / windowRatio;
  const int WINDOW_HEIGHT = GetMonitorHeight (CURRENT_MONITOR) / windowRatio;
  CloseWindow ();

  const float GROUND_LEVEL = WINDOW_HEIGHT;
  const float GRAVITY = 9.8f * 100; // gravitační konstanta * 100
  const string GAME_NAME = "Mikesh's Geometry Trash"; // Název hry
  const char *windowTitle = GAME_NAME.c_str ();       // Nadpis okna
  int fps = 60;                                       // Target FPS of the app
  bool playIntro = !DEBUG;                            // Play raylib intro
  bool playMusic = !DEBUG;                            // Play background music
  int playerX = WINDOW_WIDTH / 2;  // horizontální pozice hráče
  int playerY = WINDOW_HEIGHT / 2; // vertikální pozice hráče
  float playerRadius = 20.0f;      // rádius míče
  float jumpVel = -1000.0f;        // síla skoku
  float bounceFactor = 0.3f;       // faktor odrazu míče
  float squashFactor = 0.5f;       // faktor sploštění míče
  float moveSpeed = 800.0f;        // rychlost pohybu

  // --- Init Main window ---

  InitWindow (WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle);
  SetTargetFPS (fps);
  SetExitKey (KEY_F10);

  // Inicializace hudby na pozadí
  InitAudioDevice ();
  Music music = LoadMusicStream ("audio/music.ogg");
  SetMusicVolume (music, 1.0f);
  PlayMusicStream (music);

  // === INTRO ===

  Intro intro (GAME_NAME); // Raylib Intro screen

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
               GRAVITY, jumpVel, moveSpeed, bounceFactor, squashFactor);

  while (WindowShouldClose () == false)
    {
      // --- Music ---

      if (playMusic)
        {
          UpdateMusicStream (music);
        }

      // --- 1. Event handling ---

      if (IsKeyDown (KEY_SPACE))
        player.Jump (GROUND_LEVEL); // Výskok
      if (IsKeyPressed (KEY_F))
        ToggleFullscreen (); // Toggle Fullscreen

      float horizontalInput = 0.0f;
      if (IsKeyDown (KEY_A))
        horizontalInput = -1.0f; // Pohyb doleva
      if (IsKeyDown (KEY_D))
        horizontalInput = 1.0f; // Pohyb doprava

      // --- 2. Updating state ---

      float deltaTime = GetFrameTime ();
      player.MoveX (horizontalInput, deltaTime);
      player.Update (deltaTime, GROUND_LEVEL);

      // --- 3. Drawing ---

      BeginDrawing ();
      ClearBackground (BLACK);        // Clear the background
      DrawFPS (WINDOW_WIDTH - 90, 5); // Write FPS to right corner
      player.Draw (deltaTime);        // Draw player
                                      // Zobrazení rychlosti
      DrawText (TextFormat ("Speed X: %.2f px/s",
                            player.GetCurrentSpeed (deltaTime).x),
                10, 40, 20, WHITE);
      DrawText (TextFormat ("Speed Y: %.2f px/s",
                            player.GetCurrentSpeed (deltaTime).y),
                10, 70, 20, WHITE);
      EndDrawing ();
    }

  // === CLOSING ROUTINE ===

  UnloadMusicStream (music);
  CloseAudioDevice ();
  CloseWindow ();
}