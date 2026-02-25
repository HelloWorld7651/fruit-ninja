//
// server.cpp
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
#include "Sword.h" 

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
      LM.writeLog("Server: Accepted connection!");
      SwordPosMsg out;
      out.msg_size = sizeof(SwordPosMsg);
      out.type     = MessageType::SWORD_POS;
      out.x        = 0.0f;
      out.y        = 0.0f;
      NM.send(&out, out.msg_size);
      return 1;
    }
    else if (p_ne->getLabel() == df::NetworkEventLabel::CLOSE) {
      GM.setGameOver();
      return 1;
    }
    else {
      // Incoming Data
      return handleData(p_ne);
    }
  }
  
  return Object::eventHandler(p_e);
}

int Server::handleData(const df::EventNetwork *p_en) {
  // Read just the base header to figure out what type of message this is safely
  const BaseMsg *p_base = (const BaseMsg *)p_en->getMessage();

  // Identify which client sent this (needed for supporting multiple swords later)
  int socket_index = p_en->getSocket();

  // Handle incoming mouse position from client
  if (p_base->type == MessageType::MOUSE_POS) {
    const MousePosMsg *p_msg = (const MousePosMsg *)p_en->getMessage();
    df::Vector pos(p_msg->x, p_msg->y);
    
    // Update the authoritative sword's position
    df::ObjectList swords = WM.objectsOfType(SWORD_STRING);
    if (swords.getCount() > 0) {
      swords[0]->setPosition(pos);
    }
    
    // Broadcast updated sword position back to all clients
    SwordPosMsg out;
    out.msg_size = sizeof(SwordPosMsg);
    out.type = MessageType::SWORD_POS;
    out.x = pos.getX();
    out.y = pos.getY();
    
    NM.send(&out, out.msg_size);
    int bytes_sent = NM.send(&out, out.msg_size);
        if (bytes_sent < 0) {
    LM.writeLog("ERROR: Server failed to send message! Return value: %d", bytes_sent);
} else {
    LM.writeLog("Server successfully sent %d bytes.", bytes_sent);
}
    }
    return 1;
}