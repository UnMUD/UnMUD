// MUD Programming
// Ron Penton
// (C)2003
// Enemy.h - The class defining Enemies in the SimpleMUD
//
//

#ifndef ENEMY_H
#define ENEMY_H

#include "BasicLib/BasicLib.h"
#include "DatabasePointer.h"
#include "Item.h"
#include <list>
#include <math.h>

using BasicLib::sint64;
using std::istream;
using std::list;
using std::ostream;

namespace SimpleMUD {

typedef std::pair<item, int> loot;

class EnemyTemplate : public Entity {
public:
  EnemyTemplate();
  friend istream &operator>>(istream &p_stream, EnemyTemplate &t);
  friend void ParseRow(const pqxx::const_result_iterator::reference &row, 
                        const pqxx::result &lootResult, EnemyTemplate &t);

  int m_hitpoints;
  int m_accuracy;
  int m_dodging;
  int m_strikedamage;
  int m_damageabsorb;
  int m_experience;
  item m_weapon;
  money m_moneymin;
  money m_moneymax;

  list<loot> m_loot;

}; // end class EnemyTemplate

class Enemy : public Entity {
public:
  Enemy();

  int &HitPoints() { return m_hitpoints; }
  room &CurrentRoom() { return m_room; }
  sint64 &NextAttackTime() { return m_nextattacktime; }

  void LoadTemplate(enemytemplate p_template);

  std::string &Name();
  int Accuracy();
  int Dodging();
  int StrikeDamage();
  int DamageAbsorb();
  int Experience();
  item Weapon();
  money MoneyMin();
  money MoneyMax();
  list<loot> &LootList();

  friend ostream &operator<<(ostream &p_stream, const Enemy &e);
  friend istream &operator>>(istream &p_stream, Enemy &e);
  friend std::string DumpSQL(Enemy &e);
  friend void ParseRow(const pqxx::const_result_iterator::reference &row, Enemy &e);

protected:
  enemytemplate m_template;
  int m_hitpoints;
  room m_room;
  BasicLib::sint64 m_nextattacktime;

}; // end class Enemy

istream &operator>>(istream &p_stream, EnemyTemplate &e);
ostream &operator<<(ostream &p_stream, const Enemy &e);
istream &operator>>(istream &p_stream, Enemy &e);
std::string DumpSQL(Enemy &e);
void ParseRow(const pqxx::const_result_iterator::reference &row, Enemy &e);

} // end namespace SimpleMUD

#endif
