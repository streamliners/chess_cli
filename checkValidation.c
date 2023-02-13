#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "moveValidation.h"
#include "checkValidation.h"

//verify if useless imports among these
extern const char emptySquare;
extern const char whitesTurn ;
extern const char blacksTurn;
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



extern char whiteUntakablePieces[];
extern char blackUntakablePieces[] ;


const char *blackCheckedException = "The black king is still checked.";
const char *whiteCheckedException = "The white king is still checked.";

const char *statusChecked = "Check.";
const char *statusCheckmate = "Checkmate!";
const char *statusStalemate = "Stalemate!";	


void displayStatus(const char* message) {
	printf("%s\n\n", message);
} 

void blackGetsChecked(const char* statusChecked, char piecePos[]) {
	displayStatus(statusChecked);
	piecePos[69] = blacksAreChecked;
}

void whiteGetsChecked(const char* statusChecked, char piecePos[]) {
	displayStatus(statusChecked);
	piecePos[69] = whitesAreChecked;
}

int areCheckedNextTurn(char piecePos[], int positionKing1, int positionKing2, int positionKing3, char enemyPawn, char enemyKnight, char enemyBishop, char enemyTower, char enemyQueen) {
	//is it exposed to the pawns ?
	//left side
	if (positionKing1 > 0 && positionKing2 < 8 && piecePos[positionKing3 - 9] == enemyPawn) {
		return 1;
	}
	//right side
	if (positionKing1 < 7 && positionKing2 < 8 && piecePos[positionKing3 - 7] == enemyPawn) {
		return 1;
	}

	//is it exposed to the knights ?
	if (positionKing2 < 7) {
		if (positionKing1 != 0 && piecePos[positionKing3 - 17] == enemyKnight) {	
			return 1;
		} else if (positionKing1 != 7 && piecePos[positionKing3 - 15] == enemyKnight) {
			return 1;
		}
	}
	if (positionKing2 < 8) {
		if (positionKing1 > 1 && piecePos[positionKing3 - 10] == enemyKnight) {	
			return 1;
		} else if (positionKing1 < 6 && piecePos[positionKing3 - 6] == enemyKnight) {
			return 1;
		}
	}
	if (positionKing2 > 1){
		if (positionKing1 > 1 && piecePos[positionKing3 + 6] == enemyKnight) {
			return 1;
		} else if (positionKing1 < 6 && piecePos[positionKing3 + 10] == enemyKnight) {
			return 1;
		}
	}
	if (positionKing2 > 2) {
		if (positionKing1 > 1 && piecePos[positionKing3 + 15] == enemyKnight) {
			return 1;
		} else if (positionKing1 < 7 && piecePos[positionKing3 + 17] == enemyKnight) {
			return 1;
		}
	}
	//is it exposed to bishops or diagonally to the queen ?
	//up, right diagonal 
	for (int i = 1; i < 8 - positionKing1; i++) {
		char square = piecePos[positionKing3 - 7 * i];
		if (square != emptySquare) {
			if (square == enemyBishop || square == enemyQueen) {
				return 1;
			} else {
				break;
			}
		}
	}
	//down, left diagonal 
	for (int i = 1; i < positionKing1 + 1; i++){
		char square = piecePos[positionKing3 + 7 * i];
		if (square != emptySquare) {
			if (square == enemyBishop || square == enemyQueen) {
				return 1;
			} else {
				break;
			}
		}
	}
	//down, right diagonal
	for (int i = 1; i < 8 - positionKing1; i++) {
		char square = piecePos[positionKing3 + 9 * i];
		if (square != emptySquare) {
			if (square == enemyBishop || square == enemyQueen) {
				return 1;
			} else {
				break;
			}
		}
	}
	//up, left diagonal 
	for (int i = 1; i < positionKing1 + 1; i++) {
		char square = piecePos[positionKing3 - 9 * i];
		if (square != emptySquare) {
			if (square == enemyBishop || square == enemyQueen) {
				return 1;
			} else {
				break;
			}
		}
	}

	//is it exposed to towers or to the queen ?
	//right
	for (int i = 1; i < 8 - positionKing1; i++) {
		char square = piecePos[positionKing3 + i];
		if (square != emptySquare) {
			if (square == enemyTower || square == enemyQueen) {
				return 1;
			} else {
				break;
			}
		}
	}
	//left
	for (int i = 1; i < positionKing1 + 1; i++) {
		char square = piecePos[positionKing3 - i];
		if (square != emptySquare) {
			if (square == enemyTower || square == enemyQueen) {
				return 1;
			} else {
				break;
			}
		}
	}
	//up
	for (int i = 1; i < 9 - positionKing2; i++) {
		char square = piecePos[positionKing3  - 8 * i];
		if (square != emptySquare) {
			if (square == enemyTower || square == enemyQueen) {
				return 1;
			} else {
				break;
			}
		}
	}
	//down
	for (int i = 1; i < positionKing2; i++) {
		char square = piecePos[positionKing3  + 8 * i];
		if (square != emptySquare) {
			if (square == enemyTower || square == enemyQueen) {
				return 1;
			} else {
				break;
			}
		}
	}
	return 0;	
}

int processChecks(char piecePos[], char previousState, bool mustPrint) {
	//implement check condition here
	//c -> the blacks are checked, C -> the whites are checked
	char* p = strstr(piecePos, "k");
	int positionKing3 = strlen(piecePos) - strlen(p);
	int positionKing1 = positionKing3%8;
	int positionKing2 = 8 - positionKing3/8;
		
	int whitesCheckedNextTurn = areCheckedNextTurn(piecePos, positionKing1, positionKing2, positionKing3, blackPawn, blackKnight, blackBishop, blackTower, blackQueen);
	//is it exposed to the pawns ?
	
	//same but for K now
	int blacksCheckedNextTurn = 0;
	if (whitesCheckedNextTurn != 1) {
		p = strstr(piecePos, "K");
		positionKing3 = strlen(piecePos) - strlen(p);
		positionKing1 = positionKing3%8;
		positionKing2 = 8 - positionKing3/8;
		
		blacksCheckedNextTurn = areCheckedNextTurn(piecePos, positionKing1, positionKing2, positionKing3, whitePawn, whiteKnight, whiteBishop, whiteTower, whiteQueen);
	}

   	for(int loop = 0; loop < 69; loop++)
     		 printf("%c ", piecePos[loop]);
	printf("\npreviousState: %c\n", previousState);
	printf("previousState: %c\n", previousState);
	printf("whitesCheckedNextTurn: %d\n", whitesCheckedNextTurn);
	printf("blacksCheckedNextTurn: %d\n", blacksCheckedNextTurn);
		
	if (whitesCheckedNextTurn == 1) {
		if (previousState == blacksAreChecked) {
			return throwInvalidMoveException(whiteCheckedException, mustPrint);
		} else {
			// means C -> c or normal -> c
			displayStatus(statusChecked);
			piecePos[69] = blacksAreChecked;
			return 0;
		}
	} else if (blacksCheckedNextTurn == 1) {
		if (previousState == whitesAreChecked) {
			return throwInvalidMoveException(blackCheckedException, mustPrint);
		} else {
			// means c -> C  or normal -> C
			displayStatus(statusChecked);
			piecePos[69] = whitesAreChecked;
			return 0;
		}
	} else {
		// means no check whatsoever
		piecePos[69] = '_';
		return 0;
	}
}



void mainMoveAndPawnPromotion(char piecePos[], char* currentPiece, int previousPlace3, int nextPlace3) {
	//pawn promotions
	if (*currentPiece == whitePawn) {
		// promote to a queen
		if (nextPlace3 >= 56 && nextPlace3 <= 63){
			*currentPiece = whiteQueen;
		}
	} else if (*currentPiece == blackPawn) {
		// promote to a queen
		if (nextPlace3 >= 0 && nextPlace3 <= 7) {
			*currentPiece = blackQueen;
		}
	}

	//do the move		
	piecePos[previousPlace3] = emptySquare;
	piecePos[nextPlace3] = *currentPiece;
}

int foundSolution(char piecePos[], char* currentPiece, char currentState, int previousPlace1, int previousPlace2, int nextPlace1, int nextPlace2) {
	bool mustPrint = false;
	
	int previousPlace3 = 64 - ((previousPlace2*8) - previousPlace1);
	int nextPlace3 = 64 - ((nextPlace2*8) - nextPlace1);
	char* untakablePieces[7];
	if (*currentPiece == whiteKing || *currentPiece == whiteKnight || *currentPiece == whiteQueen || *currentPiece == whiteBishop || *currentPiece == whiteTower || *currentPiece == whitePawn) {
		//*untakablePieces = whiteUntakablePieces;
		if (0 != validateAllMoves(piecePos, whiteUntakablePieces, currentPiece, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint)) {
			return 1;
		}
	} else {
		//*untakablePieces = blackUntakablePieces;
		if (0 != validateAllMoves(piecePos, blackUntakablePieces, currentPiece, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint)) {
			return 1;
		}
	}
	
	mainMoveAndPawnPromotion(piecePos, currentPiece, previousPlace3, nextPlace3);
	
	//change from previousState to 69
	//printf("previousState: %c\n", previousState);
	//printf("piecePos[69]: %c\n", piecePos[69]);
	if (currentState != '_' && 0 != processChecks(piecePos, piecePos[69], mustPrint) && currentState == piecePos[69]) {
		return 1;
	}
	
	// not in main, new functionality
	//printf("previousState: %c\n", previousState);
	//printf("piecePos[69]: %c\n", piecePos[69]);
	/*if (previousState != '_' && previousState == piecePos[69]) {
		return 1;
	}*/
	return 0;
}

char getInputFromXCoordinates(int coordinate) {
	if (coordinate == 0) {
		return  'a';
	} else if (coordinate == 1) {
		return 'b';
	} else if (coordinate == 2) {
		return 'c';
	} else if (coordinate == 3) {
		return 'd';
	} else if (coordinate == 4) {
		return 'e';
	} else if (coordinate == 5) {
		return 'f';
	} else if (coordinate == 6) {
		return 'g';
	} else if (coordinate == 7) {
		return 'h';
	} 
}

int isCheckmate(char piecePos[], char arrayPieces[], char previousState) {
	for (int i = 0; i < 6; i++) {
		//this serves to make the code faster: if there is no x pieces, it is no use trying to loop through every case to find it
		char string1[2] = "G";
		string1[0] = arrayPieces[i];
		char *p = strstr(piecePos, string1);
		if (p == NULL) {
			continue;
		}
		for (int j = 1; j < 9; j++) {
			for (int k = 1; k < 9; k++) {
				for (int o = 1; o < 9; o++) {
					for (int q = 1; q < 9; q++) {	
						char newPiecePos[71];
						strcpy(newPiecePos, piecePos);
						char* currentPiece = arrayPieces;
						if (0 == foundSolution(piecePos, currentPiece, previousState, j, k, o, q))  {
							printf("%c %c%d %c%d is a possible solution.\n", arrayPieces[i], getInputFromXCoordinates(j), k, getInputFromXCoordinates(o), q);
							return 0;
						}
					}		
				}

			}
		}	
	}

	displayStatus(statusCheckmate);
	return 1;
}

int isStalemate(char piecePos[], char arrayPieces[], char previousState) {
	for (int i = 0; i < 6; i++) {
		//this serves to make the code faster: if there is no x pieces, it is no use trying to loop through every case to find it
		char string1[2] = "G";
		string1[0] = arrayPieces[i];
		char *p = strstr(piecePos, string1);
		if (p == NULL) {
			continue;
		}
		for (int j = 1; j < 9; j++) {
			for (int k = 1; k < 9; k++) {
				for (int o = 1; o < 9; o++) {
					for (int q = 1; q < 9; q++){	
						char newPiecePos[71];
						strcpy(newPiecePos, piecePos);
						char* currentPiece = arrayPieces;
						if (0 == foundSolution(piecePos, currentPiece, previousState, j, k, o, q))  {
							return 0;
						}
					}		
				}

			}
		}	
	}
	
	displayStatus(statusStalemate);
	return 1;
}