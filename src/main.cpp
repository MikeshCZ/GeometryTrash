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

  // --- Flags

  SetConfigFlags (FLAG_FULLSCREEN_MODE);

  // --- Constants ---

  // Musi byt inicializované okno pro zjištění rozlišení monitoru
  InitWindow (800, 600, "");
  const int CURRENT_MONITOR = GetCurrentMonitor ();
  const int WINDOW_WIDTH = GetMonitorWidth (CURRENT_MONITOR);
  const int WINDOW_HEIGHT = GetMonitorHeight (CURRENT_MONITOR);
  CloseWindow ();

  // --- Set variables ---

  int fps = 60;           // Target FPS of the app
  bool playIntro = false; // Play raylib intro
  bool playMusic = false; // Play background music
  int playerX = WINDOW_WIDTH / 2;
  int playerY = WINDOW_HEIGHT / 2;
  int playerRadius = 40;

  // Nadpis okna
  const string GAME_NAME = "Mikesh's Geometry Trash";
  const char *windowTitle = GAME_NAME.c_str ();

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

  Intro intro{}; // Raylib Intro screen

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

  while (WindowShouldClose () == false)
    {
      if (playMusic)
        {
          UpdateMusicStream (music);
        }

      // --- 1. Event handling ---

      // --- 2. Updating state ---
      
      // --- 3. Drawing ---

      // Drawing the window
      BeginDrawing ();
      ClearBackground (BLACK);        // Clear the background
      DrawFPS (WINDOW_WIDTH - 90, 5); // Write FPS to right corner
      DrawCircle (playerX, playerY, playerRadius, RED);

      EndDrawing ();
    }
  UnloadMusicStream (music);
  CloseAudioDevice ();
  CloseWindow ();
}