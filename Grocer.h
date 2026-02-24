//
// Grocer.h
//

// Engine includes.
#include "EventStep.h"
#include "Object.h"

// Game includes.
#include "game.h"
#include "Fruit.h"

const std::string GROCER_STRING = "Grocer";

class Grocer : public df::Object {

 private:
  int m_spawn;        // countdown to next fruit, in ticks
  int m_wave;         // wave number
  int m_wave_spawn;   // current wave countdown, in ticks
  int m_wave_end;     // current wave end, in ticks
  float m_wave_speed; // current fruit speed, in spaces/tick

  // Handle step events.
  int step(const df::EventStep *p_e);

public:

  // Constructor.
  Grocer();

  // Handle events.
  int eventHandler(const df::Event *p_e) override;

  // Do game over actions.
  void gameOver();
};
