#include "obstacle.hpp"

Obstacle::Obstacle (bool debug, Vector2 v1, Vector2 v2, Vector2 v3,
                    Color color)
    : DEBUG (debug), v1 (v1), v2 (v2), v3 (v3), color (color)
{
  hitbox = Rectangle{ v3.x - 4, v3.y + 16, 8, v1.y - v3.y - 16 };
  
  if (DEBUG)
    {
      hitboxColor = { 255, 0, 0, 255 };
    }
  else
    {
      hitboxColor = { 255, 0, 0, 0 };
    }
}

Obstacle::~Obstacle () {}

void
Obstacle::Update ()
{
}

void
Obstacle::Draw ()
{
  DrawTriangle (v1, v2, v3, color);
  DrawRectangleLinesEx (hitbox, 3, hitboxColor);
}

Rectangle
Obstacle::GetHitbox ()
{
  return hitbox;
}
