#include "player.h"
#include "card.h"
#include "game.h"

//==============================================================================
void Player::Init(Game* game){
  game_ = game;
}
//==============================================================================
UnoSuit Player::FindMostCommonSuit() const {
  std::map<UnoSuit, int> suit_hist;
  for(const auto& card:hand_) {
    if(card.suit_ != kWildSuit) {
      if(suit_hist.count(card.suit_) == 0) {
        suit_hist[card.suit_] = 1;
      } else {
        suit_hist[card.suit_] = suit_hist[card.suit_] + 1;
      }
    }
  }
  for(const auto& hist:suit_hist) {
    std::cout << "Suit: " << kUnoSuitReadable.at(hist.first) << 
      ", count: " << hist.second << std::endl;
  }
  using pair_type = std::pair<UnoSuit, int>;
  return std::max_element(suit_hist.begin(), suit_hist.end(), 
      [] (const pair_type& p1, const pair_type& p2) { 
      return p1.second < p2.second;
      })->first;
}
//==============================================================================
void Player::TakeTurn(const UnoCard& top_of_discard,
    const UnoSuit& requested_suit){
  UnoSuit playable_suit(kUninitializedSuit);
  if(top_of_discard.suit_ == kWildSuit) {
    playable_suit = requested_suit;
  } else {
    playable_suit = top_of_discard.suit_;
  }
  //std::cout << "Top of pile suit: " << kUnoSuitReadable.at(top_of_discard.suit_) << ", looking for suit: " << kUnoSuitReadable.at(playable_suit) << std::endl;
  // See if any non-wild plays
  for(const auto& card:hand_) {
    if( (card.suit_ == playable_suit) ||
        (card.rank_ == top_of_discard.rank_)) {
      game_->PlayCard(card, this);
      return;
    } 
  }
  // See if we have any wild cards
  for(const auto& card:hand_) {
    if(card.suit_ == kWildSuit) {
      requested_suit_ = FindMostCommonSuit();
      std::cout << name_ << " requested suit: " << 
        kUnoSuitReadable.at(requested_suit_) << std::endl;
      game_->PlayCard(card, this);
      return;
    } 
  }
  // Nothing played, so get a card
  UnoCard card = game_->DrawCard();
  std::cout << GetName() << ", Drawing Card; got: " << card << 
    std::endl;
  AddCardToHand(card);
};
//==============================================================================
void Player::AddCardToHand(const UnoCard& card){
  hand_.push_back(card);
};
//==============================================================================
void Player::PrintHand() const {
  if(hand_.size() > 0) {
    auto card = hand_.begin();
    for(;card != --(hand_.end()); card++) {
      std::cout << card->ShortLabel() << ", ";
    }
    std::cout << card->ShortLabel();
  }
};
//==============================================================================
void Player::RemoveCardFromHand(const UnoCard& card) { 
  for(auto hand_card = hand_.begin(); hand_card != hand_.end(); hand_card++) {
    if(*hand_card == card) {
      hand_.erase(hand_card);
      return;
    }
  }
}
