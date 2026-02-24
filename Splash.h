//
// Splash.h
// 

#ifndef SPLASH_H
#define SPLASH_H

// Engine includes.
#include "EventStep.h"
#include "Object.h"

// Timing (ticks) for splash events.
const int FRUIT_TIME = 0;
const int NINJA_TIME = 30;
const int SLICE_TIME = 75;
const int END_TIME = 135;

const std::string SPLASH_STRING = "Splash";

class Splash : public df::Object {

 private:
  int m_time;          // Count of ticks for event actions.
  
  // Handle step events.
  int step(const df::EventStep *p_s);

  // Handle keyboard events.
  int keyboard(const df::EventKeyboard *p_k);

 public:

  // Constructor.
  Splash();

  // Handle events.
  int eventHandler(const df::Event *p_e)  override;
};
 
#endif // SPLASH_H
