// MUD Programming
// Ron Penton
// (C)2003
// StoreDatabase.cpp - The class that stores all of the stores within the MUD
//
//

#include "StoreDatabase.h"
#include "BasicLib/BasicLib.h"
#include "SimpleMUDLogs.h"
#include <fstream>

using BasicLib::LowerCase;
using BasicLib::tostring;

namespace SimpleMUD {

StoreDatabase &StoreDatabase::GetInstance() {
  static StoreDatabase storeDatabase;
  return storeDatabase;
}

bool StoreDatabase::Load() {
  std::ifstream file("stores/stores.str");
  entityid id;
  std::string temp;

  while (file.good()) {
    file >> temp >> id; // load ID
    m_map[id].ID() = id;
    file >> m_map[id] >> std::ws;                     // load store
    USERLOG.Log("Loaded Store: " + m_map[id].Name()); // log it
  }
  return true;
}

} // end namespace SimpleMUD
