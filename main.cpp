//
//  main.cpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Game.hpp"
#include "FishPlayer.hpp"
#include <iostream>
#include <fstream>

//STUDENT: include your own playere file here (named after you, like "RenjiePlayer")...
//         Use your player below, instead of the default Player class

std::ostream& getOutput() {return std::cout;}

int main(int argc, const char * argv[]) {
  
  ECE141::FishPlayer player1;  //Use yorur own player class here...
  ECE141::FishPlayer player2;  //Make two players from YOUR player class

  ECE141::Game::run(player1,player2, getOutput());
  
  return 0;
}
