#ifndef UNO_GAME_H_
#define UNO_GAME_H_

#include <future>
#include <list>
#include <vector>

#include "player.h"
#include "card.h"
#include "deck.h"

const int kStartingHandCardCount = 7;
const int kGameOverScore = 500;

const std::map<UnoRank, int> kUnoRankValue({
    { kUninitializedRank, 0 },
    { kZero, 0 },
    { kOne, 1 },
    { kTwo, 2 },
    { kThree, 3 },
    { kFour, 4 },
    { kFive, 5 },
    { kSix, 6 },
    { kSeven, 7 },
    { kEight, 8 },
    { kNine, 9 },
    { kSkip, 20 },
    { kReverse, 20 },
    { kDrawTwo, 20 },
    { kWild, 50 },
    { kWildDrawFour, 50 },
});

class Game {
 public:
  //////////////////////////////////////////////////////////////////////////////
  Game() {};
  //////////////////////////////////////////////////////////////////////////////
  ~Game() {};
  //////////////////////////////////////////////////////////////////////////////
  // Setup game
  // - shuffle deck
  // - deal cards
  // - play first card
  void Setup(const std::list<Player>& players);
  //////////////////////////////////////////////////////////////////////////////
  void Deal();
  //////////////////////////////////////////////////////////////////////////////
  void PrintAllPlayersHands();
  //////////////////////////////////////////////////////////////////////////////
  void PrintGameState();
  //////////////////////////////////////////////////////////////////////////////
  void MainGameLoop();
  //////////////////////////////////////////////////////////////////////////////
  void StartGame() {
    std::future<void> status = std::async(std::launch::async, 
        &Game::MainGameLoop, this);
  }
  //////////////////////////////////////////////////////////////////////////////
  UnoCard DrawCard();
  //////////////////////////////////////////////////////////////////////////////
  bool PlayCard(const UnoCard& card, Player* player);
  //////////////////////////////////////////////////////////////////////////////
  int ScoreHand(const std::vector<UnoCard>& hand);
 private:
  //////////////////////////////////////////////////////////////////////////////
  bool CheckIfAnyPlayerWonHand(Player* winner);
  //////////////////////////////////////////////////////////////////////////////
  bool CheckIfGameOver(Player* winner);
  //////////////////////////////////////////////////////////////////////////////
  void TransferDiscardToDraw();
  //////////////////////////////////////////////////////////////////////////////
  bool IsCardInHand(const std::vector<UnoCard>& hand, const UnoCard& card);
  //////////////////////////////////////////////////////////////////////////////
  bool CardPlaysOnDiscardPile(const UnoCard& card, 
    const std::vector<UnoCard>& hand);
  //////////////////////////////////////////////////////////////////////////////
  bool NoNonWildDrawFourPlayableCard(const std::vector<UnoCard>& hand, 
    const UnoCard& top_of_discard);
  //////////////////////////////////////////////////////////////////////////////
  void AdvancePlayer(std::list<Player>::iterator* current_player);
  //////////////////////////////////////////////////////////////////////////////
  void SetupHand();
  //////////////////////////////////////////////////////////////////////////////
  void PlayHand();
  //////////////////////////////////////////////////////////////////////////////
  void PrintScores();
 private:
  // players
  std::list<Player> players_;
  // deck
  UnoDeck draw_pile_;
  // discard pile
  std::vector<UnoCard> discard_pile_;
  bool turn_valid_{false};
  bool card_played_this_turn_{false};
  bool player_direction_forward_{true};
  UnoSuit requested_suit_{kUninitializedSuit};
  std::list<Player>::iterator current_player_;
  std::list<Player>::iterator dealer_;
  std::map<std::string, int> scores_;
};

#endif // UNO_GAME_H_
