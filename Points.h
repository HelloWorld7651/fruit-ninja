//
// Points.h
// 

// Engine includes.
#include "Event.h"
#include "ViewObject.h"

const std::string POINTS_STRING = "Points";

class Points : public df::ViewObject {

 public:

  // Constructor.
  Points();

  // Set value.
  void setValue(int value) override;
};
