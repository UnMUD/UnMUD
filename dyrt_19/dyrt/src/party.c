/*****************************************************************************
 ** PARTY GROUPING CODE
 ** by Marty - 1996
 ** This code is free to everyone who wants to use it, if they are so kind to
 ** give me some credit for it.
 ** You may change this file if you like, if you see a way on how to improve
 ** this code please mail it to me, so I can bring out a better version of
 ** this code.
 ** 24 May: Added fixes for bugs found by Manic & me
 ****************************************************************************/
#define PARTY_C

/****************************************************************************
 ** IMPORT
 ****************************************************************************/
#include "kernel.h"
#include "bprintf.h"
#include "sendsys.h"
#include "parse.h"
#include "log.h"
#include "sflags.h"
#include "communicate.h"
#include <stdlib.h>
#include "colors.h"

/****************************************************************************
 ** PROTOTYPES
 ****************************************************************************/
void pstatus(void); /* Party Status, strength score etc etc */
void pcreate(void); /* Create a new party object            */
void pleave(void);  /* Leave a party                        */
void padd(void);    /* Add a member to the party            */
void pfollow(void);
void pshare(void);
void pleader(void);
void partyname(void);
char *whopartytst();
void psay(void);
void partyannounce(char *text);

static char partybuff[MAX_COM_LEN];

/*****************************************************************************
 ** PCREATE
 ** Creates a new party and make the creator the leader.
 ****************************************************************************/
void pcreate(void)
{
   if (in_group(mynum))
   {
      bprintf("Leave the other group first.\n");
      return;
   }
   if (plev(mynum) >= 17)
   {
      bprintf("You can no longer take part in parties.\n");
      return;
   }
   else
   {
      cur_player->party = NEW(PARTY, sizeof(PARTY)); /* Allocate memory */
      if (cur_player->party == NULL)
      {
         mudlog("ERROR: couldn't allocate memory for a Party Object");
         bprintf("An internal error occured, party's are not functioning atm. Sorry ;)");
         return;
      }
      cur_player->party->leader = mynum;       /* Set leader            */
      cur_player->share = plev(mynum);         /* Set shares for player */
      cur_player->party->shares = plev(mynum); /* Set total shares      */
      cur_player->party->name[0] = '\0';       /* Default name is no name */
      cur_player->party->xp = 0;               /* Total xp given        */
      bprintf("New party created.  Use the pname command to name your party.\n");
   }
}

#define DOUBLELINE "============================================================================="
/***************************************************************************
 ** PSTATUS
 ** Show the party status: party name, party shares, party xp,
 **       name of player,  strength,  shares for player, status
 ***************************************************************************/
void pstatus(void)
{
   int plx, leader;

   if (!in_group(mynum)) /* We always need this to prevent a */
   {
      erreval(); /* crash caused by assigning values */
      return;    /* to a NULL pointer                */
   }
   else
   {
      leader = p_group_leader(mynum);
      bprintf("&+B%s&*\n", DOUBLELINE);
      bprintf("&+cParty name     : &+W%s\n",
              (p_group_name(mynum)[0] != '\0') ? p_group_name(mynum) : "None");
      bprintf("&+cParty shares   : &+W%d\n", p_group_shares(mynum));
      bprintf("&+cTotal xp gained: &+W%d\n", p_group_xp(mynum));

      bprintf("&+B==&+W[Name]&+B===========&+W[Strength]&+B==&+W[Shares]&+B==&+W[Score]&+B==&+W[Status]&+B====================&*\n");
      for (plx = 0; plx < max_players; plx++)
      {
         if (is_in_game(plx) && in_group(plx))
         {
            if (p_group(plx) == p_group(mynum))
            {
               bprintf("%-16s   &+g[&+W%.3d&+g/&+m%.3d&+g]     &+W%.2d       &+W%-6d  &+c%s\n",
                       pname(plx), pstr(plx), pmaxstrength(plev(plx)), p_share(plx), pscore(plx),
                       (plx == leader) ? "(Leader)" : ((players[plx].i_follow == leader) ? "(following)" : ""));
            }
         }
      }
      bprintf("&+B%s\n", DOUBLELINE);
   }
}

/* Small functions used to remove and add players */
void leaveplayer(int plx)
{
   p_group_shares(plx) -= p_share(plx);
   p_share(plx) = 0;
   p_group(plx) = NULL;
}

void addplayer(int plx, PARTY *p)
{
   p_group(plx) = p;
   p_share(plx) = plev(plx);
   p_group_shares(plx) += plev(plx);
}

/*****************************************************************************
 ** PLEAVE
 ** Let a player leave the group
 *****************************************************************************/
void pleave(void)
{
   int plx;

   if (!in_group(mynum))
   {
      erreval();
      return;
   }
   else
   {
      if (p_group_leader(mynum) == mynum) /* Player is the leader so */
      {                                   /* Drop the whole group.   */

         sprintf(partybuff, "Party is disbanded");
         partyannounce(partybuff);
         for (plx = 0; plx < max_players; plx++)
         {
            if (is_in_game(plx) && (p_group(plx) == p_group(mynum)) &&
                plx != mynum)
            {
               leaveplayer(plx);
            }
         } /* Now only the leader is in the group */
         p_group_shares(mynum) -= p_share(mynum);
         p_share(mynum) = 0;
         free(p_group(mynum)); /* Loose allocated mem */
         p_group(mynum) = NULL;
      }
      else
      { /* Lose one player, not the party, We dont have to check if the
           Player is the last player, because the last player is always
           the leader, which is handled in the bit of code above */
         sprintf(partybuff, "%s has left the party", pname(mynum));
         partyannounce(partybuff);
         leaveplayer(mynum);
      }
   }
}

/****************************************************************************
 ** PADD
 ** Add a player to the party..
 ****************************************************************************/
void padd(void)
{
   int plx;

   if (!in_group(mynum)) /* Standard check */
   {
      erreval();
      return;
   }
   if (brkword() == -1) /* Get the name */
   {
      bprintf("Add who?\n");
      return;
   }
   if (p_group_leader(mynum) != mynum) /* Check if player is leader */
   {
      bprintf("Only the leader can add players.\n");
      return;
   }
   plx = pl1;
   if (plx >= max_players) /* Check if new player is mobile */
   {
      bprintf("You can't add mobiles to the group\n");
      return;
   }
   if (plev(plx) >= 17)
   {
      bprintf("You cannot add immortals to the group.\n");
      return;
   }
   if (ploc(plx) != ploc(mynum)) /* Check if the player is in the same room */
   {
      bprintf("%s isn't here.\n", pname(plx));
      return;
   }
   if (players[plx].i_follow != mynum) /* Player should follow the leader */
   {
      bprintf("%s is not following you.\n", pname(plx));
      return;
   }
   if (!in_group(plx) && is_in_game(plx))
   {
      addplayer(plx, p_group(mynum));
      sprintf(partybuff, "%s has joined the party", pname(plx));
      partyannounce(partybuff);
   }
   else
   {
      bprintf("Sorry I can't add %s\n", pname(plx));
   }
}

/***************************************************************************
 ** PARTNAME
 ** Change the name of the party, shows up in status and who list (later)
 ***************************************************************************/
void partyname(void)
{
   int len;

   if (!in_group(mynum))
   {
      erreval();
      return;
   }
   if (p_group_leader(mynum) != mynum) /* Check if player is leader */
   {
      bprintf("Only the leader can pick a party name.\n");
      return;
   }
   getreinput(wordbuf);
   len = strlen(wordbuf);
   if (len <= 0 || len > PNAME_LEN)
   {
      bprintf("Set the name to what?\n");
      return;
   }
   else
   {
      strcpy(cur_player->party->name, item1);
      sprintf(partybuff, "New name is: '%s'", item1);
      partyannounce(partybuff);
   }
}

/*************************************************************************
 ** PFOLLOW
 ** Follow the leader unless you are the leader ;)
 *************************************************************************/
void pfollow(void)
{
   if (!in_group(mynum))
   {
      erreval();
      return;
   }
   if (p_group_leader(mynum) == mynum)
   {
      bprintf("You are the leader, you lead the way.\n");
      return;
   }
   else if (cur_player->i_follow != p_group_leader(mynum))
   {
      bprintf("You now follow your leader %s\n", pname(p_group_leader(mynum)));
      cur_player->i_follow = p_group_leader(mynum);
      sendf(p_group_leader(mynum), "%s starts following you.\n", pname(mynum));
      return;
   }
   else
   {
      bprintf("You stop following %s\n", pname(cur_player->i_follow));
      sendf(p_group_leader(mynum), "%s stops following you.\n", pname(mynum));
      cur_player->i_follow = -1;
      return;
   }
}

/****************************************************************************
 ** PSHARE
 ** Set the amount of shares for the player. The amount of shares has a
 ** boundry relative to the level of the player. Of course this can be
 ** changed to suit your personal taste
 ****************************************************************************/
void pshare(void)
{
   int plx = -1, newsh;
   int dif;

   if (!in_group(mynum))
   {
      erreval();
      return;
   }
   if (p_group_leader(mynum) != mynum)
   {
      bprintf("Only the leader can change the shares.\n");
      return;
   }
   if (brkword() == -1)
   {
      bprintf("Change shares on who?\n");
      return;
   }
   plx = pl1;
   if (!in_group(plx))
   {
      bprintf("%s isn't in your group\n", pname(plx));
      return;
   }
   else if (p_group(plx) != p_group(mynum))
   {
      bprintf("%s isn't in your group\n", item1);
      return;
   }
   if (brkword() == -1)
   {
      bprintf("Set shares to what?\n");
      return;
   }
   newsh = atoi(item2);
   if (newsh <= 0 || (newsh < (plev(plx) - 5) || newsh > (plev(plx) + 10)))
   {
      bprintf("%s shares must be between %d and %d\n", pname(plx),
              (plev(plx) - 5) < 1 ? 1 : (plev(plx) - 5), plev(plx) + 10);
      return;
   }
   dif = newsh - p_share(plx);
   p_share(plx) += dif;
   p_group_shares(plx) += dif;

   sprintf(partybuff, "%s now owns %d shares", pname(plx), newsh);
   partyannounce(partybuff);
}

/****************************************************************************
 ** PLEADER
 ** Change the leader of the party
 ****************************************************************************/
void pleader()
{
   int plx, oldleader;

   if (!in_group(mynum))
   {
      erreval();
      return;
   }
   if (p_group_leader(mynum) != mynum)
   {
      bprintf("Only the leader can change the leader.\n");
      return;
   }
   if (brkword() == -1)
   {
      bprintf("Make who leader?\n");
      return;
   }
   if (pl1 < 0 || pl1 > max_players)
   {
      bprintf("I don't know anyone with that name.\n");
      return;
   }
   if (!in_group(pl1))
   {
      bprintf("%s is not in a group, add %s first.\n", pname(pl1), him_or_her(pl1));
      return;
   }
   if (p_group(pl1) != p_group(mynum))
   {
      bprintf("%s is not in your group\n", pname(pl1));
      return;
   }

   oldleader = mynum;
   p_group_leader(mynum) = pl1;
   bprintf("%s is now the new leader.\n", pname(pl1));
   for (plx = 0; plx < max_players; plx++)
   {
      if (in_group(plx))
      {
         if (p_group(plx) == p_group(mynum) && players[plx].i_follow == mynum &&
             plx != pl1)
         {
            players[plx].i_follow = pl1;
         }
      }
   }
   /* clear possible follow of new leader */
   players[pl1].i_follow = -1;
   sprintf(partybuff, "New leader is %s", pname(pl1));
   partyannounce(partybuff);
}

char *whopartytst()
{
   if (!in_group(mynum))
   {
      return "";
   }
   else
   {
      if (p_group_leader(mynum) == mynum)
         sprintf(partybuff, " (Leader of %s)", (p_group_name(mynum)[0] == '\0') ? "a party" : p_group_name(mynum));
      else
         sprintf(partybuff, " (Member of %s)", (p_group_name(mynum)[0] == '\0') ? "a party" : p_group_name(mynum));
      return partybuff;
   }
   return partybuff;
}

/*****************************************************************************
 ** PSAY
 ** Communication between party members
 ******************************************************************************/

/* Test function
 * See if the player is on and in a group, then check if the person is in the
 * same group as the sender, and not the sender itself. Then send the message,
 * Else send nothing at all. (standard send_g_msg() testfunction)
 */
char *psay_test(int player, int sender, char *text)
{
   static char buff[MAX_COM_LEN];
   char *buffer = NULL;

   if (players < max_players)
      if (in_group(player) && is_in_game(player))
      {
         if (p_group(sender) == p_group(player) && sender != player)
         {
            buffer = build_voice_text(pname(sender), "psays", text, "'",
                                      COLOR_NORMAL_CYAN, COLOR_HILIGHT_WHITE);
            sprintf(buff, "%s", buffer);
            free(buffer);
            return buff;
         }
      }
   return NULL;
}

void psay(void)
{
   char blob[MAX_COM_LEN];

   if (!in_group(mynum))
   {
      erreval();
      return;
   }

   getreinput(blob);
   if (EMPTY(blob))
      bprintf("What do you want to say\n");
   else
   {
      send_g_msg(DEST_ALL, psay_test, mynum, blob);
      if (ststflg(mynum, SFL_SAYBACK))
         bprintf("&+cYou psay &+W'%s'&*\n", blob);
      else
         bprintf("Ok\n");
   }
}

/* used by partyannounce() */
char *pannounce_test(int player, int sender, char *text)
{
   static char buff[MAX_COM_LEN];

   if (player < max_players)
      if (in_group(player))
      {
         if (p_group(sender) == p_group(player))
         {
            sprintf(buff, "&+cParty Info: &+W%s&*\n", text);
            return buff;
         }
      }
   return NULL;
}

/**************************************************************************
 ** This function is used for information that concerns all party members
 ** such as a change of shares, or a change of name etc...
 **************************************************************************/
void partyannounce(char *text)
{
   if (text == NULL)
      return;
   else
      send_g_msg(DEST_ALL, pannounce_test, mynum, text);
}
