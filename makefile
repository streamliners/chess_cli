# *****************************************************
# variables

CC = g++
CFLAGS = -Wall -g

# ****************************************************
# targets needed to bring the executable up to date

chessMake: chessMain.o moveValidation.o checkValidation.o global.o
	$(CC) $(CFLAGS) -o chessMake chessMain.o moveValidation.o checkValidation.o global.o

# The chess.o target can be written more simply
chessMain.o: chessMain.cpp moveValidation.h checkValidation.h global.h
	$(CC) $(CFLAGS) -c chessMain.cpp

moveValidation.o: moveValidation.h

checkValidation.o: checkValidation.h

global.o: global.h