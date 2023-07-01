// MUD Programming
// Ron Penton
// (C)2003
// Demo10-01.cpp - SimpleMUD - Enemies, Enemy Databases, and the Game Loop
//
//

#include <sstream>

#include "SocketLib/SocketLib.h"

#include "SimpleMUD/EnemyDatabase.h"
#include "SimpleMUD/ItemDatabase.h"
#include "SimpleMUD/PlayerDatabase.h"
#include "SimpleMUD/RoomDatabase.h"
#include "SimpleMUD/StoreDatabase.h"

#include "SimpleMUD/Game.h"
#include "SimpleMUD/GameLoop.h"
#include "SimpleMUD/Logon.h"

#include "SimpleMUD/SimpleMUDLogs.h"

using namespace SocketLib;
using namespace SimpleMUD;

int main(int argc, char **argv) {
  try {
    if(argc < 2){
      printf("Use: %s <Port>\n", argv[0]);
      return 0;
    }

    GameLoop gameloop;

    ListeningManager<Telnet, Logon> lm;

    const int maxdatarate = 128, sentimeout = 60, maxbuffered = 65536;
    ConnectionManager<Telnet, Logon> cm(maxdatarate, sentimeout, maxbuffered);

    lm.SetConnectionManager(&cm);

    const SocketLib::port port = BasicLib::totype<SocketLib::port>(argv[1]);
    lm.AddPort(port);

    printf("Game running at port %d\n", port);
    while (Game::Running()) {
      lm.Listen();
      cm.Manage();
      gameloop.Loop();
      ThreadLib::YieldThread();
    }

  }

  catch (SocketLib::Exception &e) {
    ERRORLOG.Log("Fatal Socket Error: " + e.PrintError());
  }

  catch (ThreadLib::Exception &) {
    ERRORLOG.Log("Fatal Thread Error");
  }

  catch (std::exception &e) {
    ERRORLOG.Log("Standard Error: " + std::string(e.what()));
  }

  catch (...) {
    ERRORLOG.Log("Unspecified Error");
  }
}
