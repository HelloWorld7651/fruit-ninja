//
// Sword.h
//

#ifndef SWORD_H
#define SWORD_H

// Engine includes.
#include "Color.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "EventStep.h"
#include "Object.h"

const char SWORD_CHAR = '+';
const std::string SWORD_STRING = "Sword";

class Sword : public df::Object {

 private:
  df::Color m_color;         // trail color
  df::Vector m_old_position; // previous position
  int m_sliced;              // fruits sliced this move
  int m_old_sliced;          // previous sliced
  
  // Handle step event.
  int step(const df::EventStep *p_e);

  // Handle mouse event.
  int mouse(const df::EventMouse *p_e);

  // Handle keyboard event.
  int keyboard(const df::EventKeyboard *p_e);

public:

  // Constructor.
  Sword();

  // Handle events.
  int eventHandler(const df::Event *p_e) override;

  // Draw.
  int draw(void) override;
};

#endif // SWORD_H
