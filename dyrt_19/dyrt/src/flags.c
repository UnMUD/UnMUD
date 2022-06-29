#include "kernel.h"
#include "mflagnames.h"
#include "pflagnames.h"
#include "sflagnames.h"
#include "lflagnames.h"
#include "levelnames.h"
#include "questnames.h"
#include "stdinc.h"
char *TF[3] =
  {"False", "True", TABLE_END};
char *OO[3] =
  {"Off", "On", TABLE_END};

void show_bits (int *bits, int n, char **table)
{
  int *b;
  int bno;
  int xno;
  int cno;
  int c2;
  char **t;
  char *p;
  Boolean t_end;
  char buff[16];
  
  cno = n;
  b = bits + n - 1;
  bno = 0;
  xno = 0;
  c2 = 0;
  t = table;
  t_end = False;
  for (; b >= bits; --b, ++cno)
    {
      for (bno = 0; bno < 32; bno++, xno++, t++)
	{
	  if (((*b) & (1 << bno)) != 0)
	    {
	      if (t_end)
                {
		  p = NULL;
		}
	      else if ((p = *t) == TABLE_END)
		{
		  t_end = True;
		  p = NULL;
		}
	      if (p == NULL)
		{
		  sprintf (buff, "<Spare%d>", xno);
		  p = buff;
		}
	      if (++c2 == 6)
		{
		  bprintf ("\n");
		  c2 = 1;
		}
	      bprintf ("%-14s\t", p);
	    }
	}
    }
  bprintf ("\n");
}

Boolean dump_pflags (void)
{
  static char *t[] =
    {"Pflags", "Mask", TABLE_END};
  int k, k2;
  
  if (plev (mynum) < LVL_ARCHWIZARD)
    {
      return False;
    }
  if (brkword () == -1)
    {
      return False;
    }
  if ((k = tlookup (wordbuf, t)) < 0)
    {
      return False;
    }
  if (brkword () == -1)
    {
      return False;
    }
  if ((k2 = tlookup (wordbuf, WizLevels)) < LEV_APPRENTICE)
    {
      return False;
    }
  bprintf ("%s for %s:\n", t[k], WizLevels[k2]);
  if (k == 0)
    {
      show_bits ((int *) &the_world->w_pflags[k2 - 1],
		 sizeof (PFLAGS) / sizeof (int), Pflags);
    }
  else
    {
      show_bits ((int *) &the_world->w_mask[k2 - 1],
		 sizeof (PFLAGS) / sizeof (int), Pflags);
    }
  return True;
}

void xshow_lflags (int loc)
{
  char k[80];
  LFLAGS p;
  
  p = xlflags (loc);
  bprintf ("Room: %s\nLflags:\n", buildname (k, loc));
  if (!ltstflg (loc, LFL_REALTIME))
    {
      bprintf ("Light: %s           Temp: %s\n",
  	       L_Light[(p & LFL_L_MASK) >> LFL_L_SHIFT],
	       L_Temp[(p & LFL_T_MASK) >> LFL_T_SHIFT]);
    }
  else
    {
      bprintf ("Light: %s           Temp: %s\n",
	       real_light_state (loc),
	       real_temp_state (loc));
    }
  p &= ~(LFL_L_MASK | LFL_T_MASK);
  show_bits ((int *) &p, sizeof (LFLAGS) / sizeof (int), Lflags);
}

void pflagscom (void)
{
  int a, b, c, l;
  char *n;
  Boolean f;
  PFLAGS *p;
  PERSONA d;
  
  f = False;
  if (brkword () == -1)
    {
      a = mynum;
      p = &(pflags (a));
      n = pname (a);
      l = plev (a);
    }
  else if ((a = fpbn (wordbuf)) != -1)
    {
      if (a != mynum && !ptstflg (mynum, PFL_PFLAGS) && a < max_players)
	{
	  bprintf ("You can only change/see your own P-flags.\n");
	  return;
	}
      p = &(pflags (a));
      n = pname (a);
      l = plev (a);
    }
  else if (!ptstflg (mynum, PFL_UAF))
    {
      bprintf ("Who's that?\n");
      return;
    }
  else if (!ptstflg (mynum, PFL_PFLAGS))
    {
      bprintf ("You can only change/see your own P-flags.\n");
      return;
    }
  else if (!getuaf (wordbuf, &d))
    {
      bprintf ("No such persona in system.\n");
      return;
    }
  else
    {
      f = True;
      p = &(d.p_pflags);
      n = d.p_name;
      l = d.p_level;
    }
  if (l >= LVL_MAX)
    {
      bprintf("No such persona in system.\n");
      return;
    }
  if ((a != mynum && wlevel (plev (mynum)) <= wlevel (l) && 
      plev (mynum) < LVL_GOD)
      || (a >= max_players && plev (mynum) < LVL_APPREN))
    {
      bprintf ("That is beyond your powers.\n");
      return;
    }
  if (brkword () == -1)
    {
      bprintf ("Player: %s\n", n);
      bprintf ("Privileges:\n");
      show_bits ((int *) p, sizeof (PFLAGS) / sizeof (int), Pflags);
      return;
    }
  else if ((b = tlookup (wordbuf, Pflags)) == -1
	   || (!ptstmsk (mynum, b) && !OPERATOR (pname (mynum))))
    {
      bprintf ("%s: No such P-Flag.\n", wordbuf);
      return;
    }
  else if (brkword () == -1)
    {
      c = tst_doublebit (p,b) ? 1 : 0;
      bprintf ("Value of %s is %s\n", Pflags[b], TF[c]);
      return;
    }
  else if (a != mynum && !ptstflg (mynum, PFL_PFLAGS))
    {
      bprintf ("You can only change your own P-flags.\n");
      return;
    }
  else if ((c = tlookup (wordbuf, TF)) == -1)
    {
      bprintf ("Value must be True or False.\n");
      return;
    }
  if (!EQ (n, pname (mynum)))
    {
      mudlog ("PFLAG: %s by %s, %s := %s",
	      n, pname (mynum), Pflags[b], TF[c]);
    }
  if (c == 0)
    {
      clr_doublebit (p, b);
    }
  else
    {
      set_doublebit (p, b);
    }
  if (f)
    {
      putuaf (&d);
    }
}

void maskcom (void)
{
  int a, b, c;
  char *n;
  Boolean f;
  PFLAGS *p;
  PERSONA d;
  
  f = False;
  if (brkword () == -1 || !ptstflg (mynum, PFL_MASK))
    {
      a = mynum;
      p = &(pmask (a));
      n = pname (a);
    }
  else if ((a = fpbn (wordbuf)) != -1)
    {
      p = &(pmask (a));
      n = pname (a);
    }
  else if (!ptstflg (mynum, PFL_UAF))
    {
      bprintf ("Who's that?\n");
      return;
    }
  else if (!getuaf (wordbuf, &d))
    {
      bprintf ("No such persona in system.\n");
      return;
    }
  else
    {
      f = True;
      p = &(d.p_mask);
      n = d.p_name;
    }
  if (brkword () == -1)
    {
      bprintf ("Player: %s\nMask:\n", n);
      show_bits ((int *) p, sizeof (PFLAGS) / sizeof (int), Pflags);
      return;
    }
  else if ((b = tlookup (wordbuf, Pflags)) == -1
	   || (!ptstmsk (mynum, b) && !ptstflg (mynum, PFL_MASK)))
    {
      bprintf ("%s: No such Mask-bit.\n", wordbuf);
      return;
    }
  else if (!ptstflg (mynum, PFL_MASK) || brkword () == -1)
    {
      c = tst_doublebit (p, b) ? 1 : 0;
      bprintf ("Value of %s is %s\n", Pflags[b], TF[c]);
      return;
    }
  else if ((c = tlookup (wordbuf, TF)) == -1)
    {
      bprintf ("Value must be True or False.\n");
      return;
    }
  mudlog ("MASK: %s by %s, %s := %s", n, pname (mynum), Pflags[b], TF[c]);
  if (c == 0)
    {
      clr_doublebit (p, b);
    }
  else
    {
      set_doublebit (p, b);
    }
  if (f)
    {
      putuaf (&d);
    }
}

void mflagscom (void)
{
  int a, b, c;
  
  if (plev (mynum) < LVL_APPREN)
    {
      bprintf ("Whatever?\n");
      return;
    }
  if (brkword () == -1)
    {
      bprintf ("Whose MFlags?\n");
      return;
    }
  if ((a = fpbn (wordbuf)) < max_players)
    {
      bprintf ("No mobile with this name!\n");
      return;
    }
  if (brkword () == -1 || (b = tlookup (wordbuf, Mflags)) == -1)
    {
      bprintf ("Mobile: %s\n", pname (a));
      bprintf ("Mflags:\n");
      show_bits ((int *) (&mflags (a)), sizeof (MFLAGS) / sizeof (int), Mflags);
      return;
    }
  if (brkword () == -1)
    {
      c = mtstflg (a, b) ? 1 : 0;
      bprintf ("Value of %s is %s\n", Mflags[b], TF[c]);
      return;
    }
  if (!ptstflg (mynum, PFL_MFLAGS))
    {
      bprintf ("You are not authorised to do this...\n");
      return;
    }
  if ((c = tlookup (wordbuf, TF)) == -1)
    {
      bprintf ("\001A\033[31m\003Set flag to True or False.\001A\033[0m\003\n");
      return;
    }
  if (c == 0)
    {
      mclrflg (a, b);
    }
  else
    {
      msetflg (a, b);
    }
}

void lflagscom (void)
{
  int a, b, c, d = 0;
  char k[80];
  
  if (plev (mynum) < LVL_APPREN)
    {
      bprintf ("I'm afraid I didn't catch that one...?\n");
      return;
    }
  if ((a = getroomnum ()) == 0)
    a = ploc (mynum);
  if (((b == LFL_WIZONLY) && (plev(mynum) < LVL_APPREN)) ||
      ((b == LFL_AWIZONLY) && (plev(mynum) < LVL_ARCHWIZARD)) ||
      ((b == LFL_DGODONLY) && (plev(mynum) < LVL_DEMI)) ||
      ((b == LFL_GODONLY) && (plev(mynum) < LVL_GOD)))
    {
      bprintf("Sorry, you are not high enough level to alter that lflag.\n");
      return;
    }
  if ((b == LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("Hmm...doesn't seem to have changed, sorry\n");
    return; }
  if (brkword () == -1)
    {
      xshow_lflags (a);
      return;
    }
  buildname (k, a);
  if ((b = tlookup (wordbuf, Lflags)) == -1)
    {
      if ((b = tlookup (wordbuf, L_Light)) > -0)
	d = 1;
      else if ((b = tlookup (wordbuf, L_Temp)) >= 0)
	d = 2;
      else
	{
	  bprintf ("%s: No such Location-Flag.\n", wordbuf);
	  return;
	}
    }
  if (brkword () == -1)
    {
      if (d == 1)
	{
	  bprintf ("Value of Light in %s is %s\n", k,
		   L_Light[(xlflags (a) & LFL_L_MASK) >> LFL_L_SHIFT]);
	}
      else if (d == 2)
	{
	  bprintf ("Value of Temp in %s is %s\n", k,
		   L_Temp[(xlflags (a) & LFL_T_MASK) >> LFL_T_SHIFT]);
	}
      else
	{
	  c = ltstflg (a, b);
	  c = (c != 0);
	  bprintf ("Value of %s in %s is %s\n", Lflags[b], k, TF[c]);
	}
      return;
    }
  if (!ptstflg (mynum, PFL_ROOM))
    {
      bprintf ("You are not authorised to do this.\n");
      return;
    }
  if ((c = tlookup (wordbuf, TF)) == -1)
    {
      bprintf ("\001A\033[31m\003Set flag to True or False.\001A\033[0m\003\n");
      return;
    }
  if (c)
    lsetflg (a, b);
  else
    lclrflg (a, b);
}

void set_xpflags (int y, PFLAGS * p, PFLAGS * m)
{
  
  if (y >= LVL_MASTER)
    {
      *p = the_world->w_pflags[9];
      *m = the_world->w_mask[9];
    }
  else if (y >= LVL_GOD)
    {
      *p = the_world->w_pflags[8];
      *m = the_world->w_mask[8];
    }
  else if (y >= LVL_HIGHDEMI)
    {
      *p = the_world->w_pflags[7];
      *m = the_world->w_mask[7];
    }
  else if (y >= LVL_DEMI)
    {
      *p = the_world->w_pflags[6];
      *m = the_world->w_mask[6];
    }
  else if (y >= LVL_HIGHARCH)
    {
      *p = the_world->w_pflags[5];
      *m = the_world->w_mask[5];
    }
  else if (y >= LVL_ARCHWIZARD)
    {
      *p = the_world->w_pflags[4];
      *m = the_world->w_mask[4];
    }
  else if (y >= LVL_SENIOR)
    {
      *p = the_world->w_pflags[3];
      *m = the_world->w_mask[3];
    }
  else if (y >= LVL_WIZARD)
    {
      *p = the_world->w_pflags[2];
      *m = the_world->w_mask[2];
    }
  else if (y > LVL_APPREN)
    {
      *p = the_world->w_pflags[1];
      *m = the_world->w_mask[1];
    }
  else if (y == LVL_APPREN)
    {
      *p = the_world->w_pflags[0];
      *m = the_world->w_mask[0];
    }
  else
    {
      p->b3 = p->b2 = p->b1 = 0;
      if (y >= LVL_FIFTEEN)
	{
	  set_doublebit (p, PFL_EMOTE);
	}
      *m = *p;
    }
}
