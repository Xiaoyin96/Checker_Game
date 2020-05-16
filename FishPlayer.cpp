//
//  Player.cpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "FishPlayer.hpp"
#include "Game.hpp"
#include <stdlib.h>
#include <tuple>

namespace ECE141 {
  
  
  bool FishPlayer::takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog){
   
   size_t theCount=aGame.countAvailablePieces(color);
    
    // first check whether has available jump pieces
    bool jump = false;
    int max_step = 0;
    std::vector<Location> bestJump;
    int bestPos = 0;
    for(int pos=0;pos<theCount;pos++) {
      const Piece* thePiece = aGame.getAvailablePiece(this->color, pos);
      Location cur = thePiece->getLocation();
      dfs(*thePiece, aGame, cur, cur, false); // get all jumpList of the piece using dfs
      
      if (!jumpList.empty()){
        jump = true;
        auto best = getBestJump(jumpList);
        if (best.second > max_step){
          max_step = best.second;
          bestJump = best.first;
          bestPos = pos;
        }
      }
      
      jumpList.clear();
    }
    
    // perform jump or multiple jumps
    if (jump){
      const Piece* bestPiece = aGame.getAvailablePiece(this->color, bestPos);
      for (int i=0; i<bestJump.size();i++){
        aGame.movePieceTo(*bestPiece, bestJump[i]);
      }
      return true;
    }
    
    // if not jump, random select forward
    for(int pos=0;pos<theCount;pos++) {
      const Piece *thePiece = aGame.getAvailablePiece(this->color, pos);
      std::vector<Location> forwardList = NeighborForward(aGame, *thePiece);
    
      if (!forwardList.empty()){
        size_t s = forwardList.size();
        size_t idx = rand() % s;
        Location loc(forwardList.at(idx));
        aGame.movePieceTo(*thePiece, loc);
        return true;
      }
    }
    
    return false; //no place to move
    }

  
  bool FishPlayer::canJumpTo(const Piece &aPiece, Game &aGame, Location &thePLoc, int aDeltaX, int aDeltaY) { // check whether a piece can jump to a position
//    Location thePLoc = aPiece.getLocation();
    Location theStep(thePLoc.row+aDeltaY,thePLoc.col+aDeltaX);
    if(const Tile *theTile=aGame.getTileAt(theStep)) {
      const Piece* const thePiece=theTile->getPiece();
      if(thePiece && !thePiece->hasColor(aPiece.getColor())) {
        Location theJump(theStep.row+aDeltaY,theStep.col+aDeltaX);
        if(const Tile *theTile=aGame.getTileAt(theJump)) {
          return theTile->getPiece()==nullptr;
        }
      }
    }
    return false;
  }
  
  
  bool FishPlayer::isAvailable(const Tile *aTile){ // check whether a tile is occupied
    return (!aTile->getPiece()) && (TileColor::dark==aTile->getColor());
  }
  
  bool FishPlayer::validLocation(const Location &aLocation) {
    if(aLocation.row>=0 && aLocation.row<8) {
      return (aLocation.col>=0 && aLocation.col<8);
    }
    return false;
  }
  
  std::vector<Location> FishPlayer::NeighborJump(Game &aGame, const Piece &aPiece){ // any available jump
    PieceColor aColor = aPiece.getColor();
    Location cur = aPiece.getLocation();
    std::vector<Location> JumpLoc;
    Location loc1(cur.row+moreY(aColor)*2,cur.col+lessX(aColor)*2);
    Location loc2(cur.row+moreY(aColor)*2, cur.col+moreX(aColor)*2);
    Location loc3(cur.row+lessY(aColor)*2,cur.col+lessX(aColor)*2);
    Location loc4(cur.row+lessY(aColor)*2,cur.col+moreX(aColor)*2);
    
    if(validLocation(loc1) && canJumpTo(aPiece, aGame, cur, lessX(aColor), moreY(aColor))) {
      JumpLoc.push_back(loc1);
      }
      
    if(validLocation(loc2) && canJumpTo(aPiece, aGame, cur, moreX(aColor), moreY(aColor))) {
      JumpLoc.push_back(loc2);
    }
      
    if (PieceKind::king==aPiece.getKind()) { // if it's a king, have to check other 2 directions
      if(validLocation(loc3) && canJumpTo(aPiece, aGame, cur, lessX(aColor), lessY(aColor))) {
        JumpLoc.push_back(loc3);
      }
        
      if(validLocation(loc4) && canJumpTo(aPiece, aGame, cur, moreX(aColor), lessY(aColor))) {
        JumpLoc.push_back(loc4);
      }
    }
    return JumpLoc;
    }
  
  void FishPlayer::dfs(const Piece &aPiece, Game &aGame, Location &cur, Location &prev, bool flag){ //  get all jumps including multiple jumps -> in jumpList
    PieceColor aColor = aPiece.getColor();
    PieceKind aKind = aPiece.getKind();
    
    
    
    if (aKind == PieceKind::king){
      int dirs[][2] = {{moreY(aColor), lessX(aColor)}, {moreY(aColor), moreX(aColor)}, {lessY(aColor),lessX(aColor)}, {lessY(aColor),moreX(aColor)}};
      for (auto d : dirs) {
        int r = d[0]*2 + cur.row;
        int c = d[1]*2 + cur.col;
        Location loc(r,c);
        bool isSame = (prev.col==loc.col && prev.row==loc.row); // check whether the same location as previous
        if ((!isSame) && validLocation(loc) && canJumpTo(aPiece, aGame, cur, d[1], d[0])){ // cannot go back to previous same tile
          path.push_back(loc);
          flag = true;
          dfs(aPiece, aGame, loc, cur, false);
        }
      }
      
    }
    else{
      int dirs[][2] = {{moreY(aColor), lessX(aColor)}, {moreY(aColor), moreX(aColor)}}; // only consider forward
      for (auto d : dirs) {
        int r = d[0]*2 + cur.row;
        int c = d[1]*2 + cur.col;
        Location loc(r,c);
        bool isSame = (prev.col==loc.col && prev.row==loc.row);
        if ( (!isSame) && validLocation(loc) && canJumpTo(aPiece, aGame, cur, d[1], d[0])){
          path.push_back(loc);
          flag = true;
          dfs(aPiece, aGame, loc, cur, false);
        }
      }
    }
    
    if (!flag && !path.empty()){ // no further path
      jumpList.push_back(path);
      path.clear();
    }
    
    
    
  }

  std::vector<Location> FishPlayer::NeighborForward(Game &aGame, const Piece &aPiece){ // any available place to move forward
    PieceColor aColor = aPiece.getColor();
    Location cur = aPiece.getLocation();
    std::vector<Location> ForwardLoc;
    Location loc1(cur.row+moreY(aColor),cur.col+lessX(aColor));
    Location loc2(cur.row+moreY(aColor), cur.col+moreX(aColor));
    Location loc3(cur.row+lessY(aColor),cur.col+lessX(aColor));
    Location loc4(cur.row+lessY(aColor),cur.col+moreX(aColor));
    
    if(validLocation(loc1) && isAvailable(aGame.getTileAt(loc1))) {
      ForwardLoc.push_back(loc1);
      }
      
    if(validLocation(loc2) && isAvailable(aGame.getTileAt(loc2))) {
    ForwardLoc.push_back(loc2);
    }
      
    if (PieceKind::king==aPiece.getKind()) { // if it's a king, have to check other 2 directions
      if(validLocation(loc3) && isAvailable(aGame.getTileAt(loc3))) {
      ForwardLoc.push_back(loc3);
      }
        
      if(validLocation(loc4) && isAvailable(aGame.getTileAt(loc4))) {
      ForwardLoc.push_back(loc4);
      }
    }
    return ForwardLoc;
    }
  
  std::pair<std::vector<Location>,int> FishPlayer::getBestJump(std::vector<std::vector<Location>> jumpList){ // find best jump in given jumpList
    size_t max = 0;
    std::vector<Location> move;
    
    if (!jumpList.empty()){
      for (int i=0; i< jumpList.size();i++){
        if (jumpList[i].size() > max){
          max = jumpList[i].size();
          move = jumpList[i];
        }
      }
    }
    std::pair<std::vector<Location>,int> temp;
    temp = std::make_pair(move, max);
    return temp;
  }
}
  
