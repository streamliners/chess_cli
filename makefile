# *****************************************************
# variables

CC = g++
CFLAGS = -Wall -g

# ****************************************************
# targets needed to bring the executable up to date

chessMake: chess.o moveValidation.o checkValidation.o global.o
	$(CC) $(CFLAGS) -o chessMake chess.o moveValidation.o checkValidation.o global.o

# The chess.o target can be written more simply
chess.o: chess.cpp moveValidation.h checkValidation.h global.h
	$(CC) $(CFLAGS) -c chess.cpp

moveValidation.o: moveValidation.h

checkValidation.o: checkValidation.h

global.o: global.h