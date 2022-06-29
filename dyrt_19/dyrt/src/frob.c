#include "kernel.h"
#include "stdinc.h"

#define PFLAGS_FROB ((1<<PFL_FROB)|(1<<PFL_CH_SCORE)|(1<<PFL_CH_LEVEL))
#define cant_fr(x,l,w,a,d,g) ((x<a && l>=w) || (x<d && l>=a) || (x<g && l>=d))
#define cant_frob(lev) \
cant_fr(plev(mynum),lev,LVL_APPREN,LVL_ARCHWIZARD,LVL_DEMI, LVL_GOD)
#define cant_fro(lev) (lev < LVL_MIN || lev >= LVL_MAX || cant_frob(lev))
     struct _f
{
  int state;
  int oldwork;
  int oldlev;
  int level;
  int strength;
  int score;
  int ingame;
  char *oldprompt;
  char name[PNAME_LEN];
};

static void log (char *n, int lev, int sco, int str)
{
  mudlog ("FROB: %s by %s: Lev = %d, Sco = %d, Str = %d",
	  n, pname (mynum), lev, sco, str);
}

void frobcom (char *line)
{
  PERSONA p;
  struct _f *f;

  int x;
  int n;
  if (line == NULL)
    {				/* First (initial) time */
      if (!ptstflg(mynum, PFL_FROB))
	{
	  erreval ();
	  return;
	}
      if (brkword () == -1
	  || ((x = fpbn (wordbuf)) == -1 && !ptstflg (mynum, PFL_UAF)))
	{
	  bprintf ("Frob who?\n");
	  return;
	}
      if (x == -1)
	{
	  if (!getuaf (wordbuf, &p))
	    {
	      bprintf ("No such persona in system.\n");
	      return;
	    }
	  if (cant_frob (p.p_level))
	    {
	      bprintf ("You can't frob %s!\n", wordbuf);
	      return;
	    }
	}
      else if (x >= max_players)
	{
	  bprintf ("You can't frob mobiles!\n");
	  return;
	}
      else if (cant_frob (plev (x)))
	{
	  bprintf ("You can't frob %s!\n", wordbuf);
	  return;
	}
      else
	{
	  p.p_level = plev (x);
	  p.p_strength = pstr (x);
	  p.p_score = pscore (x);
	  strcpy (p.p_name, pname (x));
	}
      f = NEW (struct _f, 1);
      strcpy (f->name, p.p_name);
      f->ingame = fpbn(f->name);
      f->state = 0;
      f->level = p.p_level;
      f->oldlev = p.p_level;
      f->strength = p.p_strength;
      f->score = p.p_score;
      f->oldprompt = COPY (cur_player->prompt);
      bprintf("Level name  Level  Score  Strength   Level name  Level  Score  Strength\n");
      bprintf("----------  -----  -----  --------   ----------  -----  -----  --------\n");
      bprintf("Novice      1      0      56         Wanderer    2      400    62      \n");
      bprintf("Sightseer   3      1000   68         Nomad       4      2000   74      \n");
      bprintf("Warrior     5      3500   80         Champion    6      6000   86      \n");
      bprintf("Squire      7      9500   92         Knight      8      14000  98      \n");
      bprintf("Prince      9      20000  104        Ruler       10     30000  110     \n");
      bprintf("Conqueror   11     45000  116        Magician    12     65000  122     \n");
      bprintf("Evoker      13     95000  128        Honorable   14     130000 134     \n");
      bprintf("Holy        15     160000 140        Priest      16     200000 146     \n");
      bprintf("Appren Wiz  17     250000 152        Emerit.Wiz  18     --     158     \n");
      bprintf("Wizard      1000   --     6050       Senior Wiz  5000   --     30050   \n");
      bprintf("Archwiz     10000  --     60050      High Arch   30000  --     180050  \n");
      bprintf("Demigod     50000  --     300050     High Demi   80000  --     480050  \n");
      bprintf("----------  -----  -----  --------   ----------  -----  -----  --------\n");
      strcpy (cur_player->prompt, "New Level: ");
      f->oldwork = cur_player->work;
      cur_player->work = (int) f;
      bprintf ("Level is: %d\n", f->level);
      push_input_handler (frobcom);
    }
  else
    {
      while (*line == ' ' || *line == '\t')
	++line;
      f = (struct _f *) cur_player->work;
      switch (f->state)
	{
	case 0:
	  if (*line == '\0')
	    {
	      x = f->level;
	    }
	  else
	    {
	      x = atoi (line);
	    }
	  if (x < LVL_ONE || x > LVL_MAX)
	    {
	      bprintf ("Level must be between %d and %d\n", LVL_ONE, LVL_MAX);
	      f->state = 20;
	    }
	  else if (cant_fro (x))
	    {
	      bprintf ("You can't do that.\n");
	      f->state = 20;
	    }
	  else
	    {
	      f->level = x;
	      f->state = 1;
	      bprintf ("Score is: %d\n", f->score);
	      strcpy (cur_player->prompt, "New Score: ");
	    }
	  break;
	case 1:
	  if (*line == '\0')
	    {
	      x = f->score;
	    }
	  else
	    {
	      x = atoi (line);
	    }
	  f->score = x;
	  f->state = 2;
	  bprintf ("Strength is: %d\n", f->strength);
	  strcpy (cur_player->prompt, "New Strength: ");
	  break;
	case 2:
	  if (*line == '\0')
	    {
	      x = f->strength;
	    }
	  else
	    {
	      x = atoi (line);
	    }
	  if (x <= 0)
	    {
	      bprintf ("Strength must be positive.\n");
	      f->state = 20;
	    }
	  else
	    {
	      f->strength = x;
	      if ((x = fpbn (f->name)) == -1)
		{
		  if (!ptstflg (mynum, PFL_UAF))
		    {
		      bprintf ("%s isn't here.\n", f->name);
		      f->state = 20;
		    }
		  else if (!getuaf (f->name, &p))
		    {
		      bprintf ("No player named %s.\n", f->name);
		      f->state = 20;
		    }
		}
	      else
		{
		  p.p_level = plev (x);
		  p.p_strength = pstr (x);
		  p.p_score = pscore (x);
		}
	      if (f->state == 2)
		{
		  log (f->name, f->level, f->score, f->strength);
		  if (x >= 0)
		    {
		      setpstr (x, f->strength);
		      setpscore (x, f->score);
		      setplev (x, f->level);
		      if (f->level >= 4) {
                      set_doublebit (&pflags (x), PFL_TITLES);
		      set_doublebit (&pflags (x), PFL_EMOTE);
		      }
		      if (wlevel (f->level) != wlevel (f->oldlev))
			{
			  set_xpflags (f->level, &pflags (x), &pmask (x));
			}
		      if (f->level <= LVL_APPREN || f->oldlev <= LVL_APPREN)
			{
			  setptitle (x, std_title (f->level, psex (x)));
			}
		    }
		  else
		    {
		      p.p_level = f->level;
		      p.p_strength = f->strength;
		      p.p_score = f->score;
		      if (wlevel (f->level) != wlevel (f->oldlev))
			{
			  set_xpflags (f->level, &p.p_pflags, &p.p_mask);
			}
		      if (f->level <= LVL_APPREN || f->oldlev <= LVL_APPREN)
			{
			  strcpy (p.p_title,
				  std_title (f->level, xtstbit (p.p_sflags, SFL_FEMALE)));
			}
		      putuaf (&p);
		    }
		  update_wizlist (f->name, wlevel (f->level));
		  bprintf ("Ok.\n");
		  f->state = 20;
		}
	    }
	  break;
	}
      if (f->state == 20)
	{
	  strcpy (cur_player->prompt, f->oldprompt);
	  free (f->oldprompt);
	  cur_player->work = f->oldwork;
          if(f->ingame >=0 && is_in_game(f->ingame)) 
            {
              n = mynum;
              setup_globals(f->ingame);
              saveme();
              bprintf("%s", cur_player->prompt);
              setup_globals(n); 
            }
	  free (f);
	  pop_input_handler ();
	}
    }
  bprintf ("%s", cur_player->prompt);
}

void wizfrobcom (void)
{
  int x;

  if (!ptstflg(mynum, PFL_FROB))
    {
       erreval();
       return;
    }
  if (brkword() == -1 || ((x = fpbn(wordbuf)) == -1 ))
    {
      bprintf("Frob who?\n");
      return;
    }
    if (x >= max_players)
      {
        bprintf ("You can't frob mobiles!\n");
        return;
      }
    else if (cant_frob (plev (x)))
      {
        bprintf ("You can't frob %s!\n", wordbuf);
        return;
      }
    else
      {
	setplev(x, 10);
	setpstr(x, 110);
	setpscore(x, 42000);
	psetflg(x, PFL_EMOTE);
	psetflg(x, PFL_TITLES); 
        setptitle (x, std_title (10, ststflg(x,SFL_FEMALE)));
	mudlog("FROB: %s has given %s a wizfrob.",pname(mynum), pname(x));
  	bprintf("You have given %s a wizfrob.\n", pname(x));
      }
}

void awizfrobcom (void)
{
  int x;

  if (!ptstflg(mynum, PFL_FROB))
    {
       erreval();
       return;
    }
  if (brkword() == -1 || ((x = fpbn(wordbuf)) == -1 ))
    {
      return;
    }
    if (x >= max_players)
      {
        bprintf ("You can't frob mobiles!\n");
        return;
      }
    else if (cant_frob (plev (x)))
      {
        bprintf ("You can't frob %s!\n", wordbuf);
        return;
      }
    else
      {
        setplev(x, 11);
        setpstr(x, 116);
        setpscore(x, 62000);
        psetflg(x, PFL_EMOTE);
        psetflg(x, PFL_TITLES);
        setptitle (x, std_title (11, ststflg(x,SFL_FEMALE)));
        mudlog("FROB: %s has given %s an awizfrob.",pname(mynum), pname(x));
  	bprintf("You have given %s a awizfrob.\n", pname(x));
      }
}

void dgodfrobcom (void)
{
  int x;

  if (!ptstflg(mynum, PFL_FROB))
    {
       erreval();
       return;
    }
  if (brkword() == -1 || ((x = fpbn(wordbuf)) == -1 ))
    {
      return;
    }
    if (x >= max_players)
      {
        bprintf ("You can't frob mobiles!\n");
        return;
      }
    else if (cant_frob (plev (x)))
      {
        bprintf ("You can't frob %s!\n", wordbuf);
        return;
      }
    else
      {
        setplev(x, 12);
        setpstr(x, 122);
        setpscore(x, 82000);
        psetflg(x, PFL_EMOTE);
        psetflg(x, PFL_TITLES);
        setptitle (x, std_title (12, ststflg(x,SFL_FEMALE)));
        mudlog("FROB: %s has given %s a dgodfrob.",pname(mynum), pname(x));
  	bprintf("You have given %s a dgodfrob.\n", pname(x));
      }
}

void godfrobcom (void)
{
  int x;

  if (!ptstflg(mynum, PFL_FROB))
    {
       erreval();
       return;
    }
  if (brkword() == -1 || ((x = fpbn(wordbuf)) == -1 ))
    {
      return;
    }
    if (x >= max_players)
      {
        bprintf ("You can't frob mobiles!\n");
        return;
      }
    else if (cant_frob (plev (x)))
      {
        bprintf ("You can't frob %s!\n", wordbuf);
        return;
      }
    else
      {
        setplev(x, 13);
        setpstr(x, 128);
        setpscore(x, 102000);
        psetflg(x, PFL_EMOTE);
        psetflg(x, PFL_TITLES);
        setptitle (x, std_title (13, ststflg(x,SFL_FEMALE)));
        mudlog("FROB: %s has given %s a godfrob.",pname(mynum), pname(x));
   	bprintf("You have given %s a godfrob.\n", pname(x));
      }
}
