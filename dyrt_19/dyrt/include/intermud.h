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
 * $Log: intermud.h,v $
 * Revision 1.1  1996/02/15  01:36:15  kender
 * Initial revision
 *
 *
 *********************************************************************/

#ifndef __INTERMUD_H__
#define __INTERMUD_H__

extern int imfd;
extern int oobfd;
extern int udpfd;

extern char *rname;
extern char *raddr;
extern int rport;
extern int pport;
extern int tport;
extern int uport;
extern char *mudlib;
extern char *basemudlib;
extern char *driver;
extern char *mudType;
extern char *openStatus;
extern char *mudname;
extern char *adminEmail;

/*
 * files used by intermud
 */
extern char *mudListFile;
extern char *bannedUserFile;
extern char *bannedMudFile;

/*
 * intermud commands that the
 * rest of the world needs to know about.
 */
void imTell();
void imWho();
void imFinger();
void imLocate();
void imGossip();
void imPacket(int fd);
void imMudlist();
void imShutdown(int delay);
void imShutdownWrap();
void imBoot();
void imInvis();
void imNogos();

int boot_intermud(FILE *f, char *fname);

#endif __INTERMUD_H__
