// MUD Programming
// Ron Penton
// (C)2003
// Store.h - The class defining Stores in the SimpleMUD
//
//

#ifndef SIMPLEMUDSTORE_H
#define SIMPLEMUDSTORE_H

#include "BasicLib/BasicLib.h"
#include <iostream>
#include <list>
#include <string>
#include <utility>

#include "DatabasePointer.h"
#include "Entity.h"
#include "Item.h"

using std::istream;
using std::ostream;
using namespace BasicLib;

namespace SimpleMUD {

// --------------------------------------------------------------------
//  Class that stores information about stores
// --------------------------------------------------------------------
class Store : public Entity {
public:
  typedef std::list<item>::iterator iterator;

  Store() : m_items() {}

  item find(const string &p_item) {
    iterator itr = BasicLib::double_find_if(
        begin(), end(), matchentityfull(p_item), matchentity(p_item));

    if (itr != end()) // item found?
      return *itr;    // return ID
    return 0;         // else, item not found.
  }

  iterator begin() { return m_items.begin(); }
  iterator end() { return m_items.end(); }
  size_t size() { return m_items.size(); }

  bool has(entityid p_item) {
    return std::find(begin(), end(), p_item) != end();
  }

  friend istream &operator>>(istream &p_stream, Store &s);

protected:
  std::list<item> m_items;

}; // end class Room

inline istream &operator>>(istream &p_stream, Store &s) {
  string temp;

  // read name
  p_stream >> temp >> std::ws;
  std::getline(p_stream, s.Name());

  // clear the item listing on load, in case this is being reloaded
  s.m_items.clear();

  // now load in the item listing.
  entityid last;
  p_stream >> temp; // chew up "[ITEMS]" tag

  while (extract(p_stream, last) != 0) // loop while item ID's valid
    s.m_items.push_back(last);         // add item

  return p_stream;
}

} // end namespace SimpleMUD

#endif
