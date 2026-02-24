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
#include "NetworkManager.h" // ADDED FOR MULTIPLAYER
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
  setAltitude(df::MAX_ALTITUDE); 

  registerInterest(df::MSE_EVENT);
  registerInterest(df::KEYBOARD_EVENT);
  registerInterest(df::STEP_EVENT);
  
  df::Vector p(WM.getBoundary().getHorizontal()/2,
               WM.getBoundary().getVertical()/2);
  setPosition(p);

  m_old_position = getPosition();
  m_color = df::CYAN;
  m_sliced = 0;
  m_old_sliced = 0;
}

int Sword::eventHandler(const df::Event *p_e) {
  if (p_e->getType() == df::MSE_EVENT) return mouse((df::EventMouse *) p_e);
  if (p_e->getType() == df::STEP_EVENT) return step((df::EventStep *) p_e);
  if (p_e->getType() == df::KEYBOARD_EVENT) return keyboard((df::EventKeyboard *) p_e);
  return 0;
}

int Sword::step(const df::EventStep *p_e) {
  if (m_old_position == getPosition()) {
    m_sliced = 0;
    return 1;
  }

  create_trail(getPosition(), m_old_position);
  df::Line line(getPosition(), m_old_position);
  df::ObjectList ol = WM.solidObjects();
  
  for (int i=0; i<ol.getCount(); i++) {
    if (!(dynamic_cast <Fruit *> (ol[i])) && !(dynamic_cast <Bomb *> (ol[i]))) continue;
    
    df::Object *p_o = ol[i];
    df::Box box = getWorldBox(p_o);
    if (lineIntersectsBox(line, box)) {
      df::EventCollision c(this, p_o, p_o->getPosition());
      p_o -> eventHandler(&c);
      m_sliced += 1;
      
      if (m_sliced > 2 && m_sliced > m_old_sliced) new Kudos();
      m_old_sliced = m_sliced;
    }
  }
  
  float dist = df::distance(getPosition(), m_old_position);
  if (dist > 15) {
    std::string sound = "swipe-" + std::to_string(rand()%7 + 1);
    play_sound(sound);
  }
  
  int penalty = -1 * (int)(dist/10.0f);
  df::EventView ev("Points", penalty, true);
  WM.onEvent(&ev);
  
  m_old_position = getPosition();
  return 1;
}

// Handle mouse event.
int Sword::mouse(const df::EventMouse *p_e) {
  // SERVER OVERRIDE: Prevent the server player from moving the sword locally
  if (NM.isServer()) {
    return 0;
  }

  // If "move", change position to mouse position.
  if (p_e -> getMouseAction() == df::MOVED) {
    setPosition(p_e -> getMousePosition());
    return 1;
  }
  return 0;
}

int Sword::keyboard(const df::EventKeyboard *p_e) {
  if (p_e->getKey() == df::Keyboard::Q && p_e->getKeyboardAction() == df::KEY_PRESSED) {
    df::ObjectList ol = WM.objectsOfType(GROCER_STRING);
    if (ol.getCount() > 0 && (dynamic_cast <Grocer *> (ol[0]))) {
      Grocer *p_g = dynamic_cast <Grocer *> (ol[0]);
      p_g->gameOver();
    }
    
    ol = WM.objectsOfType(TIMER_STRING);
    if (ol.getCount() > 0 && (dynamic_cast <Timer *> (ol[0]))) {
      Timer *p_t = dynamic_cast <Timer *> (ol[0]);
      p_t->setValue(0);
    }
    return 1;
  }
  return 0;
}

int Sword::draw() {
  return DM.drawCh(getPosition(), SWORD_CHAR, m_color);
}