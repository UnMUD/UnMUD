// MUD Programming
// Ron Penton
// (C)2003
// SocketSet.cpp - This file defines a socket set, which will be used for
//                 polling many sockets at the same time for activity.
//                 It is essentially a select() wrapper.

#include "SocketSet.h"
#include <algorithm>

namespace SocketLib {

SocketSet::SocketSet()
    : m_set(), m_activityset(),
#ifndef WIN32
      m_socketdescs()
#endif
{
  FD_ZERO(&m_set);
  FD_ZERO(&m_activityset);
}

void SocketSet::AddSocket(const Socket &p_sock) {
  // add the socket desc to the set
  FD_SET(p_sock.GetSock(), &m_set);

// if linux, then record the descriptor into the vector,
// and check if it's the largest descriptor.
#ifndef WIN32
  m_socketdescs.insert(p_sock.GetSock());
#endif
}

void SocketSet::RemoveSocket(const Socket &p_sock) {
  FD_CLR(p_sock.GetSock(), &m_set);

#ifndef WIN32
  // remove the descriptor from the vector
  m_socketdescs.erase(p_sock.GetSock());
#endif
}

} // namespace SocketLib
