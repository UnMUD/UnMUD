// MUD Programming
// Ron Penton
// (C)2003
// Game.cpp - This class is the game handler for SimpleMUD.
//
//

#include "Game.h"
#include "BasicLib/BasicLib.h"
#include "EnemyDatabase.h"
#include "ItemDatabase.h"
#include "Logon.h"
#include "PlayerDatabase.h"
#include "RoomDatabase.h"
#include "StoreDatabase.h"
#include "Train.h"

using namespace SocketLib;
using namespace BasicLib;
using std::string;

namespace SimpleMUD {

// declare the static instance of the timer
BasicLib::Timer Game::s_timer;
bool Game::s_running = false;

// ------------------------------------------------------------------------
//  This handles incomming commands. Anything passed into this function
//  is assumed to be a complete command from a client.
// ------------------------------------------------------------------------
void Game::Handle(string p_data) {
  Player &p = *m_player;

  // check if the player wants to repeat a command
  if (p_data == "/") {
    p_data = m_lastcommand;
  } else {
    // if not, record the command.
    m_lastcommand = p_data;
  }

  // get the first word and lowercase it.
  string firstword = BasicLib::LowerCase(ParseWord(p_data, 0));

  // ------------------------------------------------------------------------
  //  REGULAR access commands
  // ------------------------------------------------------------------------
  if (firstword == "chat" || firstword == ":") {
    string text = RemoveWord(p_data, 0);
    SendGame(magenta + bold + p.Name() + " chats: " + white + text);
    return;
  }

  if (firstword == "experience" || firstword == "exp") {
    p.SendString(PrintExperience());
    return;
  }

  if (firstword == "help" || firstword == "commands") {
    p.SendString(PrintHelp(p.Rank()));
    return;
  }

  if (firstword == "inventory" || firstword == "inv") {
    p.SendString(PrintInventory());
    return;
  }

  if (firstword == "quit") {
    m_connection->Close();
    LogoutMessage(p.Name() + " has left the realm.");
    return;
  }

  if (firstword == "remove") {
    RemoveItem(ParseWord(p_data, 1));
    return;
  }

  if (firstword == "stats" || firstword == "st") {
    p.SendString(PrintStats());
    return;
  }

  if (firstword == "time") {
    p.SendString(
        bold + cyan + "The current system time is: " + BasicLib::TimeStamp() +
        " on " + BasicLib::DateStamp() +
        "\r\nThe system has been up for: " + s_timer.GetString() + ".");
    return;
  }

  if (firstword == "use") {
    UseItem(RemoveWord(p_data, 0));
    return;
  }

  if (firstword == "whisper") {
    // get the players name
    string name = ParseWord(p_data, 1);
    string message = RemoveWord(RemoveWord(p_data, 0), 0);

    Whisper(message, name);
    return;
  }

  if (firstword == "who") {
    p.SendString(WhoList(BasicLib::LowerCase(ParseWord(p_data, 1))));
    return;
  }

  if (firstword == "look" || firstword == "l") {
    p.SendString(PrintRoom(p.CurrentRoom()));
    return;
  }

  if (firstword == "north" || firstword == "n") {
    Move(NORTH);
    return;
  }
  if (firstword == "east" || firstword == "e") {
    Move(EAST);
    return;
  }
  if (firstword == "south" || firstword == "s") {
    Move(SOUTH);
    return;
  }
  if (firstword == "west" || firstword == "w") {
    Move(WEST);
    return;
  }

  if (firstword == "get" || firstword == "take") {
    GetItem(RemoveWord(p_data, 0));
    return;
  }

  if (firstword == "drop") {
    DropItem(RemoveWord(p_data, 0));
    return;
  }

  if (firstword == "train") {
    if (p.CurrentRoom()->Type() != TRAININGROOM) {
      p.SendString(red + bold + "You cannot train here!");
      return;
    }

    if (p.Train()) {
      p.SendString(green + bold + "You are now level " + tostring(p.Level()));
    } else {
      p.SendString(red + bold + "You don't have enough experience to train!");
    }

    return;
  }

  if (firstword == "editstats") {
    if (p.CurrentRoom()->Type() != TRAININGROOM) {
      p.SendString(red + bold + "You cannot edit your stats here!");
      return;
    }

    GotoTrain();
    return;
  }

  if (firstword == "list") {
    if (p.CurrentRoom()->Type() != STORE) {
      p.SendString(red + bold + "You're not in a store!");
      return;
    }

    p.SendString(StoreList(p.CurrentRoom()->Data()));
    return;
  }

  if (firstword == "buy") {
    if (p.CurrentRoom()->Type() != STORE) {
      p.SendString(red + bold + "You're not in a store!");
      return;
    }

    Buy(RemoveWord(p_data, 0));
    return;
  }

  if (firstword == "sell") {
    if (p.CurrentRoom()->Type() != STORE) {
      p.SendString(red + bold + "You're not in a store!");
      return;
    }

    Sell(RemoveWord(p_data, 0));
    return;
  }

  if (firstword == "attack" || firstword == "a") {
    PlayerAttack(RemoveWord(p_data, 0));
    return;
  }

  // ------------------------------------------------------------------------
  //  GOD access commands
  // ------------------------------------------------------------------------
  if (firstword == "kick" && p.Rank() >= GOD) {
    // find a player to kick
    PlayerDatabase::iterator itr =
        PlayerDatabase::GetInstance().findloggedin(ParseWord(p_data, 1));

    if (itr == PlayerDatabase::GetInstance().end()) {
      p.SendString(red + bold + "Player could not be found.");
      return;
    }

    if (itr->Rank() > p.Rank()) {
      p.SendString(red + bold + "You can't kick that player!");
      return;
    }

    itr->Conn()->Close();
    LogoutMessage(itr->Name() + " has been kicked by " + p.Name() + "!!!");

    return;
  }

  // ------------------------------------------------------------------------
  //  ADMIN access commands
  // ------------------------------------------------------------------------
  if (firstword == "announce" && p.Rank() >= ADMIN) {
    Announce(RemoveWord(p_data, 0));
    return;
  }

  if (firstword == "changerank" && p.Rank() >= ADMIN) {
    string name = ParseWord(p_data, 1);

    PlayerDatabase::iterator itr = PlayerDatabase::GetInstance().find(name);

    if (itr == PlayerDatabase::GetInstance().end()) {
      p.SendString(red + bold + "Error: Could not find user " + name);
      return;
    }

    PlayerRank rank = GetRank(ParseWord(p_data, 2));

    itr->Rank() = rank;
    SendGame(green + bold + itr->Name() +
             "'s rank has been changed to: " + GetRankString(rank));
    return;
  }

  if (firstword == "reload" && p.Rank() >= ADMIN) {
    string db = BasicLib::LowerCase(ParseWord(p_data, 1));

    if (db == "items") {
      ItemDatabase::GetInstance().Load();
      p.SendString(bold + cyan + "Item Database Reloaded!");
    } else if (db == "player") {
      string user = ParseWord(p_data, 2);
      PlayerDatabase::iterator itr =
          PlayerDatabase::GetInstance().findfull(user);

      if (itr == PlayerDatabase::GetInstance().end()) {
        p.SendString(bold + red + "Invalid Player Name");
      } else {
        bool a = itr->Active();
        if (a)
          itr->Conn()->Handler()->Leave();
        PlayerDatabase::GetInstance().LoadPlayer(itr->Name());
        if (a)
          itr->Conn()->Handler()->Enter();

        p.SendString(bold + cyan + "Player " + itr->Name() + " Reloaded!");
      }
    } else if (db == "rooms") {
      RoomDatabase::GetInstance().LoadTemplates();
      p.SendString(bold + cyan + "Room Template Database Reloaded!");
    } else if (db == "stores") {
      StoreDatabase::GetInstance().Load();
      p.SendString(bold + cyan + "Store Database Reloaded!");
    } else if (db == "enemies") {
      EnemyTemplateDatabase::GetInstance().Load();
      p.SendString(bold + cyan + "Enemy Database Reloaded!");
    } else {
      p.SendString(bold + red + "Invalid Database Name");
    }
    return;
  }

  if (firstword == "shutdown" && p.Rank() >= ADMIN) {
    Announce("SYSTEM IS SHUTTING DOWN");
    Game::Running() = false;
    return;
  }

  // ------------------------------------------------------------------------
  //  Command not recognized, send to room
  // ------------------------------------------------------------------------
  SendRoom(bold + p.Name() + " says: " + dim + p_data, p.CurrentRoom());
}

// ------------------------------------------------------------------------
//  This notifies the handler that there is a new connection
// ------------------------------------------------------------------------
void Game::Enter() {
  USERLOG.Log(GetIPString(m_connection->GetRemoteAddress()) + " - User " +
              m_player->Name() + " entering Game state.");

  m_lastcommand = "";

  Player &p = *m_player;
  p.CurrentRoom()->AddPlayer(p.ID());
  p.Active() = true;
  p.LoggedIn() = true;

  SendGame(bold + green + p.Name() + " has entered the realm.");

  if (p.Newbie())
    GotoTrain();
  else
    p.SendString(PrintRoom(p.CurrentRoom()));
}

void Game::Leave() {
  USERLOG.Log(GetIPString(m_connection->GetRemoteAddress()) + " - User " +
              m_player->Name() + " leaving Game state.");

  // remove the player from his room
  m_player->CurrentRoom()->RemovePlayer(m_player);
  m_player->Active() = false;

  // log out the player from the database if the connection has been closed
  if (m_connection->Closed()) {
    PlayerDatabase::GetInstance().Logout(m_player);
  }
}

// ------------------------------------------------------------------------
//  This notifies the handler that a connection has unexpectedly hung up.
// ------------------------------------------------------------------------
void Game::Hungup() {
  USERLOG.Log(GetIPString(m_connection->GetRemoteAddress()) + " - User " +
              m_player->Name() + " hung up.");

  Player &p = *m_player;
  LogoutMessage(p.Name() + " has suddenly disappeared from the realm.");
}

// ------------------------------------------------------------------------
//  This notifies the handler that a connection is being kicked due to
//  flooding the server.
// ------------------------------------------------------------------------
void Game::Flooded() {
  USERLOG.Log(GetIPString(m_connection->GetRemoteAddress()) + " - User " +
              m_player->Name() + " flooded.");

  Player &p = *m_player;
  LogoutMessage(p.Name() + " has been kicked for flooding!");
}

// ------------------------------------------------------------------------
//  Sends a string to everyone connected.
// ------------------------------------------------------------------------
void Game::SendGlobal(const string &p_str) {
  operate_on_if(PlayerDatabase::GetInstance().begin(),
                PlayerDatabase::GetInstance().end(), playersend(p_str),
                playerloggedin());
}

// ------------------------------------------------------------------------
//  Sends a string to everyone "within the game"
// ------------------------------------------------------------------------
void Game::SendGame(const std::string &p_str) {
  operate_on_if(PlayerDatabase::GetInstance().begin(),
                PlayerDatabase::GetInstance().end(), playersend(p_str),
                playeractive());
}

// ------------------------------------------------------------------------
//  Sends a logout message
// ------------------------------------------------------------------------
void Game::LogoutMessage(const string &p_reason) {
  SendGame(SocketLib::red + SocketLib::bold + p_reason);
}

// ------------------------------------------------------------------------
//  Sends a system announcement
// ------------------------------------------------------------------------
void Game::Announce(const string &p_announcement) {
  SendGlobal(SocketLib::cyan + SocketLib::bold +
             "System Announcement: " + p_announcement);
}

// ------------------------------------------------------------------------
//  Sends a whisper string to the requested player
// ------------------------------------------------------------------------
void Game::Whisper(std::string p_str, std::string p_player) {
  // find the player
  PlayerDatabase::iterator itr =
      PlayerDatabase::GetInstance().findactive(p_player);

  // if no match was found
  if (itr == PlayerDatabase::GetInstance().end()) {
    m_player->SendString(red + bold + "Error, cannot find user.");
  } else {
    itr->SendString(yellow + m_player->Name() + " whispers to you: " + reset +
                    p_str);

    m_player->SendString(yellow + "You whisper to " + itr->Name() + ": " +
                         reset + p_str);
  }
}

// ------------------------------------------------------------------------
//  Functor that generates a who-listing for a single player
// ------------------------------------------------------------------------
struct wholist {
  string str;

  wholist() : str() {}

  void operator()(Player &p) {
    str += " " + tostring(p.Name(), 17) + "| ";
    str += tostring(p.Level(), 10) + "| ";

    if (p.Active())
      str += green + "Online  " + white;
    else if (p.LoggedIn())
      str += yellow + "Inactive" + white;
    else
      str += red + "Offline " + white;

    str += " | ";
    switch (p.Rank()) {
    case REGULAR:
      str += white;
      break;
    case GOD:
      str += yellow;
      break;
    case ADMIN:
      str += green;
      break;
    }

    str += GetRankString(p.Rank());

    str += white + "\r\n";
  }
};

// ------------------------------------------------------------------------
//  This prints up the who-list for the realm.
// ------------------------------------------------------------------------
string Game::WhoList(const string &p_who) {
  using namespace BasicLib;

  string str = white + bold +
               "---------------------------------------------------------------"
               "-----------------\r\n" +
               " Name             | Level     | Activity | Rank\r\n" +
               "---------------------------------------------------------------"
               "-----------------\r\n";

  wholist who;
  if (p_who == "all") {
    who = BasicLib::operate_on_if(PlayerDatabase::GetInstance().begin(),
                                  PlayerDatabase::GetInstance().end(),
                                  wholist(), always<Player>());
  } else {
    who = BasicLib::operate_on_if(PlayerDatabase::GetInstance().begin(),
                                  PlayerDatabase::GetInstance().end(),
                                  wholist(), playerloggedin());
  }

  str += who.str;

  str += "---------------------------------------------------------------------"
         "-----------";

  return str;
}

// ------------------------------------------------------------------------
//  Prints out a help listing based on a user's rank.
// ------------------------------------------------------------------------
string Game::PrintHelp(PlayerRank p_rank) {
  static string help =
      white + bold +
      "--------------------------------- Command List "
      "---------------------------------\r\n" +
      " /                          - Repeats your last command exactly.\r\n" +
      " chat <mesg>                - Sends message to everyone in the "
      "game\r\n" +
      " experience                 - Shows your experience statistics\r\n" +
      " help                       - Shows this menu\r\n" +
      " inventory                  - Shows a list of your items\r\n" +
      " quit                       - Allows you to leave the realm.\r\n" +
      " remove <'weapon'/'armor'>  - removes your weapon or armor\r\n" +
      " stats                      - Shows all of your statistics\r\n" +
      " time                       - shows the current system time.\r\n" +
      " use <item>                 - use an item in your inventory\r\n" +
      " whisper <who> <msg>        - Sends message to one person\r\n" +
      " who                        - Shows a list of everyone online\r\n" +
      " who all                    - Shows a list of everyone\r\n" +
      " look                       - Shows you the contents of a room\r\n" +
      " north/east/south/west      - Moves in a direction\r\n" +
      " get/drop <item>            - Picks up or drops an item on the "
      "ground\r\n" +
      " train                      - Train to the next level (TR)\r\n" +
      " editstats                  - Edit your statistics (TR)\r\n" +
      " list                       - Lists items in a store (ST)\r\n" +
      " buy/sell <item>            - Buy or Sell an item in a store (ST)\r\n" +
      " attack <enemy>             - Attack an enemy\r\n";

  static string god =
      yellow + bold +
      "--------------------------------- God Commands "
      "---------------------------------\r\n" +
      " kick <who>                 - kicks a user from the realm\r\n";

  static string admin =
      green + bold +
      "-------------------------------- Admin Commands "
      "--------------------------------\r\n" +
      " announce <msg>             - Makes a global system announcement\r\n" +
      " changerank <who> <rank>    - Changes the rank of a player\r\n" +
      " reload <db>                - Reloads the requested database\r\n" +
      " shutdown                   - Shuts the server down\r\n";

  static string end = white + bold +
                      "--------------------------------------------------------"
                      "------------------------";

  if (p_rank == REGULAR)
    return help + end;
  else if (p_rank == GOD)
    return help + god + end;
  else if (p_rank == ADMIN)
    return help + god + admin + end;
  else
    return "ERROR";
}

// ------------------------------------------------------------------------
//  This prints up the stats of the player
// ------------------------------------------------------------------------
string Game::PrintStats() {
  using namespace BasicLib;
  Player &p = *m_player;

  return white + bold +
         "---------------------------------- Your Stats "
         "----------------------------------\r\n" +
         " Name:          " + p.Name() + "\r\n" +
         " Rank:          " + GetRankString(p.Rank()) + "\r\n" +
         " HP/Max:        " + tostring(p.HitPoints()) + "/" +
         tostring(p.GetAttr(MAXHITPOINTS)) + "  (" +
         tostring(percent(p.HitPoints(), p.GetAttr(MAXHITPOINTS))) + "%)\r\n" +
         PrintExperience() + "\r\n" +
         " Strength:      " + tostring(p.GetAttr(STRENGTH), 16) +
         " Accuracy:      " + tostring(p.GetAttr(ACCURACY)) + "\r\n" +
         " Health:        " + tostring(p.GetAttr(HEALTH), 16) +
         " Dodging:       " + tostring(p.GetAttr(DODGING)) + "\r\n" +
         " Agility:       " + tostring(p.GetAttr(AGILITY), 16) +
         " Strike Damage: " + tostring(p.GetAttr(STRIKEDAMAGE)) + "\r\n" +
         " StatPoints:    " + tostring(p.StatPoints(), 16) +
         " Damage Absorb: " + tostring(p.GetAttr(DAMAGEABSORB)) + "\r\n" +
         "---------------------------------------------------------------------"
         "-----------";
}

// ------------------------------------------------------------------------
//  This prints up the experience of the player
// ------------------------------------------------------------------------
string Game::PrintExperience() {
  using namespace BasicLib;
  Player &p = *m_player;

  return white + bold + " Level:         " + tostring(p.Level()) + "\r\n" +
         " Experience:    " + tostring(p.Experience()) + "/" +
         tostring(p.NeedForLevel(p.Level() + 1)) + " (" +
         tostring(percent(p.Experience(), p.NeedForLevel(p.Level() + 1))) +
         "%)";
}

// ------------------------------------------------------------------------
//  This prints up the inventory-list of the player
// ------------------------------------------------------------------------
string Game::PrintInventory() {
  using namespace BasicLib;
  Player &p = *m_player;

  // Inventory
  string itemlist = white + bold +
                    "-------------------------------- Your Inventory "
                    "--------------------------------\r\n" +
                    " Items:  ";

  for (int i = 0; i < PLAYERITEMS; i++) {
    if (p.GetItem(i) != 0) {
      itemlist += p.GetItem(i)->Name() + ", ";
    }
  }

  // chop off the extraneous comma, and add a newline.
  itemlist.erase(itemlist.size() - 2, 2);
  itemlist += "\r\n";

  // Weapon/Armor
  itemlist += " Weapon: ";
  if (p.Weapon() == 0)
    itemlist += "NONE!";
  else
    itemlist += p.Weapon()->Name();

  itemlist += "\r\n Armor:  ";
  if (p.Armor() == 0)
    itemlist += "NONE!";
  else
    itemlist += p.Armor()->Name();

  // Money
  itemlist += "\r\n Money:  $" + tostring(p.Money());

  itemlist += "\r\n------------------------------------------------------------"
              "--------------------";

  return itemlist;
}

// ------------------------------------------------------------------------
//  This finds and attempts to "use" an item in your inventory.
// ------------------------------------------------------------------------
bool Game::UseItem(const std::string &p_item) {
  Player &p = *m_player;

  int i = p.GetItemIndex(p_item);

  if (i == -1) {
    p.SendString(red + bold + "Could not find that item!");
    return false;
  }

  Item &itm = *p.GetItem(i);

  switch (itm.Type()) {
  case WEAPON:
    p.UseWeapon(i);
    SendRoom(green + bold + p.Name() + " arms a " + itm.Name(),
             p.CurrentRoom());
    return true;

  case ARMOR:
    p.UseArmor(i);
    SendRoom(green + bold + p.Name() + " puts on a " + itm.Name(),
             p.CurrentRoom());
    return true;

  case HEALING:
    p.AddBonuses(itm.ID());
    p.AddHitpoints(static_cast<int>(BasicLib::RandomInt(itm.Min(), itm.Max())));
    p.DropItem(i);
    SendRoom(green + bold + p.Name() + " uses a " + itm.Name(),
             p.CurrentRoom());

    return true;
  }
  return false;
}

// ------------------------------------------------------------------------
//  This removes your weapon or your armor
// ------------------------------------------------------------------------
bool Game::RemoveItem(std::string p_item) {
  Player &p = *m_player;

  p_item = BasicLib::LowerCase(p_item);

  if (p_item == "weapon" && p.Weapon() != 0) {
    SendRoom(green + bold + p.Name() + " puts away a " + p.Weapon()->Name(),
             p.CurrentRoom());
    p.RemoveWeapon();
    return true;
  }

  if (p_item == "armor" && p.Armor() != 0) {
    SendRoom(green + bold + p.Name() + " takes off a " + p.Armor()->Name(),
             p.CurrentRoom());
    p.RemoveArmor();
    return true;
  }

  p.SendString(red + bold + "Could not Remove item!");
  return false;
}

string Game::PrintRoom(room p_room) {
  string desc = "\r\n" + bold + white + p_room->Name() + "\r\n";
  string temp;
  int count;

  desc += bold + magenta + p_room->Description() + "\r\n";
  desc += bold + green + "exits: ";

  for (int d = 0; d < NUMDIRECTIONS; d++) {
    if (p_room->Adjacent(d) != 0)
      desc += DIRECTIONSTRINGS[d] + "  ";
  }
  desc += "\r\n";

  // ---------------------------------
  // ITEMS
  // ---------------------------------
  temp = bold + yellow + "You see: ";
  count = 0;
  if (p_room->Money() > 0) {
    count++;
    temp += "$" + tostring(p_room->Money()) + ", ";
  }

  std::list<item>::iterator itemitr = p_room->Items().begin();
  while (itemitr != p_room->Items().end()) {
    count++;
    temp += (*itemitr)->Name() + ", ";
    ++itemitr;
  }

  if (count > 0) {
    // chop off the trailing ", "
    temp.erase(temp.size() - 2, 2);
    // add a newline
    desc += temp + "\r\n";
  }

  // ---------------------------------
  // PEOPLE
  // ---------------------------------
  temp = bold + cyan + "People: ";
  count = 0;
  std::list<player>::iterator playeritr = p_room->Players().begin();
  while (playeritr != p_room->Players().end()) {
    temp += (*playeritr)->Name() + ", ";
    count++;
    ++playeritr;
  }

  if (count > 0) {
    temp.erase(temp.size() - 2, 2);
    desc += temp + "\r\n";
  }

  // ---------------------------------
  // ENEMIES
  // ---------------------------------
  temp = bold + red + "Enemies: ";
  count = 0;
  std::list<enemy>::iterator enemyitr = p_room->Enemies().begin();
  while (enemyitr != p_room->Enemies().end()) {
    temp += (*enemyitr)->Name() + ", ";
    count++;
    ++enemyitr;
  }

  if (count > 0) {
    temp.erase(temp.size() - 2, 2);
    desc += temp + "\r\n";
  }

  return desc;
}

void Game::SendRoom(string p_text, room p_room) {
  std::for_each(p_room->Players().begin(), p_room->Players().end(),
                playersend(p_text));
}

void Game::Move(int p_direction) {
  Player &p = *m_player;
  room next = p.CurrentRoom()->Adjacent(p_direction);
  room previous = p.CurrentRoom();

  if (next == 0) {
    SendRoom(red + p.Name() + " bumps into the wall to the " +
                 DIRECTIONSTRINGS[p_direction] + "!!!",
             p.CurrentRoom());
    return;
  }

  previous->RemovePlayer(p.ID());

  SendRoom(green + p.Name() + " leaves to the " +
               DIRECTIONSTRINGS[p_direction] + ".",
           previous);
  SendRoom(green + p.Name() + " enters from the " +
               DIRECTIONSTRINGS[OppositeDirection(p_direction)] + ".",
           next);
  p.SendString(green + "You walk " + DIRECTIONSTRINGS[p_direction] + ".");

  p.CurrentRoom() = next;
  next->AddPlayer(p.ID());

  p.SendString(PrintRoom(next));
}

void Game::GetItem(string p_item) {
  Player &p = *m_player;

  if (p_item[0] == '$') {
    // clear off the '$', and convert the result into a number.
    p_item.erase(0, 1);
    money m = BasicLib::totype<money>(p_item);

    // make sure there's enough money in the room
    if (m > p.CurrentRoom()->Money()) {
      p.SendString(red + bold + "There isn't that much here!");
    } else {
      p.Money() += m;
      p.CurrentRoom()->Money() -= m;
      SendRoom(cyan + bold + p.Name() + " picks up $" + tostring(m) + ".",
               p.CurrentRoom());
    }
    return;
  }

  item i = p.CurrentRoom()->FindItem(p_item);

  if (i == 0) {
    p.SendString(red + bold + "You don't see that here!");
    return;
  }

  if (!p.PickUpItem(i)) {
    p.SendString(red + bold + "You can't carry that much!");
    return;
  }

  p.CurrentRoom()->RemoveItem(i);
  SendRoom(cyan + bold + p.Name() + " picks up " + i->Name() + ".",
           p.CurrentRoom());
}

void Game::DropItem(string p_item) {
  Player &p = *m_player;

  if (p_item[0] == '$') {
    // clear off the '$', and convert the result into a number.
    p_item.erase(0, 1);
    money m = BasicLib::totype<money>(p_item);

    // make sure there's enough money in the inventory
    if (m > p.Money()) {
      p.SendString(red + bold + "You don't have that much!");
    } else {
      p.Money() -= m;
      p.CurrentRoom()->Money() += m;
      SendRoom(cyan + bold + p.Name() + " drops $" + tostring(m) + ".",
               p.CurrentRoom());
    }
    return;
  }

  int i = p.GetItemIndex(p_item);

  if (i == -1) {
    p.SendString(red + bold + "You don't have that!");
    return;
  }

  SendRoom(cyan + bold + p.Name() + " drops " + p.GetItem(i)->Name() + ".",
           p.CurrentRoom());
  p.CurrentRoom()->AddItem(p.GetItem(i));
  p.DropItem(i);
}

void Game::GotoTrain() {
  Player &p = *m_player;
  p.Active() = false;
  p.Conn()->AddHandler(new Train(*m_connection, p.ID()));
  LogoutMessage(p.Name() + " leaves to edit stats");
}

string Game::StoreList(entityid p_store) {
  Store &s = StoreDatabase::GetInstance().get(p_store);

  string output = white + bold +
                  "------------------------------------------------------------"
                  "--------------------\r\n";
  output += " Welcome to " + s.Name() + "!\r\n";
  output += "------------------------------------------------------------------"
            "--------------\r\n";
  output += " Item                           | Price\r\n";
  output += "------------------------------------------------------------------"
            "--------------\r\n";

  for (auto &itr : s) {
    output += " " + tostring(itr->Name(), 31) + "| ";
    output += tostring(itr->Price()) + "\r\n";
  }

  output += bold + "-----------------------------------------------------------"
                   "---------------------\r\n";

  return output;
}

void Game::Buy(const string &p_item) {
  Player &p = *m_player;
  Store &s = StoreDatabase::GetInstance().get(p.CurrentRoom()->Data());

  item i = s.find(p_item);
  if (i == 0) {
    p.SendString(red + bold + "Sorry, we don't have that item!");
    return;
  }

  if (p.Money() < i->Price()) {
    p.SendString(red + bold + "Sorry, but you can't afford that!");
    return;
  }

  if (!p.PickUpItem(i)) {
    p.SendString(red + bold + "Sorry, but you can't carry that much!");
    return;
  }

  p.Money() -= i->Price();
  SendRoom(cyan + bold + p.Name() + " buys a " + i->Name(), p.CurrentRoom());
}

void Game::Sell(const string &p_item) {
  Player &p = *m_player;
  Store &s = StoreDatabase::GetInstance().get(p.CurrentRoom()->Data());

  int index = p.GetItemIndex(p_item);
  if (index == -1) {
    p.SendString(red + bold + "Sorry, you don't have that!");
    return;
  }

  item i = p.GetItem(index);
  if (!s.has(i)) {
    p.SendString(red + bold + "Sorry, we don't want that item!");
    return;
  }

  p.DropItem(index);
  p.Money() += i->Price();
  SendRoom(cyan + bold + p.Name() + " sells a " + i->Name(), p.CurrentRoom());
}

void Game::EnemyAttack(enemy p_enemy) {
  Enemy &e = *p_enemy;
  room r = e.CurrentRoom();

  std::list<player>::iterator itr = r->Players().begin();

  std::advance(itr, BasicLib::RandomInt(0, r->Players().size() - 1));

  Player &p = **itr;

  sint64 now = Game::GetTimer().GetMS();

  int damage;
  if (e.Weapon() == 0) {
    damage = static_cast<int>(BasicLib::RandomInt(1, 3));
    e.NextAttackTime() = now + seconds(1);
  } else {
    damage = static_cast<int>(
        BasicLib::RandomInt(e.Weapon()->Min(), e.Weapon()->Max()));
    e.NextAttackTime() = now + seconds(e.Weapon()->Speed());
  }

  if (BasicLib::RandomInt(0, 99) >= e.Accuracy() - p.GetAttr(DODGING)) {
    Game::SendRoom(white + e.Name() + " swings at " + p.Name() + " but misses!",
                   e.CurrentRoom());
    return;
  }

  damage += e.StrikeDamage();
  damage -= p.GetAttr(DAMAGEABSORB);

  if (damage < 1)
    damage = 1;

  p.AddHitpoints(-damage);

  Game::SendRoom(red + e.Name() + " hits " + p.Name() + " for " +
                     tostring(damage) + " damage!",
                 e.CurrentRoom());

  if (p.HitPoints() <= 0)
    PlayerKilled(p.ID());
}

void Game::PlayerKilled(player p_player) {
  Player &p = *p_player;

  Game::SendRoom(red + bold + p.Name() + " has died!", p.CurrentRoom());

  // drop the money
  money m = p.Money() / 10;
  if (m > 0) {
    p.CurrentRoom()->Money() += m;
    p.Money() -= m;
    Game::SendRoom(cyan + "$" + tostring(m) + " drops to the ground.",
                   p.CurrentRoom());
  }

  // drop an item
  if (p.Items() > 0) {
    int index = -1;

    // loop through random numbers until you hit a valid item.
    while (p.GetItem(index = static_cast<int>(RandomInt(0, PLAYERITEMS - 1))) ==
           0)
      ;
    item i = p.GetItem(index);
    p.CurrentRoom()->AddItem(i);
    p.DropItem(index);

    Game::SendRoom(cyan + i->Name() + " drops to the ground.", p.CurrentRoom());
  }

  // subtract 10% experience
  int exp = p.Experience() / 10;
  p.Experience() -= exp;

  // remove the player from the room and transport him to room 1.
  p.CurrentRoom()->RemovePlayer(p_player);
  p.CurrentRoom() = 1;
  p.CurrentRoom()->AddPlayer(p_player);

  // set the hitpoints to 70%
  p.SetHitpoints(static_cast<int>(p.GetAttr(MAXHITPOINTS) * 0.7));
  p.SendString(white + bold + "You have died, but have been ressurected in " +
               p.CurrentRoom()->Name());
  p.SendString(red + bold + "You have lost " + tostring(exp) + " experience!");
  Game::SendRoom(white + bold + p.Name() + " appears out of nowhere!!",
                 p.CurrentRoom());
}

void Game::PlayerAttack(const string &p_enemy) {
  Player &p = *m_player;
  sint64 now = Game::GetTimer().GetMS();

  if (now < p.NextAttackTime()) {
    p.SendString(red + bold + "You can't attack yet!");
    return;
  }

  enemy ptr = p.CurrentRoom()->FindEnemy(p_enemy);

  if (ptr == 0) {
    p.SendString(red + bold + "You don't see that here!");
    return;
  }

  Enemy &e = *ptr;

  int damage;
  if (p.Weapon() == 0) {
    damage = static_cast<int>(BasicLib::RandomInt(1, 3));
    p.NextAttackTime() = now + seconds(1);
  } else {
    damage = static_cast<int>(
        BasicLib::RandomInt(p.Weapon()->Min(), p.Weapon()->Max()));
    p.NextAttackTime() = now + seconds(p.Weapon()->Speed());
  }

  if (BasicLib::RandomInt(0, 99) >= p.GetAttr(ACCURACY) - e.Dodging()) {
    SendRoom(white + p.Name() + " swings at " + e.Name() + " but misses!",
             p.CurrentRoom());
    return;
  }

  damage += p.GetAttr(STRIKEDAMAGE);
  damage -= e.DamageAbsorb();

  if (damage < 1)
    damage = 1;

  e.HitPoints() -= damage;

  SendRoom(red + p.Name() + " hits " + e.Name() + " for " + tostring(damage) +
               " damage!",
           p.CurrentRoom());

  if (e.HitPoints() <= 0)
    EnemyKilled(e.ID(), m_player);
}

void Game::EnemyKilled(enemy p_enemy, player p_player) {
  Enemy &e = *p_enemy;

  SendRoom(cyan + bold + e.Name() + " has died!", e.CurrentRoom());

  // drop the money
  money m = BasicLib::RandomInt(e.MoneyMin(), e.MoneyMax());
  if (m > 0) {
    e.CurrentRoom()->Money() += m;
    SendRoom(cyan + "$" + tostring(m) + " drops to the ground.",
             e.CurrentRoom());
  }

  for (auto &loot : e.LootList()) {
    if (BasicLib::RandomInt(0, 99) < loot.second) {
      e.CurrentRoom()->AddItem(loot.first);
      SendRoom(cyan + (loot.first)->Name() + " drops to the ground.",
               e.CurrentRoom());
    }
  }

  // add experience to the player who killed it
  Player &p = *p_player;
  p.Experience() += e.Experience();
  p.SendString(cyan + bold + "You gain " + tostring(e.Experience()) +
               " experience.");

  // remove the enemy from the game
  EnemyDatabase::GetInstance().Delete(p_enemy);
}

} // end namespace SimpleMUD
