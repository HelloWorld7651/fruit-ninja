//
// bomb.h
//

#ifndef BOMB_H
#define BOMB_H

// System includes.
#include <string>

// Engine includes.
#include "Event.h"
#include "EventCollision.h"
#include "EventOut.h"
#include "Object.h"

class Bomb: public df::Object{
    private:
        bool first_spawn; 
        
        //handle out event
        int out(const df::EventOut *p_e);

        //handle collision event
        int collide(const df::EventCollision *p_e);
    public:
        //constructor
        Bomb();
        // Destructor - if inside the game world, create explosion.
        ~Bomb();

        //handle event
        int eventHandler(const df::Event *p_e) override;

        // Setup starting conditions.
        void start(float speed);
};

#endif