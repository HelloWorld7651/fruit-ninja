//
// Client.h 
//

#ifndef CLIENT_H
#define CLIENT_H

// System includes.
#include <string>

// Engine includes.
#include "EventNetwork.h"
#include "Object.h"

class Client : public df::Object {

 private:
  // Handle incoming network data
  int handleData(const df::EventNetwork *p_e);

 public:
  // Constructor, connecting to server.
  Client(std::string server_name);

  // Handle event.
  // Return 0 if ignored, else 1.
  int eventHandler(const df::Event *p_e) override;
};

#endif