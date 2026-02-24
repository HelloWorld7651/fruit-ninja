//
// Fruit.h
//

#ifndef FRUIT_H
#define FRUIT_H

// System includes.
#include <string>

// Engine includes.
#include "Event.h"
#include "EventCollision.h"
#include "EventOut.h"
#include "Object.h"

class Fruit : public df::Object {

 private:
  bool m_first_out;

  // Handle out events.
  int out(const df::EventOut *p_e);

  // Handle collision events.
  int collide(const df::EventCollision *p_e);

public:

  // Constructor - supply name of Fruit (matches Sprite).
  Fruit(std::string name);

  // Destructor - if inside the game world, create explosion.
  ~Fruit();

  // Handle events.
  int eventHandler(const df::Event *p_e) override;

  // Setup starting conditions.
  void start(float speed);
};

#endif // FRUIT_H
