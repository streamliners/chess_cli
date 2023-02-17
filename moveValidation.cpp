#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "global.h"
#include "moveValidation.h"
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
					
const char *wrongCoordinatesException = "The coordinates are wrong.";			
const char *invalidPieceException = "Such a piece does not exist.";					
const char *movingNothingException = "You are not moving anything. Try again.";							
const char *pieceIsntHereException = "You are moving a piece that is not here.";								
const char *wrongTurnBlackException = "This is not your turn. This is the whites' turn.";								
const char *wrongTurnWhiteException = "This is not your turn. This is the blacks' turn.";	
							
const char *towerMoveException = "Towers cannot move like this.";					
const char *towerPassException = "Towers cannot pass over other pieces.";								
const char *towerTakeException = "This tower cannot take this piece.";	
								
const char *bishopMoveException = "Bishops cannot move like this.";
const char *bishopPassException = "Bishops cannot pass over other pieces.";
const char *bishopTakeException = "This bishop cannot take this piece.";
									
const char *kingMoveException = "The king cannot move like this.";								
const char *kingTakeException = "This king cannot take this piece.";
				
const char *knightMoveException = "Knights cannot move like this.";									
const char *knightTakeException = "This knight cannot take this piece.";
										
const char *queenMoveException = "Queens cannot move like this.";					
const char *queenPassException = "Queens cannot pass over other pieces.";
const char *queenTakeException = "This queen cannot take this piece.";							
	
const char *pawnMoveException = "Pawns cannot move like this.";
const char *pawnTakeException = "This pawn cannot take this piece.";


int throwInvalidMoveException(const char* message, bool mustPrint) {
	if (mustPrint == true) {
		cout << "INVALID: " << message << endl;
	}
	return 1;
}
		
int isTakingAnUntakablePiece(char targetPiece, char untakablePieces[], int size, const char* message, bool mustPrint) {
	for (int i = 0; i < size; i++) {
		if (targetPiece == untakablePieces[i]) {
			return throwInvalidMoveException(message, mustPrint);
		}
	}
	return 0;
}


int validateTowerMove(char piecePos[], char untakablePieces[], int previousPlace1, int previousPlace2, int previousPlace3, int nextPlace1, int nextPlace2, int nextPlace3, bool mustPrint) {
		
	if (previousPlace1 != nextPlace1 && previousPlace2 != nextPlace2) {
		return throwInvalidMoveException(towerMoveException, mustPrint);
	}
	
	if (previousPlace1 > nextPlace1) {
		int var = previousPlace1 - nextPlace1;
		for (int i = 1; i < var; i++) {
			if (piecePos[previousPlace3 - i] != emptySquare) {
				return throwInvalidMoveException(towerPassException, mustPrint);
			}
		} 
	}
	if (previousPlace1 < nextPlace1) {
		int var = nextPlace1 - previousPlace1;
		for (int i = 1; i < var; i++) {
			if (piecePos[previousPlace3 + i] != emptySquare) {
				return throwInvalidMoveException(towerPassException, mustPrint);
			}
		} 
	}
	if (previousPlace2 > nextPlace2) {
		int var = previousPlace2 - nextPlace2;
		for (int i = 1; i < var; i++) {
			if (piecePos[previousPlace3 + 8*i] != emptySquare) {
				return throwInvalidMoveException(towerPassException, mustPrint);
			}
		} 
	}
	if (previousPlace2 < nextPlace2) {
		int var = nextPlace2 - previousPlace2;
		for (int i = 1; i < var; i++) {
			if (piecePos[previousPlace3 - 8*i] != emptySquare) {
				return throwInvalidMoveException(towerPassException, mustPrint);
			}
		} 
	}

	//preventing the tower from eating its own pieces and the enemy king
	return isTakingAnUntakablePiece(piecePos[nextPlace3], untakablePieces, 7, towerTakeException, mustPrint);
}


int validateBishopMove(char piecePos[], char untakablePieces[], int previousPlace1, int previousPlace2, int previousPlace3, int nextPlace1, int nextPlace2, int nextPlace3, bool mustPrint) {
	if (fabs(previousPlace1 - nextPlace1) != fabs(previousPlace2 - nextPlace2)) {
		return throwInvalidMoveException(bishopMoveException, mustPrint);
	}
	//down, left
	if (previousPlace1 > nextPlace1 && previousPlace2 > nextPlace2) {
		int var = previousPlace1 - nextPlace1;
		for (int i = 1; i < var; i++) {
			if (piecePos[previousPlace3 + 7*i] != emptySquare) {
				return throwInvalidMoveException(bishopPassException, mustPrint);
			}
		} 
	}
	//up, left
	if (previousPlace1 > nextPlace1 && previousPlace2 < nextPlace2) {
		int var = previousPlace1 - nextPlace1;
		for (int i = 1; i < var; i++) {
			if (piecePos[previousPlace3 - 9*i] != emptySquare) {
				return throwInvalidMoveException(bishopPassException, mustPrint);
			}
		} 
	}
	//down, right
	if (previousPlace1 < nextPlace1 && previousPlace2 > nextPlace2) {
		int var = nextPlace1 - previousPlace1;
		for (int i = 1; i < var; i++) {
			if (piecePos[previousPlace3 + 9*i] != emptySquare) {
				return throwInvalidMoveException(bishopPassException, mustPrint);
			}
		} 
	}
	//up, right
	if (previousPlace1 < nextPlace1 && previousPlace2 < nextPlace2) {
		int var = nextPlace1 - previousPlace1;
		for (int i = 1; i < var; i++) {
			if (piecePos[previousPlace3 - 7*i] != emptySquare) {
				return throwInvalidMoveException(bishopPassException, mustPrint);
			}
		} 
	}
	//preventing the bishop from eating its own pieces and the enemy king
	return isTakingAnUntakablePiece(piecePos[nextPlace3], untakablePieces, 7, bishopTakeException, mustPrint);
}


int validateKingMove(char piecePos[], char untakablePieces[], char currentPiece, int previousPlace1, int previousPlace2, int previousPlace3, int nextPlace1, int nextPlace2, int nextPlace3, bool mustPrint) {
//if (fabs(previousPlace1 - nextPlace1) != 1 || fabs(previousPlace2 - nextPlace2) != 1){
	if (!( (fabs(previousPlace1 - nextPlace1) == 1 && fabs(previousPlace2 - nextPlace2) == 1) 
		|| (fabs(previousPlace1 - nextPlace1) == 0 && fabs(previousPlace2 - nextPlace2) == 1) 
		|| (fabs(previousPlace1 - nextPlace1) == 1 && fabs(previousPlace2 - nextPlace2) == 0)))  {			
		return throwInvalidMoveException(kingMoveException, mustPrint);
	}

	char* enemyKing;
	char* enemyQueen;
	char* enemyBishop;
	char* enemyKnight;
	char* enemyTower;

	if (currentPiece == whiteKing) {
		*enemyKing = blackKing;
		*enemyQueen = blackQueen;
		*enemyBishop = blackBishop;
		*enemyKnight = blackKnight;
		*enemyTower = blackTower;
	} else {
		*enemyKing = whiteKing;
		*enemyQueen = whiteQueen;
		*enemyBishop = whiteBishop;
		*enemyKnight = whiteKnight;
		*enemyTower = whiteTower;
	}

	//avoid moving in the reach of the other king
	if (nextPlace1 > 0) {
		if (piecePos[nextPlace3 - 1] == *enemyKing) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		} 
		if (nextPlace2 < 8 && piecePos[nextPlace3 - 9] == *enemyKing) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		if (nextPlace2 > 1 && piecePos[nextPlace3 + 7] == *enemyKing) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}	
	} 
	if (nextPlace1 < 7) {
		if (piecePos[nextPlace3 + 1] == *enemyKing) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		if (nextPlace2 < 8 && piecePos[nextPlace3 - 7] == *enemyKing) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		if (nextPlace2 > 1 && piecePos[nextPlace3 + 9] == *enemyKing) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
	}

	if (nextPlace2 < 8 && piecePos[nextPlace3 - 8] == *enemyKing) {
		return throwInvalidMoveException(kingMoveException, mustPrint);
	}
	if (nextPlace2 > 1 && piecePos[nextPlace3 + 8] == *enemyKing) {
		return throwInvalidMoveException(kingMoveException, mustPrint);
	}
	

	//avoid moving in the reach of the other pawns
	if (currentPiece == whiteKing){
		//left side
		if (nextPlace1 > 0 && nextPlace2 > 1 && piecePos[nextPlace3 + 7] == blackPawn) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		//right side
		if (nextPlace1 < 7 && nextPlace2 > 1 && piecePos[nextPlace3 + 9] == blackPawn) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
	}
	if (currentPiece == blackKing){
		//left side
		if (nextPlace1 > 0 && nextPlace2 < 8 && piecePos[nextPlace3 - 9] == whitePawn) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		//right side
		if (nextPlace1 < 7 && nextPlace2 < 8 && piecePos[nextPlace3 - 7] == whitePawn){ 
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
	}

	//avoid moving in the reach of the other knights
	if (nextPlace2 < 7) {
		if (nextPlace1 != 0 && piecePos[nextPlace3 - 17] == *enemyKnight) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		if (nextPlace1 != 7 && piecePos[nextPlace3 - 15] == *enemyKnight) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
	}
	if (nextPlace2 < 8) {
		if (nextPlace1 > 1 && piecePos[nextPlace3 - 10] == *enemyKnight) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		if (nextPlace1 < 6 && piecePos[nextPlace3 - 6] == *enemyKnight) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
	}
	if (nextPlace2 > 1) {
		if (nextPlace1 > 1 && piecePos[nextPlace3 + 6] == *enemyKnight) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		if (nextPlace1 < 6 && piecePos[nextPlace3 + 10] == *enemyKnight) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
	}
	if (nextPlace2 > 2) {
		if (nextPlace1 > 1 && piecePos[nextPlace3 + 15] == *enemyKnight) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
		if (nextPlace1 < 7 && piecePos[nextPlace3 + 17] == *enemyKnight) {
			return throwInvalidMoveException(kingMoveException, mustPrint);
		}
	}


	//avoid moving in the reach of the other bishops and the queen diagonally
	//up, right diagonal 
	for (int i = 1; i < 8 - nextPlace1; i++) {
		char currentSquare = piecePos[nextPlace3 - 7 * i];
		if (currentSquare != emptySquare) {
			if (currentSquare != *enemyBishop && currentSquare != *enemyQueen) {
				break;
			} else {
				return throwInvalidMoveException(kingMoveException, mustPrint);
			}
		}
	}
	//down, left diagonal 
	for (int i = 1; i < nextPlace1 + 1; i++) {
		char currentSquare = piecePos[nextPlace3 + 7 * i];
		if (currentSquare != emptySquare) {
			if (currentSquare != *enemyBishop && currentSquare != *enemyQueen) {
				break;
			} else {
				return throwInvalidMoveException(kingMoveException, mustPrint);
			}
		}
	}
	//down, right diagonal
	for (int i = 1; i < 8 - nextPlace1; i++) {
		char currentSquare = piecePos[nextPlace3 + 9 * i];
		if (currentSquare != emptySquare) {
			if (currentSquare != *enemyBishop && currentSquare != *enemyQueen) {
				break;
			} else {
				return throwInvalidMoveException(kingMoveException, mustPrint);
			}
		}
	}
	//up, left diagonal 
	for (int i = 1; i < nextPlace1 + 1; i++) {
		char currentSquare = piecePos[nextPlace3 - 9 * i];
		if (currentSquare != emptySquare) {
			if (currentSquare != *enemyBishop && currentSquare != *enemyQueen) {
				break;
			} else {
				return throwInvalidMoveException(kingMoveException, mustPrint);
			}
		}
	}


	//avoid moving in the reach of the towers and queens
	//right
	for (int i = 1; i < 8 - nextPlace1; i++) {
		char currentSquare = piecePos[nextPlace3 + i];
		if (currentSquare != emptySquare) {
			if (currentSquare != *enemyTower && currentSquare != *enemyQueen) {
				break;
			} else {
				return throwInvalidMoveException(kingMoveException, mustPrint);
			}
		}
	}
	//left
	for (int i = 1; i < nextPlace1 + 1; i++) {
		char currentSquare = piecePos[nextPlace3 - i];
		if (currentSquare != emptySquare) {
			if (currentSquare != *enemyTower && currentSquare != *enemyQueen) {
				break;
			} else {
				return throwInvalidMoveException(kingMoveException, mustPrint);
			}
		}
	}
	//up
	for (int i = 1; i < 9 - nextPlace2; i++) {
		char currentSquare = piecePos[nextPlace3 - 8 * i];
		if (currentSquare != emptySquare) {
			if (currentSquare != *enemyTower && currentSquare != *enemyQueen) {
				break;
			} else {
				return throwInvalidMoveException(kingMoveException, mustPrint);
			}
		}
	}
	//down
	for (int i = 1; i < nextPlace2; i++) {
		char currentSquare = piecePos[nextPlace3 + 8 * i];
		if (currentSquare != emptySquare) {
			if (currentSquare != *enemyTower && currentSquare != *enemyQueen) {
				break;
			} else {
				return throwInvalidMoveException(kingMoveException, mustPrint);
			}
		}
	}
	
	//preventing the king from eating its own pieces and the enemy king
	return isTakingAnUntakablePiece(piecePos[nextPlace3], untakablePieces, 7, kingTakeException, mustPrint);
}

int validateKnightMove(char piecePos[], char untakablePieces[], int previousPlace1, int previousPlace2, int nextPlace1, int nextPlace2, int nextPlace3, bool mustPrint) {
	if (fabs(previousPlace1 - nextPlace1) > 2) {
		return throwInvalidMoveException(knightMoveException, mustPrint);
	}
	if (fabs(previousPlace2 - nextPlace2) > 2) {
		return throwInvalidMoveException(knightMoveException, mustPrint);
	} else if (fabs(previousPlace1 - nextPlace1) == 2) { 
		if (fabs(previousPlace2 - nextPlace2) != 1) {
			return throwInvalidMoveException(knightMoveException, mustPrint);
		}
	} else if (fabs(previousPlace2 - nextPlace2) == 2) {
		if (fabs(previousPlace1 - nextPlace1) != 1) {
			return throwInvalidMoveException(knightMoveException, mustPrint);
		}
	} else if (fabs(previousPlace1 - nextPlace1) == 1 && fabs(previousPlace2 - nextPlace2) == 1){
		return throwInvalidMoveException(knightMoveException, mustPrint);
	}
	
	//preventing the knight from eating its own pieces and the enemy king
	return isTakingAnUntakablePiece(piecePos[nextPlace3], untakablePieces, 7, knightTakeException, mustPrint);
}

int validateQueenMove(char piecePos[], char untakablePieces[], int previousPlace1, int previousPlace2, int previousPlace3, int nextPlace1, int nextPlace2, int nextPlace3, bool mustPrint) {
	if ((previousPlace1 != nextPlace1 && previousPlace2 != nextPlace2) && (fabs(previousPlace1 - nextPlace1) != fabs(previousPlace2 - nextPlace2))){
		return throwInvalidMoveException(queenMoveException, mustPrint);
	}

	//tower condition
	if (previousPlace1 == nextPlace1 || previousPlace2 == nextPlace2){
		return validateTowerMove(piecePos, untakablePieces, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
	//bishop condition
	else if (fabs(previousPlace1 - nextPlace1) == fabs(previousPlace2 - nextPlace2)){
		return validateBishopMove(piecePos, untakablePieces, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
	
	//preventing the queen from eating its own pieces and the enemy king
	return isTakingAnUntakablePiece(piecePos[nextPlace3], untakablePieces, 7, queenTakeException, mustPrint);
}

int validateWhitePawnMove(char piecePos[], char untakablePieces[], int previousPlace1, int previousPlace2, int previousPlace3, int nextPlace1, int nextPlace2, int nextPlace3, bool mustPrint) {
	if ((nextPlace3 - previousPlace3 == 7 || nextPlace3 - previousPlace3 == 9) && piecePos[nextPlace3] != emptySquare){
		//does nothing, this is correct. This is when a pawn takes another piece.	
	} else if (nextPlace3 - previousPlace3 == 16 && (previousPlace3 >= 8 && previousPlace3 <= 15) && piecePos[previousPlace3 + 8] == emptySquare && piecePos[previousPlace3 + 16] == emptySquare){
		//does nothing, this is correct. This is when a pawn advances by two spots.
	} else if ((nextPlace3 - previousPlace3) != 8){
		return throwInvalidMoveException(pawnMoveException, mustPrint);
	} else if (piecePos[nextPlace3] != emptySquare){
		return throwInvalidMoveException(pawnTakeException, mustPrint);
	}

	//preventing the pawn from eating its own pieces and the enemy king
	return isTakingAnUntakablePiece(piecePos[nextPlace3], untakablePieces, 7, pawnTakeException, mustPrint);
}

int validateBlackPawnMove(char piecePos[], char untakablePieces[], int previousPlace1, int previousPlace2, int previousPlace3, int nextPlace1, int nextPlace2, int nextPlace3, bool mustPrint) {
	if ((nextPlace3 - previousPlace3 == -7 || nextPlace3 - previousPlace3 == -9) && piecePos[nextPlace3] != emptySquare){
		//does nothing, this is correct	
	} else if (nextPlace3 - previousPlace3 == -16 && (previousPlace3 >= 48 && previousPlace3 <= 55) && piecePos[previousPlace3 - 8] == emptySquare && piecePos[previousPlace3 - 16] == emptySquare) {
		//does nothing, this is correct. This is when a pawn advances by two spots.
	} else if ((nextPlace3 - previousPlace3) != -8) {
		return throwInvalidMoveException(pawnMoveException, mustPrint);
	} else if (piecePos[nextPlace3] != emptySquare) {
		return throwInvalidMoveException(pawnTakeException, mustPrint);
	}

	//preventing the pawn from eating its own pieces and the enemy king
	return isTakingAnUntakablePiece(piecePos[nextPlace3], untakablePieces, 7, pawnTakeException, mustPrint);
}

int validateAllMoves(char piecePos[], char untakablePieces[], char currentPiece, int previousPlace1, int previousPlace2, int previousPlace3, int nextPlace1, int nextPlace2, int nextPlace3, bool mustPrint) {
			
	//potentially, rework the order if the checkmate is too long OKAY ?
	//preventing wrong coordinates from being acted on
	if (previousPlace3 > 63 || nextPlace3 > 63 || previousPlace1 > 7 || previousPlace1 < 0 || previousPlace2 > 8 || previousPlace2 < 1 || nextPlace1 > 7 || nextPlace1 < 0 || nextPlace2 > 8 || nextPlace2 < 1){
		return throwInvalidMoveException(wrongCoordinatesException, mustPrint);
	}
	//preventing inexisting pieces from being entered in the game
	if (!(currentPiece == whiteKing || currentPiece == whiteKnight || currentPiece == whiteQueen || currentPiece == whiteBishop || currentPiece == whiteTower || currentPiece == whitePawn || currentPiece == blackKing || currentPiece == blackKnight || currentPiece == blackQueen || currentPiece == blackBishop || currentPiece == blackTower || currentPiece == blackPawn)){
		return throwInvalidMoveException(invalidPieceException, mustPrint);
	}

	//preventing the case where the coordinates for the previous and next locations are the same
	if (previousPlace1 == nextPlace1 && previousPlace2 == nextPlace2){
		return throwInvalidMoveException(movingNothingException, mustPrint);
	}
	//preventing the appearance of inexisting pieces
	if (piecePos[previousPlace3] != currentPiece){
		return throwInvalidMoveException(pieceIsntHereException, mustPrint);
	}		

	//turn-taking, prevents using the wrong color at a given time
	if (piecePos[67] == whitesTurn && (currentPiece == whiteKing || currentPiece == whiteKnight || currentPiece == whiteQueen || currentPiece == whiteBishop || currentPiece == whiteTower || currentPiece == whitePawn)){
		return throwInvalidMoveException(wrongTurnBlackException, mustPrint);
	}
	if (piecePos[67] == blacksTurn && (currentPiece == blackKing || currentPiece == blackKnight || currentPiece == blackQueen || currentPiece == blackBishop || currentPiece == blackTower || currentPiece == blackPawn)){
		return throwInvalidMoveException(wrongTurnWhiteException, mustPrint);
	}

		
	//block illegal moves
	//for towers
	if (currentPiece == whiteTower || currentPiece == blackTower){
		return validateTowerMove(piecePos, untakablePieces, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
	
	//for bishops
	else if (currentPiece == whiteBishop || currentPiece == blackBishop){
		return validateBishopMove(piecePos, untakablePieces, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
		
	//for the kings
	else if (currentPiece == whiteKing || currentPiece == blackKing){
		return validateKingMove(piecePos, untakablePieces, currentPiece, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
		
	//for the knights
	else if (currentPiece == whiteKnight || currentPiece == blackKnight){
		return validateKnightMove(piecePos, untakablePieces, previousPlace1, previousPlace2, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
		
	//for the queens
	else if (currentPiece == whiteQueen || currentPiece == blackQueen){
		return validateQueenMove(piecePos, untakablePieces, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
		
	//for the white pawns
	else if (currentPiece == whitePawn){
		return validateWhitePawnMove(piecePos, untakablePieces, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
		
	//for the black pawns
	else if (currentPiece == blackPawn) {
		return validateBlackPawnMove(piecePos, untakablePieces, previousPlace1, previousPlace2, previousPlace3, nextPlace1, nextPlace2, nextPlace3, mustPrint);
	}
		
	return 0;
}