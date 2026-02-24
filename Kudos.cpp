//
// Kudos.cpp
//

// System includes.
#include <stdlib.h>  // for rand()

// Engine includes.
#include "EventStep.h"
#include "EventView.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "Kudos.h"
#include "Points.h"
#include "util.h"

Kudos::Kudos() {
  setType(KUDOS_STRING);
  setSolidness(df::SPECTRAL);
  setAltitude(df::MAX_ALTITUDE);
  setSprite("kudos");

  // Pick random kudos to show.
  df::Animation a = getAnimation();
  const df::Sprite *p_sprite = getAnimation().getSprite();
  if (p_sprite) {
    int count = p_sprite -> getFrameCount();
    int kudos = rand() % count;
    a.setIndex(kudos);
  }
  a.setSlowdownCount(-1); // Not animated.
  setAnimation(a);

  // Play next Kudos sound.
  static int s_sound = 1;  // next kudos sound
  std::string sound = "kudos-" + std::to_string(s_sound);
  play_sound(sound);
  s_sound += 1;
  if (s_sound > 10)
    s_sound = 10;

  // Pick random location.
  df::Vector p(WM.getBoundary().getHorizontal()/8 +
               rand() % (3 * (int) WM.getBoundary().getHorizontal()/4),
               WM.getBoundary().getVertical()/8 +
               rand() % (3 * (int) WM.getBoundary().getVertical()/4));
  setPosition(p);

  // Extra points.
  df::EventView ev(POINTS_STRING, 50, true);
  WM.onEvent(&ev);

  m_time_to_live = 30; // Stays on the screen for 1 second.
  registerInterest(df::STEP_EVENT);
}

// Handle event.
// Return 0 if ignored, else 1.
int Kudos::eventHandler(const df::Event *p_e) {

  // Step event.
  if (p_e->getType() == df::STEP_EVENT)
    return step((df::EventStep *) p_e);

  // If get here, have ignored this event.
  return 0;
}

// Handle step event.
int Kudos::step(const df::EventStep *p_e) {

  m_time_to_live -= 1;

  if (m_time_to_live < 0)
    WM.markForDelete(this);

  // Handled.
  return 1;
}
