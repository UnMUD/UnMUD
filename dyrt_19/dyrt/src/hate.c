/*
 * File: hate.c
 *
 *       Deals with the code for mobiles hating players.  Any mobile who 
 *       finds the one they hate will attack them immediately if possible.
 *       James Willie(Ithor) 1994
 *       Changed to Work with pDirt code by Marty in 1996
 */

#define HATE_C
#include "kernel.h"
#include "config.h"
#include <string.h>
#include "bprintf.h"
#include "extern.h"
#include "stdinc.h"

/* clear player i's hate */
void clear_hate(int i)
{
  if ((i < 0) || (i >= numchars))
    {
      mudlog("Error: Tried to clear hate for %d", i);
      return;
    }
    ublock[i].hate[0] = '\0'; 
    return;
}

/* clear all hate fields */
void clear_all_hate(void)
{
  int i;

  for (i = 0; i < numchars; i++)
    {
      ublock[i].hate[0] = '\0';
    }
  return;
}

/* set hate for player p, to player i */
void set_hate(int p, int i)
{
  if ((i < 0) || (i >= numchars) || (p < 0) || (p >= numchars))
    {
      mudlog("Error: Tried to set hate of %d to %d", p, i);
      return;
    }
  else 
     strcpy(ublock[p].hate, pname(i));
  return;
}

/* set player p's hate to str */
void set_hate_str(int p, char *str)
{
  if ((p < 0) || (p >= numchars))
    {
        mudlog("Error: Tried to set player %d's hate", p);
    }
  strncpy(ublock[p].hate, str, MNAME_LEN);
  return;
}

/* does p hate i? */
Boolean hates_you(int p, int i)
{
  if ((i < 0) || (i >= numchars) || (p < 0) || (p >= numchars))
    {
      mudlog("Error: Tried to test hate of %d to %d", p, i);
      return False;
    }
  if (strcasecmp(ublock[p].hate, pname(i)) == 0)
    return True;
  return False;
}

/* return the number of players p hates or -1 if none */
int hates_who(int p)
{
  int j;
  
  if ((p < 0) || (p >= numchars))
    {
      mudlog("Error: Tried to test who %d hates", p);
      return -1;
    }
  for (j = 0; j < numchars; j++)
    {
      if (hates_you(p, j))
        return j;
    }
  return -1;
}

/* send_hate_message(m, p) sends a message suggesting just how much m hates p
   to the location, and to p */
void send_hate_msg(int m, int p)
{
#define HOW_MANY_MESS	4

  int i;

  if ((p < 0) || (p >= numchars) || (m < 0) || (m >= numchars))
    {
      mudlog("Error: send_hate_msg(%d, %d).", m, p);
      return;
    }
  i = randperc() % HOW_MANY_MESS;

  /* Used a switch, rather than array, as allows more */
  /* detail in the messages you send                  */
  switch (i)
    {
      case 0:
        sendf(p, "%s gives you an icy glare before charging into battle.\n",
  	      pname(m));
        send_msg(ploc(p), 0, max(pvis(m), pvis(p)), LVL_MAX,
                 p, NOBODY, "%s glares at %s before charging into battle.\n",
	         pname(m), pname(p));
        break;
      case 1:
        send_msg(ploc(p), MODE_NODEAF, 
  	         max(pvis(m), pvis(p)), LVL_MAX,
                 p, NOBODY, 
	         "%s points at %s and shouts 'You murdering swine!'\n",
	         pname(m), pname(p));
        sendf(p, "%s calls you a murdering swine and rushes to attack you!\n", 
              pname(m));
        break;
      case 2:
        send_msg(ploc(p),MODE_NODEAF,max(pvis(m), pvis(p)), LVL_MAX,p, NOBODY,
                 "%s shouts at %s 'Get ready to finish what you've started!\n",
                 pname(m),pname(p));
        sendf(p,"%s shouts at you 'Get ready to finish what you've started!\n",
              pname(m));
        break;
      case 3:
        send_msg(ploc(p),0,max(pvis(m),pvis(p)),LVL_MAX,p,NOBODY,
                "%s eyes fill with anger as %s starts to attack %s!\n",pname(m),
                he_or_she(m),pname(p));
        sendf(p,"%s eyes fill with anger as %s starts to attack you!\n",pname(m),
              he_or_she(m));
        break;
      default:
        break;
    }
  return;
}

/* should m start hating p, if so set it up */
void should_hate(int m, int p, int d)
{
  if ((m < max_players) || (m >= numchars) || (p < 0) || (p >= numchars))
  {
      return;
  }
  if (d <= 3)  /* Don't hate unless they really hurt us */
    return;
  if (plev(p)  <= LVL_THREE)  /* Give newbies a break :) */
    return;
  if (no_hate(m))
    {
      if ((randperc() % 70) < d)  /* Lets make it a little random */
	set_hate(m, p);
    }
  else
    {
      int w = hates_who(m);
      if (w < 0)  /* who we hate is not in the game */
	{
	  if ((randperc() % 20) < d)
	    set_hate(m, p);
	}
      else if (ploc(w) != ploc(m))  /* Not in same room */
	{
	  if (d >= 10)  /* If they do enough damage hate them anyway */
	    set_hate(m, p);
	}
    }
  return;
}

/* hatecom() allows people to see and modify mobile hate strings */
void hatecom()
{
  if (plev(mynum) <= LVL_WIZARD)
  {
      if ((pl1 == -1) && (pl2 == -1))
	{
	  bprintf("Awwww.  Hate is such a negative emotion...  :(\n");
	  send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
		   "Watch out everyone, %s is feeling really grumpy!!!\n",
		   pname(mynum));
	  return;
	}
      else if (pl1 == -1)
	{
	  pl1 = pl2;
	}
      if (mynum == pl1)
	{
	  bprintf("Awww don't be like that. Life's to short to waste it "
		  "hating yourself!\n");
          return;
	}
      bprintf("Awww, I'm sure they love you too.\n");
      if (ploc(pl1) == ploc(mynum))
	{
	  sendf(pl1, "I'd watch out if I was you, %s seems a little "
		"annoyed with you.\n",
		(((pvis(mynum) > 0) && (pvis(mynum) > plev(pl1))) ?
					"someone" : pname(mynum)));
	}
      return;
  }
  if ((pl1 == -1) && (pl2 == -1))
    {
      if (EMPTY(item1))
        {
	  bprintf("Awwww.  Hate is such a negative emotion... :(\n");
	  send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
		   "Watch out everyone, %s is feeling really grumpy!!!\n",
		   pname(mynum));
        }
      else 
        {
	  bprintf("Sorry, I can't find a %s in the game.\n", item1);
        }
      return;
    }
  if (pl1 == -1)
    {
      pl1 = pl2;
      pl2 = -1;
    }
  if (pl1 == -1) 
  {
      bprintf("\nusage: hate mobile_name\n"
	    "         This will tell you who, if anyone this mobile hates.\n");
      return;
  }
  if (pl1 < max_players)
    {  
      if (mynum == pl1)
        {
	  bprintf("Awww don't be like that. Life's to short to waste it "
		  "hating yourself!\n");
          return;
        }
      bprintf("Awww, I'm sure they love you too.\n");
      if (ploc(pl1) == ploc(mynum))
        {
	  sendf(pl1, "I'd watch out if I was you, %s seems a little "
		"annoyed with you.\n",
		(((pvis(mynum) > 0) && (pvis(mynum) > plev(pl1))) ?
					"someone" : pname(mynum)));
        }
      return;
  }
  if ((pl2 != -1) && (ptstflg(mynum, PFL_CH_MDATA)))
    {
      if (pl2 >= max_players)
        bprintf("Warning, setting hate to another mobile.\n");
      bprintf("%s now hates %s.\n", pname(pl1), pname(pl2));
      mudlog("HATE: %s set %s's hate to %s", 
             pname(mynum), pname(pl1), pname(pl2));
      set_hate(pl1, pl2);
    }
  else if (!EMPTY(item2) && ptstflg(mynum, PFL_CH_MDATA)
	   &&  (strcasecmp("clear", item2) == 0))
    {
      mudlog("HATE: %s cleared %s's hate", pname(mynum), pname(pl1));
      bprintf("Cleared %s's hate\n", pname(pl1));
      clear_hate(pl1);
    }
  else
    {
      if (ublock[pl1].hate[0] == '\0')
	bprintf("%s does not hate anyone.\n", pname(pl1));
      else if (hates_who(pl1) == mynum)
	bprintf("%s hates YOU!!\n", pname(pl1));
      else
	bprintf("%s hates %s.\n", pname(pl1), ublock[pl1].hate);
    }
  return;
}
