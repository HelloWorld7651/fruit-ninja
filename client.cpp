//
// client.cpp
//

#include <string.h>
#include "EventNetwork.h"
#include "GameManager.h"
#include "LogManager.h"
#include "NetworkManager.h"
#include "WorldManager.h"
#include "utility.h"

#include "game.h"
#include "client.h"
#include "Sword.h"
#include "Fruit.h"
#include "bomb.h"

Client::Client(std::string server_name) {
  setType("Client");
  NM.setServer(false);
  m_connected = false; 

  // Register for Network events (Mouse events are handled inside Sword.cpp now!)
  registerInterest(df::NETWORK_EVENT);

  std::string server_port = df::DRAGONFLY_PORT;
  LM.writeLog("Client::Client(): Connecting to %s at port %s.", server_name.c_str(), server_port.c_str());
  if (NM.connect(server_name, server_port) < 0) {
    LM.writeLog("Client::Client(): Error! Unable to connect.");
    exit(-1);
  }
}

int Client::eventHandler(const df::Event *p_e) {
  // Handle network events
  if (p_e->getType() == df::NETWORK_EVENT) {
    const df::EventNetwork *p_ne = (const df::EventNetwork *) p_e;

    if (p_ne->getLabel() == df::NetworkEventLabel::CONNECT) {
      m_connected = true; 
      LM.writeLog("Client: Connected! Now ready to send data.");
      return 1;
    }
    else if (p_ne->getLabel() == df::NetworkEventLabel::CLOSE) {
      GM.setGameOver();
      return 1;
    }
    else {
      // If it's not a CONNECT or CLOSE, it is incoming data!
      return handleData(p_ne);
    }
  }

  return Object::eventHandler(p_e);
}

int Client::handleData(const df::EventNetwork *p_en) {
  // Use the base header to discover the message type safely
  const BaseMsg *p_base = (const BaseMsg *) p_en->getMessage();

  if (p_base->type == MessageType::SWORD_POS) {
    // Cast to SwordPosMsg for full payload
    const SwordPosMsg *p_msg = (const SwordPosMsg *) p_en->getMessage();
    df::Vector pos(p_msg->x, p_msg->y);
    df::ObjectList swords = WM.objectsOfType(SWORD_STRING);
    if (swords.getCount() > 0) {
      swords[0]->setPosition(pos);
    }
  }
  else if (p_base->type == MessageType::FRUIT_SPAWN) {
    const FruitSpawnMsg *f_msg = (const FruitSpawnMsg *) p_en->getMessage();
    Fruit *p_f = new Fruit(std::string(f_msg->fruit_name));
    p_f->setPosition(df::Vector(f_msg->x, f_msg->y));
    p_f->setVelocity(df::Vector(f_msg->vx, f_msg->vy));
  }
  else if (p_base->type == MessageType::BOMB_SPAWN) {
    const BombSpawnMsg *b_msg = (const BombSpawnMsg *) p_en->getMessage();
    Bomb *p_b = new Bomb(std::string(b_msg->bomb_name));
    p_b->setPosition(df::Vector(b_msg->x, b_msg->y));
    p_b->setVelocity(df::Vector(b_msg->vx, b_msg->vy));
  }

  return 1;
}