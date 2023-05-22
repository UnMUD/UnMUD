// MUD Programming
// Ron Penton
// (C)2003
// ItemDatabase.cpp - The class that stores all of the items within the MUD
//
//

#include "ItemDatabase.h"
#include "BasicLib/BasicLib.h"
#include "SimpleMUDLogs.h"
#include <fstream>

using BasicLib::LowerCase;
using BasicLib::tostring;

namespace SimpleMUD {

ItemDatabase &ItemDatabase::GetInstance() {
  static ItemDatabase itemDatabase;
  return itemDatabase;
}

bool ItemDatabase::Load() {
  std::ifstream file("items/items.itm");
  entityid id;
  std::string temp;

  while (file.good()) {
    file >> temp >> id;
    m_map[id].ID() = id;
    file >> m_map[id] >> std::ws;
    USERLOG.Log("Loaded Item: " + m_map[id].Name());
  }
  return true;
}

} // end namespace SimpleMUD
