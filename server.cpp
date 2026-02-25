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
  m_client_socket = -1; // Initialize the socket to invalid
  NM.setServer(true);
  NM.setMaxConnections(1);
  registerInterest(df::NETWORK_EVENT);
  LM.writeLog("Server::Server(): Server started.");
}

int Server::eventHandler(const df::Event *p_e) {
  if (p_e->getType() == df::NETWORK_EVENT) {
    const df::EventNetwork *p_ne = (const df::EventNetwork *) p_e;

    if (p_ne->getLabel() == df::NetworkEventLabel::ACCEPT) {
      // Save the socket index when the client connects!
      m_client_socket = p_ne->getSocket(); 
      LM.writeLog("Server: Accepted connection on socket %d!", m_client_socket);
      return 1;
    }
    else if (p_ne->getLabel() == df::NetworkEventLabel::CLOSE) {
      m_client_socket = -1; // Client disconnected
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

  // Handle incoming mouse position from client
  if (p_base->type == MessageType::MOUSE_POS) {
    const MousePosMsg *p_msg = (const MousePosMsg *)p_en->getMessage();
    df::Vector pos(p_msg->x, p_msg->y);
    
    // Update the authoritative sword's position
    df::ObjectList swords = WM.objectsOfType(SWORD_STRING);
    if (swords.getCount() > 0) {
      swords[0]->setPosition(pos);
    }
    
    // Broadcast updated sword position back to the client
    SwordPosMsg out;
    out.msg_size = sizeof(SwordPosMsg);
    out.type = MessageType::SWORD_POS;
    out.x = pos.getX();
    out.y = pos.getY();
    
    // Send it back out using the known client socket (3 arguments!)
    if (m_client_socket != -1) {
      NM.send(&out, out.msg_size, m_client_socket);
    }
  }

  return 1;
}