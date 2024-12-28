#pragma once
#include "raylib.h"

class Intro
{
public:
  Intro ();
  void UpdateLogoScreen ();
  void DrawLogoScreen ();
  bool IsIntroFininsh ();

private:
  int framesCounter{};
  bool finishScreen{};
  int logoPositionX{};
  int logoPositionY{};
  int lettersCount{};
  int topSideRecWidth{};
  int leftSideRecHeight{};
  int bottomSideRecWidth{};
  int rightSideRecHeight{};
  int state{};          // Logo animation states
  float alpha{};        // Useful for fading
  float alphaTitle{};   // Useful for fading
  float alphaPowered{}; // Useful for fading
};
