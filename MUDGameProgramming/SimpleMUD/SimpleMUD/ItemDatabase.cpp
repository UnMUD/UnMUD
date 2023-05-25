// MUD Programming
// Ron Penton
// (C)2003
// ItemDatabase.cpp - The class that stores all of the items within the MUD
//
//

#include "Attributes.h"
#include "ItemDatabase.h"
#include "BasicLib/BasicLib.h"
#include "SimpleMUDLogs.h"
#include <fstream>
#include <iostream>
#include <pqxx/pqxx>

using BasicLib::LowerCase;
using BasicLib::tostring;

namespace SimpleMUD {

ItemDatabase &ItemDatabase::GetInstance() {
  static ItemDatabase itemDatabase;
  return itemDatabase;
}

bool ItemDatabase::Load() {
  entityid id;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("ItemDatabase::Load opened database successfully: " + std::string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("ItemDatabase::Load can't open database\n");
        return false;
    }

    /* Create SQL statement */
    std::string sql = "SELECT *, (attributes).* from Item";

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);
    
    /* Execute SQL query */
    pqxx::result queryResult( nonTransactionConnection.exec( sql ));
    nonTransactionConnection.commit();

    /* List down all the records */
    for (auto const row : queryResult) {
      id = row["id"].as<entityid>();
      m_map[id].ID() = id;
      ParseRow(row, m_map[id]);
      USERLOG.Log("Loaded Item: " + m_map[id].Name());
    }
    USERLOG.Log("ItemDatabase::Load done successfully");
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    ERRORLOG.Log("ItemDatabase::Load " + std::string(e.what()));
    return false;
  }
  return true;
}

} // end namespace SimpleMUD
