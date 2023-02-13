# ChessGame
Chess game

The conventions to use this program are:
-input form: piece previousLocalization(a,b) nextLocalization(a,b)
-color: white -> lowerCaps, black -> AllCaps

FEATURES:
* preventing wrong moves
* preventing the wrong pieces from passing over other pieces
* preventing pieces from eating those of the same color and the enemy king
* pawn promotion
* reset option
* sandbox mode to debug, or to cheat
* check detection, and prevention of any move that doesn't solve it
* turn-taking
* deal with wrong inputs
* recognizes checkmates and stalemates


TO RUN
make
./chessMake
./chessMake P e2 e3