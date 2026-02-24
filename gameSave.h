//gameSave.h

#include "Object.h"
#include "EventKeyboard.h"

class gameSave: public df:: Object{
    public:
    gameSave();
    int eventHandler(const df:: Event *p_e) override;

    void saveGame();
    void loadGame();
};