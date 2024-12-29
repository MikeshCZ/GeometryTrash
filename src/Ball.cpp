#include "ball.hpp"

Ball::Ball (bool debug, float x, float y, float radius, float gravity,
            float jumpForce, float moveSpeed, float acceleration,
            float deceleration, float bounceFactor, float squashFactor)
    : DEBUG (debug), position ({ x, y }), radius (radius), speedY (0),
      speedX (0), gravity (gravity), jumpForce (jumpForce),
      moveSpeed (moveSpeed), acceleration (acceleration),
      deceleration (deceleration), bounceFactor (bounceFactor),
      squashFactor (squashFactor), IsOnTheGround (false)
{
}

void
Ball::Update (float direction, float deltaTime, float groundLevel)
{
  previousPosition = position; // Uložení předchozí pozice

  // --- Výpočet pohybu v ose Y ---

  speedY += gravity * deltaTime;    // Aplikace gravitace
  position.y += speedY * deltaTime; // Aktualizace vertikální pozice Y

  // Kolize se zemí
  if (position.y + radius >= groundLevel)
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

  // --- Výpočet pohybu v ose X ---

  position.x += speedX * deltaTime; // Aktualizace horizontální pozice X
  MoveX (direction, deltaTime);
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
  if (direction == 0)
    {
      // Decelerace při absenci směru
      DecelX (deltaTime);
    }
  else
    {
      // Pokud je směr opačný vůči aktuálnímu pohybu
      if ((direction < 0 and GetCurrentSpeed (deltaTime).x > 0)
          or (direction > 0 and GetCurrentSpeed (deltaTime).x < 0))
        {
          // Decelerace na nulu
          DecelX (deltaTime);
        }

      // Akcelerace
      AccelX (direction, deltaTime, moveSpeed);
    }
}

Vector2
Ball::GetCurrentSpeed (float deltaTime) const
{
  float x = (position.x - previousPosition.x) / deltaTime;
  float y = (position.y - previousPosition.y) / deltaTime;

  return { x, y };
}

Vector2
Ball::GetCurrentPosition () const
{
  float x = position.x;
  float y = position.y;

  return { x, y };
}

void
Ball::DecelX (float deltaTime)
{
  if (speedX > 0)
    {
      speedX -= deceleration * deltaTime;
      if (speedX < 0)
        speedX = 0;
    }
  else if (speedX < 0)
    {
      speedX += deceleration * deltaTime;
      if (speedX > 0)
        speedX = 0;
    }
}

void
Ball::AccelX (float direction, float deltaTime, float maxSpeed)
{
  // Přidání akcelerace podle směru
  speedX += direction * acceleration * deltaTime;

  // Omezení rychlosti na maxSpeed
  if (speedX > maxSpeed)
    {
      speedX = maxSpeed;
    }
  else if (speedX < -maxSpeed)
    {
      speedX = -maxSpeed;
    }
}