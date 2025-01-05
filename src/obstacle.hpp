#pragma once
#include "raylib.h"

class Obstacle
{
public:
  Obstacle (bool DEBUG, Vector2 position, bool isSmall, Color color);
  ~Obstacle ();
  void Update ();
  void Draw ();
  Rectangle GetHitbox ();

private:
  bool DEBUG;
  Vector2 position;
  bool isSmall;
  Color color;
  Color hitboxColor;
  Rectangle hitbox;
  float obstackleHeight;
};