//
// Splash.cpp
//

// Engine includes.
#include "EventKeyboard.h"
#include "EventStep.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "Fruit.h"
#include "game.h"
#include "Splash.h"
#include "util.h"

Splash::Splash() {

  // Set Object attributes.
  setVisible(false);
  setType(SPLASH_STRING);
  setSolidness(df::SPECTRAL);
  registerInterest(df::STEP_EVENT);
  m_time = 0;
  
  // Register for keypress so can exit if pressed.
  registerInterest(df::KEYBOARD_EVENT);
}

// Handle events.
int Splash::eventHandler(const df::Event *p_e) {

  // If game over, window is closed so done.
  if (GM.getGameOver()) {
    WM.markForDelete(this);
    return 0;
  }

  // On keyboard event, exit.
  if (p_e->getType() == df::KEYBOARD_EVENT)
    return keyboard((df::EventKeyboard *) p_e);

  // On step event, count down until actions.
  if (p_e->getType() == df::STEP_EVENT)
    return step((df::EventStep *) p_e);

  // If we get here, we have ignored this event.
  return 0;
}

// Handle step events.
int Splash::step(const df::EventStep *p_s) {

  // Time 1: Spawn Fruit. Play sound.
  if (m_time == FRUIT_TIME) {

    // Fruit splash.
    df::Object *p_o = new df::Object();
    if (!p_o) {
      LM.writeLog("Splash::step(): Error! Unable to allocate Object.");
      return 0;
    }
    p_o -> setType("fruit-splash");
    p_o -> setSprite("fruit-splash");
    p_o -> setSolidness(df::SPECTRAL);
    df::Vector pos(WM.getBoundary().getHorizontal()/2,
                   WM.getBoundary().getVertical()/2 - 5);
    p_o -> setPosition(pos);

    // Plus fruit.
    Fruit *p_f = new Fruit(FRUIT[rand() % (NUM_FRUITS)]);
    if (!p_f) {
      LM.writeLog("Splash::step(): Error! Unable to allocate Fruit.");
      return 0;
    }
    pos.setX(pos.getX() + 29);
    p_f -> setPosition(pos);
    df::Animation a = p_f -> getAnimation();
    a.setSlowdownCount(-1);
    p_f -> setAnimation(a);

    // Play "impact" sound.
    play_sound("impact");
  }

  // Time 2: Spawn Ninja. Play sound.
  if (m_time == NINJA_TIME) {

    // Ninja splash.
    df::Object *p_o = new df::Object();
    if (!p_o) {
      LM.writeLog("Splash::step(): Error! Unable to allocate Object.");
      return 0;
    }
    p_o -> setType("ninja-splash");
    p_o -> setSprite("ninja-splash");
    p_o -> setSolidness(df::SPECTRAL);
    df::Vector pos(WM.getBoundary().getHorizontal()/2,
                   WM.getBoundary().getVertical()/2 + 2);
    p_o -> setPosition(pos);

    // Plus fruit.
    Fruit *p_f = new Fruit(FRUIT[rand() % (NUM_FRUITS)]);
    if (!p_f) {
      LM.writeLog("Splash::step(): Error! Unable to allocate Fruit.");
      return 0;
    }
    pos.setX(pos.getX() - 29);
    p_f -> setPosition(pos);
    df::Animation a = p_f -> getAnimation();
    a.setSlowdownCount(-1);
    p_f -> setAnimation(a);

    // Play "impact" sound.
    play_sound("impact");
  }

  // Time 3: Slice and explode.
  if (m_time == SLICE_TIME) {

    // Slice.
    df::Vector p1(WM.getBoundary().getHorizontal()/2 - 4,
                  WM.getBoundary().getVertical() - 2);
    df::Vector p2(WM.getBoundary().getHorizontal()/2 + 4,
                  2);
    create_trail(p1, p2);

    // Explode fruit-splash.
    df::ObjectList ol = WM.objectsOfType("fruit-splash");
    df::Vector where = ol[0] -> getPosition();
    int index = 0; // first frame
    df::explode(ol[0] -> getAnimation().getSprite(), index, where,
                EXPLOSION_AGE, EXPLOSION_SPEED*3.0f, EXPLOSION_ROTATE*2.0f);
    WM.markForDelete(ol[0]);

    // Explode ninja-splash.
    ol = WM.objectsOfType("ninja-splash");
    where = ol[0] -> getPosition();
    df::explode(ol[0] -> getAnimation().getSprite(), index, where,
                EXPLOSION_AGE, EXPLOSION_SPEED*3.0f, EXPLOSION_ROTATE*2.0f);
    WM.markForDelete(ol[0]);

    // Explode Fruit.
    ol = WM.solidObjects();
    for (int i=0; i<ol.getCount(); i++)
      WM.markForDelete(ol[i]);

    // Play "game start" sound.
    play_sound("game-start");
  }

  // Time 4: Delete everything. Set game over.
  if (m_time >= END_TIME) {
    df::ObjectList ol = WM.getAllObjects();
    for (int i=0; i<ol.getCount(); i++)
      WM.markForDelete(ol[i]);
    GM.setGameOver(true);
  }

  // Advance time.
  m_time += 1;

  return 1;
}

// Handle keyboard events.
int Splash::keyboard(const df::EventKeyboard *p_k) {

  // Keypress? --> exit.
  if (p_k->getKeyboardAction() == df::KEY_PRESSED) {
    m_time = END_TIME;
    return 1;
  }

  return 0;
}
