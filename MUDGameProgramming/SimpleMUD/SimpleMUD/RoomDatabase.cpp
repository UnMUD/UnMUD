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
  std::ofstream file("maps/default.data");

  for (auto &room : GetInstance()) {
    file << "[ROOMID] " << room.ID() << "\n";
    m_vector[room.ID()].SaveData(file);
    file << "\n";
  }
}

} // end namespace SimpleMUD
