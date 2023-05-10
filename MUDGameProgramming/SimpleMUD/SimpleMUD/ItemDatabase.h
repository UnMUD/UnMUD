// MUD Programming
// Ron Penton
// (C)2003
// ItemDatabase.h - The class that stores all of the items within the MUD
//
//

#ifndef SIMPLEMUDITEMDATABASE_H
#define SIMPLEMUDITEMDATABASE_H

#include <map>
#include <string>

#include "DatabasePointer.h"
#include "EntityDatabase.h"
#include "Item.h"

namespace SimpleMUD {

// --------------------------------------------------------------------
//  A database for items
// --------------------------------------------------------------------
class ItemDatabase : public EntityDatabase<Item> {
public:
  static ItemDatabase &GetInstance();
  ItemDatabase(ItemDatabase const &) = delete;
  ItemDatabase(ItemDatabase &&) = delete;
  ItemDatabase &operator=(ItemDatabase const &) = delete;
  ItemDatabase &operator=(ItemDatabase &&) = delete;

  bool Load();

private:
  ItemDatabase() {}

}; // end class ItemDatabase

} // end namespace SimpleMUD

#endif
