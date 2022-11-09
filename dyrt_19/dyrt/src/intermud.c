/*******************************************************************
 * $Header: /home/kender/muddev/marxkove/include/RCS/board.h,v 1.2 1995/02/17 22
:01:22 kender Exp $
 *
 * $Revision: 1.1 $
 * $Date: 1996/08/10 01:28:47 $
 * $State: Exp $
 * $Author: testmud $
 * $Locker:  $
 *
 *-----------------------------------------------------------------------
 *                         Kove Development Code
 *
 * $Log: intermud.c,v $
 * Revision 1.1  1996/08/10 01:28:47  testmud
 * Initial revision
 *
 * Revision 1.2  1996/02/19  07:25:44  kender
 * Added code to support dynamic arrays for mudlist instead of hash table
 * added function documentation
 * Added mudlist saving
 *
 * Revision 1.1  1996/02/15  01:25:28  kender
 * Initial revision
 *
 *
 *********************************************************************/

#include "kernel.h"
#include <sys/types.h>        /* include files for socket handling */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "stdinc.h"
#include "intermud.h"
#include "intermudP.h"

int imfd = -1;
int oobfd = -1;
int udpfd = -1;

char *rname;
char *raddr;
int rport;
int pport=0;
int tport=0;
int uport=0;
int mpasswd=0;
char *mudlib="0";
char *basemudlib="0";
char *driver="0";
char *mudType="0";
char *openStatus="0";
char *mudname="0";
char *adminEmail = "0";
int omid=0;
int ocid=0;
int numMuds=0;
char obuf[10240];


char *services[] = {
  "tell", "who", "finger", "locate", "channel", "news", "mail",
  "file", "smtp", "ftp", "nntp", "http", "rcp" , TABLE_END
};

pdarray mudlist = NULL;


imMapping Mappings[] = {
  { MESG_TELL,		"tell",			_imTell },
  { MESG_EMOTETO,	"emoteto",		_imEmote },
  { MESG_WHO_REQ,	"who-req",		_imWhoReq },
  { MESG_WHO_REPLY,	"who-reply",		_imWhoReply },
  { MESG_FINGER_REQ,	"finger-req",		_imFingerReq },
  { MESG_FINGER_REPLY,	"finger-reply",		_imFingerReply },
  { MESG_LOCATE_REQ,	"locate-req",		_imLocateReq },
  { MESG_LOCATE_REPLY,	"locate-reply",		_imLocateReply },
  { MESG_CHANLIST_REPLY,"chanlist-reply",	_imChanlReply },
  { MESG_CHANNEL_M,	"channel-m",		_imChanM },
  { MESG_CHANNEL_E,	"channel-e",		_imChanE },
  { MESG_CHANNEL_T,	"channel-t",		_imChanT },
  { MESG_CHANNEL_ADD,	"channel-add",		_imChanAdd },
  { MESG_CHANNEL_REMOVE,"channel-remove",	_imChanRem },
  { MESG_CHANNEL_ADMIN,	"channel-admin",	_imChanAdmin },
  { MESG_CHAN_WHO_REQ,	"channel-who-req",	_imChanWhoReq },
  { MESG_CHAN_WHO_REPLY,"channel-who-reply",	_imChanWhoReply},
  { MESG_CHANNEL_LISTEN,"channel-listen",	_imChanListen},
  { MESG_CHAN_USER_REQ,	"chan-user-req",	_imChanUserReq},
  { MESG_CHAN_USER_REPLY,"chan-user-reply",	_imChanUserReply },
  { MESG_NEWS_READ_REQ,	"news-read-req",	_imNewsReadReq },
  { MESG_NEWS_POST,	"news-post",		_imNewsPost },
  { MESG_NEWSGRP_LIST_REQ,"newsgrp-list-req",	_imNewsListReq },
  { MESG_MAIL,		"mail",			_imMail },
  { MESG_FILE,		"file",			_imFile },
  { MESG_ERROR,		"error",		_imError },
  { MESG_STARTUP_REQ_3,	"startup-req-3",	_imStartup3 },
  { MESG_STARTUP_REPLY,	"startup-reply",	_imStartupReply },
  { MESG_SHUTDOWN,	"shutdown",		_imShutdown },
  { MESG_MUDLIST,	"mudlist",		_imMudList },
  { MESG_UNKNOWN,	"unknown",		_imUnknown },
};

/***********************************************************
 *
 * mudSort
 *
 * Comparision function sed by the darray functionset and the 
 * qsort system call to sort the mud list.
 */

static int mudSort( const void *a, const void *b)
{
  mudInfo *mudA = * (mudInfo**)a;
  mudInfo *mudB = * (mudInfo**)b;

  return strcasecmp( mudA->name, mudB->name );
}

/***********************************************************
 * 
 * mudFind
 *
 * Comparision function used by the darray function to 
 * bsearch on the sorted mudlist.
 *
 */

static int mudFind( const void *key, const void *mud)
{
  char *find = *(char**)key;
  mudInfo *mudinfo = * (mudInfo**)mud;
  int minLen;

  if(mudinfo == NULL) return 1;

  minLen = ( strlen(find) < strlen(mudinfo->name) ) ?
               strlen(find) : strlen(mudinfo->name);

  return strncasecmp( find, mudinfo->name, minLen );
}

/***********************************************************
 * 
 * mudFind
 *
 * Comparision function used by the darray function to 
 * bsearch on the sorted mudlist.
 *
 */

static int mudStrictFind( const void *key, const void *mud)
{
  char *find = *(char**)key;
  mudInfo *mudinfo = * (mudInfo**)mud;
  int minLen;

  if(mudinfo == NULL) return 1;

  return strcasecmp( find, mudinfo->name );
}

/*****************************************************
 *
 * __iConn
 *
 *  Create the connection to the intermud3 router and
 *  send the startup-req-3 packet to initiate the login
 *  sequence
 */

int __iConn(char *addr, int port)
{
  int sock;
  static struct hostent *host;
  static struct sockaddr_in server;
  static struct in_addr inaddr;
  static int done = 0;

  if(done == 0)
    {
      host = gethostbyname(addr);
      if(!host) return -1;
      
      server.sin_family = AF_INET;
      server.sin_port = htons(port);
      server.sin_addr = (*(struct in_addr*)host->h_addr_list[0]);
      done = 1;
    }
  
  sock = socket(AF_INET,SOCK_STREAM,0);
  if (connect(sock,(struct sockaddr *) &server,sizeof(server)) <0) {       
    close(sock);
    return -1;
  }

  obuf[0] = '\0';

  /* Message Header */
  sprintf(obuf,
	  "({\"startup-req-3\",5,\"%s\",0,\"%s\",0,%d,%d,%d,%d,%d,%d," /* everything up to ports. */
	  "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\","         /* mud info */
	  "([\"tell\":1,\"who\":1,\"finger\":1,\"locate\":1,]),0,})",
	  mudname,rname,mpasswd,omid,ocid,pport,tport,uport,
	  mudlib,basemudlib,driver,mudType,openStatus,adminEmail);
  imfd = sock;
  __iSend(obuf);

  return sock;
}

/*****************************************************
 *
 * uecopy
 *
 * strdup varient that strips out escaped characters from
 * a string.
 *
 */

char *uecopy(char *oldStr)
{
  /* Unescape quotes from a string as we copy it */
  char *temp = malloc(strlen(oldStr)+1);
  char *p,*q;
  for( p = oldStr, q = temp;
       *p; )
    {
      if(*p == '\\') p++; 
      *q++ = *p++;
    }
  *q = '\0';
  return temp;
}

/*****************************************************
 *
 * __iSend
 *
 *  Sends the length of the given string, then the string
 *  itself to the i3 router.
 *
 */

int __iSend(char *str)
{
  int sendVal;
  int len = strlen( str ) + 1;
  extern fd_set sockets_fds;

  if( imfd < 0 ) return -1;
  len = htonl( len );

  if( send( imfd, &len, sizeof( int ) , 0 ) > 0 )
    {
      if( ( sendVal = send( imfd, str, strlen( str ) + 1, 0 ) ) > 0 )
        {
          /* mudlog( "INTERMUD: Sent [%s]", str );  /**/
          return sendVal;
        } 
    }

  mudlog( "INTERMUD: ERROR Sending [%d][%s]", errno, str );
  close(imfd);
  FD_CLR(imfd,&sockets_fds);
  imfd = -1;
}

/*****************************************************
 *
 * boot_intermud
 *
 * This function is called by the bootstrap function to
 * load configuration data from the intermud.conf file
 * to set up the intermud connection.
 *
 */

int boot_intermud(FILE *fp, char *fname)
{
  char buf[256];
  FILE *nfp;

  /* Read in the config file... */
  while(!feof(fp))
    {
      char line[256],*opt,*value;
      fgets(line,255,fp);
      line[strlen(line)-1] = '\0';

      if(line[0] == '#') continue;
      opt = strtok(line,":"); if(!opt) continue;
      value = strtok(NULL,":"); if(!value) continue;
      
      if(EQ(opt,"mudname")) { mudname = COPY(value); }
      else if(EQ(opt,"routerip")) { raddr = COPY(value); }
      else if(EQ(opt,"routername")) { rname = COPY(value); }
      else if(EQ(opt,"routerport")) { rport = atoi(value); }
      else if(EQ(opt,"player port")) { pport = atoi(value); }
      else if(EQ(opt,"tcp port")) {tport = atoi(value); }
      else if(EQ(opt,"udp port")) {uport = atoi(value); }
      else if(EQ(opt,"mudlib")) {mudlib = COPY(value); }
      else if(EQ(opt,"basemudlib")) {basemudlib = COPY(value); }
      else if(EQ(opt,"driver")) {driver = COPY(value); }
      else if(EQ(opt,"email")) { adminEmail = COPY(value); }
      else if(EQ(opt,"mudType")) {mudType = COPY(value); }
      else if(EQ(opt,"openStatus")) {openStatus = COPY(value); }
    }


  mudlist = NULL;
  
  /*
   * when we support OOB tcp/udb, initialize them here.
   */

  if(nfp = fopen(INTERMUD_PASS,"r")) {
    fscanf(nfp,"%d",&mpasswd);
    fclose(nfp);
  }
  if(nfp = fopen(MUDLIST_ID,"r")) {
    fscanf(nfp,"%d",&omid);
    fclose(nfp);
  }
  if(nfp = fopen(CHANLIST_ID,"r")) {
    fscanf(nfp,"%d",&ocid);
    fclose(nfp);
  }

  /*
   * build up the mudlist, if we can 
   */
  if(nfp = fopen(MUDLIST_FILE,"r")) 
    {
      int numMuds;

      fscanf( nfp, "%d ", &numMuds );

      /* Init mudlist */
      if( mudlist == NULL )
        mudlist = newArray( numMuds, 10, mudSort, mudFind );

      while( !feof( nfp ) && numMuds-- )
        {
          char line[256];

	  mudInfo *newMud = (mudInfo*)malloc( sizeof( mudInfo ) );

          fgets(line,255,nfp); line[strlen(line)-1] = '\0';
          newMud->name = COPY(line);

          fgets(line,255,nfp); line[strlen(line)-1] = '\0';
          newMud->type = COPY(line);

          fgets(line,255,nfp); line[strlen(line)-1] = '\0';
          newMud->addr = COPY(line);

          fgets(line,255,nfp); line[strlen(line)-1] = '\0';
          newMud->playerPort = atoi(line);

          fgets(line,255,nfp); line[strlen(line)-1] = '\0';
          newMud->tcpPort = atoi(line);

          fgets(line,255,nfp); line[strlen(line)-1] = '\0';
          newMud->udpPort = atoi(line);

          fgets(line,255,nfp); line[strlen(line)-1] = '\0';
          newMud->isup = atoi(line);

          arrayAdd(mudlist, (void*)newMud );
        }

      fclose(nfp);
    }

  return 1;
}

/*****************************************************
 *
 * newList
 *
 *  Create a new list object.  This list represents an
 *  lpmud array, containing strings, ints, mappings, or
 *  other lists.
 */

listObject *newList(int numelements)
{
  listObject *newObj = (listObject*)malloc(sizeof(listObject));

  newObj->numElements = numelements;
  newObj->data = (char**)malloc(sizeof(char*) * numelements);
  newObj->type = (elemTypes*)malloc( numelements * sizeof(elemTypes) );
  return newObj;
}

/*****************************************************
 *
 * mappingObject
 *
 *  Create a new mapping object.  A mapping object is
 *  an associative list of form key : value, where key
 *  and value can both be one of all four lp types.
 *
 */

mappingObject *newMap(int numelements)
{
  mappingObject *map = (mappingObject*)malloc(sizeof(mappingObject));

  map->numElements = numelements;
  map->keyType = (elemTypes*)malloc(sizeof(elemTypes)*map->numElements);
  map->valueType = (elemTypes*)malloc(sizeof(elemTypes)*map->numElements);
  map->keys = (char**)malloc(sizeof(char*)*map->numElements);
  map->values = (char**)malloc(sizeof(char*)*map->numElements);
  return map;
}

/*****************************************************
 *
 * parseList2
 *
 *  Second stage of the i3 list parser.  This function 
 *  may be called recursively if the list we're parsing
 *  has another list inside of it.  Mail kender@hollyfeld.org
 *  if there are more detailed questions about the parsing 
 *  functions.
 *
 */

void parseList2(listObject *list)
{
  int elementLength = 0; /* The length of this element */
  int elementID = 0;     /* The offset of this element */
  
  /* Ok, first, let's figure out just what thell it is we're looking at... */
  if( list->origString[0] == '}' )
    {
      /*
       * We've found the end of the list that we're looking at.
       * Start unwinding our parser
       */
      if(list->numElements)
	{
	  list->type = (elemTypes*)malloc( list->numElements * sizeof(elemTypes) );
	  list->data = (char**)malloc( list->numElements * sizeof(char*) );
	}
      return;
    }

  /* Check to see if we're looking at a string */
  if( list->origString[0] == '"' )
    {
      char *start = list->origString;
      char *p = start+1;
      
      /* Step to the next quote */
      for( ;*p && (*p != '"'); p++ )
	if(*p == '\\') p++; /* Step past the next char, don't care what it is now */
      
      if(*p)
	{
	  int myIndex = list->numElements++;
	  *p = '\0';  /* mark off the end of this string */
	  
	  /* We have a string! */
	  list->origString = ( *(p+1) == ',') ? (p+2) : (p+1);

	  /* Continue parsing down the list */
	  parseList2(list);

	  /* Fill in the data */
	  list->type[myIndex] = TYPE_STRING;
	  list->data[myIndex] = uecopy(start+1);
	}
    }

  /* Check to see if we're a number */
  else if( isdigit( list->origString[0] ) || ( list->origString[0] == '-' ) )
    {
      int myIndex = list->numElements++;
      char *p,*start = list->origString;

      for( p = start; (*p != ',') && (*p != '}'); p++)
	if(*p == '\\') p++;

      /* ok, p now points to the end of the number. */
      if(*p == ',') *p++ = '\0';
      list->origString = p;

      parseList2(list);

      list->type[myIndex] = TYPE_INT;
      list->data[myIndex] = (char*)atoi(start);
    }
  
  /* Check to see if we're looking at a mapping or a list... */
  else if( list->origString[0] == '(' )
    {
      /* We have a mapping or a list, pick one.. */
      if( list->origString[1] == '[' )
	{
	  /* We have a mapping... */
	  char *start = list->origString;
	  char *p = start;
	  int myIndex = list->numElements++;

	  mappingObject *map = parseMapping(p);
	  if(map) p = map->origString;
	  
	  /* list->origString now points to the closing 
	  /* p now points to the closing ]. */
	  if( *(p + 2) == ',' ) list->origString = (p+3);
	  else list->origString = (p+2);

	  /* Continue parsing the list */
	  parseList2(list);
	  
	  list->type[myIndex] = TYPE_MAPPING;
	  list->data[myIndex] = (char*)map;
	}
      else if( list->origString[1] == '{' )
	{
	  /* We have a list... */
	  char *start = list->origString;
	  char *p = start;
	  int myIndex = list->numElements++;

	  listObject *newList = parseList(p);
	  if(newList) p = newList->origString;
	  /* list->origString now points to the closing */
	  /* p now points to the closing ]. */
	  if( *(p + 2) == ',' ) list->origString = (p+3);
	  else list->origString = (p+2);

	  /* Continue parsing the list */
	  parseList2(list);
	  
	  list->type[myIndex] = TYPE_LIST;
	  list->data[myIndex] = (char*)newList;
	}
    }
}

/*****************************************************
 *
 * parseList
 *
 *  This is the first stage in parsing a list, it creates
 *  the lsit object, initializes it, and calls parseList2
 *  to start the parsing process.
 *
 */

listObject *parseList(char *line)
{
  listObject *list;
  
  /* This isn't a list! */
  if(strncmp(line,"({",2)) return NULL;

  list = (listObject*)malloc(sizeof(listObject));

  list->origString = line+2;
  list->numElements = 0;
  list->type = NULL;
  list->data = NULL;

  parseList2(list);

  return list;
}

/*****************************************************
 *
 * parseMap2
 *
 *  This is the second stage in a mapping parsing.  This
 *  function works almost like the parseList2 function,
 *  with the exception that it parses out both keys and
 *  values, instead of just elements.  Note: unlike full
 *  lp mappings, keys must be strings or ints.  We don't
 *  support lists or other mappings as keys.
 *
 */

void parseMap2(mappingObject *map)
{
  
  /* First, are we at the end of the mapping? */
  if(map->origString[0] == ']')
    {
      map->keyType = (elemTypes*)malloc(sizeof(elemTypes)*map->numElements);
      map->valueType = (elemTypes*)malloc(sizeof(elemTypes)*map->numElements);
      map->keys = (char**)malloc(sizeof(char*)*map->numElements);
      map->values = (char**)malloc(sizeof(char*)*map->numElements);
    }
  else
    {
      /* Ok, we weren't at the end of the mapping, then we are
       * at the beginning of a key/value pair.  Find the next
       * key.
       */
      char *p = map->origString;
      char *key = map->origString;
      elemTypes keyType = TYPE_STRING;
      int myIndex;

      if(*p=='"')
	{
	  key++;
	  for(p = key;*p && *p != '"'; p++)
	    if(*p == '\\') p++;
	  
	  if(!(*p))
	    {
	      /* We have no key!  whoops! */
	      return;
	    }
	  
	  *p++ = '\0'; /* p now points to the : */
          key = uecopy(key);
	}
      else if( isdigit(*p) )
	{
	  key = (char*)atoi(p);
	  while(*p != ':') p++;
	  keyType = TYPE_INT;
	}
      p++;         /* p now points to the first value past the colon */
      myIndex = map->numElements++;

      /*
       * Now that we have the key, we need the element info.
       */
      if(*p == '"') /* We found a string */
	{
	  char *start = ++p;

	  /* Step to the next quote */
	  for( ;*p && (*p != '"'); p++)
	    if(*p == '\\')p++;

	  if(*p)
	    {
	      *p = '\0';
	      map->origString = ( *(p+1) == ',') ? (p+2) : (p+1);

	      parseMap2(map);
	      map->keyType[myIndex] = keyType;
	      map->valueType[myIndex] = TYPE_STRING;
	      map->keys[myIndex] = key;
	      map->values[myIndex] = uecopy(start);
	      return;
	    }
	}
      else if ( ( isdigit (*p ) ) || ( *p == '-' ) )
	{
	  char *start = p;
	  for( ; (*p != ',') && (*p != ']'); p++)
	    if(*p == '\\') p++;

	  /* Ok, p now points to the end of the number. */
	  if(*p == ',') *p++ = '\0';
	  map->origString = p;

	  parseMap2(map);
	  map->keyType[myIndex] = keyType;
	  map->valueType[myIndex] = TYPE_INT;
	  map->keys[myIndex] = key;
	  map->values[myIndex] = (char*)atoi(start);
	  return;
	}
      else if( *p == '(' )
	{
	  if ( *(p+1) == '[' )
	    {
	      char *start = p;

	      mappingObject *newMap = parseMapping(p);
	      if(newMap) p = newMap->origString;

	      if( *(p + 2) == ',') map->origString = (p+3);
	      else map->origString = (p+2);

	      parseMap2(map);
	      map->keyType[myIndex] = keyType;
	      map->valueType[myIndex] = TYPE_MAPPING;
	      map->keys[myIndex] = key;
	      map->values[myIndex] = (char*)newMap;
	      return;
	    }
	  else if( *(p+1) == '{' )
	    {
	      char *start = p;

	      listObject *newList = parseList(p);
	      if(newList) p = newList->origString;

	      if( *(p + 2) == ',') map->origString = (p+3);
	      else map->origString = (p+2);

	      parseMap2(map);
	      
	      map->keyType[myIndex] = keyType;
	      map->valueType[myIndex] = TYPE_LIST;
	      map->keys[myIndex] = key;
	      map->values[myIndex] = (char*)newList;
	      return;
	    }
	}
    }
  
}

/*****************************************************
 *
 * parseMapping
 *
 *  This function is the first stage in the map parsing.
 *  It creates a mapping object, and starts the processing
 *  with parseMap2.
 *
 */

mappingObject *parseMapping(char *line)
{
  int depth;
  mappingObject *map;
  char *p = line+2;

  if(strncmp(line,"([",2)) return NULL;

  map = (mappingObject*)malloc( sizeof(mappingObject) );
  map->origString = line+2;
  map->numElements = 0;
  map->keyType = NULL;
  map->valueType = NULL;
  map->keys = NULL;
  map->values = NULL;

  parseMap2(map); 

  return map;
}

/*****************************************************
 *
 * destroyList
 *
 *  This function will destory a list object, recursively
 *  destroying lists and maps that are in the list.
 *
 */

void destroyList(listObject *list)
{
  int x;

  if(!list) return;
  for( x = 0; x < list->numElements; x++ )
    {
      if(list->type[x] == TYPE_LIST)
	destroyList( (listObject*) list->data[x] );
      else if( list->type[x] == TYPE_MAPPING )
	destroyMapping( (mappingObject*) list->data[x] );
      else if( list->type[x] == TYPE_STRING )
        if( list->data[x] ) free(list->data[x]);
    }
  free(list->data);
  free(list->type);
  free(list);
}

/*****************************************************
 *
 * destroyMapping
 *
 *  This function will destroy a mapping object, recursively
 *  destroying lists and maps that are in the mapping objects.
 *
 */

void destroyMapping(mappingObject *map)
{
  int x;
  
  if(!map) return;
  for(x = 0; x < map->numElements; x++)
    {
      switch(map->keyType[x])
	{
	case TYPE_LIST: destroyList((listObject*)map->keys[x]); break;
	case TYPE_MAPPING: destroyMapping((mappingObject*)map->keys[x]); break;
        case TYPE_STRING: if(map->keys[x]) free(map->keys[x]); break;
	}
      switch(map->valueType[x])
	{
	case TYPE_LIST: destroyList((listObject*)map->values[x]); break;
	case TYPE_MAPPING: destroyMapping((mappingObject*)map->values[x]); break;
        case TYPE_STRING: if(map->values[x]) free(map->values[x]); break;
	}
    }
  free(map->keys);
  free(map->values);
  free(map->keyType);
  free(map->valueType);
  free(map);
}
    
/*****************************************************
 *
 * buildImPacket2
 *
 *  This function will build an intermud packet from
 *  a mapping object, turning it into a mudmode compatable
 *  string.
 *
 */

int buildImPacket2(mappingObject *map,char *str)
{
  int x;
  char temp[256];

  strcat(str,"([");
  for( x = 0; x < map->numElements; x++)
    {
      switch(map->keyType[x])
        {
          case TYPE_STRING: 
	    strcat(str,"\"");
            strcat(str,map->keys[x]);
	    strcat(str,"\"");
 	    break;
	  case TYPE_INT:
	    sprintf(temp,"%d",map->keys[x]);
	    strcat(str,temp);
	    break;
 	  case TYPE_LIST:
	    buildImPacket((listObject*)map->keys[x],str);
	    break;
	  case TYPE_MAPPING:
	    buildImPacket2((mappingObject*)map->keys[x],str);
            break;
        }
      strcat(str,":");
      switch(map->valueType[x])
        {
          case TYPE_STRING: 
	    strcat(str,"\"");
            strcat(str,map->values[x]);
	    strcat(str,"\"");
 	    break;
	  case TYPE_INT:
	    sprintf(temp,"%d",map->values[x]);
	    strcat(str,temp);
	    break;
 	  case TYPE_LIST:
	    buildImPacket((listObject*)map->values[x],str);
	    break;
	  case TYPE_MAPPING:
	    buildImPacket2((mappingObject*)map->values[x],str);
            break;
        }
      strcat(str,",");
    }
  strcat(str,"])");
}

/*****************************************************
 *
 * buildImPacket
 * 
 *  Builds an intermud packet from a list object.
 *
 */

int buildImPacket(listObject *list,char *str)
{
  int x;
  char temp[256];

  strcat(str,"({");
  for( x = 0; x < list->numElements; x++)
    {
      switch(list->type[x])
        {
          case TYPE_STRING: 
	    strcat(str,"\"");
            strcat(str,list->data[x]);
	    strcat(str,"\"");
 	    break;
	  case TYPE_INT:
	    sprintf(temp,"%d",list->data[x]);
	    strcat(str,temp);
	    break;
 	  case TYPE_LIST:
	    buildImPacket((listObject*)list->data[x],str);
	    break;
	  case TYPE_MAPPING:
	    buildImPacket2((mappingObject*)list->data[x],str);
            break;
 	}
      strcat(str,",");
    }
  strcat(str,"})");
}

static int packetLen = 0;
static int tlen = 0;
static char *rbuf = NULL;

/*******************************************************
 *
 * imPacket
 *
 *   This function will read a packet in off of the intermud
 *   file descriptor.  It currently assumes that there will 
 *   be only one file descriptor.  This will change in the near
 *   future when OOB support is put in.  Once it reads the data
 *   in it will break out the packet into a listObject, and the
 *   dispatch the function that handles the given message type.
 *
 */
void imPacket(int fd)
{
  char *type,*orMud,*orUser,*tMud,*tUser;
  int x,ttl;
  listObject *packet;
  
  if( packetLen == 0 ) /* We're not in the middle of reading a packet */
    {
      /*
       * get the length of the imud packet
       */
      read(fd,&packetLen,sizeof(int));
      packetLen = ntohl(packetLen);
      tlen = 0;

      if(packetLen > 0)
        {
          /* mudlog("DEBUG: Packet length is [%d]\n",packetLen); /**/
          rbuf = malloc(packetLen);
        }
      else return;
    }

  do {
    int x;
    x = read(fd,rbuf+tlen,packetLen-tlen);
    if( x <= 0 ) return;
    /* mudlog("INTERMUD: ping [%d:%d][%s]",tlen,packetLen,rbuf); /**/
    tlen += x;
  } while (tlen < packetLen);
      
  /* mudlog("INTERMUD: recieved [%d:%d][%s]",tlen,packetLen,rbuf); /**/
      
  packet = parseList(rbuf);
      
  if(packet) 
    {
      type = packet->data[0];
      ttl = (int)packet->data[1];
      orMud = packet->data[2];
      orUser = packet->data[3];
      tMud = packet->data[4];
      tUser = packet->data[5];
          
      for( x = 0; Mappings[x].type != MESG_UNKNOWN ; x++ )
        {
          if(EQ(type,Mappings[x].name))
            {
              Mappings[x].handler(type,ttl,orMud,orUser,
    			          tMud,tUser,packet);
              break;
            }
        }     
      destroyList(packet);
      free(rbuf);
    }
  rbuf = NULL;
  packetLen = 0;
  return;
}

/***********************************************************\
 *
 * Intermud Message Handlers
 *
 * These functions will handle the parsing of individual
 * intermud messages.
\***********************************************************/ 

/*
 * _imStartupReply
 *
 * This function handles an i3 startup
 * reply packet.
 */
DECLARE_HANDLER(_imStartupReply )
{
  if( packet->type[6] == TYPE_LIST )
    {
      mudlog("INTERMUD: Startup reply router list recieved.");
    }
  if( packet->type[7] == TYPE_INT )
    {
      FILE *nfp;
      
      mudlog("INTERMUD: Mud password recieved.");
      if(nfp = fopen(INTERMUD_PASS,"w"))
	{
	  fprintf(nfp,"%d",(int)packet->data[7]);
	  fclose(nfp);
	}
    }
  mudlog("INTERMUD: Successful connection.");
}

/*
 * _imMudList - this command will send in
 *              the intermud mud list.
 */

DECLARE_HANDLER(_imMudList )
{
  FILE *nfp;
  if( packet->type[6] == TYPE_INT )
    {
      
      /* mudlog("INTERMUD: Mudlist ID %d recieved.",(int)packet->data[6]); /**/
      if(nfp = fopen(MUDLIST_ID,"w"))
	{
	  omid = (int)packet->data[6];
	  fprintf(nfp,"%d",omid);
	  fclose(nfp);
	}
    }
  if( packet->type[7] == TYPE_MAPPING)
    {
      mappingObject *mudMap = (mappingObject*)packet->data[7];
      int x;
      
      /* mudlog("INTERMUD: Mudlist mapping recieved, filling hash table."); /**/

      /*
       * init hash table here.
       */
      if(mudlist == NULL)
	mudlist = newArray( 10, 10, mudSort, mudFind);

      /*
       * For each mud that came in the list
       */
      for( x = 0; x < mudMap->numElements ; x++)
	{
	    {
	      char *name = COPY(mudMap->keys[x]);
	      char *p;
	      mudInfo *oldMud;
	      
	      for(p=name;*p;p++) *p = tolower(*p);
		  
	      if ( mudMap->valueType[x] == TYPE_LIST)
		{
		  listObject *mudItems = (listObject*)mudMap->values[x];
		  mudInfo *newMud;

		  oldMud = NULL;
		  if ( oldMud = (mudInfo*)arrayLFindWith(mudlist,(void*) name , mudStrictFind ) )
		    {
		      newMud = oldMud;
		      free(oldMud->name);
		      if(oldMud->type) free(oldMud->type);
		      if(oldMud->addr) free(oldMud->addr);
		    }
		  else
		    {
		      numMuds++;
		      newMud = (mudInfo *)malloc(sizeof(mudInfo));
		    }
		  
		  newMud->name = COPY(mudMap->keys[x]);
		  newMud->type = NCOPY(mudItems->data[8]);
		  newMud->addr = NCOPY(mudItems->data[1]);
		  newMud->playerPort = (int)mudItems->data[2];
		  newMud->tcpPort = (int)mudItems->data[3];
		  newMud->udpPort = (int)mudItems->data[4];
		  newMud->isup = ( (int)mudItems->data[0] ) == -1;
		  
		  /*
		   * see if this mud is in our hash table already
		   */
		  if(!oldMud)
                    {
		      arrayAdd(mudlist, (void*) newMud );
                    }
		}
	      else if ( mudMap->valueType[x] == TYPE_INT )
		{
		  mudInfo *oldMud;
		  /* We won't delete muds just yet :) 
		  if ( oldMud = (mudInfo*)arrayFind(mudlist,(void*)name) )
		    {
		      arrayRemove(mudlist, (void*)oldMud );
		      numMuds--;
		      free(oldMud->name);
		      if(oldMud->type) free(oldMud->type);
		      if(oldMud->addr) free(oldMud->addr);
		      free(oldMud);
		    }
                  /**/
		}
	    }

	}
      /* 
       * Write out updated mudlist file
       * and id
       */
      arraySort(mudlist);

      if( nfp = fopen( MUDLIST_FILE, "w" ) )
        {
          int loop;

          fprintf(nfp,"%d\n", arrayGetSize( mudlist ) );

          for( loop = 0; loop < arrayGetSize( mudlist ); loop++ )
            {
              mudInfo *mud = arrayGet( mudlist, loop );
              if(!mud) continue;
              fprintf(nfp,"%s\n%s\n%s\n%d\n%d\n%d\n%d\n", 
                      mud->name,mud->type,mud->addr,
                      mud->playerPort,mud->tcpPort,mud->udpPort,mud->isup);
            }
          fclose(nfp);
        }
    }
}

/*******************************************************
 *
 * _imTell
 *
 *  packet handler to handle an intermud3 tell packet
 *  This function will try to find the user, and if they
 *  are available to intermud, will send the message to that user.
 *
 */
DECLARE_HANDLER(_imTell )
{
  if(packet->type[5] == TYPE_STRING)
    {
      int target;

      if( (target = fpbns(packet->data[5])) == -1)
	{
	  /* Bad player, do something here! */
	  return;
	}

      if( !ptstflg( target, PFL_INTERMUD ) )
        {
	  /* User can't access intermud, fail silently */
	  return;
        }
   
      if( ststflg( target, SFL_INTERVIS ) )
        {
 	  /* Target is ignoring intermud, fail silently */
	  return;
	}

      if(packet->type[7] == TYPE_STRING)
        {
	  sendf(target,"%s@%s tells you '%s'\n",
	        packet->data[6],
	        packet->data[2],
	        packet->data[7]);
        }
    }
}

/*******************************************************
 *
 * _imEmote
 *
 *   This function is the packet handler for the emoteto 
 *   packet type.
 *
 */
DECLARE_HANDLER(_imEmote )
{
  if(packet->type[5] == TYPE_STRING)
    {
      int target;
      char *message,*mud,*name,*q,*p,outbuf[2048];

      message = packet->data[7];
      mud = packet->data[2];
      name = packet->data[6];
      if(!message) message = "";
      if(!mud) mud = "";
      if(!name) name = "";
      
      if( (target = fpbns(packet->data[5])) == -1)
	{
	  /* Bad player, do something here! */
	  return;
	}
      if( !ptstflg( target, PFL_INTERMUD ) )
        {
	  /* User can't access intermud, fail silently */
	  return;
        }
   
      if( ststflg( target, SFL_INTERVIS ) )
        {
 	  /* Target is ignoring intermud, fail silently */
	  return;
	}

      for(p = message, q = outbuf; *p; *p++,*q++)
	{
	  if(*p == '$')
            {
	      if(*(p+1) == 'N')
	        {
	  	  char *r;
		
		  for(r=name;*r;) *q++ = *r++;
		  *q++ = '@';
		  for(r=mud;*r;) *q++ = *r++;
		  p++;
		  q--;
		  continue;
	      }
            }
	  *q = *p;
	}
      *q = '\0';
      sendf(target,"%s\n",outbuf);

    }
}

/*******************************************************
 *
 * _imWhoReq
 *
 *  This function handles the building of a who list in
 *  respones to a who-request packet, and sends out a 
 *  who-reply packet in response.
 *
 */
DECLARE_HANDLER(_imWhoReq ) 
{
  listObject *list = newList(max_players);

  int i,num;

  sprintf(obuf,"({\"who-reply\",5,\"%s\",0,\"%s\",\"%s\",",
          mudname,
	  packet->data[2],
          packet->data[3]);
  for(i = 0, num = 0; i < max_players; i++)
    {
      listObject *thisPlayer;

      if ( EMPTY(pname(i))
        || !players[i].iamon
        || pstr(i) < 0
        || !ptstflg(i,PFL_INTERMUD) /* Can't access, can't be seen */
        || ststflg( i, SFL_INTERVIS ) /* hiding from intermud? */
        || pvis(i) > 0 ) { continue; }

      num;
      thisPlayer = newList(3);
      thisPlayer->type[0] = TYPE_STRING;
      thisPlayer->data[0] = COPY(pname(i));
      thisPlayer->type[1] = TYPE_INT;
      thisPlayer->data[1] = (time(0) -players[i].last_command);
      thisPlayer->type[2] = TYPE_INT;
      thisPlayer->data[2] = (char*)0;
      list->type[num] = TYPE_LIST;
      list->data[num++] = (char*)thisPlayer;
    }
  list->numElements = num;
  buildImPacket(list,obuf);
  destroyList(list);
  strcat(obuf,",})");
  __iSend(obuf);
}

/*******************************************************
 *
 * _imWhoReply
 *
 *  This function handles a who-reply that comes in from another
 *  mud, it will build hte mudlist and send it to the user.
 *
 */

DECLARE_HANDLER(_imWhoReply ) 
{
  if(packet->type[5] == TYPE_STRING)
    {
      int target,x;
      char *mud;
      listObject *who;

      mud = packet->data[2];
      if(!mud) mud = "";

      if( (target = fpbns(packet->data[5])) == -1)
	{
	  /* Bad player, do something here! */
	  return;
	}
     
      who = (listObject*)packet->data[6];
      sendf(target,"Players active on %s\n",mud);
      sendf(target,"%-15.15s %-10.10s %s\n","Name","Idle For","Misc");
      sendf(target,"----------------------------------------------------\n");
      for(x = 0; x < who->numElements; x++)
        {
	  listObject *person = (listObject*)who->data[x];
          char *name = person->data[0];
          int  idle = (int)person->data[1]; 
          char idleStr[256];
	  char *misc = person->data[2];
          int  hours = idle/3600;  /* How many hours idle... */
          int  mins  = (idle - (hours * 3600)) / 60;
          int  secs  = idle - (hours * 3600) - (mins * 60);
   
	  if(!name) continue;
          sprintf(idleStr,"%03d:%02d:%02d",hours,mins,secs);
	  sendf(target,"%-15.15s %-10.10s %s\n",
                name,idleStr,(misc)?misc:"");
        }
      sendf(target,"Total of %d players visible to intermud.\n",x);
    }
}	

/*******************************************************
 *
 * _imFingerReq
 *
 *  This function works somewhat like showplayer, but in response
 *  to a finger-req packet from another mud.  It will prepare and
 *  ssend out a finger-reply packet.
 *
 */

DECLARE_HANDLER(_imFingerReq ) 
{
  PERSONA   d;
  int       b;
  int       max_str;
  int       i, w, armor, is_mobile;
  char      *title, *name,ontime[256],level[32],titlestr[512],*p,*q;
  Boolean   in_file;

  if(packet->type[6] != TYPE_STRING)
    {
      sprintf(obuf,"{(\"error\",5,\"%s\",0,\"%s\",\"%s\",\"unk-user\","
                    "\"No target user name given!\"",
                    mudname,
                    packet->data[2],
                    packet->data[3]);
      buildImPacket(packet,obuf);
      __iSend(obuf);
      return;
    }

  if ( (b = fpbns(packet->data[6])) != -1 
       && pvis(b) == 0 
       && b < max_players
       && ptstflg(b, PFL_INTERMUD ) 
       && !ststflg(b, SFL_INTERVIS ) ) {
    in_file = False;
    is_mobile = b >= max_players;
    player2pers(&d,NULL,b);
    name = pname(b);
    title = is_mobile ? NULL : ptitle(b);
    d.p_home = is_mobile ? ploc_reset(b) : phome(b);
    max_str  = is_mobile ? pstr_reset(b) : maxstrength(b);
  } else if (getuaf(packet->data[6], &d)) {
    in_file = True;
    is_mobile = False;
    title = d.p_title;
    name  = d.p_name;
    max_str = pmaxstrength(d.p_level);
  } else {
    sprintf(obuf,"{(\"error\",5,\"%s\",0,\"%s\",\"%s\",\"unk-user\","
                "\"User %s@%s unknown.\",",
            mudname,
            packet->data[2],
            packet->data[3],
            packet->data[6],packet->data[4]);
    buildImPacket(packet,obuf);
    __iSend(obuf);
    return;
  }
 
  /*
   * Ok, let's build up the packet 
   */
  if(is_mobile)
    {
      sprintf(ontime,"Creation");
    }
  else if(!in_file)
    {
      sprintf(ontime,"%s",sec_to_hhmmss (time(0) - plogged_on(b)));
    }
  else
    {
      sprintf(ontime,"%s",time2ascii(d.p_last_on));
    }
  if (plev(b) >= LVL_GOD) sprintf(level,"God");
    else if (plev(b) >= LVL_DEMI) sprintf(level,"Demi-God");
    else if (plev(b) >= LVL_ARCHWIZARD) sprintf(level,"Arch Wizard");
    else if (plev(b) >= LVL_WIZARD) sprintf(level,"Wizard");
    else if (plev(b) > 0 )
     {
       if(in_file)
         sprintf(level,"%s",( (xtstbit(d.p_sflags,SFL_FEMALE))?FLevels:MLevels) [plev(b)]);
       else sprintf(level,"%s",( (ststflg(b,SFL_FEMALE))?FLevels:MLevels)[plev(b)]);
     }
    else sprintf(level,"Mobile");

  /* Escape quotes in the title */
  for( p = make_title(title,name), q = titlestr; *p ; )
    {
      if(*p == '"' || *p == '\\' ) *q++ = '\\';
      *q++ = *p++; 
    }
  *q = '\0';
  sprintf(obuf,"({\"finger-reply\",5,\"%s\",0,\"%s\",\"%s\"," /* packet header */
               "\"%s\"," 	/* User name */
               "\"%s\"," 	/* User title */
 	       "0," 	/* Real Name (nothing for now) */
 	       "0," 	/* Email address */
               "\"%s\","	/* laston/logged in */
               "%d,"	/* idle time */
               "\"%s\","	/* from where */
               "\"%s\","	/* what level */
               "0,})",
          mudname,packet->data[2],packet->data[3],
          name,
          titlestr,
          ontime,
          (in_file) ? -1 : (time(0) - players[b].last_command),
          "Nowhere for now",
          level);
  __iSend(obuf);
  return;
}

/*******************************************************
 *
 * _imFingerReply
 *
 *  This function handles a finger-reply packet when it comes
 *  in, breaking out the data, and giving the response back to 
 *  the originating user (providing they're still logged in)
 *
 */

DECLARE_HANDLER(_imFingerReply ) 
{
  if(packet->type[5] == TYPE_STRING)
    {
      int target;
      char *misc,*name,*title,*realName,*email,*loggedOn,*where, *level;
      int idle;
      char **types = packet->type;
      char **data = packet->data;

      if( (target = fpbns(data[5])) == -1)
	{
	  /* Bad player, do something here! */
          mudlog("INTERMUD:ARGH - Bad thing found!\n");
	  return;
	}
      if(types[6] == TYPE_STRING) name = data[6]; else name = "Unknown";
      if(types[7] == TYPE_STRING) title = data[7]; else title = "";
      if(types[8] == TYPE_STRING) realName = data[8]; else realName = "";
      if(types[9] == TYPE_STRING) email = data[9]; else email = "Confidential";
      if(types[10] == TYPE_STRING) loggedOn = data[10]; else loggedOn = "Unknown";
      if(types[11] == TYPE_INT) idle = (int)data[11]; else idle = 0;
      if(types[12] == TYPE_STRING) where = data[12]; else where = "Unknown";
      if(types[13] == TYPE_STRING) level = data[13]; else level = "";
      if(types[15] == TYPE_STRING) misc = data[14]; else misc = "";
      
      sendf(target,
            "Finger information on %s from %s\n"
            "%s (%s)\n"
            "Email: %s\n"
            "%s: %s\n"
            "Site: %s\n",
            name,data[2],
            title,realName,
            email,
            (idle==-1)?"Last on: ":"On Since: ",
            loggedOn,
            where);
    }
}
/*******************************************************
 *
 * _imLocateReq
 *
 *  This function handles a locate-req packet, and sends back 
 *  a locate-reply packet if the user being looked for is on
 *  the mud and visible to intermud.
 *
 */

DECLARE_HANDLER(_imLocateReq ) 
{
  if(packet->type[6] == TYPE_STRING)
    {
      int target;

      if( (target = fpbns(packet->data[6])) == -1)
	{
          return;
        }
      if( pvis( target ) > 0 )
        {
          /* User is invis, fail silently */
          return;
        }
      if( !ptstflg( target, PFL_INTERMUD ) )
        {
	  /* User can't access intermud, fail silently */
	  return;
        }
   
      if( ststflg( target, SFL_INTERVIS ) )
        {
 	  /* Target is ignoring intermud, fail silently */
	  return;
	}
      sprintf(obuf,"({\"locate-reply\",5,\"%s\",0,\"%s\",\"%s\","
                   "\"%s\",\"%s\",%d,0,})",
              mudname,
              packet->data[2],
              packet->data[3],
              mudname,
              pname(target),
              (time(0) - players[target].last_command));
      __iSend(obuf);
    }
}

/*******************************************************
 *
 * _imLocateReply
 *
 *  This function handles a locate-reply packet, informing
 *  the originator of the locate request of who was found/where.
 */

DECLARE_HANDLER(_imLocateReply ) 
{
  if(packet->type[5] == TYPE_STRING)
    {
      int target;

      if( (target = fpbns(packet->data[5])) == -1)
	{
          return;
        }
      sendf(target,"User %s has been found on mud %s\n",
            packet->data[7], packet->data[6]);
    }
}

DECLARE_HANDLER(_imChanlReply ) {}
/*******************************************************
 *
 * _imChanM
 *
 *  Handles a channel-m packet - currently only for the
 *  imud_gossip line.
 *
 */

DECLARE_HANDLER(_imChanM )
{
  if(packet->type[6] == TYPE_STRING)
    {
      /* We have a channelName */
      char *channelName = packet->data[6];

      if(EQ(channelName,"imud_gossip"))
	{
	  char *name = packet->data[7];
	  char *mud = packet->data[2];
	  char *message = packet->data[8];

	  if(!name) name = "Noone";
	  if(!mud) mud = "Nowhere";
	  if(!message) mud = "";
	  send_msg(DEST_ALL,MODE_COLOR|MODE_NSFLAG|MS(SFL_NO_IGOSSIP),
                   LVL_WIZARD,LVL_MAX,NOBODY,NOBODY,
		   "&+R<ig>%s@%s<ig> %s\n",
		   name,mud,message);
	}
    }
}

/*******************************************************
 *
 * _imChanE
 * 
 *  Handles a channel-e packet, currently only for the
 *  imud_gossip line.
 * 
 */

DECLARE_HANDLER(_imChanE )
{
  if(packet->type[6] == TYPE_STRING)
    {
      /* We have a channelName */
      char *channelName = packet->data[6];

      if(EQ(channelName,"imud_gossip"))
	{
	  char *name = packet->data[7];
	  char *mud = packet->data[2];
	  char *message = packet->data[8];
	  char outbuf[2048],*p,*q;
	  
	  if(!name) name = "Noone";
	  if(!mud) mud = "Nowhere";
	  if(!message) message = "";

	  for(p = message, q = outbuf; *p; *p++,*q++)
	    {
	      if(*p == '$')
		if(*(p+1) == 'N')
		  {
		    char *r;

		    for(r=name;*r;) *q++ = *r++;
		    *q++ = '@';
		    for(r=mud;*r;) *q++ = *r++;
		    p++;
		    q--;
		    continue;
		  }
	      *q = *p;
	    }
	  *q = '\0';
	  send_msg(DEST_ALL,MODE_COLOR|MODE_NSFLAG|MS(SFL_NO_IGOSSIP),
                   LVL_WIZARD,LVL_MAX,NOBODY,NOBODY,
		   "&+R<ig>%s\n",outbuf);
		   
	}
    }
}

DECLARE_HANDLER(_imChanT ) {}
DECLARE_HANDLER(_imChanAdd ) {}
DECLARE_HANDLER(_imChanRem ) {}
DECLARE_HANDLER(_imChanAdmin ) {}
DECLARE_HANDLER(_imChanWhoReq ) {}
DECLARE_HANDLER(_imChanWhoReply) {}
DECLARE_HANDLER(_imChanListen) {}
DECLARE_HANDLER(_imChanUserReq) {}
DECLARE_HANDLER(_imChanUserReply ) {}
DECLARE_HANDLER(_imNewsReadReq ) {}
DECLARE_HANDLER(_imNewsPost ) {}
DECLARE_HANDLER(_imNewsListReq ) {}
DECLARE_HANDLER(_imMail ) {}
DECLARE_HANDLER(_imFile ) {}
/*******************************************************
 *
 * _imError
 *
 *    Handles when an error message comes in from the i3 network.
 *
 */

DECLARE_HANDLER(_imError )
{
  if(packet->type[5] == TYPE_STRING)
    {
      /* This error is going to someone. */
      int target;

      if( (target = fpbns(packet->data[5])) == -1)
	{
          if(packet->type[7] == TYPE_STRING)
	      mudlog("INTERMUD: Error: %s\n",packet->data[7]);
	  return;
	}

      if(packet->type[7] == TYPE_STRING)
	sendf(target,"Error: %s\n",
	      packet->data[7]);
    }
  else if ( packet->type[6] == TYPE_STRING )
    {
      if ( EQ( packet->data[6], "bad-name" ) )
	{
	  extern fd_set sockets_fds;
	  FD_CLR( imfd, &sockets_fds );
	  mudlog("INTERMUD: Bad Mud Name, shutting down Intermud Connection.\n");
	  close(imfd);
	  imfd = -1;
	}
    }
}

DECLARE_HANDLER(_imStartup3 ) {}
DECLARE_HANDLER(_imUnknown ) {}

DECLARE_HANDLER(_imShutdown ) {}

