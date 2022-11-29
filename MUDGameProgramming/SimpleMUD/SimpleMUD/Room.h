// MUD Programming
// Ron Penton
// (C)2003
// Room.h - The class defining Rooms in the SimpleMUD
// 
// 


#ifndef SIMPLEMUDROOM_H
#define SIMPLEMUDROOM_H

#include <string>
#include <iostream>
#include <list>
#include "BasicLib/BasicLib.h"

#include "Entity.h"
#include "Attributes.h"
#include "Item.h"
#include "DatabasePointer.h"

using std::ostream;
using std::istream;
using std::list;

namespace SimpleMUD
{


// --------------------------------------------------------------------
//  Class that stores information about rooms
// --------------------------------------------------------------------
class Room : public Entity
{
public:
    Room();

    inline RoomType& Type()         { return m_type; }
    inline int& Data()              { return m_data; }
    inline string& Description()    { return m_description; }
    inline entityid& Adjacent( int p_dir ) { return m_rooms[p_dir]; }

    inline enemytemplate& SpawnWhich() { return m_spawnwhich; }
    inline int& MaxEnemies()        { return m_maxenemies; }

    inline list<item>& Items()      { return m_items; }
    inline money& Money()           { return m_money; }

    inline list<enemy>& Enemies()   { return m_enemies; }
    inline list<player>& Players()  { return m_players; }

    void AddPlayer( player p_player );
    void RemovePlayer( player p_player );

    item FindItem( const string& p_item );
    void AddItem( item p_item );
    void RemoveItem( item p_item );

    enemy FindEnemy( const string& p_enemy );
    void AddEnemy( enemy p_enemy );
    void RemoveEnemy( enemy p_enemy );

    void LoadTemplate( istream& p_stream );
    void LoadData( istream& p_stream );
    void SaveData( ostream& p_stream );


protected:
    // -----------------------------------------
    //  template information
    // -----------------------------------------
    RoomType m_type;
    int m_data;         // auxilliary data defined by room type
    string m_description;
    entityid m_rooms[NUMDIRECTIONS];

    enemytemplate m_spawnwhich;
    int m_maxenemies;


    // -----------------------------------------
    //  volatile data (save to disk)
    // -----------------------------------------
    list<item> m_items;
    money m_money;


    // -----------------------------------------
    //  volatile data (do not save to disk)
    // -----------------------------------------
    list<player> m_players;
    list<enemy> m_enemies;


};  // end class Room






}   // end namespace SimpleMUD


#endif
