# README - Fruit Ninja

Copyright Mark Claypool and WPI, 2024.

This is a "Fruit Ninja" type game for Dragonfly.

This is iterated upon Fruit-Ninja-6, taken from Dragonfly tutorial. 


## To Run

2a 
Platform:MAC
To Run:
1. MAKE
2. ./fruit-ninja
Clean the game:
1. make clean

2b
In the video, for the audio, I meant I was unsure if my computer picks up the fruit-ninja sound. But I do have sound in the game itself.
The video that I have recorded is in the file itself.  
What I added was essentially bombs that serve as obstacles or remove points from the point counter. I added 3 bombs in total, each with their own gimmick. Each bomb can also be 4 different colors, mainly red, blue, green, or yellow. So the normal bomb is 25 point reduction, average speed as fruits. Then we have the small bomb, which moves faster, takes 5 points reduction and can summon 5 additonal small bombs with it, mainly to swarm and confuse. Then we have the big bomb, which serves to end the game immediatly if you hit it. Mainly for code that I added, I have a list below. Mainly, most of the bomb code is in bomb.cpp, and for the extensions, those were added to make bomb either spawn(grocer), make sword hit bomb(sword), or sprite setting(util).

The code that I added are:
1. bomb.cpp
2. bomb.h
3. gameSave.cpp
4. gameSave.h
5. new sprite, ending in bomb
6. explosion sound
7. Extensions to grocer, util, sword. 


