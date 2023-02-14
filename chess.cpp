#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "global.h"
#include "moveValidation.h"
#include "checkValidation.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

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

int main(int argc, char* argv[]) {

	float startTime = (float)clock()/CLOCKS_PER_SEC;
	char piecePos[71] = "tnbqkbntpppppppp                                PPPPPPPPTNBQKBNT___1__";

	FILE *savefile = fopen(saveFileName, "r+");
	if (savefile == NULL) {
		// this occurs when it's a new game
		savefile = fopen(saveFileName, "w+");
		cout << welcomeMessage;
		drawBoard(piecePos);
		exit(0);
	} 
	
	string argv1 = argv[1];
	// reset option (input format is "./chessMake reset")
	if (argc == 2 && argv1.compare(codeReset) == 0){
		char rm[17] = "rm ";
		system(strcat(rm, saveFileName));
		drawBoard(piecePos);
		return 0;
	}
	
	fread(piecePos, 1, 71, savefile);
	fclose(savefile);		

	// sandbox mode (input format is "./chessMake sandbox piece nextPlace(a,b)")
	// makes a new piece appear at given coordinates
	if (argc == 4 && argv1.compare(codeSandbox) == 0) {
		char *currentPiece = argv[2];
		int nextPlace1 = getXCoordinatesFromInput(argv[3]);
		int nextPlace2 = getYCoordinatesFromInput(argv[3]);
		int nextPlace3 = 64 - ((nextPlace2*8) - nextPlace1);
		piecePos[nextPlace3] = *currentPiece;
		drawBoard(piecePos);
		
	} else if (argc == 2 && argv1.compare(codeShow) == 0) {
		// show mode
		// input format is "./chessMake show"
		drawBoard(piecePos);
		
	} else if (argc >= 4 && argv1.compare(codeSandbox) != 0) {
		// normal mode (new input is like "T 8a 7a")
		// input format is "./chessMake p 6,7 6,5"
		char *currentPiece = argv[1];
		char *previousPlace = argv[2];
		char *nextPlace = argv[3];
		
		normalBody(piecePos, currentPiece, getXCoordinatesFromInput(previousPlace), getYCoordinatesFromInput(previousPlace), getXCoordinatesFromInput(nextPlace), getYCoordinatesFromInput(nextPlace), savefile);

		// debug option
		if (argc == 5 && string(argv[4]).compare(debugModifier) == 0) {
			if (piecePos[67] == whitesTurn) {
				displayStatus(whitesTurnStatus);
			} else {
				displayStatus(blacksTurnStatus);
			}
			
			float endTime = (float)clock()/CLOCKS_PER_SEC;
			cout << "Time taken: " << endTime - startTime << endl << endl;
		}
	}
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
	return 0;
}
	
int getYCoordinatesFromInput(char* input) {
	return input[1]-'0';
}

int normalBody(char piecePos[], char* currentPiece, int previousPlace1, int previousPlace2, int nextPlace1, int nextPlace2, FILE* savefile) {		
	char previousState = piecePos[69];
	bool mustPrint = true;

	//previousPlace1 is x coord, previousPlace2 is y coord, previousPlace3 is location in 2d array
	int previousPlace3 = 64 - ((previousPlace2*8) - previousPlace1);
	int nextPlace3 = 64 - ((nextPlace2*8) - nextPlace1);
	if (*currentPiece == whiteKing || *currentPiece == whiteKnight || *currentPiece == whiteQueen || *currentPiece == whiteBishop || *currentPiece == whiteTower || *currentPiece == whitePawn) {	
		if (0 != validateAllMoves(piecePos, whiteUntakablePieces, currentPiece, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint)) {
			return 1;
		}
	} else {
		if (0 != validateAllMoves(piecePos, blackUntakablePieces, currentPiece, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint)) {
			return 1;
		}
	}
	
	mainMoveAndPawnPromotion(piecePos, currentPiece, previousPlace3, nextPlace3);
	
	if (0 != processChecks(piecePos, previousState, mustPrint)) {
		return 1;
	}

	drawBoard(piecePos);

	if (piecePos[69] == blacksAreChecked) {
		//checkmate checking for blacks
		isCheckmate(piecePos, whitePieces, piecePos[69]);
	} else if (piecePos[69] == whitesAreChecked) {
		//checkmate checking for whites
		isCheckmate(piecePos, blackPieces, piecePos[69]);
	} else if (piecePos[69] != blacksAreChecked && piecePos[69] != whitesAreChecked && piecePos[67] == blacksTurn) {
		//stalemate checking for the blacks
		isStalemate(piecePos, whitePieces, piecePos[69]);
	} else if (piecePos[69] != blacksAreChecked && piecePos[69] != whitesAreChecked && piecePos[67] == whitesTurn) {
		//stalemate checking for the whites
		isStalemate(piecePos, blackPieces, piecePos[69]);
	}

	return 0;
}

void drawBoard(char piecePos[]) {
	cout << endl;
	for (int i = 0; i < 18; i++){
		if (i == 17){
			cout << "\t    a   b   c   d   e   f   g   h  " << endl;
			continue;
		}
		else if ((i%2) == 0){
			cout << ("\t  ---------------------------------") << endl;
		}
		else if ((i%2) == 1){
			int yCoordinate = 8-i/2;
			int first = 8*(i/2);
			cout << "\t" << yCoordinate << " | " << piecePos[first]  << " | " << piecePos[first + 1] << " | " << piecePos[first + 2] << " | " << piecePos[first + 3] 
				<< " | " << piecePos[first + 4] << " | " << piecePos[first + 5] << " | " << piecePos[first + 6] << " | " << piecePos[first + 7] << " |" << endl;
		} 
	}
	cout << endl;

	if (piecePos[67] == whitesTurn){
		piecePos[67] = blacksTurn;
	} else if (piecePos[67] == blacksTurn){
		piecePos[67] = whitesTurn;
	}

	FILE* savefile2 = fopen(saveFileName, "w+");
	fwrite(piecePos, 1, 71, savefile2);
	fclose(savefile2);
}