// MUD Programming
// Ron Penton
// (C)2003
// EnemyDatabase.h - The class that stores all of the enemies within the MUD
//
//

#include "EnemyDatabase.h"
#include "BasicLib/BasicLib.h"
#include "Room.h"
#include "SimpleMUDLogs.h"
#include <fstream>
#include <vector>
#include <typeinfo>

using BasicLib::LowerCase;
using BasicLib::tostring;
using std::ifstream;
using std::ofstream;
using std::string;

namespace SimpleMUD {

EnemyTemplateDatabase &EnemyTemplateDatabase::GetInstance() {
  static EnemyTemplateDatabase enemyTemplateDatabase;
  return enemyTemplateDatabase;
}

void EnemyTemplateDatabase::Load() {
  // ifstream file("enemies/enemies.templates");
  entityid id;
  // std::string temp;

  // while (file.good()) {
  //   // read in the ID
  //   file >> temp >> id;

  //   // make sure there's enough room for the enemy
  //   if (m_vector.size() <= id)
  //     m_vector.resize(id + 1);

  //   m_vector[id].ID() = id;
  //   file >> m_vector[id] >> std::ws;
  //   USERLOG.Log("Loaded Enemy: " + m_vector[id].Name());
  // }

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
        USERLOG.Log("EnemyTemplateDatabase::Load opened database successfully: " + std::string(dbConnection.dbname()));
    } else {
        ERRORLOG.Log("EnemyTemplateDatabase::Load can't open database\n");
        return;
    }

    /* Create SQL statement */
    std::string sql = "SELECT * from Enemy";

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);
    
    /* Execute SQL query */
    pqxx::result queryResult( nonTransactionConnection.exec( sql ));
    m_vector.resize(queryResult.size() + 1);

    /* List down all the records */
    for (auto const row : queryResult) {
      id = row["id"].as<entityid>();
      m_vector[id].ID() = id;
      
      std::string subQuery = "SELECT itemId, itemQuantity "
                            "FROM Loot "
                            "WHERE enemyId = " + 
                            std::string(row["id"].c_str()) + " "
                            "ORDER BY itemId;";
      pqxx::result subQueryResult( nonTransactionConnection.exec( subQuery ));
      ParseRow(row, subQueryResult, m_vector[id]);
      USERLOG.Log("Loaded Enemy: " + m_vector[id].Name());
    }
    USERLOG.Log("EnemyTemplateDatabase::Load done successfully");
    dbConnection.disconnect ();
  } catch (const std::exception &e) {
    ERRORLOG.Log("EnemyTemplateDatabase::Load" + std::string(e.what()));
    return;
  }
  return;
}

EnemyDatabase &EnemyDatabase::GetInstance() {
  static EnemyDatabase enemyDatabase;
  return enemyDatabase;
}

void EnemyDatabase::Create(entityid p_template, room p_room) {
  entityid id = FindOpenID();
  Enemy &e = m_map[id];

  e.ID() = id;
  e.LoadTemplate(p_template);
  e.CurrentRoom() = p_room;

  p_room->AddEnemy(id);
}

void EnemyDatabase::Delete(enemy p_enemy) {
  p_enemy->CurrentRoom()->RemoveEnemy(p_enemy);
  m_map.erase(p_enemy);
}

void EnemyDatabase::Load() {
  ifstream file("enemies/enemies.instances");
  entityid id;
  std::string temp;

  file >> std::ws; // eat the whitespace
  while (file.good()) {
    file >> temp >> id;
    m_map[id].ID() = id;
    file >> m_map[id] >> std::ws;
    m_map[id].CurrentRoom()->AddEnemy(id);
  }
}

void EnemyDatabase::Save() {
  ofstream file("enemies/enemies.instances");

  for (auto &enemy : GetInstance()) {
    file << "[ID]             " << enemy.ID() << "\n";
    file << enemy << "\n";
  }
}

} // end namespace SimpleMUD
