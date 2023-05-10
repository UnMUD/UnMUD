// MUD Programming
// Ron Penton
// (C)2003
// Logon.cpp - This class is the logon handler for SimpleMUD.
//
//

#include "Logon.h"
#include "BasicLib/BasicLib.h"
#include "Game.h"
#include "PlayerDatabase.h"

using namespace SocketLib;

namespace SimpleMUD {

// ------------------------------------------------------------------------
//  This handles incomming commands. Anything passed into this function
//  is assumed to be a complete command from a client.
// ------------------------------------------------------------------------
void Logon::Handle(string p_data) {

  if (m_errors == 5) {
    m_connection->Protocol().SendString(
        *m_connection,
        red + bold + "Too many incorrect responses, closing connection..." +
            newline);
    m_connection->Close();
    return;
  }

  if (m_state == NEWCONNECTION) // has not entered name yet
  {
    if (BasicLib::LowerCase(p_data) == "new") {
      m_state = NEWUSER;
      m_connection->Protocol().SendString(
          *m_connection, yellow + "Please enter your desired name: " + reset);
    } else {
      PlayerDatabase::iterator itr =
          PlayerDatabase::GetInstance().findfull(p_data);
      if (itr == PlayerDatabase::GetInstance().end()) {
        // name does not exist
        m_errors++;
        m_connection->Protocol().SendString(
            *m_connection,
            red + bold + "Sorry, the user \"" + white + p_data + red +
                "\" does not exist.\r\n" +
                "Please enter your name, or \"new\" if you are new: " + reset);
      } else {
        // name exists, go to password entrance.
        m_state = ENTERPASS;
        m_name = p_data;
        m_pass = itr->Password();

        m_connection->Protocol().SendString(
            *m_connection, green + bold + "Welcome, " + white + p_data + red +
                               newline + green +
                               "Please enter your password: " + reset);
      }
    }

    return;
  }

  if (m_state == NEWUSER) {
    // check if the name is taken:
    if (PlayerDatabase::GetInstance().hasfull(p_data)) {
      m_errors++;
      m_connection->Protocol().SendString(
          *m_connection, red + bold + "Sorry, the name \"" + white + p_data +
                             red + "\" has already been taken." + newline +
                             yellow +
                             "Please enter your desired name: " + reset);
    } else {
      if (!AcceptibleName(p_data)) {
        m_errors++;
        m_connection->Protocol().SendString(
            *m_connection, red + bold + "Sorry, the name \"" + white + p_data +
                               red + "\" is unacceptible." + newline + yellow +
                               "Please enter your desired name: " + reset);
      } else {
        m_state = ENTERNEWPASS;
        m_name = p_data;
        m_connection->Protocol().SendString(
            *m_connection,
            green + "Please enter your desired password: " + reset);
      }
    }

    return;
  }

  if (m_state == ENTERNEWPASS) {
    if (p_data.find_first_of(BasicLib::WHITESPACE) != string::npos) {
      m_errors++;
      m_connection->Protocol().SendString(
          *m_connection, red + bold + "INVALID PASSWORD!" + green +
                             "Please enter your desired password: " + reset);
      return;
    }

    m_connection->Protocol().SendString(
        *m_connection,
        green + "Thank you! You are now entering the realm..." + newline);

    Player p;
    p.Name() = m_name;
    p.Password() = p_data;

    // make the player the administrator if he's the first to log in.
    if (PlayerDatabase::GetInstance().size() == 0) {
      p.Rank() = ADMIN;
      p.ID() = 1;
    } else {
      p.ID() = PlayerDatabase::GetInstance().LastID() + 1;
    }

    // add the player
    PlayerDatabase::GetInstance().AddPlayer(p);

    // enter the game as a newbie.
    GotoGame(true);

    return;
  }

  if (m_state == ENTERPASS) {
    if (m_pass == p_data) {
      m_connection->Protocol().SendString(
          *m_connection,
          green + "Thank you! You are now entering the realm..." + newline);

      // enter the game
      GotoGame();
    } else {
      m_errors++;
      m_connection->Protocol().SendString(
          *m_connection, red + bold + "INVALID PASSWORD!" + newline + yellow +
                             "Please enter your password: " + reset);
    }

    return;
  }
}

// ------------------------------------------------------------------------
//  This notifies the handler that there is a new connection
// ------------------------------------------------------------------------
void Logon::Enter() {
  USERLOG.Log(GetIPString(m_connection->GetRemoteAddress()) +
              " - entered login state.");

  m_connection->Protocol().SendString(
      *m_connection,
      red + bold + "Welcome To SimpleMUD v1.0\r\n" +
          "Please enter your name, or \"new\" if you are new: " + reset);
}

// ------------------------------------------------------------------------
//  This changes the game state so that the player enters the game.
// ------------------------------------------------------------------------
void Logon::GotoGame(bool p_newbie) {
  Player &p = *PlayerDatabase::GetInstance().findfull(m_name);

  // log off the user if he's already connected.
  if (p.LoggedIn()) {
    p.Conn()->Close();
    p.Conn()->Handler()->Hungup();
    p.Conn()->ClearHandlers();
  }

  p.Newbie() = p_newbie;

  // record the users new connection
  p.Conn() = m_connection;

  // go to the game.
  p.Conn()->SwitchHandler(new Game(*p.Conn(), p.ID()));
}

// ------------------------------------------------------------------------
//  This checks if a user name is acceptible.
// ------------------------------------------------------------------------
bool Logon::AcceptibleName(const string &p_name) {
  static string inv = " \"'~!@#$%^&*+/\\[]{}<>()=.,?;:";

  // must not contain any invalid characters
  if (p_name.find_first_of(inv) != string::npos)
    return false;

  // must be less than 17 chars and more than 2
  if (p_name.size() > 16 || p_name.size() < 3)
    return false;

  // must start with an alphabetical character
  if (!isalpha(p_name[0]))
    return false;

  if (p_name == "new")
    return false;

  return true;
}

} // end namespace SimpleMUD
