//
// Server.h
//
#ifndef SERVER_H
#define SERVER_H

#include "Event.h"
#include "EventNetwork.h"
#include "Object.h"

class Server : public df::Object {
 private:
  int handleData(const df::EventNetwork *p_e);

 public:
 int m_client_socket;
  Server();
  int eventHandler(const df::Event *p_e) override;
};

#endif