#ifndef MOVE_VALIDATION_H
#define MOVE_VALIDATION_H

int throwInvalidMoveException(const char*, bool);
int isTakingAnUntakablePiece(char, char[], int, const char*, bool);
int validateTowerMove(char[], char[], int, int, int, int, int, int, bool);
int validateBishopMove(char[], char[], int, int, int, int, int, int, bool);
int validateKingMove(char[], char[], char*, int, int, int, int, int, int, bool);
int validateKnightMove(char[], char[], int, int, int, int, int, bool);
int validateQueenMove(char[], char[], int, int, int, int, int, int, bool);
int validateWhitePawnMove(char[], char[], int, int, int, int, int, int, bool);
int validateBlackPawnMove(char[], char[], int, int, int, int, int, int, bool);
int validateAllMoves(char[], char[], char*, int, int, int, int, int, int, bool);

#endif