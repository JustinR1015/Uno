#ifndef UNO_CARD_H_
#define UNO_CARD_H_

#include <iostream>
#include <map>
#include <sstream>

enum UnoSuit {
  kUninitializedSuit,
  kRedSuit,
  kBlueSuit,
  kYellowSuit,
  kGreenSuit,
  kWildSuit,
};

const std::map<UnoSuit, std::string> kUnoSuitReadable({
    { kUninitializedSuit, "Uninitialized Suit" },
    { kRedSuit, "Red" },
    { kBlueSuit, "Blue" },
    { kYellowSuit, "Yellow" },
    { kGreenSuit, "Green" },
    { kWildSuit, "Wild" },
});

const std::map<UnoSuit, std::string> kUnoSuitReadableShort({
    { kUninitializedSuit, "U" },
    { kRedSuit, "R" },
    { kBlueSuit, "B" },
    { kYellowSuit, "Y" },
    { kGreenSuit, "G" },
    { kWildSuit, "W" },
});

enum UnoRank {
  kUninitializedRank,
  kZero,
  kOne,
  kTwo,
  kThree,
  kFour,
  kFive,
  kSix,
  kSeven,
  kEight,
  kNine,
  kSkip,
  kReverse,
  kDrawTwo,
  kWild,
  kWildDrawFour
};

const std::map<UnoRank, std::string> kUnoRankReadable({
    { kUninitializedRank, "Uninitialized Rank" },
    { kZero, "Zero" },
    { kOne, "One" },
    { kTwo, "Two" },
    { kThree, "Three" },
    { kFour, "Four" },
    { kFive, "Five" },
    { kSix, "Six" },
    { kSeven, "Seven" },
    { kEight, "Eight" },
    { kNine, "Nine" },
    { kSkip, "Skip" },
    { kReverse, "Reverse" },
    { kDrawTwo, "Draw Two" },
    { kWild, "Wild" },
    { kWildDrawFour, "Wild Draw Four" },
});

const std::map<UnoRank, std::string> kUnoRankReadableShort({
    { kUninitializedRank, "U" },
    { kZero, "0" },
    { kOne, "1" },
    { kTwo, "2" },
    { kThree, "3" },
    { kFour, "4" },
    { kFive, "5" },
    { kSix, "6" },
    { kSeven, "7" },
    { kEight, "8" },
    { kNine, "9" },
    { kSkip, "S" },
    { kReverse, "R" },
    { kDrawTwo, "D" },
    { kWild, "W" },
    { kWildDrawFour, "F" },
});

//==============================================================================
template <class SuitType, class RankType>
class Card {
 public:
  //////////////////////////////////////////////////////////////////////////////
  Card(SuitType suit, RankType rank):suit_(suit), rank_(rank){};
  //////////////////////////////////////////////////////////////////////////////
  bool operator==(const Card& rhs) const { 
    return suit_ == rhs.suit_ && rank_ == rhs.rank_;
  }
 //protected:
 public: // data
  // suit
  SuitType suit_; 
  // rank
  RankType rank_;
};

//==============================================================================
class UnoCard : public Card<UnoSuit, UnoRank> {
 public:
  //////////////////////////////////////////////////////////////////////////////
  UnoCard(UnoSuit suit = kUninitializedSuit, UnoRank rank = kUninitializedRank)
    :Card(suit, rank){};
  //////////////////////////////////////////////////////////////////////////////
  friend std::ostream& operator<<(std::ostream&, const UnoCard&);
  //////////////////////////////////////////////////////////////////////////////
  std::string ShortLabel() const {
    std::stringstream ss;
    ss << kUnoSuitReadableShort.at(suit_) << kUnoRankReadableShort.at(rank_);
    return ss.str();
  }
};

//==============================================================================
std::ostream& operator<<(std::ostream& os, const UnoCard& card);

#endif // UNO_CARD_H_
