#pragma once
#include "raylib.h"
#include "trailPoint.hpp"
#include <deque>

class Ball
{
public:
  // konstruktor
  Ball (bool debug, float x, float y, float gravity);

  // destruktor
  ~Ball ();

  // aktualizace pozice hráče
  void Update (float direction, float deltaTime, float groundLevel);

  // vykreslení hráče
  void Draw (float deltaTime) const;

  // metoda pro skok
  void Jump (float groundLevel);

  // Výpočet aktuální rychlosti hráče
  Vector2 GetCurrentSpeed (float deltaTime) const;

  // Výpočet aktuální pozice
  Vector2 GetCurrentPosition () const;

  // vrátí hodnotu rádiusu míče
  float GetPlayerRadius () const;

  // vykreslení trailu
  void DrawTrail ();

private:
  const bool DEBUG;             // DEBUG režim
  const float playerRadius;     // rádius míče
  const float jumpVel;          // síla skoku
  const float bounceFactor;     // faktor odrazu míče
  const float squashFactor;     // faktor sploštění míče
  const float maxSpeed;         // rychlost pohybu
  const float acceleration;     // faktor akcelerace pohybu x
  const float deceleration;     // faktor decelerace pohybu x
  Vector2 position;             // pozice hráče
  Vector2 previousPosition;     // Minulá pozice míče
  float speedY;                 // vertikální rychlost
  float speedX;                 // horizontální rychlost
  float gravity;                // síla gravitace
  bool IsOnTheGround;           // náraz do země
  bool IsHitSoundPlayed;        // přehrál se zvuk bouchnutí
  Sound bounceSound;            // načtení zvuku
  std::deque<TrailPoint> trail; // trail za míčem
  float trailDuration;          // čas jak dlouho se zobrazuje trail
  float currentTime;            // aktuální čas

  // Výpočet decelerace
  void DecelX (float deltaTime);

  // Výpočet akcelerace
  void AccelX (float direction, float deltaTime, float maxSpeed);
};