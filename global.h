#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>

extern const char emptySquare;
//1 means "Whites played. Next turn: black", and 2 means "Blacks played. Next turn: white"
extern const char whitesTurn;
extern const char blacksTurn;
extern const char* whitesTurnStatus;
extern const char* blacksTurnStatus;

extern const char blacksAreChecked;
extern const char whitesAreChecked;
	
extern const char whitePawn;
extern const char whiteTower;
extern const char whiteKnight;
extern const char whiteBishop;
extern const char whiteQueen;
extern const char whiteKing;

extern const char blackPawn;
extern const char blackTower;
extern const char blackKnight;
extern const char blackBishop;
extern const char blackQueen;
extern const char blackKing;

//ordered so that it will take less time to look for solutions: more pawns, the king is always present, the queen is valuable and carefully kept, etc.
extern char blackPieces[6];
extern char whitePieces[6];
// allies and enemy king
extern char whiteUntakablePieces[7];		
extern char blackUntakablePieces[7];

extern const char saveFileName[13]; 

extern const std::string codeSandbox;
extern const std::string codeReset;
extern const std::string codeShow;
extern const std::string debugModifier;
extern const std::string welcomeMessage;

#endif