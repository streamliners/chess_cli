#ifndef CHECK_VALIDATION_H
#define CHECK_VALIDATION_H

void displayStatus(const char*);
int areCheckedNextTurn(char piecePos[], int, int, int, char, char, char, char, char);
int processChecks(char[], char, bool);
void mainMoveAndPawnPromotion(char[], char*, int, int);
char getInputFromXCoordinates(int);
int foundSolution(char[], char*, char, int, int, int, int);
int isCheckmate(char[], char[], char);
int isStalemate(char[], char[], char);

#endif