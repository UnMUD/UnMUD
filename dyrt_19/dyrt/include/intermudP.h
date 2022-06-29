/*******************************************************************
 * $Header: /home/kender/muddev/marxkove/include/RCS/board.h,v 1.2 1995/02/17 22
:01:22 kender Exp $
 *
 * $Revision: 1.1 $
 * $Date: 1996/02/15 01:36:15 $
 * $State: Exp $
 * $Author: kender $
 * $Locker: kender $
 *
 *-----------------------------------------------------------------------
 *                         Kove Development Code
 *
 * $Log: intermudP.h,v $
 * Revision 1.1  1996/02/15  01:36:15  kender
 * Initial revision
 *
 *
 *********************************************************************/

#ifndef __INTERMUDP_H__
#define __INTERMUDP_H__

#include "intermud.h"
#include "darray.h"

typedef enum __serviceTypes {
 SERV_TELL, SERV_WHO, SERV_FINGER, SERV_LOCATE, SERV_CHANNEL,
 SERV_NEWS, SERV_FILE, SERV_SMTP, SERV_FTP, SERV_NNTP, SERV_HTTP,
 SERV_RCP} serviceTypes;

extern char *services[];

typedef enum __messageTypes {
  MESG_TELL, MESG_WHO_REQ, MESG_WHO_REPLY, MESG_FINGER_REQ,
  MESG_FINGER_REPLY, MESG_LOCATE_REQ, MESG_LOCATE_REPLY,
  MESG_CHANLIST_REPLY, MESG_CHANNEL_M, MESG_CHANNEL_E,
  MESG_CHANNEL_T, MESG_CHANNEL_ADD, MESG_CHANNEL_REMOVE,
  MESG_CHANNEL_ADMIN, MESG_CHAN_WHO_REQ, MESG_CHAN_WHO_REPLY,
  MESG_CHANNEL_LISTEN, MESG_CHAN_USER_REQ, MESG_CHAN_USER_REPLY,
  MESG_NEWS_READ_REQ, MESG_NEWS_POST, MESG_NEWSGRP_LIST_REQ,
  MESG_MAIL, MESG_FILE, MESG_ERROR, MESG_STARTUP_REQ_3,
  MESG_STARTUP_REPLY, MESG_SHUTDOWN, MESG_MUDLIST,
  MESG_EMOTETO,
  MESG_UNKNOWN } messageTypes;

typedef enum __elemTypes {
  TYPE_STRING, TYPE_LIST, TYPE_MAPPING, TYPE_INT, TYPE_OTHER
} elemTypes;

typedef struct __listObject {
  char		*origString;  
  int		numElements;  
  elemTypes	*type;
  char 		**data;
} listObject;

typedef struct __mappingObject {
  char		*origString;
  int		numElements;
  elemTypes	*keyType,*valueType;
  char		**keys,**values;
} mappingObject;

listObject *parseList(char *list);
void destroyList(listObject *);
mappingObject *parseMapping(char *mapping);
void destroyMapping(mappingObject *);

typedef void (*imHandler)(char *type,
			  int  ttl,
			  char *origMud,
			  char *origUser,
			  char *targetMud,
			  char *targetUser,
			  listObject *packet);

#define DECLARE_HANDLER(name) \
void (name)(char *type, int ttyl, \
	    char *origMud, char *origUser, \
	    char *targetMud, char *targetUser, \
	    listObject *packet)

typedef struct __imMapping {
  messageTypes		type;
  char			*name;
  imHandler		handler;
} imMapping;

extern char *immessages[];
extern pdarray mudlist;
extern int numMuds;

typedef struct __mudInfo {
  char		*name;
  char		*type;
  char		*addr;
  int		isup;
  int		playerPort;
  int		tcpPort;
  int		udpPort;
} mudInfo;
    
#define INTERMUD_DIR	"INTERMUD/"
#define MUDLIST_ID INTERMUD_DIR "mudlist.id"
#define CHANLIST_ID INTERMUD_DIR "chanlist.id"
#define INTERMUD_PASS INTERMUD_DIR "impass.id"
#define MUDLIST_FILE INTERMUD_DIR "mudlist"
#define BANNED_MUD_FILE INTERMUD_DIR "banned.mud"


#define NCOPY(a) ( (a) ? COPY(a) : NULL )

/*
 * Normal message handler declarations
 */

DECLARE_HANDLER(_imTell );
DECLARE_HANDLER(_imEmote );
DECLARE_HANDLER(_imWhoReq );
DECLARE_HANDLER(_imWhoReply );
DECLARE_HANDLER(_imFingerReq );
DECLARE_HANDLER(_imFingerReply );
DECLARE_HANDLER(_imLocateReq );
DECLARE_HANDLER(_imLocateReply );
DECLARE_HANDLER(_imChanlReply );
DECLARE_HANDLER(_imChanM );
DECLARE_HANDLER(_imChanE );
DECLARE_HANDLER(_imChanT );
DECLARE_HANDLER(_imChanAdd );
DECLARE_HANDLER(_imChanRem );
DECLARE_HANDLER(_imChanAdmin );
DECLARE_HANDLER(_imChanWhoReq );
DECLARE_HANDLER(_imChanWhoReply);
DECLARE_HANDLER(_imChanListen);
DECLARE_HANDLER(_imChanUserReq);
DECLARE_HANDLER(_imChanUserReply );
DECLARE_HANDLER(_imNewsReadReq );
DECLARE_HANDLER(_imNewsPost );
DECLARE_HANDLER(_imNewsListReq );
DECLARE_HANDLER(_imMail );
DECLARE_HANDLER(_imFile );
DECLARE_HANDLER(_imError );
DECLARE_HANDLER(_imStartup3 );
DECLARE_HANDLER(_imStartupReply );
DECLARE_HANDLER(_imShutdown );
DECLARE_HANDLER(_imMudList );
DECLARE_HANDLER(_imUnknown );

char *__imAddStr(char *orig, char *str);
char *__imAddInt(char *orig, int num);
char *__imAddlStr(char *orig, char *str);
char *__imAddlInt(char *orig, int num);
char *__imStartList(char *orig);
char *__imStopList(char *orig,int comma);
char *__imStartMap(char *orig);
char *__imStopMap(char *orig);
int   __iSend(char *str);
int  buildImPacket(listObject *list,char *str);

#endif __INTERMUDP_H__
