// MUD Programming
// Ron Penton
// (C)2003
// PlayerDatabase.cpp - The class that stores all of the players within the MUD
//
//

#include "PlayerDatabase.h"
#include "BasicLib/BasicLib.h"
#include "SimpleMUDLogs.h"
#include <fstream>
#include <pqxx/pqxx>

using BasicLib::LowerCase;
using BasicLib::tostring;
using std::ifstream;
using std::ofstream;
using std::string;

namespace SimpleMUD {

PlayerDatabase &PlayerDatabase::GetInstance() {
  static PlayerDatabase playerDatabase;
  return playerDatabase;
}

void PlayerDatabase::LoadPlayer(string p_name) {
  entityid id;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("PlayerDatabase::LoadPlayer opened database successfully: " + std::string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("PlayerDatabase::LoadPlayer can't open database\n");
        return;
    }

    /* Create SQL statement */
    std::string sql = "SELECT p.*, (p.attributes).*, ARRAY_AGG(i.itemId) AS itemIds "
                      "FROM Player p "
                      "LEFT JOIN Inventory i ON p.id = i.playerid "
                      "WHERE p.name = " + p_name + " "
                      "GROUP BY p.id;";

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);
    
    /* Execute SQL query */
    pqxx::row queryResult( nonTransactionConnection.exec1( sql ));

    id = queryResult["id"].as<entityid>();
    m_map[id].ID() = id;
    ParseRow(queryResult, m_map[id]);
    USERLOG.Log("Loaded Player: " + m_map[id].Name());

    USERLOG.Log("PlayerDatabase::LoadPlayer done successfully");
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    ERRORLOG.Log("PlayerDatabase::LoadPlayer " + std::string(e.what()));
    return;
  }
  return;
}

void PlayerDatabase::SavePlayer(entityid p_player) {
  std::map<entityid, Player>::iterator itr = m_map.find(p_player);
  if (itr == m_map.end())
    return;

  std::string name = PlayerFileName(itr->second.Name());
  ofstream file(name.c_str());

  file << "[ID]             " << p_player << "\n";
  file << itr->second;
}

bool PlayerDatabase::Load() {
  entityid id;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("PlayerDatabase::Load opened database successfully: " + std::string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("PlayerDatabase::Load can't open database\n");
        return false;
    }

    /* Create SQL statement */
    std::string sql = "SELECT p.*, (p.attributes).*, ARRAY_AGG(i.itemId) AS itemIds "
                      "FROM Player p "
                      "LEFT JOIN Inventory i ON p.id = i.playerid "
                      "GROUP BY p.id;";


    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);
    
    /* Execute SQL query */
    pqxx::result queryResult( nonTransactionConnection.exec( sql ));

    /* List down all the records */
    for (auto const row : queryResult) {
      id = row["id"].as<entityid>();
      m_map[id].ID() = id;
      ParseRow(row, m_map[id]);
      USERLOG.Log("Loaded Player: " + m_map[id].Name());
    }
    USERLOG.Log("PlayerDatabase::Load done successfully");
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    ERRORLOG.Log("PlayerDatabase::Load " + std::string(e.what()));
    return false;
  }
  return true;
}

bool PlayerDatabase::Save() {
  ofstream file("players/players.txt");

  for (auto &player : GetInstance()) {
    file << player.Name() << "\n";
    SavePlayer(player.ID());
  }

  return true;
}

bool PlayerDatabase::AddPlayer(Player &p_player) {
  if (has(p_player.ID()))
    return false;
  if (hasfull(p_player.Name()))
    return false;

  // insert the player into the map
  m_map[p_player.ID()] = p_player;

  // add the players name to the players.txt file
  std::ofstream file("players/players.txt", std::ios::app);
  file << p_player.Name() << "\n";

  // write the initial player out to disk
  SavePlayer(p_player.ID());

  return true;
}

void PlayerDatabase::Logout(entityid p_player) {
  Player &p = get(p_player);

  USERLOG.Log(SocketLib::GetIPString(p.Conn()->GetRemoteAddress()) +
              " - User " + p.Name() + " logged off.");

  p.Conn() = 0;
  p.LoggedIn() = false;
  p.Active() = false;

  // make sure the player is saved to disk
  SavePlayer(p_player);
}

} // end namespace SimpleMUD
