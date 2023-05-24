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
  entityid id;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("StoreDatabase::Load opened database successfully: " + std::string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("StoreDatabase::Load can't open database\n");
        return false;
    }

    /* Create SQL statement */
    std::string sql = "SELECT s.id, s.name, ARRAY_AGG(sv.itemId) AS itemIds "
                      "FROM Store s "
                      "LEFT JOIN StoreVendeItem sv ON s.id = sv.storeId "
                      "GROUP BY s.id, s.name;";

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);
    
    /* Execute SQL query */
    pqxx::result queryResult( nonTransactionConnection.exec( sql ));

    /* List down all the records */
    for (auto const row : queryResult) {
      id = row["id"].as<entityid>();
      m_map[id].ID() = id;
      ParseRow(row, m_map[id]);
      USERLOG.Log("Loaded Store: " + m_map[id].Name());
    }
    std::cout << "Operation done successfully" << std::endl;
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

} // end namespace SimpleMUD
