// MUD Programming
// Ron Penton
// (C)2003
// PlayerDatabase.h - The class that stores all of the players within the MUD
//
//

#ifndef SIMPLEMUDPLAYERDATABASE_H
#define SIMPLEMUDPLAYERDATABASE_H

#include <cmath>
#include <map>
#include <set>
#include <string>

#include "DatabasePointer.h"
#include "EntityDatabase.h"
#include "Player.h"

namespace SimpleMUD {

class PlayerDatabase : public EntityDatabase<Player> {
public:
  static PlayerDatabase &GetInstance();
  PlayerDatabase(PlayerDatabase const &) = delete;
  PlayerDatabase(PlayerDatabase &&) = delete;
  PlayerDatabase &operator=(PlayerDatabase const &) = delete;
  PlayerDatabase &operator=(PlayerDatabase &&) = delete;

  bool Load();
  bool Save();
  bool AddPlayer(Player &p_player);

  // helpers
  inline string PlayerFileName(const string &p_name);
  void LoadPlayer(string p_name);
  void SavePlayer(entityid p_player);

  entityid LastID() {
    // since the ID's are sorted in ascending order within the map,
    // the highest ID will be the ID of the last item.
    return m_map.rbegin()->first;
  }

  iterator findactive(const std::string &p_name) {
    return BasicLib::double_find_if(begin(), end(), matchentityfull(p_name),
                                    matchentity(p_name), playeractive());
  }

  iterator findloggedin(const std::string &p_name) {
    return BasicLib::double_find_if(begin(), end(), matchentityfull(p_name),
                                    matchentity(p_name), playerloggedin());
  }

  void Logout(entityid p_player);

private:
  PlayerDatabase() {}

}; // end class PlayerDatabase

inline string PlayerDatabase::PlayerFileName(const string &p_name) {
  return string("players/" + p_name + ".plr");
}

} // end namespace SimpleMUD

#endif
