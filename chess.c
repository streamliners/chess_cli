//chess game
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "moveValidation.h"
#include "checkValidation.h"

//functions declarations

int getXCoordinatesFromInput(char*);
int getYCoordinatesFromInput(char*);
void drawBoard(char[]);
int normalBody(char[], char*, int, int, int, int, FILE*);

//input form: piece previousLocalization(a,b) nextLocalization(a,b)
//color: white -> lowerCaps, black -> AllCaps

/* FEATURES:
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
*/

/* TODO
	use the ANSI characters for chess pieces
	correct the order
	fix the checkmate/check bugs
*/

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

char whiteUntakablePieces[7] = {whitePawn, whiteTower, whiteKnight, whiteBishop, whiteQueen, whiteKing, blackKing};		
char blackUntakablePieces[7] = {blackPawn, blackTower, blackKnight, blackBishop, blackQueen, blackKing, whiteKing};

//const int saveFileNameLength = 13;
const char saveFileName[13] = "savefile.txt"; 

const char codeSandbox[8] = "sandbox";
const char codeReset[6] = "reset";
const char codeShow[5] = "show";


int main(int argc, char* argv[]) {

	float startTime = (float)clock()/CLOCKS_PER_SEC;

	char piecePos[71] = "tnbqkbntpppppppp                                PPPPPPPPTNBQKBNT___1__";

	FILE *savefile = fopen(saveFileName, "r+");
	if (savefile == NULL) {
		//this occurs when it's a new game
		savefile = fopen(saveFileName, "w+");
		printf("Welcome, the chess game will begin soon !\n\n");
		drawBoard(piecePos);
		exit(0);
	} 
	
	//reset option (input format is "./a.out reset")
	if (argc == 2 && strcmp(argv[1], codeReset) == 0){
		char rm[17] = "rm ";
		system(strcat(rm, saveFileName));
		drawBoard(piecePos);
		return 0;
	}

	
	fread(piecePos, 1, 71, savefile);
	fclose(savefile);	

	
	printf("ARGC: %d\n", argc); 
	printf("ARGV[1]: %s\n", argv[1]); 
	
	
	
	//sandbox mode (input format is "./a.out sandbox piece nextPlace(a,b)")
	// makes a new piece appear at given coordinates
	if (argc == 4 && strcmp(argv[1], codeSandbox) == 0){
		char *currentPiece = argv[2];
		char *nextPlace = argv[3];
		int nextPlace1 = getXCoordinatesFromInput(argv[3]);
		int nextPlace2 = getYCoordinatesFromInput(argv[3]);
		int nextPlace3 = 64 - ((nextPlace2*8) - nextPlace1);
		piecePos[nextPlace3] = *currentPiece;
		drawBoard(piecePos);
	}


	// show mode
	// input format is "./a.out show"
	if (argc == 2 && strcmp(argv[1], codeShow)  == 0) {
		drawBoard(piecePos);
	}

	//normal mode (new input is piece VertiHori, e.g. T 8a 7a)
	// input format is "./chessMake p 6,7 6,5"
	if (argc >= 4 && strcmp(argv[1], codeSandbox) != 0){
		char *currentPiece = argv[1];
		char *previousPlace = argv[2];
		char *nextPlace = argv[3];

		//int previousPlace3 = 64 - ((previousPlace2*8) - previousPlace1);
		//int nextPlace3 = 64 - ((nextPlace2*8) - nextPlace1);
		
		normalBody(piecePos, currentPiece, getXCoordinatesFromInput(previousPlace), getYCoordinatesFromInput(previousPlace), getXCoordinatesFromInput(nextPlace), getYCoordinatesFromInput(nextPlace), savefile);
	}
	
	float	endTime = (float)clock()/CLOCKS_PER_SEC;
	printf("Time taken: %f\n\n", endTime - startTime);
}		

int getXCoordinatesFromInput(char* input) {
	if (*input == 'a') {
		return 0;
	} else if (*input == 'b') {
		return 1;
	} else if (*input == 'c') {
		return 2;
	} else if (*input == 'd') {
		return 3;
	} else if (*input == 'e') {
		return 4;
	} else if (*input == 'f') {
		return 5;
	} else if (*input == 'g') {
		return 6;
	} else if (*input == 'h') {
		return 7;
	} 
	
	//return (input[0]-'0') - 1;
}
	
int getYCoordinatesFromInput(char* input) {
	// 	return input[1]-'0';
	return input[1]-'0';
}	
		
		
int normalBody(char piecePos[], char* currentPiece, int previousPlace1, int previousPlace2, int nextPlace1, int nextPlace2, FILE* savefile) {		
	char previousState = piecePos[69];
	bool mustPrint = true;
	
	//allies and enemy kind
	//compute immunePieces	
	char whitePieces[6] = {whitePawn, whiteTower, whiteKnight, whiteBishop, whiteQueen, whiteKing};
	char blackPieces[6] = {blackPawn, blackTower, blackKnight, blackBishop, blackQueen, blackKing};

	//previousPlace1 is x coord, previousPlace2 is y coord, previousPlace3 is location in 2d array
	int previousPlace3 = 64 - ((previousPlace2*8) - previousPlace1);
	int nextPlace3 = 64 - ((nextPlace2*8) - nextPlace1);
	char untakablePieces[6];
	if (*currentPiece == whiteKing || *currentPiece == whiteKnight || *currentPiece == whiteQueen || *currentPiece == whiteBishop || *currentPiece == whiteTower || *currentPiece == whitePawn) {
		//untakablePieces = whiteUntakablePieces;
			
		printf("currentPiece: %c\n", *currentPiece);
		//printf("untakablePieces[0]: %s\n", *untakablePieces);
	
		if (0 != validateAllMoves(piecePos, whiteUntakablePieces, currentPiece, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint)) {
			return 1;
		}
	} else {
		//untakablePieces = blackUntakablePieces;
		
		printf("currentPiece: %c\n", *currentPiece);
		//printf("untakablePieces[0]: %s\n", *untakablePieces);
	
		if (0 != validateAllMoves(piecePos, blackUntakablePieces, currentPiece, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint)) {
			return 1;
		}
	}
			


	
	mainMoveAndPawnPromotion(piecePos, currentPiece, previousPlace3, nextPlace3);
	
	if (0 != processChecks(piecePos, previousState, mustPrint)) {
		return 1;
	}

	drawBoard(piecePos);



	//checkmate checking for blacks
	if (piecePos[69] == blacksAreChecked) {
		//ordered so that it will take less time to run: more pawns, the king is always present, the queen is valuable and carefully kept, etc.
		char arrayPieces[6] = {'p', 'k', 'q', 't', 'n', 'b'};
		isCheckmate(piecePos, arrayPieces, piecePos[69]);
	}

	//checkmate checking for whites
	else if (piecePos[69] == whitesAreChecked) {
		//ordered so that it will take less time to run: more pawns, the king is always present, the queen is valuable and carefully kept, etc.
		char arrayPieces[6] = {'P', 'K', 'Q', 'T', 'N', 'B'};
		isCheckmate(piecePos, arrayPieces, piecePos[69]);
	}

	//stalemate checking for the blacks
	else if (piecePos[69] != blacksAreChecked && piecePos[69] != whitesAreChecked && piecePos[67] == blacksTurn) {
		//ordered so that it will take less time to run: more pawns, the king is always present, the queen is valuable and carefully kept, etc.
		char arrayPieces[6] = {'p', 'k', 'q', 't', 'n', 'b'};
		isStalemate(piecePos, arrayPieces, piecePos[69]);
	}

	//stalemate checking for the whites
	else if (piecePos[69] != blacksAreChecked && piecePos[69] != whitesAreChecked && piecePos[67] == whitesTurn) {
		//ordered so that it will take less time to run: more pawns, the king is always present, the queen is valuable and carefully kept, etc.
		char arrayPieces[6] = {'P', 'K', 'Q', 'T', 'N', 'B'};
		isStalemate(piecePos, arrayPieces, piecePos[69]);
	}
	
	if (piecePos[67] == whitesTurn) {
		displayStatus(whitesTurnStatus);
	} else {
		displayStatus(blacksTurnStatus);
	}

	return 0;
}

void drawBoard(char piecePos[]) {
	for (int i = 0; i < 18; i++){
		if (i == 17){
			printf("\t    a   b   c   d   e   f   g   h  \n");
			continue;
		}
		else if ((i%2) == 0){
			printf("\t  ---------------------------------\n");
		}
		else if ((i%2) == 1){
			int first = 8*(i/2);
			printf("\t%d | %c | %c | %c | %c | %c | %c | %c | %c |\n", 8-i/2, piecePos[first], piecePos[first+1], piecePos[first+2], piecePos[first+3], piecePos[first+4], piecePos[first+5], piecePos[first+6], piecePos[first+7]);
		} 
	}

	if (piecePos[67] == whitesTurn){
		piecePos[67] = blacksTurn;
	} else if (piecePos[67] == blacksTurn){
		piecePos[67] = whitesTurn;
	}

	FILE* savefile2 = fopen(saveFileName, "w+");
	//fseek(savefile2, 0, SEEK_SET);
	fwrite(piecePos, 1, 71, savefile2);
	fclose(savefile2);

	//tmp
	printf("%c\n", piecePos[67]);
	printf("%c\n", piecePos[68]);
	printf("%c\n", piecePos[69]);
	//tmp
}