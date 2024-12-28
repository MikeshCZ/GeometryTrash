#include "raylib.h"

class Ball
{
private:
  Vector2 position;         // pozice hráče
  float radius;             // radius míče
  float speedY;             // vertikální rychlost
  float speedX;             // horizontální rychlost
  float gravity;            // síla gravitace
  float jumpForce;          // síla výskoku
  float moveSpeed;          // rychlost pohybu
  float bounceFactor;       // faktor odražení
  float squashFactor;       // Faktor pro zploštění
  bool IsHitGround;         // Náraz do země
  bool IsBounceSoundPlayed; // Přehrál se zvuk bouchnutí
  Sound bounceSound = LoadSound ("audio/basketball.ogg"); // Načtení zvuku

public:
  // inicializace hráče jako míče
  Ball (float x, float y, float radius, float gravity, float jumpForce,
        float moveSpeed, float bounceFactor, float squashFactor);
  void Update (float deltaTime, float groundLevel); // aktualizace pozice hráče
  void Draw () const;                               // vykreslení hráče
  void Jump (float groundLevel);                    // metoda pro skok
  void Move (float direction,
             float deltaTime); // Metoda pro horizontální pohyb
};