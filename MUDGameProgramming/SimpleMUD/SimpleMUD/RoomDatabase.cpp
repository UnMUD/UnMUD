// MUD Programming
// Ron Penton
// (C)2003
// PlayerDatabase.cpp - The class that stores all of the players within the MUD
// 
// 

#include <fstream>
#include <vector>
#include "BasicLib/BasicLib.h"
#include "SimpleMUDLogs.h"
#include "RoomDatabase.h"

using BasicLib::LowerCase;
using BasicLib::tostring;
using std::string;
using std::ifstream;
using std::ofstream;


namespace SimpleMUD
{

RoomDatabase& RoomDatabase::GetInstance()
{
    static RoomDatabase roomDatabase;
    return roomDatabase;
}

void RoomDatabase::LoadTemplates()
{
    std::ifstream file( "maps/default.map" );
    entityid id;
    std::string temp;

    while( file.good() )
    {
        // read the ID from disk
        file >> temp >> id;

        // make sure there's enough room for the room
        if( m_vector.size() <= id )
            m_vector.resize( id + 1 );

        m_vector[id].ID() = id;
        m_vector[id].LoadTemplate( file );     
        file >> std::ws;
    }
}

void RoomDatabase::LoadData()
{
    std::ifstream file( "maps/default.data" );

    string temp;
    entityid roomid;

    while( file.good() )
    {
        // load in the room id
        file >> temp >> roomid;

        // load the entry
        m_vector[roomid].LoadData( file );
        file >> std::ws;
    }
}

void RoomDatabase::SaveData()
{
    std::ofstream file( "maps/default.data" );

    for(auto& room : GetInstance()){
        file << "[ROOMID] " << room.ID() << "\n";
        m_vector[room.ID()].SaveData( file );
        file << "\n";
    }
}


}   // end namespace SimpleMUD
