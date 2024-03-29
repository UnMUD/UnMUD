// MUD Programming
// Ron Penton
// (C)2003
// GameLoop.h - This class performs the game loop in SimpleMUD
//
//

#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <string>

#include "EnemyDatabase.h"
#include "Game.h"
#include "ItemDatabase.h"
#include "PlayerDatabase.h"
#include "RoomDatabase.h"
#include "SimpleMUDLogs.h"
#include "StoreDatabase.h"

using std::string;

namespace SimpleMUD {

class GameLoop {
public:
  GameLoop() : m_savedatabases(), m_nextround(), m_nextregen(), m_nextheal() {
    LoadDatabases();
  }
  ~GameLoop() { SaveDatabases(); }

  void LoadDatabases(); // load all databases
  void SaveDatabases(); // save all databases
  void Loop();          // perform one loop iteration

  void Load();         // load gameloop data
  void Save();         // save gameloop data
  void PerformRound(); // perform combat round
  void PerformRegen(); // perform enemy regen round
  void PerformHeal();  // perform healing round

protected:
  BasicLib::sint64 m_savedatabases;
  BasicLib::sint64 m_nextround;
  BasicLib::sint64 m_nextregen;
  BasicLib::sint64 m_nextheal;

}; // end class GameLoop

} // end namespace SimpleMUD

#endif
