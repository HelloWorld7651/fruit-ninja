//
// Server.cpp
//

// System includes.
#include <string.h>

// Engine includes.
#include "LogManager.h"
#include "GameManager.h"
#include "NetworkManager.h"
#include "WorldManager.h"
#include "utility.h"

// Game includes.
#include "game.h"
#include "server.h"
#include "Sword.h" // Needed to identify the sword

Server::Server() {
  setType("Server");
  NM.setServer(true);
  NM.setMaxConnections(1);
  registerInterest(df::NETWORK_EVENT);
  LM.writeLog("Server::Server(): Server started.");
}

int Server::eventHandler(const df::Event *p_e) {
  if (p_e->getType() == df::NETWORK_EVENT) {
    const df::EventNetwork *p_ne = (const df::EventNetwork *) p_e;

    if (p_ne->getLabel() == df::NetworkEventLabel::ACCEPT) {
      return 1;
    }
    if (p_ne->getLabel() == df::NetworkEventLabel::CLOSE) {
      GM.setGameOver();
      return 1;
    }
    if (p_ne->getLabel() == df::NetworkEventLabel::DATA) {
      return handleData(p_ne);
    }
  }
  return Object::eventHandler(p_e);
}

int Server::handleData(const df::EventNetwork *p_en) {
  int msg_size = p_en->getBytes();
  char *buff = (char *) malloc(msg_size);
  if (!buff) return -1;
  
  memcpy(buff, p_en->getMessage(), msg_size);
  MessageType type;
  memcpy(&type, buff + sizeof(int), sizeof(MessageType));

  switch(type) {
    case MessageType::SWORD_POS: {
      // Unpack vector
      df::Vector pos;
      memcpy(&pos, buff + sizeof(int) + sizeof(MessageType), sizeof(df::Vector));
      
      // Update sword position
      df::ObjectList swords = WM.objectsOfType(SWORD_STRING);
      if (swords.getCount() > 0) {
        swords[0]->setPosition(pos);
      }
      break;
    }
    default:
      break;
  }

  free(buff);
  return 1;
}