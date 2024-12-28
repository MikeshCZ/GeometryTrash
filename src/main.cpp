#include "Ball.hpp"
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

  const float GROUND_LEVEL = WINDOW_HEIGHT;
  const float GRAVITY = 9.8f * 100; // gravitační konstanta * 100
  const string GAME_NAME = "Mikesh's Geometry Trash"; // Název hry
  const char *windowTitle = GAME_NAME.c_str ();       // Nadpis okna

  // --- Set variables ---

  int fps = 60;                    // Target FPS of the app
  bool playIntro = false;          // Play raylib intro
  bool playMusic = false;          // Play background music
  int playerX = WINDOW_WIDTH / 2;  // horizontální pozice hráče
  int playerY = WINDOW_HEIGHT / 2; // vertikální pozice hráče
  float playerRadius = 20.0f;      // rádius míče
  float jumpVel = -1000.0f;        // síla skoku
  float bounceFactor = 0.5f;       // faktor odrazu míče
  float squashFactor = 0.9f;       // faktor sploštění míče
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

  // Hlavní postava hráče
  Ball player (WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, playerRadius,
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
      if (IsKeyDown (KEY_LEFT))
        horizontalInput = -1.0f; // Pohyb doleva
      if (IsKeyDown (KEY_RIGHT))
        horizontalInput = 1.0f; // Pohyb doprava

      // --- 2. Updating state ---

      float deltaTime = GetFrameTime ();
      player.Move (horizontalInput, deltaTime);
      player.Update (deltaTime, GROUND_LEVEL);

      // --- 3. Drawing ---

      BeginDrawing ();
      ClearBackground (BLACK);        // Clear the background
      DrawFPS (WINDOW_WIDTH - 90, 5); // Write FPS to right corner
      player.Draw ();                 // Draw player
      EndDrawing ();
    }

  // === CLOSING ROUTINE ===

  UnloadMusicStream (music);
  CloseAudioDevice ();
  CloseWindow ();
}