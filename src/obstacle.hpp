#pragma once
#include "raylib.h"

class Obstacle
{
public:
  Obstacle (bool DEBUG, Vector2 position, Color color);
  ~Obstacle ();
  void Update ();
  void Draw ();
  Rectangle GetHitbox ();

private:
  bool DEBUG;
  Vector2 position;
  Color color;
  Color hitboxColor;
  Rectangle hitbox;
};