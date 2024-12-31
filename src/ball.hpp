#pragma once
#include "raylib.h"

class Ball
{
public:
  // konstruktor
  Ball (bool debug, float x, float y, float radius, float gravity,
        float jumpForce, float moveSpeed, float acceleration,
        float deceleration, float bounceFactor, float squashFactor);

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

private:
  const bool DEBUG;         // DEBUG režim
  Vector2 position;         // pozice hráče
  Vector2 previousPosition; // Minulá pozice míče
  float radius;             // radius míče
  float speedY;             // vertikální rychlost
  float speedX;             // horizontální rychlost
  float gravity;            // síla gravitace
  float jumpForce;          // síla výskoku
  float moveSpeed;          // rychlost pohybu
  float acceleration;       // decelerace v pohybu x
  float deceleration;       // decelerace v pohybu x
  float bounceFactor;       // faktor odražení
  float squashFactor;       // Faktor pro zploštění
  bool IsOnTheGround;       // Náraz do země
  bool IsHitSoundPlayed;    // Přehrál se zvuk bouchnutí
  Sound bounceSound;        // Načtení zvuku

  // Výpočet decelerace
  void DecelX (float deltaTime);

  // Výpočet akcelerace
  void AccelX (float direction, float deltaTime, float maxSpeed);
};