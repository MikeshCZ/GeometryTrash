#include "ball.hpp"

Ball::Ball (bool debug, float x, float y, float gravity)
    : DEBUG (debug), position ({ x, y }), gravity (gravity),
      playerRadius (20.0f), jumpVel (-400.0f), bounceFactor (0.3f),
      squashFactor (0.5f), maxSpeed (800.0f), acceleration (1500.0f),
      deceleration (1500.0f), IsOnTheGround (false), trailDuration (1.5f),
      lives (5), isInHitbox (false), IsHitSoundPlayed (false), speedX (0),
      speedY (0), trail (), isAlive (true), doRestart (false)
{
  bounceSound = LoadSound ("audio/basketball.ogg"); // Načtení zvuku
}

Ball::~Ball ()
{
  UnloadSound (bounceSound); // Uvolnění zvuku
}

void
Ball::Update (float direction, float deltaTime, float groundLevel)
{
  previousPosition = position; // Uložení předchozí pozice
  currentTime = GetTime ();

  if (isAlive)
    {
      // --- Výpočet pohybu v ose Y ---

      speedY += gravity * deltaTime;    // Aplikace gravitace
      position.y += speedY * deltaTime; // Aktualizace vertikální pozice Y

      // Kolize se zemí
      if (position.y + playerRadius >= groundLevel)
        {
          // Odraz
          position.y = groundLevel - playerRadius;
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
          AccelX (direction, deltaTime, maxSpeed);
        }
    }

  // --- Zpracování trailů ---

  trail.push_back (
      { position, currentTime }); // přidání aktuální pozice do trailu

  // Odstranění starých bodů z trailu
  while (!trail.empty ()
         && currentTime - trail.front ().timestamp > trailDuration)
    {
      trail.pop_front ();
    }

  // --- Kontrola životů ---

  if (lives <= 0)
    {

      // Generování fragmentů
      if (fragments.empty () and isAlive == true)
        {

          for (int i = 0; i < 500; ++i)
            {
              float angle = GetRandomValue (0, 360) * DEG2RAD;
              float speed = GetRandomValue (100, 2000);
              fragments.push_back (Fragment (
                  position, { cos (angle) * speed, sin (angle) * speed },
                  static_cast<float> (GetRandomValue (3, 6)), 2.0f));
            }
        }

      isAlive = false;

      // Aktualizace fragmentů
      for (auto &fragment : fragments)
        {
          fragment.velocity.y += gravity * deltaTime;
          fragment.position.x += fragment.velocity.x * deltaTime;
          fragment.position.y += fragment.velocity.y * deltaTime;
          fragment.life -= deltaTime;
        }

      // Odstranění prošlých fragmentů
      fragments.erase (
          std::remove_if (fragments.begin (), fragments.end (),
                          [] (const Fragment &f) { return f.life <= 0; }),
          fragments.end ());

      if (fragments.empty ())
        {
          doRestart = true;
        }
    }
}

void
Ball::Draw (float deltaTime)
{
  float currentSquash = 1.0f;

  if (GetCurrentSpeed (deltaTime).y > 0 and IsOnTheGround)
    {
      currentSquash = squashFactor;
    }
  if (lives > 0)
    {
      DrawTrail ();
      DrawEllipse (position.x, position.y, playerRadius,
                   playerRadius * currentSquash, MAROON);
    }
  else
    {
      DrawFragments ();
    }
}

void
Ball::DrawFragments () const
{
  for (const auto &fragment : fragments)
    {
      float alpha = fragment.life / 2.0f; // Fragment postupně zmizí
      Color color = Fade (RED, alpha);
      DrawCircleV (fragment.position, 3.0f, color);
    }
}

void
Ball::Jump (float groundLevel)
{
  if (IsOnTheGround)
    {
      speedY = jumpVel;
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

float
Ball::GetPlayerRadius () const
{
  return playerRadius;
}

void
Ball::DrawTrail ()
{
  for (size_t i = 1; i < trail.size (); i++)
    {
      float age = currentTime - trail[i - 1].timestamp;
      float alpha = 1.0f - (age / trailDuration);
      Color trailColor = Fade (BEIGE, alpha);
      DrawLineBezier (trail[i - 1].position, trail[i].position, alpha * 10,
                      trailColor);
    }
}

bool
Ball::CheckCollision (const Rectangle &obstacle)
{
  if (!isInHitbox
      and CheckCollisionCircleRec (position, playerRadius, obstacle))
    {
      lives -= 1;
      isInHitbox = true;
      return true;
    }
  else if (isInHitbox
           and CheckCollisionCircleRec (position, playerRadius, obstacle))
    {
      return true;
    }
  else
    {
      isInHitbox = false;
      return false;
    }
}

int
Ball::GetLives ()
{
  return lives;
}

bool
Ball::GetDoRestart ()
{
  return doRestart;
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