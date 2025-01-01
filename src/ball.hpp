#pragma once
#include "fragment.hpp"
#include "raylib.h"
#include "trailPoint.hpp"
#include <deque>
#include <vector>

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
  void Draw (float deltaTime);

  // metoda pro skok
  void Jump (float groundLevel);

  // Výpočet aktuální rychlosti hráče
  Vector2 GetCurrentSpeed (float deltaTime) const;

  // Výpočet aktuální pozice
  Vector2 GetCurrentPosition () const;

  // vrátí hodnotu rádiusu míče
  float GetPlayerRadius () const;

  // kontrola kolize, vrátí true, pokud je v kolizi
  bool CheckCollision (const Rectangle &obstacle);

  // vrátí počet životů
  int GetLives ();

  // Restartuj třídu Ball
  bool GetDoRestart ();

private:
  const bool DEBUG;                // DEBUG režim
  const float playerRadius;        // rádius míče
  const float jumpVel;             // síla skoku
  const float bounceFactor;        // faktor odrazu míče
  const float squashFactor;        // faktor sploštění míče
  const float maxSpeed;            // rychlost pohybu
  const float acceleration;        // faktor akcelerace pohybu x
  const float deceleration;        // faktor decelerace pohybu x
  Vector2 position;                // pozice hráče
  Vector2 previousPosition;        // Minulá pozice míče
  float speedY;                    // vertikální rychlost
  float speedX;                    // horizontální rychlost
  float gravity;                   // síla gravitace
  bool IsOnTheGround;              // náraz do země
  bool IsHitSoundPlayed;           // přehrál se zvuk bouchnutí
  bool IsKillSoundPlayed;           // přehrál se zvuk zabití
  Sound bounceSound;               // načtení zvuku
  Sound startSound;                // zvuk nafouknutí míče
  Sound killSound;                 // zvuk prasknutí míče
  std::deque<TrailPoint> trail;    // trail za míčem
  float trailDuration;             // čas jak dlouho se zobrazuje trail
  float currentTime;               // aktuální čas
  int lives;                       // životy
  bool isInHitbox;                 // je v hitboxu
  std::vector<Fragment> fragments; // fragmenty při smrti hráče
  bool isAlive;                    // hráč je naživu
  bool doRestart;                  // restartuj třídu Ball

  // Výpočet decelerace
  void DecelX (float deltaTime);

  // Výpočet akcelerace
  void AccelX (float direction, float deltaTime, float maxSpeed);

  // vykreslení trailu
  void DrawTrail ();

  // vykreslení fragmentů při smrti hráče
  void DrawFragments () const;
};