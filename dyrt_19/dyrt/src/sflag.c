#include "verbs.h"
#include "kernel.h"
#include "stdinc.h"
#include "sflag.h"
#include "colors.h"
#include "communicate.h"

static void _wiz (int level, char *text);

void deafcom (void)
{
  int a;

   if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("You can't cast spells in this room\n");
    return;
   }
 
  if ((a = vicf2 (SPELL_REFLECTS, 4)) != -1)
    {
      if (do_okay (mynum, a, PFL_NOMAGIC))
        {
          sendf (a, "&+RYou have been magically deafened!&*\n");
          ssetflg (a, SFL_DEAF);
          mudlog("%s DEAFENed %s.", pname(mynum), pname(a));
        }
      else
	{
	  bprintf("&+GYou can't just deafen %s at will, you know!&*\n", pname(a));
	  sendf (a, "&+G%s tried to deafen you!&*\n", see_name (a, mynum));
	}
    }
}

void blindcom (void)
{
  int a;
 
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("You can't cast spells in this room\n");
    return;
   }
 
  if ((a = vicf2 (SPELL_REFLECTS, 4)) != -1)
    {
      if (do_okay (mynum, a, PFL_NOMAGIC))
        {
          sendf (a, "&+RYou have been struck magically blind!&*\n");
          ssetflg (a, SFL_BLIND);
          mudlog("%s BLINDed %s.", pname(mynum), pname(a));
        }
      else
        {
          bprintf("&+GYou can't just blind %s at will, you know!&*\n", pname(a));
          sendf (a, "&+G%s tried to blind you!&*\n", see_name (a, mynum));
        }
    }
}

void curecom (void)
{
  int a;
  
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("You can't cast spells in this room\n");
    return;
   }
  if (EMPTY (item1))
    {
      bprintf ("Cure who?\n");
      return;
    }
  if ((a = vicf2 (SPELL_PEACEFUL, 8)) >= 0)
    {
      sclrflg (a, SFL_DUMB);
      sclrflg (a, SFL_CRIPPLED);
      sclrflg (a, SFL_BLIND);
      sclrflg (a, SFL_DEAF);
      sendf (a, "&+YAll of your ailments have been cured by %s.&*\n", pname(mynum));
      if (a != mynum)
        bprintf ("With a laying on of hands, you miraculously &+Ycure&* %s.\n", pname (a));
      mudlog("%s CUREd %s.", pname(mynum), pname(a));
    }
}

void dumbcom (void)
{
  int a;
  
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("You can't cast spells in this room\n");
    return;
   }

  if ((a = vicf2 (SPELL_REFLECTS, 4)) >= 0)
    {
      if (do_okay (mynum, a, PFL_NOMAGIC))
        {
          sendf (a, "&+RYou have been struck magically dumb!&*\n");
          ssetflg (a, SFL_DUMB);
          mudlog("%s has MUTEd %s.", pname(mynum), pname(a));
        }
      else
	{
	  sendf (a, "&+G%s tried to mute you!&*\n", see_name (a, mynum));
	  bprintf("&+GYou can't just mute %s at will, you know!&*\n", pname(a));
	}
    }
}

void cripplecom (void)
{
  int a;
  
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("You can't cast spells in this room\n");
    return;
   }
  if ((a = vicf2 (SPELL_REFLECTS, 4)) >= 0)
    {
      if (do_okay (mynum, a, PFL_NOMAGIC))
        {
          sendf (a, "&+RYou have been magically crippled!&*\n");
          ssetflg (a, SFL_CRIPPLED);
          mudlog("%s has CRIPPLEd %s.", pname(mynum), pname(a));
        }
      else
	{
	  sendf (a, "&+G%s tried to cripple you!&*\n", see_name (a, mynum));
	  bprintf("&+GYou can't just cripple %s at will, you know!&*\n", pname(a));
	}
    }
}

void colorcom(void)
{
  if (ststflg (mynum, SFL_COLOR))
    {
      bprintf ("Back to black and white.\n");
      sclrflg (mynum, SFL_COLOR);
    }
  else
    {
      bprintf ("Welcome to the wonderful world of &+RC&+YO&+BL&+GO&+WR&+r!\n");
      ssetflg (mynum, SFL_COLOR);
    }
}

void aloofcom(void)
{
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
    }
  if (ststflg (mynum, SFL_ALOOF))
    {
      bprintf ("&+RYou feel less distant.&*\n");
      sclrflg (mynum, SFL_ALOOF);
    }
  else
    {
      bprintf ("&+RFrom now on you'll ignore some mortals' actions.&*\n");
      ssetflg (mynum, SFL_ALOOF);
    }
}

void nobattlecom(void)
{
  if (ststflg(mynum, SFL_NOBATTLE))
    {
      bprintf ("&+WYou will once again see others' battles.&*\n");
      sclrflg (mynum, SFL_NOBATTLE);
    }
  else
    {
      bprintf ("&+WYou will no longer see others' battles.&*\n");
      ssetflg (mynum, SFL_NOBATTLE);
    }
}

void saybackcom(void)
{
  if (ststflg(mynum, SFL_SAYBACK)) 
    {
      bprintf ("&+RYou will stop seeing what you say.&*\n");
      sclrflg(mynum, SFL_SAYBACK);
    }
  else 
    {
      bprintf ("&+RYou will now see what you say.&*\n");
      ssetflg(mynum, SFL_SAYBACK);
    }
}

void autoexitcom(void)
{
  
  if (ststflg(mynum, SFL_AUTOEXIT)) 
    {
      bprintf ("&+YYou will no longer automatically see exits.&*\n");
      sclrflg(mynum, SFL_AUTOEXIT);
    }
  else 
    {
      bprintf ("&+YYou will now see exits upon entering the rooms.&*\n");
      ssetflg(mynum, SFL_AUTOEXIT);
    }
}

void noslaincom (void)
{
  if (ststflg(mynum, SFL_NOSLAIN)) 
    {
      bprintf("You uncover your eyes to reveal &+Rdeath and destruction.&*\n");
      sclrflg(mynum, SFL_NOSLAIN);
    }
  else 
    {
      bprintf("You turn your eyes from the rampant &+Rbloodshed.&*\n");
      ssetflg(mynum, SFL_NOSLAIN);
    }
}

void nobeepcom(void)
{
  if(ststflg(mynum, SFL_NOBEEP))
    {
      bprintf("You'll now receive \a&+Gbeeps.&*\n");
      sclrflg(mynum, SFL_NOBEEP);
    } 
  else
    {
      bprintf("You will no longer receive &+Gbeeps.&*\n");
      ssetflg(mynum, SFL_NOBEEP);
    }
}

void busycom(void)
{
  if (ststflg(mynum, SFL_BUSY))
    {
      bprintf("You are no longer marked as &+Bbusy.&*\n");
      sclrflg(mynum, SFL_BUSY);
    }
  else
    {
      bprintf("You are marked as &+Bbusy.&*\n");
      ssetflg(mynum, SFL_BUSY);
    }
}

void quietcom(void)
{
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
    }
  bprintf ("Quiet mode ");
  if (ststflg (mynum, SFL_QUIET))
    {
      bprintf ("&+Roff.&*\n");
      sclrflg (mynum, SFL_QUIET);
    }
  else
    {
      bprintf ("&+Ron.&*\n");
      ssetflg (mynum, SFL_QUIET);
    }
}

void briefcom (void)
{
  bprintf ("Brief mode ");
  if (ststflg (mynum, SFL_BRIEF))
    {
      bprintf ("&+Roff.&*\n");
      sclrflg (mynum, SFL_BRIEF);
    }
  else
    {
      bprintf ("&+Ron.&*\n");
      ssetflg (mynum, SFL_BRIEF);
    }
}

void noinventorycom(void)
{
  if (ststflg(mynum, SFL_NOINVEN)) 
    {
      bprintf ("You will once again see others &+Winventories&* when walking");
      bprintf ("through rooms.\n");
      sclrflg (mynum, SFL_NOINVEN);
    }
  else 
    {
      bprintf ("You will no longer see others &+Winventories&* when walking ");
      bprintf ("through rooms.\n");
      ssetflg (mynum, SFL_NOINVEN);
    }
}

void awaycom(void)
{
  if (ststflg(mynum, SFL_AWAY))
    {
      send_msg(DEST_ALL,MODE_COLOR|MODE_QUIET,LVL_APPREN,LVL_MAX,mynum,
	       NOBODY, "[\001p%s\003 has returned to the keyboard]\n",pname(mynum));
      bprintf("You are no longer marked as &+Baway.&*\n");
      sclrflg(mynum, SFL_AWAY);
    }
  else
    {
      send_msg(DEST_ALL, MODE_COLOR|MODE_QUIET, LVL_APPREN, LVL_MAX,
	       mynum, NOBODY, "[\001p%s\003 has left the keyboard]\n",pname(mynum));
      bprintf("You are marked as &+Baway.&*\n");
      ssetflg(mynum, SFL_AWAY);
    }
}

void codecom (void)
{
  if (!ptstflg(mynum, PFL_MAYCODE))
    {
      bprintf ("&+RSorry&*, but you are not allowed to code.\n");
      return;
    }
  bprintf ("Marking you as: ");
  if (!ststflg (mynum, SFL_CODING))
    {
      bprintf ("&+BCoding.&*\n");
      ssetflg (mynum, SFL_CODING);
    }
  else
    {
      bprintf ("&+BNOT&* Coding.\n");
      sclrflg (mynum, SFL_CODING);
    }
  return;
}

void compactcom(void)
{
  if (!ststflg(mynum, SFL_COMPACT))
    {
      bprintf("&+RCompact output ON.&*\n");
      ssetflg(mynum, SFL_COMPACT);
    }
 else
    {
      bprintf("&+RCompact output OFF.&*\n");
      sclrflg(mynum, SFL_COMPACT);
    }
}

void notalklinecom(int vb)
{
  int flg = 0;
  char line[256];
  
  switch(vb)
    {
    case VERB_NOGOD:
      flg = SFL_NO_GOD; sprintf(line, "God");
      if(plev(mynum) < LVL_GOD)
	{
	  bprintf("How can you ignore what you can not hear?\n");
	  return;
	}
      break;
    case VERB_NODGOD:
      flg = SFL_NO_DGOD; sprintf(line, "demi-god");
      if (plev(mynum) < LVL_DEMI)
	{
	  bprintf("How can you ignore what you can not hear?\n");
	  return;
	}
      break;
    case VERB_NOAWIZ:
      flg = SFL_NO_AWIZ; sprintf(line, "awiz");
      if (plev(mynum) < LVL_ARCHWIZARD)
	{
	  bprintf("How can you ignore what you can not hear?\n");
	  return;
	}
      break;
    case VERB_NOWIZ:
      flg = SFL_NO_WIZ; sprintf(line, "wiz");
      if (plev(mynum) < LVL_APPREN)
	{
	  bprintf("How can you ignore what you can not hear?\n");
	  return;
	}
      break;
    case VERB_NOGOSSIP:
      flg = SFL_NO_GOSSIP; sprintf(line, "gossip");
      break;
    }
  if(!ststflg(mynum,flg))
    {
      bprintf("You are no longer listening to the &+Y%s&* line.\n",line);
      ssetflg(mynum,flg);
    }
  else
    {
      bprintf("You are once again listening to the &+Y%s&* line.\n",line);
      sclrflg(mynum,flg);
    }
}

/*
void talklinecom(int vb)
{
  int flg = 0;
  int min = 0;
  char asp = '>';
  char bsp[32],clr[4];
  
  switch(vb)
    {
    case VERB_GOD:
      flg = SFL_NO_GOD;  min = LVL_GOD; asp='<'; sprintf(bsp,">");
      sprintf(clr,"&+R");
      break;
    case VERB_DGOD:
      flg = SFL_NO_DGOD;  min = LVL_DEMI; asp='*'; sprintf(bsp,"*");
      sprintf(clr,"&+B");
      break;
    case VERB_AWIZ:
      flg = SFL_NO_AWIZ;  min = LVL_ARCHWIZARD; asp='#'; sprintf(bsp,"#");
      sprintf(clr,"&+G");
      break;
    case VERB_WIZ:
      flg = SFL_NO_WIZ;  min = LVL_APPREN; asp=':'; sprintf(bsp,":");
      sprintf(clr,"&+Y");
      break;
    case VERB_GOSSIP:
      flg = SFL_NO_GOSSIP; min = 0; asp=' ' ;sprintf(bsp," gossips:");
      sprintf(clr,"&+W");
      break;
    }
  if(ststflg(mynum,flg))
    {
      bprintf("You don't want to listen to them, why should they listen to you?\n");
      return;
    }
  if(plev(mynum)< min)
    {
      if(vb==VERB_WIZ) bprintf("You wiz all over the ground.\n");
      else bprintf("That seems to be beyond your powers.\n");
      return;
    }
  getreinput(wordbuf);
  send_msg(DEST_ALL, MODE_COLOR|MODE_NSFLAG | MS(flg), min, LVL_MAX, NOBODY,
	   NOBODY, "%s%c\001p%s\003%s %s\n",clr,asp,pname(mynum),bsp,wordbuf);
}
*/

extern char * gossip_test (int player, int sender, char *text)
{
  static char buff[MAX_COM_LEN],*buffer;

  if (ststflg(player, SFL_NO_GOSSIP) ||
      (plev(player) < LVL_APPREN && ststflg(player, SFL_DEAF)) ||
      (ploc(player) != ploc(sender) &&
       (ltstflg(ploc(sender), LFL_SOUNDPROOF) ||
        ltstflg(ploc(player), LFL_SOUNDPROOF))))
    return NULL;

    buffer = build_voice_text(see_name(player, sender), "gossips", text, "'",
                              COLOR_HILIGHT_WHITE, COLOR_HILIGHT_WHITE);
    sprintf(buff, "%s", buffer );
    free( buffer );

  return buff;
}

void gossipcom(void)
{
  char blob[MAX_COM_LEN];

  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("I wouldn't be worrying about the gossip right now\n");
    bprintf ("You should be cooling off\n");
    return;
   }
  if (ststflg(mynum, SFL_GHOST))
    {
      bprintf("In your ghostly form, you find you cannot communicate with others.\n");
      return;
    }
  if (plev (mynum) < LVL_APPREN && ststflg (mynum, SFL_NO_GOSSIP)) {
    bprintf ("I'm sorry, you can't gossip anymore.\n");
    return;
  }

  getreinput (blob);
  if (EMPTY (blob)) {
    bprintf ("What do you want to gossip?\n");

    return;
  }

  if (time(0) == cur_player->p_last_comm) {
     bprintf ("You need to slow down and enunciate your gossip\n");
     return; }
  cur_player->p_last_comm = time(0);

  send_g_msg (DEST_ALL, gossip_test, mynum, blob);
}

void 
wizcom (void)
{
  if (plev (mynum) < LVL_APPRENTICE)
    {
      bprintf ("Very well. You wiz all over the floor.\n");
      return;
    }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("I'm sorry, this room isn't in service right now.\n");
    return;
   }
  getreinput (wordbuf);
  _wiz (LVL_APPRENTICE, wordbuf);
}

void 
awizcom (void)
{
  if (plev (mynum) < LVL_ARCHWIZARD)
    {
      bprintf ("Such advanced conversation is beyond you.\n");
      return;
    }
  getreinput (wordbuf);
  _wiz (LVL_ARCHWIZARD, wordbuf);
}

void 
godcom (void)
{
  if (plev (mynum) < LVL_GOD)
    {
      bprintf ("Such advanced conversation is beyond you.\n");
      return;
    }
  getreinput (wordbuf);
  _wiz (LVL_GOD, wordbuf);
}

void 
demigodcom (void)
{
  if (plev (mynum) < LVL_DEMI)
    {
      bprintf ("Such advanced conversation is beyond you.\n");
      return;
    }
  getreinput (wordbuf);
  _wiz (LVL_DEMI, wordbuf);
}

/* handles wiz communication */


static void 
_wiz (int level, char *text)
{
  register int t;
  int plx;
  char *buffer;
  if (strlen (text) > 230)
    text[230] = '\0';

  if (ststflg (mynum, SFL_NO_WIZ) && level==LVL_APPRENTICE)
    {
      bprintf ("Maybe you should turn NOWIZ off first!\n");
      return;
    }
  if (ststflg (mynum, SFL_NO_AWIZ) && level==LVL_ARCHWIZARD)
    {
      bprintf ("Maybe you should turn NOAWIZ off first!\n");
      return;
    }
  if (ststflg (mynum, SFL_NO_DGOD) && level==LVL_DEMI)
    {
      bprintf ("Maybe you should turn NODGOD off first!\n");
      return;
    }
  if (ststflg (mynum, SFL_NO_GOD) && level==LVL_GOD)
    {
      bprintf ("Maybe you should turn NOGOD off first!\n");
      return;
    }

  plx = mynum;
/*^&^&*/

  for (t = 0; t < max_players; t++)
    if (!EMPTY (pname (t)) && plev (t) >= level)
      {
	switch (level)
	  {
	  case LVL_APPRENTICE:
            buffer = build_voice_text (see_name(t, plx),
                                       ":",
                                       wordbuf,
                                       "",
                                       COLOR_HILIGHT_YELLOW,
                                       COLOR_HILIGHT_YELLOW);
	send_msg (t, MODE_COLOR | MODE_NSFLAG | MS(SFL_NO_WIZ), LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		  "%s", buffer);
            free(buffer);
	    break;
	  case LVL_ARCHWIZARD:
            buffer = build_voice_text (see_name(t, plx),
                                       "#",
                                       wordbuf,
                                       "",
                                       COLOR_HILIGHT_GREEN,
                                       COLOR_HILIGHT_GREEN);
	send_msg (t, MODE_COLOR | MODE_NSFLAG | MS(SFL_NO_AWIZ), LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		  "%s", buffer);
            free(buffer);
	    break;
	  case LVL_DEMI:
            buffer = build_voice_text (see_name(t,plx),
                                       "*",
                                       wordbuf,
                                       "",
                                       COLOR_HILIGHT_BLUE,
                                       COLOR_HILIGHT_BLUE);
	send_msg (t, MODE_COLOR | MODE_NSFLAG | MS(SFL_NO_DGOD), LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		  "%s", buffer);
            free(buffer);
	    break;
	  case LVL_GOD:
            buffer = build_voice_text (see_name(t, plx),
                                       ">",
                                       wordbuf,
                                       "",
                                       COLOR_NORMAL_RED,
                                       COLOR_NORMAL_RED);
	send_msg (t, MODE_COLOR | MODE_NSFLAG | MS(SFL_NO_GOD), LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		  "%s", buffer);
            free(buffer);
	    break;
	  default:
	    mudlog ("ERROR: case slip in xwiz.\n");
	    break;
	  }
      }
}
void noshoutcom ()
{
  int x;
  
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  if ((x = pl1) == -1 || x == mynum)
    {
      if (ststflg (mynum, SFL_NOSHOUT))
        {
          bprintf ("You can hear &+Mshouts&* again.\n");
          sclrflg (mynum, SFL_NOSHOUT);
        }
      else
        {
          bprintf ("From now on you won't hear &+Mshouts.&*\n");
          ssetflg (mynum, SFL_NOSHOUT);
        }
    }
  else if (plev (x) >= LVL_APPREN)
    {
      if (ststflg (x, SFL_NOSHOUT))
        {
          bprintf ("%s can hear &+Mshouts&* again.\n", pname (x));
          sclrflg (x, SFL_NOSHOUT);
        }
      else
        {
          bprintf ("From now on %s won't hear &+Mshouts.&*\n", pname (x));
          ssetflg (x, SFL_NOSHOUT);
        }
    }
  else if (ststflg (x, SFL_NOSHOUT))
    {
      bprintf ("%s is allowed to &+Mshout&* again.\n", pname (x));
      sclrflg (x, SFL_NOSHOUT);
    }
  else
    {
      bprintf ("From now on %s cannot &+Mshout.&*\n", pname (x));
      ssetflg (x, SFL_NOSHOUT);
    }
}
