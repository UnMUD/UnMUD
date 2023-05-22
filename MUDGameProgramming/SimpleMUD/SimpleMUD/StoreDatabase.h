// MUD Programming
// Ron Penton
// (C)2003
// StoreDatabase.h - The class that stores all of the stores within the MUD
//
//

#ifndef SIMPLEMUDSTOREDATABASE_H
#define SIMPLEMUDSTOREDATABASE_H

#include <map>
#include <string>

#include "EntityDatabase.h"
#include "Store.h"

namespace SimpleMUD {

// --------------------------------------------------------------------
//  A database for items
// --------------------------------------------------------------------
class StoreDatabase : public EntityDatabase<Store> {
public:
  static StoreDatabase &GetInstance();
  StoreDatabase(StoreDatabase const &) = delete;
  StoreDatabase(StoreDatabase &&) = delete;
  StoreDatabase &operator=(StoreDatabase const &) = delete;
  StoreDatabase &operator=(StoreDatabase &&) = delete;

  bool Load();

private:
  StoreDatabase() {}
}; // end class StoreDatabase

} // end namespace SimpleMUD

#endif
