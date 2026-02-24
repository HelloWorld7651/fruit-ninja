//
// client.cpp
//

#include <string.h>
#include "EventNetwork.h"
#include "EventStep.h"
#include "GameManager.h"
#include "LogManager.h"
#include "NetworkManager.h"
#include "WorldManager.h"
#include "utility.h"

#include "game.h"
#include "client.h"
#include "Sword.h"

Client::Client(std::string server_name) {
  setType("Client");
  NM.setServer(false);
  m_connected = false; 

  // Register for Network and Step events (no more mouse event spam!)
  registerInterest(df::NETWORK_EVENT);
  registerInterest(df::STEP_EVENT);

  std::string server_port = df::DRAGONFLY_PORT;
  LM.writeLog("Client::Client(): Connecting to %s at port %s.", server_name.c_str(), server_port.c_str());
  if (NM.connect(server_name, server_port) < 0) {
    LM.writeLog("Client::Client(): Error! Unable to connect.");
    exit(-1);
  }
}
int Client::eventHandler(const df::Event *p_e) {

  // Send the sword position once per frame
  if (p_e->getType() == df::STEP_EVENT) {
    if (!m_connected) return 0; 

    df::ObjectList swords = WM.objectsOfType(SWORD_STRING);
    if (swords.getCount() > 0) {
      df::Vector pos = swords[0]->getPosition();

      // Only send if the position actually changed
      static df::Vector last_pos(-1, -1);
      if (pos == last_pos) {
        return 0; 
      }
      last_pos = pos;

      // Clean, structured data packing
      SwordPosMsg msg;
      msg.msg_size = sizeof(SwordPosMsg); // Engine needs to know total size
      msg.type = MessageType::SWORD_POS;
      msg.x = pos.getX();
      msg.y = pos.getY();

      // Send the struct directly
      NM.send(&msg, msg.msg_size); 
    }
    return 0; 
  }

  // Handle network events
  if (p_e->getType() == df::NETWORK_EVENT) {
    const df::EventNetwork *p_ne = (const df::EventNetwork *) p_e;

    if (p_ne->getLabel() == df::NetworkEventLabel::CONNECT) {
      m_connected = true; 
      LM.writeLog("Client: Connected! Now ready to send data.");
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

int Client::handleData(const df::EventNetwork *p_en) {
  int msg_size = p_en->getBytes();
  char *buff = (char *) malloc(msg_size);
  if (!buff) return -1;
  
  memcpy(buff, p_en->getMessage(), msg_size);
  MessageType type;
  memcpy(&type, buff + sizeof(int), sizeof(MessageType));

  if (type == MessageType::EXIT) {
    GM.setGameOver();
  }

  free(buff);
  return 1;
}