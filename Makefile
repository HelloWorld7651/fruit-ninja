#
# Makefile for Fruit Ninja game using Dragonfly
#
# Copyright Mark Claypool and WPI, 2024
#
# 'make depend' to generate new dependency list
# 'make clean' to remove all constructed files
# 'make' to build executable
#
# Variables of interest:
#   GAMESRC is the source code files for the game
#   GAME is the game main() source
#

#### Adjust these as appropriate for build setup. ###
DF= ../dragonfly

### Uncomment only 1 of the below! ###

# 1) Uncomment below for Linux (64-bit)
#DFLIB= -ldragonfly-x64-linux
#SFMLLIB= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
#LINKLIB= $(DFLIB) $(SFMLLIB) 
#LINKDIR= -L$(DF)/lib -L$(HOME)/src/SFML/lib   
#INCDIR= -I$(DF)/include -I$(HOME)/src/SFML/include 

# 2) Uncomment below for Mac (64-bit)
DFLIB= -ldragonfly-arm64-mac
SFMLLIB= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
LINKLIB= $(DFLIB) $(SFMLLIB) 
LINKDIR= -L../SFML-3.0.2/lib -L$(DF)/lib -Wl,-rpath,../SFML-3.0.2/lib
INCDIR= -I../SFML-3.0.2/include -I$(DF)/include

######

CC= g++
CFLAGS= -std=c++17 -DNO_NETWORK

GAMESRC= \
	Fruit.cpp \
	GameOver.cpp \
	Grocer.cpp \
	Kudos.cpp \
	Points.cpp \
	Splash.cpp \
	Sword.cpp \
	Timer.cpp \
	util.cpp \
	bomb.cpp \
	gameSave.cpp \

GAME= game.cpp
EXECUTABLE= fruit-ninja
OBJECTS= $(GAMESRC:.cpp=.o)

all: $(EXECUTABLE) Makefile 

$(EXECUTABLE): $(OBJECTS) $(GAME) $(GAMESRC) Makefile
	$(CC) $(GAME) $(OBJECTS) $(CFLAGS) -o $@ $(INCDIR) $(LINKDIR) $(LINKLIB) 

.cpp.o: 
	$(CC) -c $(INCDIR) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) core *.log Makefile.bak *~

depend: 
	makedepend *.cpp 2> /dev/null

# DO NOT DELETE
