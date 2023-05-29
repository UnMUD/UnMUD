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
      USERLOG.Log("PlayerDatabase::LoadPlayer opened database successfully: " +
                  string(dbConnection.dbname()));
    } else {
      ERRORLOG.Log("PlayerDatabase::LoadPlayer can't open database\n");
      return;
    }

    /* Create SQL statement */
    string sql = fmt::format(
        "SELECT p.*, (p.attributes).*, ARRAY_AGG(i.itemId) AS itemIds "
        "FROM Player p "
        "LEFT JOIN Inventory i ON p.id = i.playerid "
        "WHERE p.name = '{}'"
        "GROUP BY p.id;",
        p_name);

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);

    /* Execute SQL query */
    pqxx::row queryResult(nonTransactionConnection.exec1(sql));
    nonTransactionConnection.commit();

    id = queryResult["id"].as<entityid>();
    m_map[id].ID() = id;
    ParseRow(queryResult, m_map[id]);
    USERLOG.Log("Loaded Player: " + m_map[id].Name());

    USERLOG.Log("PlayerDatabase::LoadPlayer done successfully");
    dbConnection.disconnect();
  } catch (const std::exception &e) {
    ERRORLOG.Log("PlayerDatabase::LoadPlayer " + string(e.what()));
    return;
  }
  return;
}

void PlayerDatabase::SavePlayer(entityid p_player) {
  std::map<entityid, Player>::iterator itr = m_map.find(p_player);
  if (itr == m_map.end())
    return;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
      USERLOG.Log("PlayerDatabase::SavePlayer opened database successfully: " +
                  string(dbConnection.dbname()));
    } else {
      ERRORLOG.Log("PlayerDatabase::SavePlayer can't open database\n");
      return;
    }
    string sql = fmt::format("SELECT EXISTS("
                             "SELECT 1 FROM Player "
                             "WHERE id = {})",
                             BasicLib::tostring(p_player));

    pqxx::nontransaction nonTransactionConnection(dbConnection);
    pqxx::row queryRow(nonTransactionConnection.exec1(sql));
    nonTransactionConnection.commit();

    pqxx::work transactionConnection(dbConnection);
    if (!queryRow["exists"].as<bool>()) {
      sql = fmt::format("INSERT INTO Player VALUES ( "
                        " {}, '', '', 'REGULAR', "
                        " 0, 0, 1, 1, 0, 10, 0, "
                        " ROW(0, 0, 0, 0, 0, 0, 0, 0, 0), NULL, NULL"
                        ")",
                        BasicLib::tostring(p_player));
      transactionConnection.exec(sql);
    }

    sql = fmt::format("UPDATE Player "
                      "SET {} "
                      "WHERE id = {}",
                      DumpSQL(itr->second), BasicLib::tostring(p_player));
    transactionConnection.exec(sql);
    transactionConnection.commit();

    SavePlayerInventory(p_player);
    USERLOG.Log("PlayerDatabase::SavePlayer done successfully");
    dbConnection.disconnect();
  } catch (const std::exception &e) {
    ERRORLOG.Log("PlayerDatabase::SavePlayer " + string(e.what()));
    return;
  }
  return;
}

void PlayerDatabase::SavePlayerInventory(entityid &p_player) {
  std::map<entityid, Player>::iterator itr = m_map.find(p_player);
  if (itr == m_map.end())
    return;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
      USERLOG.Log(
          "PlayerDatabase::SavePlayerInventory opened database successfully: " +
          string(dbConnection.dbname()));
    } else {
      ERRORLOG.Log("PlayerDatabase::SavePlayerInventory can't open database\n");
      return;
    }
    string sql = fmt::format("SELECT itemId "
                             "FROM Inventory "
                             "WHERE playerId = {}",
                             BasicLib::tostring(p_player));

    pqxx::nontransaction nonTransactionConnection(dbConnection);
    pqxx::result queryResult(nonTransactionConnection.exec(sql));
    nonTransactionConnection.commit();

    std::set<entityid> databaseItems;
    std::set<entityid> inGameItems;

    for (auto row : queryResult) {
      databaseItems.insert(row["itemId"].as<entityid>());
    }

    for (int i = 0; i < PLAYERITEMS; i++) {
      entityid itemId = itr->second.GetItem(i);
      if (itemId != 0) {
        inGameItems.insert(itemId);
      }
    }

    pqxx::work transactionConnection(dbConnection);

    string databaseItemsNotInGame = "(";
    for (entityid itemId : databaseItems) {
      if (inGameItems.find(itemId) == inGameItems.end()) {
        databaseItemsNotInGame += BasicLib::tostring(itemId) + ",";
      }
    }

    if (databaseItemsNotInGame.size() > 1) {
      databaseItemsNotInGame.pop_back();
      databaseItemsNotInGame.push_back(')');
      sql = fmt::format("DELETE FROM Inventory "
                        "WHERE playerId = {} "
                        "AND itemId IN {}",
                        p_player, databaseItemsNotInGame);
      transactionConnection.exec(sql);
    }

    string inGameItemsNotInDatabase = "";
    for (entityid itemId : inGameItems) {
      if (databaseItems.find(itemId) == databaseItems.end()) {
        inGameItemsNotInDatabase += fmt::format("({}, {}),", p_player, itemId);
      }
    }

    if (!inGameItemsNotInDatabase.empty()) {
      inGameItemsNotInDatabase.pop_back();
      sql = fmt::format("INSERT INTO Inventory "
                        "VALUES {}",
                        inGameItemsNotInDatabase);
      transactionConnection.exec(sql);
    }

    transactionConnection.commit();
    USERLOG.Log("PlayerDatabase::SavePlayerInventory done successfully");
    dbConnection.disconnect();
  } catch (const std::exception &e) {
    ERRORLOG.Log("PlayerDatabase::SavePlayerInventory " + string(e.what()));
    return;
  }
  return;
}

bool PlayerDatabase::Load() {
  entityid id;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
      USERLOG.Log("PlayerDatabase::Load opened database successfully: " +
                  string(dbConnection.dbname()));
    } else {
      ERRORLOG.Log("PlayerDatabase::Load can't open database\n");
      return false;
    }

    /* Create SQL statement */
    string sql = "SELECT p.*, (p.attributes).*, ARRAY_AGG(i.itemId) AS itemIds "
                 "FROM Player p "
                 "LEFT JOIN Inventory i ON p.id = i.playerid "
                 "GROUP BY p.id;";

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);

    /* Execute SQL query */
    pqxx::result queryResult(nonTransactionConnection.exec(sql));
    nonTransactionConnection.commit();

    /* List down all the records */
    for (auto const row : queryResult) {
      id = row["id"].as<entityid>();
      m_map[id].ID() = id;
      ParseRow(row, m_map[id]);
      USERLOG.Log("Loaded Player: " + m_map[id].Name());
    }
    USERLOG.Log("PlayerDatabase::Load done successfully");
    dbConnection.disconnect();
  } catch (const std::exception &e) {
    ERRORLOG.Log("PlayerDatabase::Load " + string(e.what()));
    return false;
  }
  return true;
}

bool PlayerDatabase::Save() {
  for (auto &player : GetInstance()) {
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
