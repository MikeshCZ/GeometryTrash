#include "raylib.h"
#include <string>

using namespace std;

class Ball
{
private:
  const bool DEBUG;
  Vector2 position;         // pozice hráče
  Vector2 previousPosition; // Minulá pozice míče
  float radius;             // radius míče
  float speedY;             // vertikální rychlost
  float speedX;             // horizontální rychlost
  float gravity;            // síla gravitace
  float jumpForce;          // síla výskoku
  float moveSpeed;          // rychlost pohybu
  float bounceFactor;       // faktor odražení
  float squashFactor;       // Faktor pro zploštění
  bool IsOnTheGround;       // Náraz do země
  bool IsHitSoundPlayed;    // Přehrál se zvuk bouchnutí
  Sound bounceSound = LoadSound ("audio/basketball.ogg"); // Načtení zvuku

public:
  // inicializace hráče jako míče
  Ball (bool debug, float x, float y, float radius, float gravity,
        float jumpForce, float moveSpeed, float bounceFactor,
        float squashFactor);
  void Update (float deltaTime, float groundLevel); // aktualizace pozice hráče
  void Draw (float deltaTime) const;                // vykreslení hráče
  void Jump (float groundLevel);                    // metoda pro skok
  void MoveX (float direction,
              float deltaTime); // Metoda pro horizontální pohyb
  Vector2
  GetCurrentSpeed (float deltaTime) const; // Výpočet aktuální rychlosti
};