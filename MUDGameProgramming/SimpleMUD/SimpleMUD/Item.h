// MUD Programming
// Ron Penton
// (C)2003
// Item.h - The class defining Items in the SimpleMUD
//
//

#ifndef SIMPLEMUDITEM_H
#define SIMPLEMUDITEM_H

#include "Attributes.h"
#include "BasicLib/BasicLib.h"
#include "Entity.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>

using std::istream;
using std::ostream;

namespace SimpleMUD {

typedef unsigned long int money;

// --------------------------------------------------------------------
//  Class that stores information about items
// --------------------------------------------------------------------
class Item : public Entity {
public:
  Item()
      : m_type(WEAPON), m_min(0), m_max(0), m_speed(0), m_price(),
        m_attributes() {}

  inline ItemType &Type() { return m_type; }

  inline int &GetAttr(int p_att) { return m_attributes[p_att]; }

  inline int &Min() { return m_min; }
  inline int &Max() { return m_max; }
  inline int &Speed() { return m_speed; }
  inline money &Price() { return m_price; }

  friend istream &operator>>(istream &p_stream, Item &i);
  friend void ParseRow(const pqxx::const_result_iterator::reference &row,
                       Item &i);

protected:
  // -----------------------------------------
  //  Item Information
  // -----------------------------------------
  ItemType m_type;

  // item attributes
  int m_min;
  int m_max;
  int m_speed;
  money m_price;

  // player modification attributes
  AttributeSet m_attributes;

}; // end class Item

// --------------------------------------------------------------------
//  Extracts an item in text form from a stream
// --------------------------------------------------------------------
inline istream &operator>>(istream &p_stream, Item &i) {
  std::string temp;

  std::cout << "Item operator>>:\n";
  p_stream >> temp >> std::ws;
  std::cout << temp << ": " << i.ID() << std::endl;
  std::getline(p_stream, i.m_name);
  p_stream >> temp >> temp;
  std::cout << temp << ": " << i.m_name << std::endl;
  i.m_type = GetItemType(temp);
  p_stream >> temp >> i.m_min;
  std::cout << temp << ": " << i.m_min << std::endl;
  p_stream >> temp >> i.m_max;
  std::cout << temp << ": " << i.m_max << std::endl;
  p_stream >> temp >> i.m_speed;
  std::cout << temp << ": " << i.m_speed << std::endl;
  p_stream >> temp >> i.m_price;
  std::cout << temp << ": " << i.m_price << std::endl;
  p_stream >> i.m_attributes;
  std::cout << i.m_attributes << std::endl;

  return p_stream;
}

// --------------------------------------------------------------------
//  Extracts an item in pqxx::const_result_iterator::reference form from a
//  pqxx::result
// --------------------------------------------------------------------
inline void ParseRow(const pqxx::const_result_iterator::reference &row,
                     Item &i) {
  row["name"] >> i.m_name;
  i.m_type = GetItemType(row["type"].as<std::string>());
  row["min"] >> i.m_min;
  row["max"] >> i.m_max;
  row["speed"] >> i.m_speed;
  row["price"] >> i.m_price;

  ParseRow(row, i.m_attributes);
}

} // end namespace SimpleMUD

#endif
