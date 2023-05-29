// MUD Programming
// Ron Penton
// (C)2003
// PlayerDatabase.cpp - The class that stores all of the players within the MUD
//
//

#include "RoomDatabase.h"
#include "BasicLib/BasicLib.h"
#include "SimpleMUDLogs.h"
#include <fstream>
#include <vector>

using BasicLib::LowerCase;
using BasicLib::tostring;
using std::ifstream;
using std::ofstream;
using std::string;

namespace SimpleMUD {

RoomDatabase &RoomDatabase::GetInstance() {
  static RoomDatabase roomDatabase;
  return roomDatabase;
}

void RoomDatabase::LoadTemplates() {
  entityid id;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("RoomDatabase::LoadTemplates opened database successfully: " + std::string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("RoomDatabase::LoadTemplates can't open database\n");
        return;
    }

    /* Create SQL statement */
    std::string sql = "SELECT * FROM Map;";

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);
    
    /* Execute SQL query */
    pqxx::result queryResult( nonTransactionConnection.exec( sql ));
    m_vector.resize(queryResult.size() + 1);

    /* List down all the records */
    for (auto const row : queryResult) {
      id = row["id"].as<entityid>();
      m_vector[id].ID() = id;

      std::string subQuery = "SELECT directionEnum, connectedTo "
                             "FROM Conecta "
                             "WHERE connectedFrom = " + 
                             std::string(row["id"].c_str());
      pqxx::result subQueryResult( nonTransactionConnection.exec( subQuery ));

      m_vector[id].LoadTemplate(row, subQueryResult);
      USERLOG.Log("Loaded Room: " + m_vector[id].Name());
    }
    nonTransactionConnection.commit();
    USERLOG.Log("RoomDatabase::LoadTemplates done successfully");
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    ERRORLOG.Log("RoomDatabase::LoadTemplates " + std::string(e.what()));
    return;
  }
  return;
}

void RoomDatabase::LoadData() {
  entityid roomid;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("RoomDatabase::LoadData opened database successfully: " + std::string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("RoomDatabase::LoadData can't open database\n");
        return;
    }

    /* Create SQL statement */
    std::string sql = "SELECT m.id, m.money, ARRAY_AGG(mg.itemId) AS itemIds "
                      "FROM MapVolatile m "
                      "LEFT JOIN MapVolatileGuardaItem mg ON m.id = mg.mapVolatileId "
                      "GROUP BY m.id, m.money;";

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);
    
    /* Execute SQL query */
    pqxx::result queryResult( nonTransactionConnection.exec( sql ));
    nonTransactionConnection.commit();
    m_vector.resize(queryResult.size() + 1);

    /* List down all the records */
    for (auto const row : queryResult) {
      roomid = row["id"].as<entityid>();
      m_vector[roomid].ID() = roomid;
      m_vector[roomid].LoadData(row);
      USERLOG.Log("Loaded Room Volatile: " + m_vector[roomid].Name());
    }
    USERLOG.Log("RoomDatabase::LoadData done successfully");
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    ERRORLOG.Log("RoomDatabase::LoadData " + std::string(e.what()));
    return;
  }
  return;
}

void RoomDatabase::SaveData() {
  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("RoomDatabase::SaveData opened database successfully: " + std::string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("RoomDatabase::SaveData can't open database\n");
        return;
    }

    for (auto &room : GetInstance()) {
      pqxx::work transactionConnection(dbConnection);
      string sql = fmt::format(
        "UPDATE MapVolatile "
        "SET money = {} "
        "WHERE id = {}",
        m_vector[room.ID()].Money(), BasicLib::tostring(room.ID())
      );
      transactionConnection.exec( sql );
      transactionConnection.commit();
      SaveDataItems(room.ID());
    }

    USERLOG.Log("RoomDatabase::SaveData done successfully");
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    ERRORLOG.Log("RoomDatabase::SaveData " + std::string(e.what()));
    return;
  }
  return;
}

void RoomDatabase::SaveDataItems(entityid &p_room) {
  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("RoomDatabase::SaveDataItems opened database successfully: " + string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("RoomDatabase::SaveDataItems can't open database\n");
        return;
    }
    string sql = fmt::format(
                        "SELECT itemId "
                        "FROM MapVolatileGuardaItem "
                        "WHERE mapVolatileId = {}",
                        BasicLib::tostring(p_room)
                      );
    
    pqxx::nontransaction nonTransactionConnection(dbConnection);
    pqxx::result queryResult( nonTransactionConnection.exec( sql ));
    nonTransactionConnection.commit();

    std::set<entityid> databaseItems;
    std::set<entityid> inGameItems;
    
    for(auto row : queryResult) {
      databaseItems.insert(row["itemId"].as<entityid>());
    }

    for(auto item : m_vector[p_room].Items()) {
      inGameItems.insert(item.m_id);
    }

    pqxx::work transactionConnection(dbConnection);

    string databaseItemsNotInGame = "(";
    for (entityid itemId : databaseItems) {
      if (inGameItems.find(itemId) == inGameItems.end()) {
        databaseItemsNotInGame += BasicLib::tostring(itemId) + ",";
      }
    }

    if(databaseItemsNotInGame.size() > 1) {
      databaseItemsNotInGame.pop_back();
      databaseItemsNotInGame.push_back(')');
      sql = fmt::format(
        "DELETE FROM MapVolatileGuardaItem "
        "WHERE mapVolatileId = {} "
        "AND itemId IN {}",
        p_room, databaseItemsNotInGame
      );
      transactionConnection.exec( sql );
    }

    string inGameItemsNotInDatabase = "";
    for (entityid itemId : inGameItems) {
      if (databaseItems.find(itemId) == databaseItems.end()) {
        inGameItemsNotInDatabase += fmt::format(
          "({}, {}),", itemId, p_room
        );
      }
    }

    if(!inGameItemsNotInDatabase.empty()){
      inGameItemsNotInDatabase.pop_back();
      sql = fmt::format(
        "INSERT INTO MapVolatileGuardaItem "
        "VALUES {}",
        inGameItemsNotInDatabase
      );
      transactionConnection.exec( sql );
    }

    transactionConnection.commit();
    USERLOG.Log("RoomDatabase::SaveDataItems done successfully");
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    ERRORLOG.Log("RoomDatabase::SaveDataItems " + string(e.what()));
    return;
  }
  return;
}

} // end namespace SimpleMUD
