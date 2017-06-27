#include <iostream>

#include "card.h"
#include "deck.h"
#include "player.h"
#include "game.h"

int main() {
  Game uno_game;
  Player p0("Player 0");
  p0.Init(&uno_game);
  Player p1("Player 1");
  p1.Init(&uno_game);
  uno_game.Setup({p0, p1});
  uno_game.MainGameLoop();
  return 0;
}
