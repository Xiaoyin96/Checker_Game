//
//  Player.cpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Player.hpp"
#include "Game.hpp"
#include <stdlib.h>

namespace ECE141 {
  
  int Player::pcount = 0; //init our static member to track # of players...
  
  static PieceColor autoColor() { //auto assigns a color
    return (++Player::pcount % 2) ? PieceColor::blue : PieceColor::gold;
  }
  
  Player::Player() : color(autoColor()) {}
 
  bool Player::takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog) { // it's virtual
    return false; //if you return false, you forfeit!
  }
  
  }
  
