#include "obstacle.hpp"

Obstacle::Obstacle (bool debug, Vector2 position, Color color)
    : DEBUG (debug), position (position), color (color)
{
  hitbox = Rectangle{ position.x + 8, position.y - 35, 4, 35 };

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
  DrawTriangle ({ position.x + 0, position.y + 0 },
                { position.x + 20, position.y + 0 },
                { position.x + 10, position.y - 40 }, color);
  DrawRectangleLinesEx (hitbox, 1, hitboxColor);
}

Rectangle
Obstacle::GetHitbox ()
{
  return hitbox;
}
