#include "card.h"

//==============================================================================
std::ostream& operator<<(std::ostream& os, const UnoCard& card) {
  os << kUnoSuitReadable.at(card.suit_) << " " << 
    kUnoRankReadable.at(card.rank_);
}
