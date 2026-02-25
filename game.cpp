//
// game.cpp - Fruit Ninja
// 

// Engine includes.
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "utility.h"
#include <sys/types.h>  
#include <unistd.h>     
#include "Grocer.h"

// Game includes.
#include "game.h"
#include "util.h"
#include "Sword.h"
#include "server.h"
#include "client.h"

///////////////////////////////////////////////
int main(int argc, char *argv[]) {
  bool is_server;
  //handout if 0 is server, 1 is client
  if (argc != 1 && argc != 2) {
    fprintf(stderr, "Usage: game [server name]\n");
    exit(1);
  }

  // Setup logfile: server or client (from Handout)
  if (argc == 1) {
    is_server = true;
    setenv("DRAGONFLY_LOG", "server.log", 1);
    setenv("DRAGONFLY_CONFIG", "df-config-server.txt", 1);
  } else {
    is_server = false;
    std::string logfile = "client" + std::to_string(getpid()) + ".log";
    setenv("DRAGONFLY_LOG", logfile.c_str(), 1);
  }

  // Start up game manager.
  if (GM.startUp())  {
    LM.writeLog("Error starting game manager!");
    GM.shutDown();
    return 0;
  }

loadResources();

 if (is_server) {
    new Server;
    new Sword; 
    new Grocer;
  } else {
    new Client(argv[1]);
    new Sword; 
  }

  // Setup logging.
  LM.setFlush(true);
  LM.setLogLevel(1);
  LM.writeLog("Fruit Ninja (v%.1f)", VERSION);

  // Dragonfly splash screen.
  //df::splash();

  // Fruit Ninja splash screen.
  //splash();

  GM.run();

  // Shut everything down.
  GM.shutDown();

  // All is well.
  return 0;
}
