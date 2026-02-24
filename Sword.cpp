//
// Sword.cpp
//

// Engine includes.
#include "DisplayManager.h"
#include "EventCollision.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "EventView.h"
#include "Fader.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "Fruit.h"
#include "bomb.h"
#include "Grocer.h"
#include "Kudos.h"
#include "Sword.h"
#include "Timer.h"
#include "util.h"

// Constructor.
Sword::Sword() {

  setType(SWORD_STRING);
  setSolidness(df::SPECTRAL);
  setAltitude(df::MAX_ALTITUDE); // Make Sword in foreground.

  registerInterest(df::MSE_EVENT);
  registerInterest(df::KEYBOARD_EVENT);
  registerInterest(df::STEP_EVENT);
  
  // Start sword in center of world.
  df::Vector p(WM.getBoundary().getHorizontal()/2,
               WM.getBoundary().getVertical()/2);
  setPosition(p);

  m_old_position = getPosition();
  m_color = df::CYAN;
  m_sliced = 0;
  m_old_sliced = 0;
}

// Handle event.
// Return 0 if ignored, else 1.
int Sword::eventHandler(const df::Event *p_e) {

  // Mouse event.
  if (p_e->getType() == df::MSE_EVENT)
    return mouse((df::EventMouse *) p_e);

  // Step event.
  if (p_e->getType() == df::STEP_EVENT)
    return step((df::EventStep *) p_e);

  // Keyboard event.
  if (p_e->getType() == df::KEYBOARD_EVENT)
    return keyboard((df::EventKeyboard *) p_e);

  // If get here, have ignored this event.
  return 0;
}

// Handle step event.
int Sword::step(const df::EventStep *p_e) {

  // Check if moved since last step.
  if (m_old_position == getPosition()) {
    m_sliced = 0;
    return 1;
  }

  // Make a trail from last position to current.
  create_trail(getPosition(), m_old_position);
  
  // Check if line intersects any Fruit objects.
  df::Line line(getPosition(), m_old_position);
  df::ObjectList ol = WM.solidObjects();
  
  for (int i=0; i<ol.getCount(); i++) {
    
    // Only slice Fruit or Bomb.
    if (!(dynamic_cast <Fruit *> (ol[i])) && !(dynamic_cast <Bomb *> (ol[i])))
      continue;
    
    // If line from previous position intersects --> slice!
    df::Object *p_o = ol[i];
    df::Box box = getWorldBox(p_o);
    if (lineIntersectsBox(line, box)) {
      df::EventCollision c(this, p_o, p_o->getPosition());
      p_o -> eventHandler(&c);
      m_sliced += 1;
      
      // Spawn kudos for combo.
      if (m_sliced > 2 && m_sliced > m_old_sliced)
        new Kudos();
      
      m_old_sliced = m_sliced;
      
    } // End of box-line check.
    
  } // End of loop through all objects.
  
  // If travel far enough, play "swipe" sound.
  float dist = df::distance(getPosition(), m_old_position);
  if (dist > 15) {
    std::string sound = "swipe-" + std::to_string(rand()%7 + 1);
    play_sound(sound);
  }
  
  // Lose points for distance traveled.
  int penalty = -1 * (int)(dist/10.0f);
  df::EventView ev("Points", penalty, true);
  WM.onEvent(&ev);
  
  m_old_position = getPosition();

  // Handled.
  return 1;
}

// Handle mouse event.
int Sword::mouse(const df::EventMouse *p_e) {

  // If "move", change position to mouse position.
  if (p_e -> getMouseAction() == df::MOVED) {
    setPosition(p_e -> getMousePosition());
    return 1;
  }
  
  // If get here, not handled.
  return 0;
}

// Handle keyboard event.
int Sword::keyboard(const df::EventKeyboard *p_e) {

  if (p_e->getKey() == df::Keyboard::Q &&
     p_e->getKeyboardAction() == df::KEY_PRESSED) {

    // Tell Grocer game over.
    df::ObjectList ol = WM.objectsOfType(GROCER_STRING);
    if (ol.getCount() > 0 && (dynamic_cast <Grocer *> (ol[0]))) {
      Grocer *p_g = dynamic_cast <Grocer *> (ol[0]);
      p_g->gameOver();
    }
    
    // Set Timer to 0.
    ol = WM.objectsOfType(TIMER_STRING);
    if (ol.getCount() > 0 && (dynamic_cast <Timer *> (ol[0]))) {
      Timer *p_t = dynamic_cast <Timer *> (ol[0]);
      p_t->setValue(0);
    }

    return 1;
  }

  // If get here, not handled.
  return 0;
}

// Draw sword on window.
int Sword::draw() {
  return DM.drawCh(getPosition(), SWORD_CHAR, m_color);
}
