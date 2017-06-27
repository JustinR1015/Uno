#ifndef UNO_DECK_H_
#define UNO_DECK_H_

#include <algorithm>
#include <random>
#include <chrono>

//==============================================================================
template <class CardType>
class Deck {
 public:
  //////////////////////////////////////////////////////////////////////////////
  void Shuffle() {
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck_.begin(), deck_.end(), std::default_random_engine(seed));
  }
  //////////////////////////////////////////////////////////////////////////////
  bool DrawTop(CardType* drawn_card) {
    if(deck_.size() < 1) { return false; }
    *drawn_card = deck_.back();
    deck_.pop_back();
    return true;
  }
  //////////////////////////////////////////////////////////////////////////////
  void AddTop(const CardType& card) {
    deck_.push_back(card);
  }
 protected:
  // NOTE: "Top" of deck is the end of the vector
  std::vector<CardType> deck_;
};

const int kWildCount = 4;
const int kNumDecksInPlay = 2;
//==============================================================================
class UnoDeck : public Deck<UnoCard> {
 public:
  //////////////////////////////////////////////////////////////////////////////
  UnoDeck() {
   // Push back a whole deck
   AddWholeSuit(kRedSuit, &deck_);
   AddWholeSuit(kBlueSuit, &deck_);
   AddWholeSuit(kYellowSuit, &deck_);
   AddWholeSuit(kGreenSuit, &deck_);
   AddAllWilds(&deck_);
  };
  //////////////////////////////////////////////////////////////////////////////
  void AddAllWilds(std::vector<UnoCard>* deck) {
   for(int i = 0; i < kWildCount; i++) {
     deck->push_back(UnoCard(kWildSuit, kWild));
     deck->push_back(UnoCard(kWildSuit, kWildDrawFour));
   }
  }
  //////////////////////////////////////////////////////////////////////////////
  void AddWholeSuit(const UnoSuit& suit, std::vector<UnoCard>* deck) {
    // Only one zero card
    deck->push_back(UnoCard(suit, kZero));
    for(int i = 0; i < kNumDecksInPlay; i++) {
      deck->push_back(UnoCard(suit, kOne));
      deck->push_back(UnoCard(suit, kTwo));
      deck->push_back(UnoCard(suit, kThree));
      deck->push_back(UnoCard(suit, kFour));
      deck->push_back(UnoCard(suit, kFive));
      deck->push_back(UnoCard(suit, kSix));
      deck->push_back(UnoCard(suit, kSeven));
      deck->push_back(UnoCard(suit, kEight));
      deck->push_back(UnoCard(suit, kNine));
      deck->push_back(UnoCard(suit, kSkip));
      deck->push_back(UnoCard(suit, kReverse));
      deck->push_back(UnoCard(suit, kDrawTwo));
    }
  }
};

#endif // UNO_DECK_H_
