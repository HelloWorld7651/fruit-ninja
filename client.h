//
// client.h 
//

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "EventNetwork.h"
#include "Object.h"

class Client : public df::Object {
 private:
  bool m_connected; 
  int handleData(const df::EventNetwork *p_e);

 public:
  Client(std::string server_name);
  int eventHandler(const df::Event *p_e) override;
};

#endif