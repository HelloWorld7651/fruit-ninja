//
// Client.cpp
//

// System includes.
#include <string.h>

// Engine includes.
#include "EventNetwork.h"
#include "EventMouse.h"
#include "GameManager.h"
#include "LogManager.h"
#include "NetworkManager.h"
#include "utility.h"

// Game includes.
#include "game.h"
#include "client.h"

// Constructor, connecting to server.
Client::Client(std::string server_name) {
  setType("Client");
  NM.setServer(false);

  // Register for network and mouse events.
  registerInterest(df::NETWORK_EVENT);
  registerInterest(df::MSE_EVENT);

  std::string server_port = df::DRAGONFLY_PORT;
  LM.writeLog("Client::Client(): Connecting to %s at port %s.", server_name.c_str(), server_port.c_str());
  if (NM.connect(server_name, server_port) < 0) {
    LM.writeLog("Client::Client(): Error! Unable to connect.");
    exit(-1);
  }

  LM.writeLog("Client::Client(): Client started.");
}

int Client::eventHandler(const df::Event *p_e) {

  // Handle mouse events: Send position to Server
  if (p_e->getType() == df::MSE_EVENT) {
    const df::EventMouse *p_me = (const df::EventMouse *) p_e;
    
    if (p_me->getMouseAction() == df::MOVED) {
      int msg_size = sizeof(int) + sizeof(MessageType) + sizeof(df::Vector);
      char *buff = (char *) malloc(msg_size);
      if (buff) {
        MessageType type = MessageType::SWORD_POS;
        df::Vector pos = p_me->getMousePosition();
        
        memcpy(buff, &msg_size, sizeof(int));
        memcpy(buff + sizeof(int), &type, sizeof(MessageType));
        memcpy(buff + sizeof(int) + sizeof(MessageType), &pos, sizeof(df::Vector));
        
        NM.send(buff, msg_size); 
        free(buff);
      }
    }
    return 0; // Return 0 so local Sword also handles the event
  }

  // Handle network events
  if (p_e->getType() == df::NETWORK_EVENT) {
    const df::EventNetwork *p_ne = (const df::EventNetwork *) p_e;

    if (p_ne->getLabel() == df::NetworkEventLabel::CONNECT) {
      LM.writeLog("Client::eventHandler(): connected");
      return 1;
    }
    if (p_ne->getLabel() == df::NetworkEventLabel::CLOSE) {
      LM.writeLog("Client::eventHandler(): closed connection");
      GM.setGameOver();
      return 1;
    }
    if (p_ne->getLabel() == df::NetworkEventLabel::DATA) {
      return handleData(p_ne);
    }
  }

  return Object::eventHandler(p_e);
}

int Client::handleData(const df::EventNetwork *p_en) {
  int msg_size = p_en->getBytes();
  char *buff = (char *) malloc(msg_size);
  if (!buff) return -1;
  
  memcpy(buff, p_en->getMessage(), msg_size);
  MessageType type;
  memcpy(&type, buff + sizeof(int), sizeof(MessageType));

  switch(type) {
    case MessageType::EXIT:
      GM.setGameOver();
      break;
    default:
      break;
  }

  free(buff);
  return 1;
}