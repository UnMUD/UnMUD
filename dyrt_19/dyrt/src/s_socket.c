#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include "s_socket.h"
#include <unistd.h>
#include <string.h>

int make_service(short unsigned int port,
                 char *my_hostname,
                 int my_hostnamelen,
                 struct hostent **hp,
                 struct sockaddr_in *sin)
{
  int s, i;

  if (gethostname(my_hostname, my_hostnamelen) < 0)
  {
    return -1; /* Error in hostname? */
  }
  if ((*hp = gethostbyname(my_hostname)) == NULL)
  {
    return -2; /* Error in gethostbyname */
  }
  bzero(sin, sizeof(struct sockaddr_in));
  sin->sin_family = AF_INET;
  sin->sin_addr.s_addr = INADDR_ANY;
  sin->sin_port = htons(port);
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    return -3; /* Error in socket call */
  }
  i = 2;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) == -1)
  {
    progerror("setsockopt(sndbuf)");
  }
  i = 58000;
  if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, &i, sizeof(i)) == -1)
  {
    progerror("setsockopt(sndbuf)");
  }
  if (bind(s, sin, sizeof(*sin)) < 0)
  {
    close(s);
    return -4;
  }
  if (listen(s, 5) != 0)
  {
    close(s);
    return -5;
  }
  if (fcntl(s, F_SETFL, O_NDELAY) == -1)
  {
    close(s);
    return -6;
  }
  return s;
}
