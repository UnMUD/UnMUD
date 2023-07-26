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
#include <typeinfo>
#include <vector>

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
  entityid id;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
      USERLOG.Log("EnemyTemplateDatabase::Load opened database successfully: " +
                  std::string(dbConnection.dbname()));
    } else {
      ERRORLOG.Log("EnemyTemplateDatabase::Load can't open database\n");
      return;
    }

    /* Create SQL statement */
    std::string sql = "SELECT * from Enemy";

    /* Create a non-transactional object. */
    pqxx::nontransaction nonTransactionConnection(dbConnection);

    /* Execute SQL query */
    pqxx::result queryResult(nonTransactionConnection.exec(sql));
    m_vector.resize(queryResult.size() + 1);

    /* List down all the records */
    for (auto const row : queryResult) {
      id = row["id"].as<entityid>();
      m_vector[id].ID() = id;

      std::string subQuery = "SELECT itemId, itemQuantity "
                             "FROM Loot "
                             "WHERE enemyId = " +
                             std::string(row["id"].c_str());

      pqxx::result subQueryResult(nonTransactionConnection.exec(subQuery));
      ParseRow(row, subQueryResult, m_vector[id]);
      USERLOG.Log("Loaded Enemy: " + m_vector[id].Name());
    }
    nonTransactionConnection.commit();
    USERLOG.Log("EnemyTemplateDatabase::Load done successfully");
    dbConnection.disconnect();
  } catch (const std::exception &e) {
    ERRORLOG.Log("EnemyTemplateDatabase::Load " + std::string(e.what()));
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
  entityid id;

  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
      USERLOG.Log("EnemyDatabase::Load opened database successfully: " +
                  std::string(dbConnection.dbname()));
    } else {
      ERRORLOG.Log("EnemyDatabase::Load can't open database\n");
      return;
    }

    /* Create SQL statement */
    std::string sql = "SELECT * from EnemyInstance";

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
      m_map[id].CurrentRoom()->AddEnemy(id);
      USERLOG.Log("Loaded Enemy Instance: " + m_map[id].Name());
    }
    USERLOG.Log("EnemyDatabase::Load done successfully");
    dbConnection.disconnect();
  } catch (const std::exception &e) {
    ERRORLOG.Log("EnemyDatabase::Load " + std::string(e.what()));
    return;
  }
  return;
}

void EnemyDatabase::Save() {
  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
      USERLOG.Log("EnemyDatabase::Save opened database successfully: " +
                  string(dbConnection.dbname()));
    } else {
      ERRORLOG.Log("EnemyDatabase::Save can't open database\n");
      return;
    }

    for (auto &enemy : GetInstance()) {
      string sql = fmt::format("SELECT EXISTS("
                               "SELECT 1 FROM EnemyInstance "
                               "WHERE id = {})",
                               BasicLib::tostring(enemy.ID()));

      pqxx::nontransaction nonTransactionConnection(dbConnection);
      pqxx::row queryRow(nonTransactionConnection.exec1(sql));
      nonTransactionConnection.commit();

      pqxx::work transactionConnection(dbConnection);
      if (!queryRow["exists"].as<bool>()) {
        sql = fmt::format("INSERT INTO EnemyInstance VALUES ({}, 1, 0, 1, 0)",
                          BasicLib::tostring(enemy.ID()));
        transactionConnection.exec(sql);
      }

      sql = fmt::format("UPDATE EnemyInstance "
                        "SET {} "
                        "WHERE id = {}",
                        DumpSQL(enemy), BasicLib::tostring(enemy.ID()));

      transactionConnection.exec(sql);
      transactionConnection.commit();
    }

    RemoveDeadEnemies();
    USERLOG.Log("EnemyDatabase::Save done successfully");
    dbConnection.disconnect();
  } catch (const std::exception &e) {
    ERRORLOG.Log("EnemyDatabase::Save " + string(e.what()));
    return;
  }
  return;
}

void EnemyDatabase::RemoveDeadEnemies() {
  try {
    pqxx::connection dbConnection;
    if (dbConnection.is_open()) {
      USERLOG.Log(
          "EnemyDatabase::RemoveDeadEnemies opened database successfully: " +
          string(dbConnection.dbname()));
    } else {
      ERRORLOG.Log("EnemyDatabase::RemoveDeadEnemies can't open database\n");
      return;
    }

    string sql = "SELECT id FROM EnemyInstance";

    pqxx::nontransaction nonTransactionConnection(dbConnection);
    pqxx::result queryResult(nonTransactionConnection.exec(sql));
    nonTransactionConnection.commit();

    std::set<entityid> databaseEnemies;
    std::set<entityid> inGameEnemies;

    for (auto row : queryResult) {
      databaseEnemies.insert(row["id"].as<entityid>());
    }

    for (auto &enemy : GetInstance()) {
      inGameEnemies.insert(enemy.ID());
    }

    string databaseEnemiesNotInGame = "(";
    for (entityid enemyId : databaseEnemies) {
      if (inGameEnemies.find(enemyId) == inGameEnemies.end()) {
        databaseEnemiesNotInGame += BasicLib::tostring(enemyId) + ",";
      }
    }

    pqxx::work transactionConnection(dbConnection);

    if (databaseEnemiesNotInGame.size() > 1) {
      databaseEnemiesNotInGame.pop_back();
      databaseEnemiesNotInGame.push_back(')');
      sql = fmt::format("DELETE FROM EnemyInstance "
                        "WHERE id IN {}",
                        databaseEnemiesNotInGame);
      transactionConnection.exec(sql);
    }
    transactionConnection.commit();

    USERLOG.Log("EnemyDatabase::RemoveDeadEnemies done successfully");
    dbConnection.disconnect();
  } catch (const std::exception &e) {
    ERRORLOG.Log("EnemyDatabase::RemoveDeadEnemies " + string(e.what()));
    return;
  }
  return;
}

} // end namespace SimpleMUD
