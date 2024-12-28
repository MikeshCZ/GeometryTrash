#include "Ball.hpp"
#include <string>

Ball::Ball (float x, float y, float radius, float gravity, float jumpForce,
            float moveSpeed, float bounceFactor, float squashFactor)
    : position ({ x, y }), radius (radius), speedY (0), speedX (0),
      gravity (gravity), jumpForce (jumpForce), moveSpeed (moveSpeed),
      bounceFactor (bounceFactor), squashFactor (squashFactor),
      IsHitGround (false)
{
}

void
Ball::Update (float deltaTime, float groundLevel)
{
  speedY += gravity * deltaTime;    // Aplikace gravitace
  position.y += speedY * deltaTime; // Aktualizace vertikální pozice

  // Kolize se zemí
  if (position.y >= groundLevel - radius)
    {
      position.y = groundLevel - radius;
      speedY = -speedY * bounceFactor; // Odraz
      IsHitGround = true;
      if (!IsBounceSoundPlayed)
        {
          PlaySound (bounceSound);
          IsBounceSoundPlayed = true;
        }
    }
  else
    {
      IsHitGround = false;
      IsBounceSoundPlayed = false;
    }

  position.x += speedX * deltaTime; // Aktualizace horizontální pozice
}

void
Ball::Draw () const
{
  float currentSquash = 1.0f;

  if (IsHitGround and speedY < 0)
    {
      currentSquash = squashFactor;
    }

  DrawEllipse (position.x, position.y, radius, radius * currentSquash, MAROON);
  printf ("SpeedX %d, SpeedY %d\n", speedX, speedY);
}

void
Ball::Jump (float groundLevel)
{
  if (IsHitGround)
    {
      speedY = jumpForce;
    }
}

void
Ball::Move (float direction, float deltaTime)
{
  speedX = direction * moveSpeed; // Rychlost pohybu vlevo/vpravo
}