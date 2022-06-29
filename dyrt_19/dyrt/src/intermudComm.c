/*******************************************************************
 * $Header: /home/kender/muddev/marxkove/include/RCS/board.h,v 1.2 1995/02/17 22
:01:22 kender Exp $
 *
 * $Revision: 1.2 $
 * $Date: 1996/10/09 03:28:04 $
 * $State: Exp $
 * $Author: testmud $
 * $Locker:  $
 *
 *-----------------------------------------------------------------------
 *                         Kove Development Code
 *
 * $Log: intermudComm.c,v $
 * Revision 1.2  1996/10/09 03:28:04  testmud
 * took out newline in a mudlog call
 *
 * Revision 1.1  1996/08/10 01:28:48  testmud
 * Initial revision
 *
 * Revision 1.2  1996/02/19  07:25:44  kender
 * Added documentation
 * Added code to support dynamic arrays for mudlist instead of hash table
 *
 * Revision 1.1  1996/02/15  01:25:28  kender
 * Initial revision
 *
 *
 *********************************************************************/

#include "kernel.h"
#include "stdinc.h"
#include "intermud.h"
#include "intermudP.h"

/*******************************************************
 *
 * imInvis
 *
 *  Toggles a user's visibility to the i3 network.
 */

void imInvis()
{
  if( !ptstflg( mynum, PFL_INTERMUD ) )
    {
      bprintf("Pardon?\n");
      return;
    }

  if( ststflg( mynum, SFL_INTERVIS ) )
    {
      bprintf( "You are now visible to intermud.\n" );
      sclrflg( mynum, SFL_INTERVIS );
      return;
    }
  else
    {
      bprintf( "You are no longer visible to intermud.\n" );
      ssetflg( mynum, SFL_INTERVIS );
      return;
    }
}

/*******************************************************
 *
 * imNogos
 *
 *  Toggles whether or not a user is listening to the imud_gossip line.
 *
 */
void imNogos()
{
  if( !ptstflg( mynum, PFL_INTERMUD ) )
    {
      bprintf("Pardon?\n");
      return;
    }

  if( plev( mynum ) < LVL_WIZARD )
    {
      bprintf( "You can't do that yet.\n" );
      return;
    }

  if( ststflg( mynum, SFL_NO_IGOSSIP ) )
    {
      bprintf( "You are once again listening to the Intermud Gossip Line[tm]\n" );
      sclrflg( mynum, SFL_NO_IGOSSIP );
      return;
    }
  else
    {
      bprintf( "You have shut off the noise of Intermud Gossip.\n" );
      ssetflg( mynum, SFL_NO_IGOSSIP );
      return;
    }
}

/*******************************************************
 *
 * imBoot()
 *
 *  restarts the i3 connection if shutdown.
 */
void imBoot()
{
  extern fd_set sockets_fds;

  if( plev(mynum) < LVL_GOD )
    {
      bprintf("Pardon?\n");
      return;
    }
  
  if( imfd != -1 )
    {
      bprintf("Intermud up and running.  Shut down intermud before booting.\n");
      return;
    }

  if( (imfd = __iConn(raddr, rport) ) == -1 )
    {
      bprintf("Error making connection to router.  Try again later.\n");
      return;
    }

  mudlog("INTERMUD: BOOT by %s\n",pname(mynum) );
  FD_SET( imfd, &sockets_fds );
}

/*******************************************************
 *
 * imShutdownWrap
 *
 *  Wrapper around the imShutdown function, for when a user
 *  manually shuts down the i3 connection.
 */

void imShutdownWrap()
{
  int delay;
  
  if( plev(mynum) < LVL_GOD )
    {
      bprintf("Pardon?\n");
      return;
    }
  
  if( imfd == -1 )
    {
      bprintf("Intermud already shutdown.\n");
      return;
    }

  if( EMPTY(item1) )
    delay = 0;
  else delay = atoi(item1);

  mudlog("INTERMUD: SHUTDOWN by %s",pname(mynum) );
  bprintf("Shutting down intermud.\n");
  imShutdown(delay);
  bprintf("Intermud shut down.\n");
  
}

/*******************************************************
 *
 * imShutdown
 *
 *  Shutsdown the intermud connection, the delay argument
 *  informs the i3 route of an estimage as to how long before
 *  the mud comes back online.
 */
void imShutdown(int delay)
{
  char buf[2048];
  extern fd_set sockets_fds;

  mudlog("INTERMUD: Shutting down the intermud connection.\n");
  sprintf(buf,"({\"shutdown\",5,\"%s\",0,\"%s\",0,1",
	  mudname,rname,delay);
  __iSend(buf);
  close(imfd);
  FD_CLR(imfd,&sockets_fds);
  imfd = -1;
}


/*******************************************************
 *
 * imMudlist
 *
 *  Display the list of active muds on the i3 network.
 */
void imMudlist()
{
  int x,flag = 1,c;

  if( !ptstflg( mynum, PFL_INTERMUD ) )
    {
      bprintf("Pardon?\n");
      return;
    }

  if(!mudlist)
    {
      bprintf("No muds to list yet!\n");
      return;
    }
  
  /* sort the array */
  arraySort( mudlist );

  /* Print it */
  bprintf("Known intermud accessable muds:\n"
	  "-------------------------------------------------\n");
  for(x = 0,c=0; x< arrayGetSize( mudlist ); x++)
    {
      mudInfo *mud = arrayGet( mudlist, x );
      if(!mud) continue; 
      if(!mud->isup) continue;
      bprintf("%-20.20s %6.6s %40.40s %d\n",
	      mud->name,
	      mud->type,
	      mud->addr,
	      mud->playerPort);
      c++;
    }
  bprintf("Total of %d muds listed.\n",c);
}

/*******************************************************
 *
 * imGossip
 *
 *   Sends a user's comments out over channel-m or channel-e
 *   (based on first char of user's text)
 */
void imGossip()
{
  char *text,*type,sendBuf[2048],unquoted[2048],*p,*q;
  int isemote = 0;
  
  if( !ptstflg( mynum, PFL_INTERMUD ) )
    {
      bprintf("Pardon?\n");
      return;
    }
  if( plev( mynum ) < LVL_WIZARD ) 
    {
      bprintf("You can't do that now.\n");
      return;
    }
  if( ststflg( mynum, SFL_NO_IGOSSIP ) )
    {
      bprintf("You don't want to listen to them, why should they have to listen to you?\n");
      return;
    }

  getreinput(wordbuf);
  if(EMPTY(wordbuf))
    {
      bprintf("Whatcha wanna say?\n");
      return;
    }

  if(wordbuf[0] == ':' &&
     wordbuf[1] != '^' &&
     wordbuf[1] !=')') /** it's a line emote... */
    {
      type = "channel-e";
      text = wordbuf+1;
      isemote=1;
    }
  else
    {
      type = "channel-m";
      text = wordbuf;
    }

  for ( p = text, q = unquoted; *p; )
    {
       if (*p == '$' || *p == '\\' || *p == '"' ) *q++ = '\\';
       *q++ = *p++;
    }
  *q = '\0';

  sprintf(sendBuf,"({\"%s\",5,\"%s\",\"%s\",0,0,\"imud_gossip\",\"%s\",\"%s%s\",})",
	  type,
	  mudname,
	  pname(mynum),
	  pname(mynum),
	  (isemote)?"$N ":"",
	  unquoted);
  __iSend(sendBuf);
}

/*******************************************************
 *
 * imWho
 *
 *  Sends out a who request over the i3 network to another mud.
 */
void imWho()
{
  char *mud = txt1,mesgBuf[2048];
  char *x; 
  mudInfo *mi;

  if( !ptstflg( mynum, PFL_INTERMUD ) )
    {
      bprintf("Pardon?\n");
      return;
    }

  for( x = mud; *x; x++ ) *x = tolower(*x);

  if( ! mudlist )
    {
      bprintf("Intermud not initialized.\n");
      return;
    }
  if( ! ( mi = (mudInfo*)arrayFind(mudlist,(void*)mud) ) )
    {
      bprintf("Mud %s is unknown!\n",mud);
      return;
    }

  sprintf(mesgBuf,"({\"who-req\",5,\"%s\",\"%s\",\"%s\",0,})",
	  mudname,
	  pname(mynum),
	  mi->name);
  __iSend(mesgBuf);
}

/*******************************************************
 *
 * imLocate
 *
 *  Sends out a locate request (i.e. find user xxx on all
 *  muds they are on) over the i3 network.
 */
void imLocate()
{
  char mesgBuf[2048];

  if( !ptstflg( mynum, PFL_INTERMUD ) )
    {
      bprintf("Pardon?\n");
      return;
    }

  if(EMPTY(item1))
    {
      bprintf("Locate WHO?\n");
      return;
    }
  if( ! mudlist )
    {
      bprintf("Intermud not initialized.\n");
      return;
    }

  sprintf(mesgBuf,"({\"locate-req\",5,\"%s\",\"%s\",0,0,\"%s\",})",
          mudname,pname(mynum),item1);
  __iSend(mesgBuf);
  bprintf("Locate request sent.\nMay take some time for responses to come in.\n");
}

/*******************************************************
 *
 * imFinger
 *
 *  Finger a user at another mud
 */
void imFinger()
{
  char *user,*mud,mesgBuf[2048],*x;
  mudInfo *mi;

  if( !ptstflg( mynum, PFL_INTERMUD ) )
    {
      bprintf("Pardon?\n");
      return;
    }

  for( x = txt1; *x; x++ ) *x = tolower(*x);
  user = strtok(txt1,"@");
  mud = strtok(NULL,"@");

  if(!user || EMPTY(user))
    {
      bprintf("Finger who?\n");
      return;
    }
  if(!mud || EMPTY(mud))
    {
      showplayer();
      return;
    }

  if( ! mudlist )
    {
      bprintf("Intermud not initialized.\n");
      return;
    }
  if( ! ( mi = (mudInfo*)arrayFind(mudlist,(void*)mud) ) )
    {
      bprintf("Mud %s is unknown!\n",mud);
      return;
    }
 
   sprintf(mesgBuf,"({\"finger-req\",5,\"%s\",\"%s\",\"%s\",0,\"%s\",})",
	   mudname,
	   pname(mynum),
	   mi->name,
	   user);
   __iSend(mesgBuf);
}

/*******************************************************
 *
 * imTell
 * 
 *   Send a tell or emoteto packet to a user at another mud.
 */
void imTell()
{
  char *user,*mud,mesgBuf[2048],tempBuf[2048],*p,*q;
  mudInfo *mi;
  
  if( !ptstflg( mynum, PFL_INTERMUD ) )
    {
      bprintf("Pardon?\n");
      return;
    }

  user = strtok(txt1,"@");  /* The name */
  mud = strtok(NULL,"@");   /* The rest of the line */

  if(!user || EMPTY(user))
    {
      bprintf("Tell who?\n");
      return;
    }
  if(!mud || EMPTY(mud ) )
    {
      bprintf("What mud?\n");
      return;
    }

  /*
   * let's find the proper name for the mud
   */
  if( ! mudlist )
    {
      bprintf("Intermud not initialized.\n");
      return;
    }

  /* Try to pull out a valid mud name, step forward one space at a time */
  mi = NULL;
  for( p = mud; *p; p++ )
    {
      if( *p == ' ' ) 
        {
          *p = '\0';
          if( ( mi = (mudInfo*)arrayFind(mudlist,(void*)mud) ) ) break;
          *p = ' ';
        }
    }
  if ( !mi ) 
    {
      bprintf("I'm sorry, that mud is not known.\n");
      return;
    }

  if(EMPTY(++p))
    {
      bprintf("What do you want to tell %s@%s?\n",
	      user,mud);
      return;
    }

  for(q = tempBuf; *p; q++, p++)
    {
      if(*p == '$' || *p == '"' || *p == '\\' ) *q++ = '\\';
      *q = *p;
    }
  *q = '\0';

  if( ststflg( mynum, SFL_INTERVIS ) )
    {
      bprintf(" Note: you are invis to the intermud - responses may not come back.\n" );
    }

  if(tempBuf[0] == ':' &&
     tempBuf[1] != '^' &&
     tempBuf[1] !=')') /** it's a line emote... */
    {
      sprintf(mesgBuf,"({\"emoteto\",5,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"$N %s\",})",
	      mudname,
	      pname(mynum),
	      mi->name,
	      user,
	      pname(mynum),
	      &(tempBuf[1]));
      if( ststflg( mynum, SFL_SAYBACK ) )
        bprintf( "You emoteto %s@%s '%s %s'\n",user,mi->name,pname(mynum),&(tempBuf[1]) );
      else bprintf("Ok.\n");
    }
  else
    {
      sprintf(mesgBuf,"({\"tell\",5,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",})",
	      mudname,
	      pname(mynum),
	      mi->name,
	      user,
	      pname(mynum),
	      tempBuf);
      if( ststflg( mynum, SFL_SAYBACK ) )
        bprintf( "You tell %s@%s '%s'\n",user,mi->name,tempBuf );
      else bprintf("Ok.\n");
    }
  mesgBuf[2047] = '\0';
  __iSend(mesgBuf);
}
