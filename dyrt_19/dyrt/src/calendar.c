#include "kernel.h"
#include "locations.h"
#include "stdinc.h"

CALENDAR calendar;
int temp_seed;
char old_weather;

static void _update_calendar (void);
static Boolean cold_weather (int loc);
static Boolean night_weather (void);
static void advance_weather (void);
static void announce_weather (void);
static void tell_weather (int user);

char *Weather[] =
{
  "sunny",
  "rainy",
  "cloudy",
  "clearing",
  "stormy"
};

char *c_seasons[] =
{
  "spring",
  "summer",
  "autumn",
  "winter"
};

char *c_modifiers[] =
{
  "an early-",
  "a ",
  "a late-"
};

char *c_day[] =
{
  "morning",
  "day",
  "afternoon",
  "evening",
  "night"
};

void init_calendar (void)
{
  calendar.season = 0;
  calendar.daytime = 0;
  calendar.daycount = 0;
  calendar.modifier = 1;
}

void update_calendar (void)
{
  _update_calendar ();
  compute_environment ();
}

static void _update_calendar (void)
{
  /* start with day, work up to season */
  
#ifdef MALLOC_DEBUG
  mnem_writestats ();
#endif
  
  if (calendar.daytime != NIGHT)
    {
      calendar.daytime++;
    }
  else
    {
      calendar.daytime = MORNING;	/* new day */
    }
  switch (calendar.daytime)
    {
      case MORNING:
        broad_realtime ("The &+Bsky&* &+Wlightens&* as the &+Ysun&* rises slowly in the east.\n");
        break;
      case EVENING:
        broad_realtime ("The &+Ysun&* begins to sink below the western &+Rhorizon.&*\n");
        break;
      case NIGHT:
        broad_realtime ("The &+Ysun&* sets and &+Ldarkness&* befalls the realm.\n");
        break;
      default:;
    }
  if (calendar.daytime)
    {
      return;
    }
  /* day has ended.. make a new random seed */
  temp_seed = 0;
  if (randperc () < 50)
    {
      temp_seed += randperc () % 7;
    }
  else
    {
      temp_seed -= randperc () % 7;
    }
  if (calendar.daycount != MONTH_DAYCOUNT)
    {
      calendar.daycount++;
    }
  else
    {
      calendar.daycount = 0;
    }
  if (calendar.daycount)
    {
      return;
    }
  /* 1 modifier has ended */
  if (calendar.modifier != LATE_SEASON)
    {
      calendar.modifier++;
    }
  else
    {
      calendar.modifier = EARLY_SEASON;
    }
  if (calendar.modifier)
    {
      return;
    }
  /* one season has ended */
  if (calendar.season != WINTER)
    {
      calendar.season++;
    }
  else
    {
      calendar.season = SPRING;
    }
  switch (calendar.season)
    {
      case WINTER:
        broad_realtime ("A brisk wind cuts through you as &+WW&+Bi&+Wn&+Bt&+We&+Br&* arrives.\n");
        break;
      case SUMMER:
        broad_realtime ("The &+YSummer Solstice&* arrives as the days seem longer.\n");
        break;
      case SPRING:
        broad_realtime ("The air fills with the songs of birds as &+GSpring&* "
		      "arrives.\n");
        break;
      case AUTUMN:
        broad_realtime ("The air cools and the leaves change colour as &+yA&+ru&+yt&+ru&+ym&+rn&* "
		      "arrives.\n");
        break;
    }
  if (calendar.season)
    {
      return;
    }
  /* one year done! */
  calendar.season = SPRING;
  calendar.daytime = MORNING;
  calendar.daycount = 0;
  calendar.modifier = EARLY_SEASON;
}

void compute_environment (void)
{
  int temp;
  Boolean sign = False;
  temp = 0;
  
  if (randperc () < 50)
    {
      sign = True;
    }
  if (calendar.daytime == NIGHT)
    {
      calendar.light = False;
    }
  else
    {
      calendar.light = True;
    }
  /* using celsius temperatures */
  switch (calendar.season)
    {
      case SPRING:
        switch (calendar.modifier)
  	  {
	    case EARLY_SEASON:
	      temp = SPRING_E_SEASON;
	      break;
    	    case MID_SEASON:
	      temp = SPRING_M_SEASON;
	      break;
	    case LATE_SEASON:
	      temp = SPRING_L_SEASON;
	      break;
	  }
        break;
      case SUMMER:
        switch (calendar.modifier)
  	  {
	    case EARLY_SEASON:
	      temp = SUMMER_E_SEASON;
	      break;
	    case MID_SEASON:
	      temp = SUMMER_M_SEASON;
	      break;
	    case LATE_SEASON:
	      temp = SUMMER_L_SEASON;
	      break;
	  }
        break;
      case AUTUMN:
        switch (calendar.modifier)
	  {
	    case EARLY_SEASON:
	      temp = AUTUMN_E_SEASON;
	      break;
	    case MID_SEASON:
	      temp = AUTUMN_M_SEASON;
	      break;
	    case LATE_SEASON:
	      temp = AUTUMN_L_SEASON;
	      break;
	  }
        break;
      case WINTER:
        switch (calendar.modifier)
	  {
	    case EARLY_SEASON:
	      temp = WINTER_E_SEASON;
	      break;
	    case MID_SEASON:
	      temp = WINTER_M_SEASON;
	      break;
	    case LATE_SEASON:
	      temp = WINTER_L_SEASON;
	      break;
  	  }
        break;
    }
  /* adjust for time of day */
  switch (calendar.daytime)
    {
      case MORNING:
        temp T_MORNING;
        break;
      case DAY:
        temp T_DAY;
        break;
      case AFTERNOON:
        temp T_AFTERNOON;
        break;
      case EVENING:
        break;
      case NIGHT:
        temp T_NIGHT;
        break;
    }
  temp += temp_seed;
  /* if it is raining, change temp a bit */
  if (the_world->w_weather == RAIN || the_world->w_weather == STORMY)
    {
      temp T_RAIN;
    }
  calendar.temp = temp;
}

/* change calendar settings */
void calendarcom (void)
{
  register int t;
  
  if (plev (mynum) < LVL_ARCHWIZARD)
    {
      erreval ();
      return;
    }
  if (EMPTY (item1))
    {
      bprintf ("Usage: calendar [newsetting]\n");
      return;
    }
  /* change weather */
  for (t = SPRING; t != (WINTER + 1); t++)
    if (strncasecmp (Weather[t], item1, strlen (item1)) == 0)
      {
	the_world->w_weather = t;
	send_msg (DEST_ALL, MODE_QUIET | MODE_COLOR, LVL_APPREN,
		  LVL_MAX, NOBODY, NOBODY,
		  "[%s has changed weather to %s]\n",
		  pname (mynum), Weather[t]);
	
	compute_environment ();
	return;
      }
  /* change season */
  for (t = SPRING; t != (WINTER + 1); t++)
    if (strncasecmp (c_seasons[t], item1, strlen (item1)) == 0)
      {
	calendar.season = t;
	sendf (DEST_ALL, "Season changed to %s.\n", c_seasons[t]);
	send_msg (DEST_ALL, MODE_QUIET | MODE_COLOR, LVL_APPREN,
		  LVL_MAX, NOBODY, NOBODY,
		  "[%s has changed season to %s]\n",
		  pname (mynum), c_seasons[t]);
	compute_environment ();
	return;
      }
  /* change daytime */
  for (t = MORNING; t != (NIGHT + 1); t++)
    if (strncasecmp (c_day[t], item1, strlen (item1)) == 0)
      {
	calendar.daytime = t;
	sendf (DEST_ALL, "Daytime changed to %s.\n", c_day[t]);
	send_msg (DEST_ALL, MODE_QUIET | MODE_COLOR, LVL_APPREN,
		  LVL_MAX, NOBODY, NOBODY,
		  "[%s has changed daytime to %s]\n",
		  pname (mynum), c_day[t]);
	compute_environment ();
	return;
      }
  bprintf ("Don't know how to modify that in the calendar.\n");
  return;
}

/* send a message to anyone subject to RealTime */
void broad_realtime (char *txt)
{
  register int t;
  
  for (t = 0; t < max_players; t++)
    {
/*
      if (EMPTY (pname (t)))
	{
	  continue;
	}
Bad bad bad!
*/
      if(!is_in_game(t)) continue;
      if (!ltstflg (ploc (t), LFL_REALTIME) || ststflg (t, SFL_QUIET))
	{
	  continue;
	}
      sendf (t, txt);
    }
  return;
}

void weather (void)
{
  advance_weather ();
  announce_weather ();
}

static void advance_weather (void)
{
  int t;
  
  old_weather = the_world->w_weather;
  switch (old_weather)
    {
      case SUNNY:
        if (randperc () < 50)
   	return;			/* more sunny    */
        the_world->w_weather = CLOUDY;	/* clouds        */
        break;
      case CLOUDY:
        t = randperc ();
        if (t < 15)
	return;			/* more rain     */
        if (t < 40)
 	  {
	    the_world->w_weather = STORMY;	/* storm         */
	  }
        else
	  {
	    the_world->w_weather = RAINY;	/* rain          */
    	  }
        break;
      case RAINY:
        if (randperc () > 60)
 	  {
	    return;			/* more rain     */
	  }
        the_world->w_weather = CLEARING;	/* clearing      */
        break;
      case STORMY:
        if (randperc () < 15)
	  {
	    return;			/* more storm    */
  	  }
        the_world->w_weather = CLEARING;	/* clearing      */
        break;
      case CLEARING:
        t = randperc ();
        if (t < 20)
	  {
	    return;			/* more clearing */
            if (t < 60)
	      {
		the_world->w_weather = RAINY;	/* rain          */
	      }
            else
	      {
		the_world->w_weather = SUNNY;	/* sunny         */
	      }
	  }
    }
}

/* sends a message to all users (calls tell_weather) */
static void announce_weather ()
{
  register int t;
  if (the_world->w_weather == old_weather)
    {
      return;			/* no change */
    }
  for (t = 0; t < max_players; t++)
    {
      if (is_in_game (t))
 	{
	  tell_weather (t);
	}
    }
}

/* sends the weather to a particular user */
static void tell_weather (int user)
{
  if (!ltstflg (ploc (user), LFL_OUTDOORS) || ststflg (user, SFL_QUIET))
    {
      return;
    }
  switch (the_world->w_weather)
    {
      case SUNNY:
        sendf (user, "The world around you &+Wbrightens&* a bit as the %s comes out "
	     "of the clouds.\n",
	     night_weather ()? "&+Ymoon&*" : "&+Ysun&*");
        break;
      case RAINY:
        if (cold_weather (ploc (user)))
	  {
	    sendf (user, "A few light &+Ws&+wn&+Wo&+ww&* &+Wf&+wl&+Wu&+wr&+Wr&+wi&+We&+ws&* tumble to the ground.\n");
	  }
        else
	  {
	    sendf (user, "A light &+Crain&* falls gently upon the land.\n");
	  }
        break;
      case STORMY:
        if (cold_weather (ploc (user)))
	  {
	    sendf (user, "You trudge onward through the heavy &+Ws&+wn&+Wo&+ww&+Ws&*.\n");
 	  }
        else
	  {
	    sendf (user, "There is a loud &+Rcrash&* as the sky fills with &+Wlightning&*.\n");
	  }
        break;
      case CLEARING:
        sendf (user, "The &+Cc&+Wl&+Co&+Wu&+Cd&+Ws&* begin to dissipate%s.\n",
	     night_weather ()? ", revealing a stunning night sky" : "");
        break;
      case CLOUDY:
        sendf (user, "&+LDark storm clouds&* begin to roll in.\n");
        break;
    }
}

char test_temp (void)
{
  int t = ploc (mynum);
  
  if (cold_weather (t))
    {
      return (ISCOLD);
    }
  return (ISNORMAL);
}

/* this function does not account for levels */
Boolean is_real_dark (void)
{
  if (calendar.daytime == NIGHT)
    {
      return (True);
    }
  return (False);
}

static Boolean night_weather ()
{
  if (calendar.daytime == 3 || calendar.daytime == 4)
    {
      return (True);
    }
  return (False);
}

/* Give a description of the weather when walking into a room. */
void show_weather ()
{
  if (!ltstflg (ploc (mynum), LFL_OUTDOORS))
    {
      return;
    }
  switch (the_world->w_weather)
    {
      case RAIN:
        if (cold_weather (ploc (mynum)))
	  {
	    bprintf ("&+WS&+wn&+Wo&+ww&* flurries are falling gently from the sky.\n");
   	  }
        else
	  {
	    bprintf ("A gentle &+Crain&* is falling, making the ground soft and muddy.\n");
	  }
        break;
      case STORMY:
        if (cold_weather (ploc (mynum)))
	  {
	    bprintf ("You shiver as you trudge on through the &+Wblizzard&*.\n");
	  }
        else
	  {
	    bprintf ("There are &+Lhuge, dark storm clouds&* in the sky above.\n");
	  }
        break;
      default:;
    }
}

char *short_weather ()
{
  int c = cold_weather (ploc (mynum));
  
  switch (the_world->w_weather)
    {
      case RAINY:
        return (c ? "It is &+Wsnowing&* and" : "It is &+Craining&* and");
        break;
      case STORMY:
        return (c ? "You are in a &+Wblizzard&* and the weather is" :
 	      "It is storming and the weather is");
        break;
      case SUNNY:
        return ("The sky is &+Wclear&* and the weather is");
        break;
      case CLOUDY:
        return ("The &+Csky&* is cloudy and the weather is");
        break;
      case CLEARING:
        return ("The &+Csky&* is beginning to clear and the weather is");
        break;
    }
  return("The weather just IS.");
}

char *wthr_type  (int type)
{
  static char b[20];
  
  switch (type)
    {
      case 0:
        sprintf (b, "&+Ysunny&*");
        break;
      case 1:
        sprintf (b, "&+Crainy&*");
        break;
      case 2:
        sprintf (b, "&+Ccloudy&*");
        break;
      case 3:
        sprintf (b, "&+Wclearing&*");
        break;
      case 4:
        sprintf (b, "&+Lstormy&*");
        break;
    }
  return (b);
}

static Boolean cold_weather (int loc)
{
  if ((xlflags (loc) & LFL_T_MASK) == LFL_COLD)
    {
      return (True);
    }
  /* if REALTIME and less than 4 deg celsius, it is cold */
  if (ltstflg (loc, LFL_REALTIME) && calendar.temp < 4)
    {
      return (True);
    }
  return (False);
}

char *real_light_state (int loc)
{
  static char b[15];
  
  if (is_real_dark ())
    {
      sprintf (b, "&+LNightTime&*");
    }
  else
    {
      sprintf (b, "&+CDayLight&*");
    }
  return b;
}

char *real_temp_state (int loc)
{
  static char b[15];

  compute_environment ();
  if ((xlflags (loc) & LFL_T_MASK) == LFL_COLD)
    {
      sprintf (b, "&+cVery Cold&*");
    }
  else if ((xlflags (loc) & LFL_T_MASK) == LFL_HOT)
    {
      sprintf (b, "&+RVery Hot&*");
    }
  else
    {
      if (calendar.temp > 30)
	{
	  sprintf (b, "&+RVery Hot&*");
	}
      else if (calendar.temp > 24)
	{
	  sprintf (b, "&+RHot&*");
	}
      else if (calendar.temp > 18)
	{
	  sprintf (b, "&+yWarm&*");
	}
      else if (calendar.temp > 10)
	{
	  sprintf (b, "&+CCool&*");
	}
      else if (calendar.temp > 5)
	{
	  sprintf (b, "&+CCold&*");
	}
      else
	{
	  sprintf (b, "&+cVery Cold&*");
	}
    }
  return b;
}
