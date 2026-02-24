//
// game.h
//

#ifndef GAME_H
#define GAME_H

// System includes.
#include <string>

const float VERSION=1.1f;

// Fruit settings.
const int NUM_FRUITS = 5;
const std::string FRUIT[NUM_FRUITS] = {
  "pear",
  "grapes",
  "apple",
  "banana",
  "blueberries"
};
const int EXPLOSION_AGE = 45;        // in ticks
const float EXPLOSION_SPEED = 0.05f; // in spaces/tick
const float EXPLOSION_ROTATE = 1.0f; // in degrees

// Sound settings.
const int NUM_SPLATS = 6;
const int NUM_SWIPES = 7;
const int NUM_KUDOS = 10;

// Wave settings.
const int NUM_WAVES = NUM_FRUITS;
const int WAVE_LEN = 300;      // in ticks
const int WAVE_SPAWN = 30;     // in ticks
const float WAVE_SPEED = 0.5f; // in ticks
const float SPEED_INC = 0.1f;  // in spaces/tick
const int SPAWN_INC = -5 ;     // in ticks

#endif // GAME_H
