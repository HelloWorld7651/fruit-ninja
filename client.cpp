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
    LM.writeLog("Client: HandleData Called");
      // Get a pointer to the raw bytes
    const char *data = (const char *)p_en->getMessage();

    // Read the first 4 bytes to get the total length (not used here)
    int msgLen;
    memcpy(&msgLen, data, sizeof(int));

    // Read the next 4 bytes for the message type
    int msgType;
    memcpy(&msgType, data + sizeof(int), sizeof(int));

  if (msgType == static_cast<int>(MessageType::SWORD_POS)) {
    // Cast to SwordPosMsg for full payload
    const SwordPosMsg *p_msg = (const SwordPosMsg *) p_en->getMessage();
    df::Vector pos(p_msg->x, p_msg->y);
    df::ObjectList swords = WM.objectsOfType(SWORD_STRING);
    if (swords.getCount() > 0) {
      swords[0]->setPosition(pos);
    }
  }
  if (msgType == static_cast<int>(MessageType::FRUIT_SPAWN)) {
    int offset = sizeof(int) + sizeof(int); // skip length and type

    // Extract x, y, vx, vy
    float x, y, vx, vy;
    memcpy(&x, data + offset, sizeof(float)); offset += sizeof(float);
    memcpy(&y, data + offset, sizeof(float)); offset += sizeof(float);
    memcpy(&vx, data + offset, sizeof(float)); offset += sizeof(float);
    memcpy(&vy, data + offset, sizeof(float)); offset += sizeof(float);

    // Extract the fruit name (20 bytes, ensure null-termination)
    char nameBuf[21] = {0};
    memcpy(nameBuf, data + offset, 20);

    // Create and initialise the fruit locally
    Fruit *p_f = new Fruit(std::string(nameBuf));
    p_f->setPosition(df::Vector(x, y));
    p_f->setVelocity(df::Vector(vx, vy));

    // Optionally log to confirm receipt
    LM.writeLog("Received fruit spawn: (%f,%f) velocity (%f,%f) %s",
      x, y, vx, vy, nameBuf);
    }

  else if (msgType == static_cast<int>( MessageType::BOMB_SPAWN)) {
    const BombSpawnMsg *b_msg = (const BombSpawnMsg *) p_en->getMessage();
    Bomb *p_b = new Bomb(std::string(b_msg->bomb_name));
    p_b->setPosition(df::Vector(b_msg->x, b_msg->y));
    p_b->setVelocity(df::Vector(b_msg->vx, b_msg->vy));
  }

  return 1;
}