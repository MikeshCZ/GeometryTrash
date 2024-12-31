#pragma once
#include "raylib.h"

class Obstacle
{
public:
  Obstacle (bool DEBUG, Vector2 v1, Vector2 v2, Vector2 v3, Color color);
  ~Obstacle ();
  void Update ();
  void Draw ();
  Rectangle GetHitbox ();

private:
  bool DEBUG;
  Vector2 v1;
  Vector2 v2;
  Vector2 v3;
  Color color;
  Color hitboxColor;
  Rectangle hitbox;
};