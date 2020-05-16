## Let's Play Checkers

### Rules of the Game

Checkers is a two-person board game played on an 8x8 game board (same as chess board). Each player places their 12 pieces (usually red vs black) on the board on alternating dark squares in the three rows nearest to themselves. (See the following picture).

![Image](checkers.png)

- At the start of the game, all pieces are "pawns", and can only move forward (away from their owner/player) by 1 diagonal space (on dark squares) at a time, toward the opponent's side of the board.
- If a piece player's piece is directly adjacent to an opponent's piece, it _may_ be able to "capture" the opponent's piece.
- A piece can _only_ jump an opponent's piece when the pieces are "diagonally adjacent", and the following space in the forward direction is unoccupied. Only one piece can be captured per jump; however, multiple jump may be performed sequentially during a single turn, provided the condition of diagonal adjacency is met.
- Any time a move exists on the board where an opponent's piece can be captured, the player is required to jump and capture the piece.  
- Captured pieces must be removed from the board.
- When a piece reaches the furthest row away from the player who controls that piece, it is "crowned" and becomes a King. One of the pieces that have already been removed is stacked on the Pawn, indicating it is now a King. (Kings are twice as tall as Pawns).
- Unlike Pawns which may only move forward, Kings may move forward or backward along the diagonal.
- Kings may jump other players in either a forward or backward direction, in any combination.
- To win the game, you must elimminate all of your opponents pieces from the board. 

> NOTE: Here's a video on playing checkers if you've never played before.  https://youtu.be/m0drB0cx8pQ

## The `Game` Class

It's essential that you understand the simple interface provided by the `Game` class. It only offers a few methods for interacting with the game itself. The first three methods let you get information about the current game. Call the last method to inform the game about your move decision. 

```
    virtual size_t             countAvailablePieces(PieceColor aColor)=0;
    virtual const Piece* const getAvailablePiece(PieceColor aColor, int anIndex)=0;
    virtual const Tile*  const getTileAt(const Location &aLocation)=0;
    
    virtual bool               movePieceTo(const Piece &aPiece, const Location &aLocation)=0; 
```

### `countAvailablePieces(PieceColor aColor)` 
This method tells you how many pieces of a given color remain on the board.  You might use this to control a while while analyzing options for each of your pieces on the board.

### `getAvailablePiece(PieceColor aColor, int anIndex)` 
This method retrieves the **Nth** _remaining_ piece of a given color that remains on the board. The game is free to return to pieces to you in any order.  

### `getTileAt(const Location &aLocation)` 
This method retrieves a `Tile` object for you to inspect. Each tile knows the tile color, the location (row,col), and which `Piece` (if any) is occupying the `Tile`. 

When you are considering a move, you may call the `Game` to retrieve a `Tile` you are considering. If you ask for a `Tile` at a `Location` that does not exist, this method will return a `nullptr`.

### `movePieceTo(const Piece &aPiece, const Location &aLocation)` 
Call this method when you have completed your analysis and are ready to tell the game the destination (`Location`) where you want to move on of your `Piece`'s.  As we mention else where, if you are doing a multi-jump, you may call this method more than once. Calling this method at any time when you _aren't_ performing a multi-jump will disqualify your bot from the game.

### Running the game 

The `Game` class is responsible for running the game. Each player gets a turn to analyze the board, and make a move decision. Between each move, current state of the board is displayed for reference. As sample is shown below.  Notice that board contains pieces marked "b" and "g". A piece is capitalized ("B","G") to indicate that piece has been "kinged" and can move more freely.

```
  Step 67
   |---|---|---|---|---|---|---|---|
 0 | . | . | . | . | . | . | . | . |
   |---|---|---|---|---|---|---|---|
 1 | . | . | . | . | . | . | . | . |
   |---|---|---|---|---|---|---|---|
 2 | . | . | . | . | . | B | . | . |
   |---|---|---|---|---|---|---|---|
 3 | . | . | . | . | . | . | g | . |
   |---|---|---|---|---|---|---|---|
 4 | . | . | . | . | . | . | . | . |
   |---|---|---|---|---|---|---|---|
 5 | . | . | . | . | . | . | . | . |
   |---|---|---|---|---|---|---|---|
 6 | . | . | . | . | . | g | . | g |
   |---|---|---|---|---|---|---|---|
 7 | G | . | . | . | . | . | G | . |
   |---|---|---|---|---|---|---|---|
     0   1   2   3   4   5   6   7
```

### Examining the gameboard

At the start of your turn, the game calls your `Player.takeTurn(const Game &aGame)` method call. During your turn, you can use the `Game` interface to get access to information about the state of the game (board, tiles, pieces) -- and to tell the `Game` your move.

There are a few other classes here worth noting. First is the `Location` class indicates the position of a square on the `Board` in row/column order.  The `Tile` represents the state of a tile (square) on the board. It has a color, a `Location` and if occupied a `Piece`.  Each player starts with 12 `Piece`'s on the board.  For one player, the back row starts at (0,0), left-to-right. For the other, the back row starts at (7,7).  If you think about it, that means for one player, moving foward means that location values _increase_, while for the other player, moving forward implies that location values _decrease_. 

You may be wondering, once you've examined the gameboard, how do you choose a move, given a set of possible moves?

You indicate your next move by changes interface provided by the `Game` class. Generally speaking, you'll inform the `Game` of your choice via the `movePieceTo()` method, and the `Game` will make necessary changes to the location and status of `Pieces` (for you _and_ your opponent). 

### The Neighborhood Function

A _neighbourhood function_ is a simple function that returns set of the states that are considered 'adjacent' to your current state. These are the options your `Player` must consider during its turn.  An adjacent state can be obtained by making a single modification to the current state according to whatever rules are appropriate to your problem. So, for example, given any Pawn on the gameboard, it can move only two directions : forward left, or forward right (because Pawns only move forward, and all moves are diagonal). In theory you generally have two options, until you consider that your piece may be blocked from going one or both of those directions. 

As a simple algorithm to find options for your next move, your neighborhood function might do the following: 

1. For each piece, make a list of all possible moves (forward-left, forward-right, jump-left, jump-right, etc.)
2. Eliminate any non-viable option according to game rules. For example, a Pawn _can_ move left, but may be blocked and therefore this option would be eliminated. 

Now you have a ist of possible choices. But which choice is best?

### The Objective Function

An objective function indicates the 'quality/rank' of a given option, in terms of leading to the optimal outcome. Sometimes your objective function knows exactly which is best, but it often just an approximation.

For example, consider the case where you have two Pawns that can move forward by 1 square. In a sense, these options appear to have exactly the same value. But what if one of your Pawns is only a square away from the back row of your opponent, where it can become a King. In this case, moving that piece produces a result of higher value to the outcome of your game (since Kings are more flexible and thus a much greater threat than Pawns). Between these two move options, the "move to become a King" has greater value, and is therefore ranked higher by your objective function.

For the game of checkers, there are a few keys, each with their own desireability. Below are some examples:

1. move to back-row, get "kinged"  (best)
2. One ore more safe jumps over opponent(s) (landing somewhere you aren't at risk) 
3. Jumping (and caputuring opponent) but landing in a risky spot
3. Moving safely (not diagonally adjacent to opponent)
4. Moving diagonally adjacent to opponent (risk of capture)

### Indicating your decision

After considering all your available options for each of your remaining pieces, and optimizing the options according to the "value" of the outcome as determined by your objective function, you'll either have one "best" option, or multiple "equally good" moves. If you have a single "best" move, take it. Otherwise, either of the "good" moves will be fine. 

After careful evaluation of the current state of the `Board`, your checker-bot is required to indicate a decision. Do this by calling the `Game.movePieceTo(*Piece, Location)` method. 

If you are moving a piece, it is essential that you are moving a valid piece to a legally avalable location. If either condition is not met (and you get caught trying to cheat), your checker-bot will forfeit the game -- and will enter the "hall-of-shame!". :)

If you try to jump an opponent's `Piece`, it is essential that the `Piece` is valid, that you are jumping an diagonally adjacent piece of your opponent, and that you are landing on an unoccupied location. It's also worth noting that if you are jumping, it's possible to jump more than one `Piece` in a sequence. It is therefore allowable to call `Game.movePieceTo()` more than once (for your same `Piece`) in a single turn.


