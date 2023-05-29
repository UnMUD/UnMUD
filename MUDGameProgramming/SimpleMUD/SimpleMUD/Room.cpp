// MUD Programming
// Ron Penton
// (C)2003
// Room.cpp - The class defining Rooms in the SimpleMUD
//
//

#include <iostream>
#include <string>

#include "BasicLib/BasicLib.h"
#include "Enemy.h"
#include "Room.h"

using BasicLib::extract;
using std::istream;
using std::ostream;

namespace SimpleMUD {

Room::Room()
    : m_type(PLAINROOM), m_data(0), m_description("UNDEFINED"), m_spawnwhich(0),
      m_maxenemies(0), m_money(0), m_items(), m_players(), m_enemies() {

  for (int d = 0; d < NUMDIRECTIONS; d++)
    m_rooms[d] = 0;
}

void Room::AddPlayer(player p_player) { m_players.push_back(p_player); }

void Room::RemovePlayer(player p_player) {
  m_players.erase(std::find(m_players.begin(), m_players.end(),
                            static_cast<entityid>(p_player)));
}

item Room::FindItem(const string &p_item) {
  std::list<item>::iterator itr =
      BasicLib::double_find_if(m_items.begin(), m_items.end(),
                               matchentityfull(p_item), matchentity(p_item));

  if (itr == m_items.end())
    return 0;

  return *itr;
}

void Room::AddItem(item p_item) {
  // remove the first (oldest) item if there's too many in the room.
  if (m_items.size() >= 32)
    m_items.pop_front();

  // add the new item.
  m_items.push_back(p_item);
}

void Room::RemoveItem(item p_item) {
  m_items.erase(
      std::find(m_items.begin(), m_items.end(), static_cast<entityid>(p_item)));
}

enemy Room::FindEnemy(const string &p_enemy) {
  std::list<enemy>::iterator itr =
      BasicLib::double_find_if(m_enemies.begin(), m_enemies.end(),
                               matchentityfull(p_enemy), matchentity(p_enemy));

  if (itr == m_enemies.end())
    return 0;

  return *itr;
}

void Room::AddEnemy(enemy p_enemy) { m_enemies.push_back(p_enemy); }

void Room::RemoveEnemy(enemy p_enemy) {
  m_enemies.erase(std::find(m_enemies.begin(), m_enemies.end(),
                            static_cast<entityid>(p_enemy)));
}

void Room::LoadTemplate(istream &p_stream) {
  string temp;

  p_stream >> temp >> std::ws;
  std::getline(p_stream, m_name);
  p_stream >> temp >> std::ws;
  std::getline(p_stream, m_description);
  p_stream >> temp >> temp;
  m_type = GetRoomType(temp);
  p_stream >> temp >> m_data;

  for (int d = 0; d < NUMDIRECTIONS; d++)
    p_stream >> temp >> m_rooms[d];

  p_stream >> temp >> m_spawnwhich;
  p_stream >> temp >> m_maxenemies;
}

void Room::LoadTemplate(const pqxx::const_result_iterator::reference &row,
                        const pqxx::result &connectionsResult) {
  row["name"] >> m_name;
  row["description"] >> m_description;
  m_description = BasicLib::SearchAndReplace(m_description, "\\x1B", "\x1B");
  m_type = GetRoomType(row["type"].as<std::string>());
  m_data = (row["storeId"].is_null() ? 0 : row["storeId"].as<entityid>());

  for (int d = 0; d < NUMDIRECTIONS; d++)
    m_rooms[d] = 0;

  for (auto connection : connectionsResult) {
    m_rooms[GetDirection(connection["directionEnum"].as<std::string>())] =
        connection["connectedTo"].as<entityid>();
  }

  m_spawnwhich = (row["enemyId"].is_null() ? 0 : row["enemyId"].as<entityid>());
  row["maxEnemies"] >> m_maxenemies;
}

void Room::LoadData(istream &p_stream) {
  string temp;
  p_stream >> temp;

  // clear all the existing items, then load in all the new items
  m_items.clear();
  entityid last;
  while (extract(p_stream, last) != 0)
    m_items.push_back(last);

  // load in the money
  p_stream >> temp;
  p_stream >> m_money;
}

void Room::LoadData(const pqxx::const_result_iterator::reference &row) {
  // clear all the existing items, then load in all the new items
  m_items.clear();
  auto arr = row["itemIds"].as_array();
  std::pair<pqxx::array_parser::juncture, string> elem;
  do {
    elem = arr.get_next();
    if (elem.first == pqxx::array_parser::juncture::string_value) {
      m_items.push_back(BasicLib::totype<entityid>(elem.second));
    }
  } while (elem.first != pqxx::array_parser::juncture::done);

  // load in the money
  row["money"] >> m_money;
}

void Room::SaveData(ostream &p_stream) {
  p_stream << "[ITEMS] ";

  for (auto &items : m_items) {
    p_stream << items << " ";
  }

  p_stream << "0\n";

  p_stream << "[MONEY] " << m_money << "\n";
}

} // end namespace SimpleMUD
