#include "obstacle.hpp"

Obstacle::Obstacle (bool debug, Vector2 position, bool isSmall, Color color)
    : DEBUG (debug), position (position), isSmall (isSmall), color (color)
{
  if (isSmall)
    {
      obstackleHeight = 20.0f;
    }
  else
    {
      obstackleHeight = 40.0f;
    }

  hitbox = Rectangle{ position.x + 8, position.y - obstackleHeight, 4,
                      obstackleHeight };

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
                { position.x + 10, position.y - obstackleHeight }, color);
  DrawRectangleLinesEx (hitbox, 1, hitboxColor);
}

Rectangle
Obstacle::GetHitbox ()
{
  return hitbox;
}
