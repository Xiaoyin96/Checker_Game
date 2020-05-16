//
//  Player.hpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef FishPlayer_hpp
#define FishPlayer_hpp

#include "Piece.hpp"
#include "Player.hpp"
#include <iostream>
#include <vector>
#include <utility> 

namespace ECE141 {
  
  class Player;
  
  // my subclass of Player
  class FishPlayer: public Player{
  public:
    int moreY(PieceColor aColor) {return PieceColor::blue==aColor ? -1 : 1;}
    int lessY(PieceColor aColor) {return PieceColor::blue==aColor ? 1 : -1;}
    int moreX(PieceColor aColor) {return PieceColor::blue==aColor ? 1 : -1;}
    int lessX(PieceColor aColor) {return PieceColor::blue==aColor ? -1 : 1;}
    
    bool takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog);
    std::vector<Location> NeighborForward(Game &aGame, const Piece &aPiece); // any available place to move forward
    std::vector<Location> NeighborJump(Game &aGame, const Piece &aPiece); // any available jump? if available, must jump!
    bool canJumpTo(const Piece &aPiece, Game &aGame, Location &thePLoc, int aDeltaX, int aDeltaY); // check whether the piece can jump to a tile
    bool isAvailable(const Tile *aTile); // check whether a tile is occupied
    bool validLocation(const Location &aLocation); // check whether the location is valid in plate
    void dfs(const Piece &aPiece, Game &aGame, Location &curLoc, Location &prev, bool flag=false);
    std::pair<std::vector<Location>,int> getBestJump(std::vector<std::vector<Location>> jumpList);
    
  protected:
    std::vector<std::vector<Location>> jumpList;
    std::vector<Location> path;
  };
  
  
}

#endif /* FishPlayer_hpp */
