//
// Kudos.h
//

#ifndef KUDOS_H
#define KUDOS_H

// Engine includes.
#include "Event.h"
#include "EventStep.h"
#include "Object.h"

const std::string KUDOS_STRING = "Kudos";

class Kudos : public df::Object {

 private:
  int m_time_to_live;  // in ticks

  // Handle step events.
  int step(const df::EventStep *p_e);

public:

  // Constructor.
  Kudos();

  // Handle events.
  int eventHandler(const df::Event *p_e) override;

};

#endif // KUDOS_H
