#include "ball.hpp"

Ball::Ball (bool debug, float x, float y, float radius, float gravity,
            float jumpForce, float moveSpeed, float bounceFactor,
            float squashFactor)
    : DEBUG (debug), position ({ x, y }), radius (radius), speedY (0),
      speedX (0), gravity (gravity), jumpForce (jumpForce),
      moveSpeed (moveSpeed), bounceFactor (bounceFactor),
      squashFactor (squashFactor), IsOnTheGround (false)
{
}

void
Ball::Update (float deltaTime, float groundLevel)
{
  previousPosition = position;      // Uložení předchozí pozice
  speedY += gravity * deltaTime;    // Aplikace gravitace
  position.y += speedY * deltaTime; // Aktualizace vertikální pozice

  // Kolize se zemí
  if (position.y >= groundLevel - radius)
    {
      // Odraz
      position.y = groundLevel - radius;
      speedY = -speedY * bounceFactor;
      IsOnTheGround = true;
      if (!IsHitSoundPlayed)
        {
          PlaySound (bounceSound);
          IsHitSoundPlayed = true;
        }
    }
  else
    {
      IsOnTheGround = false;
      IsHitSoundPlayed = false;
    }

  position.x += speedX * deltaTime; // Aktualizace horizontální pozice
}

void
Ball::Draw (float deltaTime) const
{
  float currentSquash = 1.0f;

  if (GetCurrentSpeed (deltaTime).y > 0 and IsOnTheGround)
    {
      currentSquash = squashFactor;
    }

  DrawEllipse (position.x, position.y, radius, radius * currentSquash, MAROON);

  if (DEBUG)
    printf ("PosX: %f, PosY %f, SpeedX %f, SpeedY %f,\n", position.x,
            position.y, speedX, speedY);
}

void
Ball::Jump (float groundLevel)
{
  if (IsOnTheGround)
    {
      speedY = jumpForce;
    }
}

void
Ball::MoveX (float direction, float deltaTime)
{
  speedX = direction * moveSpeed; // Rychlost pohybu vlevo/vpravo
}

Vector2
Ball::GetCurrentSpeed (float deltaTime) const
{
  float x = (position.x - previousPosition.x) / deltaTime;
  float y = (position.y - previousPosition.y) / deltaTime;

  if (DEBUG)
    printf ("Curr. speed X: %f, Y: %f\n", x, y);

  return { x, y };
}