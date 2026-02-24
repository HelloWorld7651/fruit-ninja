//
// Timer.h
// 

#ifndef TIMER_H
#define TIMER_H

// Engine includes.
#include "Event.h"
#include "ViewObject.h"

const std::string TIMER_STRING = "Time";

class Timer : public df::ViewObject {

private:
  // Handle step events.
  int step(const df::EventStep *p_e);

public:

  // Constructor.
  Timer();

  // Handle events.
  int eventHandler(const df::Event *p_e) override;
};

#endif // TIMER_H
