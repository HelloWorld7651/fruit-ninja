//
// GameOver.h
//

#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "ViewObject.h"

const std::string GAMEOVER_STRING = "GameOver";

class GameOver : public df::ViewObject {

 private:
  int m_time_to_live;  // in ticks

  // Handle step events.
  int step();

public:
  // Default constructor;
  GameOver();

  // Handle events
  int eventHandler(const df::Event *p_e) override;

  // Draw sprite.
  int draw() override;
};

#endif // GAMEOVER_H
