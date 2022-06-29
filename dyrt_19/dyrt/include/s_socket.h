#ifndef __S_SOCKET_H__ 
#define __S_SOCKET_H__ 


int make_service(short unsigned int port,
		 char               *my_hostname,
		 int                my_hostnamelen,
		 struct hostent     **hp,
		 struct sockaddr_in *sin);





#endif /* Add nothing past this line... */
