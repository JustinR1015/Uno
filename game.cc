#include <cstdlib>
#include <string>
#include <limits>

#include "game.h"

//==============================================================================
void Game::Setup(const std::list<Player>& players) {
  players_ = players;
  dealer_ = players_.begin();
};
//==============================================================================
void Game::Deal() {
  for(int i = 0; i < kStartingHandCardCount; i++){
    for(auto& player:players_) {
      UnoCard card;
      draw_pile_.DrawTop(&card);
      player.AddCardToHand(card);
    }
  }
};
//==============================================================================
void Game::PrintAllPlayersHands() {
  int i = 0;
  for(auto& player:players_) {
    std::cout << player.GetName() << " Hand - ";
    player.PrintHand();
    std::cout << std::endl;
  }
};
//==============================================================================
void Game::PrintGameState() {
  std::cout << "==========================================" << std::endl;
  std::cout << current_player_->GetName() << "'s turn" << std::endl;
  PrintAllPlayersHands();
  std::cout << "Discard pile size: " << discard_pile_.size() << 
    ", top of pile: " << discard_pile_.back() << std::endl;
};
//==============================================================================
bool Game::CheckIfGameOver(Player* winner) {
  bool retval = false;
  int winning_score = std::numeric_limits<int>::max();
  for(auto& player:players_) {
    int player_score = scores_[player.GetName()];
    if(player_score >= kGameOverScore) {
      retval = true;
    }
    if(player_score < winning_score) {
      winning_score = player_score;
      *winner = player;
    }
  }
  return retval;
};
//==============================================================================
bool Game::CheckIfAnyPlayerWonHand(Player* winner) {
  for(auto& player:players_) {
    if(player.GetHandSize() == 0) {
      *winner = player;
      return true;
    }
  }
  return false;
};
//==============================================================================
void Game::SetupHand() {
  // Clear player hands
  for(auto& player:players_) {
    player.ClearHand();
  }
  // Move dealer
  current_player_ = dealer_;
  // Reset all the card piles
  TransferDiscardToDraw();
  draw_pile_.Shuffle();
  // Deal
  Deal();
  // Setup the discard pile
  UnoCard card(DrawCard());
  discard_pile_.push_back(card);
  // Don't start with a wild
  while(card.suit_ == kWildSuit) {
    card = DrawCard();
    discard_pile_.push_back(card);
  }
}
//==============================================================================
void Game::PlayHand() {
  Player winner;
  bool someone_won_hand = false;
  // while not victory condition
  while(someone_won_hand == false) {
    PrintGameState();
    // Reset valid turn
    turn_valid_ = false;
    card_played_this_turn_ = false;
    // tell next player to take turn
    current_player_->TakeTurn(discard_pile_.back(), requested_suit_);
    // See if we've had a valid turn
    if(turn_valid_ == false) {
      std::cout << "ERROR: Player " << current_player_->GetName() << 
        " did not execute a valid turn." << std::endl;
      abort();
    }
    // Look at the top of the discard pile and take appropriate action 
    // (handle special ranks)
    if(card_played_this_turn_ == true) {
      if(discard_pile_.back().suit_ == kWildSuit) {
        // Save new requested color
        requested_suit_ = current_player_->GetRequestedSuit();
      }
      if(discard_pile_.back().rank_ == kSkip) {
        AdvancePlayer(&current_player_);
      } else if(discard_pile_.back().rank_ == kReverse) {
        player_direction_forward_ = !player_direction_forward_;
      } else if(discard_pile_.back().rank_ == kDrawTwo) {
        AdvancePlayer(&current_player_);
        // Send two cards to new current player
        current_player_->AddCardToHand(DrawCard());
        current_player_->AddCardToHand(DrawCard());
      } else if(discard_pile_.back().rank_ == kWildDrawFour) {
        AdvancePlayer(&current_player_);
        // Send four cards to new current player
        for(int i = 0; i < 4; i++) {
          current_player_->AddCardToHand(DrawCard());
        }
      }
    }
    // Check to see if anyone won
    someone_won_hand = CheckIfAnyPlayerWonHand(&winner);
    if(someone_won_hand) {
      break; // Game over, stop taking turns
    } else { // no one won, so get next player
      AdvancePlayer(&current_player_);
    }
  }
  if(someone_won_hand) {
    std::cout << winner.GetName() << " won hand!" << std::endl;
    std::cout << "Final hands = " << std::endl; 
    PrintAllPlayersHands();
    std::list<Player>::iterator temp_player = players_.begin();
    while(*temp_player != winner) { temp_player++; }
    AdvancePlayer(&temp_player);
    while(*temp_player != winner) {
      int score = ScoreHand(temp_player->GetHand());
      std::cout << temp_player->GetName() << " caught with " << 
        score << std::endl;
      scores_[temp_player->GetName()] = 
        scores_[temp_player->GetName()] + score;
      AdvancePlayer(&temp_player);
    }
  } else {
    std::cout << "Nobody won!" << std::endl;
  }
}
//==============================================================================
void Game::MainGameLoop() {
  Player winner;
  bool someone_won_game = false;
  while(someone_won_game == false) {
    std::cout << "**********************************************************" << 
      std::endl;
    SetupHand();
    PlayHand();
    std::cout << "Scores After Hand:" << std::endl;
    PrintScores();
    someone_won_game = CheckIfGameOver(&winner);
    if(someone_won_game) {
      break; // Game over, stop taking turns
    } else { // no one won, so advance dealer
      AdvancePlayer(&dealer_);
    }
  }
  if(someone_won_game) {
    std::cout << winner.GetName() << " won game!!!" << std::endl;
    std::cout << "Final Scores:" << std::endl;
    PrintScores();
  } else {
    std::cout << "Nobody won game!" << std::endl;
  }
}
//==============================================================================
void Game::PrintScores() {
  for(const auto& player:scores_) {
    std::cout << "\t" << player.first << " = " << player.second << 
      std::endl;
  }
}
//==============================================================================
void Game::AdvancePlayer(std::list<Player>::iterator* current_player_) {
  if(player_direction_forward_) {
    (*current_player_)++;
    if(*current_player_ == players_.end()) { // start list over if needed
      *current_player_ = players_.begin();
    }
  } else {
    // start list over if needed
    if(*current_player_ == players_.begin()) { 
      if(players_.size() == 2) {
        // make it so first player_ goes again
        *current_player_ = players_.end()--;
      } else {
        *current_player_ = players_.end();
      }
    }
    (*current_player_)--;
  }
}
//==============================================================================
void Game::TransferDiscardToDraw() {
  // Dumb way to do it
  // Save the top card
  if(discard_pile_.size() == 0) {
    return;
  }
  UnoCard top_of_discard = discard_pile_.back();
  discard_pile_.pop_back();

  // Transfer the rest
  while(discard_pile_.size() > 0) {
    draw_pile_.AddTop(discard_pile_.back());
    discard_pile_.pop_back();
  }
  // Put the top of the discard pile back
  discard_pile_.push_back(top_of_discard);
  draw_pile_.Shuffle();
}
//==============================================================================
UnoCard Game::DrawCard() {
  turn_valid_ = true; // this is a valid move during a player turn
  UnoCard card;
  if(draw_pile_.DrawTop(&card) == false) {
    // draw failed so transfer discard minus 1 card to draw_pile, shuffle, 
    // and draw again
    TransferDiscardToDraw();
    if(draw_pile_.DrawTop(&card) == false) {
      std::cout << "ERROR: Couldn't successfully draw card." << std::endl;
      abort();
    }
  }
  return card;
};
//==============================================================================
bool Game::PlayCard(const UnoCard& card, Player* player) {
  std::cout << player->GetName() << " playing: " << card << 
    std::endl;
  // Verify that card is in hand
  std::vector<UnoCard> player_hand = player->GetHand();
  if(IsCardInHand(player_hand, card) == false) {
    std::cout << "ERROR: Card not in player hand." << std::endl;
    return false;
  }
  // Verify that card plays
  if(CardPlaysOnDiscardPile(card, player_hand) == false) {
    std::cout << "ERROR: Card does not play." << std::endl;
    return false;
  }
  // Place card in discard pile
  //std::cout << "Adding card: " << card << " to discard_pile_." << std::endl;
  discard_pile_.push_back(card);
  // Remove card from player hand
  //std::cout << "Removing card: " << card << " from player hand." << std::endl;
  player->RemoveCardFromHand(card);
  turn_valid_ = true;
  card_played_this_turn_ = true;
  return true;
}
//==============================================================================
bool Game::IsCardInHand(const std::vector<UnoCard>& hand, const UnoCard& card) {
  for(const auto& hand_card:hand) {
    if(hand_card == card) {
      return true;
    }
  }
  return false;
}
//==============================================================================
bool Game::CardPlaysOnDiscardPile(const UnoCard& card, 
    const std::vector<UnoCard>& hand) {
  UnoCard top_of_discard = discard_pile_.back();
  UnoSuit playable_suit(kUninitializedSuit);
  if(top_of_discard.suit_ == kWildSuit) {
    std::cout << "Have wild on top of discard, looking for suit: " << 
      kUnoSuitReadable.at(requested_suit_) << std::endl;
    playable_suit = requested_suit_;
  } else {
    playable_suit = top_of_discard.suit_;
  }
  return card.rank_ == kWild ||
    (card.suit_ != kWildSuit && card.suit_ == playable_suit) ||
    (card.suit_ != kWildSuit && card.rank_ == top_of_discard.rank_) ||
    (card.rank_ == kWildDrawFour && 
     NoNonWildDrawFourPlayableCard(hand, top_of_discard));
}
//==============================================================================
bool Game::NoNonWildDrawFourPlayableCard(const std::vector<UnoCard>& hand, 
    const UnoCard& top_of_discard) {
  for(const auto& card:hand) {
    if(card.rank_ != kWildDrawFour) {
      if(card.suit_ != kWildSuit && card.suit_ == top_of_discard.suit_) {
        return false;
      }
    }
  }
  return true;
}
//==============================================================================
int Game::ScoreHand(const std::vector<UnoCard>& hand) {
  int score = 0;
  for(const auto& card:hand) {
    score += kUnoRankValue.at(card.rank_);
  }
  return score;
}
