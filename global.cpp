#include <string>
#include "global.h"
using namespace std;

const char emptySquare = ' ';
//1 means "Whites played. Next turn: black", and 2 means "Blacks played. Next turn: white"
const char whitesTurn = '1';
const char blacksTurn = '2';
const char* whitesTurnStatus = "White's turn";
const char* blacksTurnStatus = "Black's turn";

const char blacksAreChecked = 'c';
const char whitesAreChecked = 'C';
	
const char whitePawn = 'p';
const char whiteTower = 't';
const char whiteKnight = 'n';
const char whiteBishop = 'b';
const char whiteQueen = 'q';
const char whiteKing = 'k';

const char blackPawn = 'P';
const char blackTower = 'T';
const char blackKnight = 'N';
const char blackBishop = 'B';
const char blackQueen = 'Q';
const char blackKing = 'K';

//ordered so that it will take less time to look for solutions: more pawns, the king is always present, the queen is valuable and carefully kept, etc.
char blackPieces[6] = {blackPawn, blackKing, blackQueen, blackTower, blackKnight, blackBishop};
char whitePieces[6] = {whitePawn, whiteKing, whiteQueen, whiteTower, whiteKnight, whiteBishop};
// allies and enemy king
char whiteUntakablePieces[7] = {whitePawn, whiteTower, whiteKnight, whiteBishop, whiteQueen, whiteKing, blackKing};		
char blackUntakablePieces[7] = {blackPawn, blackTower, blackKnight, blackBishop, blackQueen, blackKing, whiteKing};

const char saveFileName[13] = "savefile.txt"; 

const string codeSandbox = "sandbox";
const string codeReset = "reset";
const string codeShow = "show";
const string debugModifier = "-debug";
const string welcomeMessage = "Welcome, the chess game will begin soon !\n\n";