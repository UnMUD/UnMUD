#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>


#define NULL ((void *)0)

#include "s_socket.h"

int make_service(short unsigned int port,
		 char               *my_hostname,
		 int                my_hostnamelen,
		 struct hostent     **hp,
		 struct sockaddr_in *sin)
{
  int s, i;
  struct hostent *h;

  if (gethostname(my_hostname,my_hostnamelen) < 0) {
    return -1; /* Error in hostname? */
  }
  if ((*hp = h = gethostbyname(my_hostname)) == NULL) {
    return -2; /* Error in gethostbyname */
  }
  bzero(sin,sizeof(struct sockaddr_in));
  sin->sin_family = AF_INET;
  sin->sin_port = htons(port);
  bcopy(h->h_addr_list[0],&(sin->sin_addr),h->h_length);

  if ((s = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    return -3; /* Error in socket call */
  }

  /* Bug fix for hanging sockets. Socket options need to be set before
   * the bind takes place or they dont do ANY good.
   */
  i = 2;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) == -1) {
	  progerror("setsockopt(reuseaddr)");
  }

  if (bind(s,sin,sizeof(*sin)) < 0) {
    close(s);
    return -4; /* Error in bind. */
  }
  if (listen(s,5) != 0) {
    close(s);
    return -5; /* Error in listen */
  }



/* Set non-blocking IO: 
 */
/*
  i=1;
  if (ioctl(s, FIONBIO, &i) == -1) {
    close(s);
    return -6;
  }
*/

  if (fcntl(s, F_SETFL, O_NDELAY) == -1) {
    close(s);
    return -6; /* Error in fcntl */
  }


  return s;
}


