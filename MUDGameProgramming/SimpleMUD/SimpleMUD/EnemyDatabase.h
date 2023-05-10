// MUD Programming
// Ron Penton
// (C)2003
// EnemyDatabase.h - The class that stores all of the enemies within the MUD
//
//

#ifndef ENEMYDATABASE_H
#define ENEMYDATABASE_H

#include "DatabasePointer.h"
#include "Enemy.h"
#include "EntityDatabase.h"

namespace SimpleMUD {

class EnemyTemplateDatabase : public EntityDatabaseVector<EnemyTemplate> {
public:
  static EnemyTemplateDatabase &GetInstance();
  EnemyTemplateDatabase(EnemyTemplateDatabase const &) = delete;
  EnemyTemplateDatabase(EnemyTemplateDatabase &&) = delete;
  EnemyTemplateDatabase &operator=(EnemyTemplateDatabase const &) = delete;
  EnemyTemplateDatabase &operator=(EnemyTemplateDatabase &&) = delete;

  void Load();

private:
  EnemyTemplateDatabase() {}
}; // end class EnemyTemplateDatabase

class EnemyDatabase : public EntityDatabase<Enemy> {
public:
  static EnemyDatabase &GetInstance();
  EnemyDatabase(EnemyDatabase const &) = delete;
  EnemyDatabase(EnemyDatabase &&) = delete;
  EnemyDatabase &operator=(EnemyDatabase const &) = delete;
  EnemyDatabase &operator=(EnemyDatabase &&) = delete;

  void Create(entityid p_template, room p_room);
  void Delete(enemy p_enemy);
  void Load();
  void Save();

private:
  EnemyDatabase() {}
}; // end class EnemyDatabase

} // end namespace SimpleMUD

#endif
