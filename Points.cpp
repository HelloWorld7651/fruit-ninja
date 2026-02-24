//
// Points.cpp
//

// Engine includes
#include "Event.h"
#include "EventStep.h"
#include "LogManager.h"

// Game includes.
#include "Points.h"

Points::Points() {
  setType(POINTS_STRING);
  setLocation(df::TOP_RIGHT);
  setViewString(POINTS_STRING);
  setColor(df::WHITE);
  setValue(0);
}

void Points::setValue(int value) {

  // Call parent.
  ViewObject::setValue(value);

  // If less than 0, set to 0.
  if (getValue() < 0)
    ViewObject::setValue(0);
}
