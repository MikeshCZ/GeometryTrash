#include "player.hpp"

Player::Player (bool debug, float x, float y, float gravity)
    : DEBUG (debug), position ({ x, y }), gravity (gravity),
      playerRadius (20.0f), jumpVel (-400.0f), bounceFactor (0.3f),
      squashFactor (0.5f), maxSpeed (600.0f), acceleration (1500.0f),
      deceleration (1500.0f), IsOnTheGround (false), trailDuration (1.5f),
      lives (1), isInHitbox (false), IsHitSoundPlayed (false), speedX (0),
      speedY (0), trail (), isAlive (true), doRestart (false)
{
  // Načtení zvuku
  bounceSound = LoadSound ("assets/basketball.ogg");
  startSound = LoadSound ("assets/start.ogg");
  killSound = LoadSound ("assets/kill.ogg");

  PlaySound (startSound);
}

Player::~Player ()
{
  // Uvolnění zvuku
  UnloadSound (bounceSound);
  UnloadSound (startSound);
  UnloadSound (killSound);
}

void
Player::Update (float direction, float deltaTime, float groundLevel)
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

      // Kolize s levým a pravým okrajem obrazovky
      if (position.x - playerRadius <= 0) // Levý okraj
        {
          position.x = playerRadius; // Uprav pozici na okraj

          if (speedY != 0) // Pokud je hráč ve výskoku
            {
              speedX = -speedX
                       - jumpVel
                             * bounceFactor; // Obrácení horizontální rychlosti
              if (isInJump)
                speedY += jumpVel; // Přidání rychlosti výskoku
            }
          else
            {
              speedX = -speedX
                       - jumpVel * bounceFactor; // Klasický horizontální odraz
            }

          PlaySound (bounceSound);
        }
      else if (position.x + playerRadius >= GetScreenWidth ()) // Pravý okraj
        {
          position.x
              = GetScreenWidth () - playerRadius; // Uprav pozici na okraj

          if (speedY != 0) // Pokud je hráč ve výskoku
            {
              speedX = -speedX
                       + jumpVel
                             * bounceFactor; // Obrácení horizontální rychlosti
              if (isInJump)
                speedY += jumpVel; // Přidání rychlosti výskoku
            }
          else
            {
              speedX = -speedX
                       + jumpVel * bounceFactor; // Klasický horizontální odraz
            }

          PlaySound (bounceSound);
        }

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
          if (IsOnTheGround)
            {
              AccelX (direction, deltaTime, maxSpeed);
            }
          else
            {
              if ((speedX > maxSpeed / 2) or (speedX < -maxSpeed / 2))
                {
                  DecelX (deltaTime);
                }
              else
                {
                  AccelX (direction, deltaTime, maxSpeed / 2);
                }
            }
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

  // --- Kontrola životů a výbuch ---

  if (lives <= 0)
    {
      if (!IsKillSoundPlayed)
        {
          PlaySound (killSound);
          IsKillSoundPlayed = true;
        }

      // Generování fragmentů
      if (fragments.empty () and isAlive == true)
        {

          for (int i = 0; i < 500; ++i)
            {
              float angle = GetRandomValue (0, 360) * DEG2RAD;
              float speed = GetRandomValue (10, 1000);
              fragments.push_back (Fragment (
                  position, { cos (angle) * speed, sin (angle) * speed },
                  static_cast<float> (GetRandomValue (100, 500) / 100.f),
                  static_cast<float> (GetRandomValue (10, 200) / 100.0f)));
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
Player::Draw (float deltaTime)
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
Player::DrawFragments () const
{
  for (const auto &fragment : fragments)
    {
      float alpha = fragment.life / 2.0f; // Fragment postupně zmizí
      Color color = Fade (RED, alpha);
      DrawCircleV (fragment.position, fragment.radius, color);
    }
}

void
Player::Jump (float groundLevel)
{
  SetIsInJump (true);

  if (IsOnTheGround)
    {
      speedY = jumpVel;
    }
}

Vector2
Player::GetCurrentSpeed (float deltaTime) const
{
  float x = (position.x - previousPosition.x) / deltaTime;
  float y = (position.y - previousPosition.y) / deltaTime;

  return { x, y };
}

Vector2
Player::GetCurrentPosition () const
{
  float x = position.x;
  float y = position.y;

  return { x, y };
}

float
Player::GetPlayerRadius () const
{
  return playerRadius;
}

void
Player::DrawTrail ()
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
Player::CheckCollision (const Rectangle &obstacle)
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
Player::GetLives ()
{
  return lives;
}

bool
Player::GetDoRestart ()
{
  return doRestart;
}

void
Player::DecelX (float deltaTime)
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
Player::AccelX (float direction, float deltaTime, float maxSpeed)
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
void
Player::SetIsInJump (bool jump)
{
  if (jump)
    {
      isInJump = true;
    }
  else
    {
      isInJump = false;
    }
}