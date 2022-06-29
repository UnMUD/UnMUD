#include "kernel.h"
#include "sendsys.h"
#include "weather.h"
#include "pflags.h"
#include "sflags.h"
#include "levels.h"
#include "lflags.h"
#include "locations.h"
#include "bprintf.h"

/*
 * Weather Routines.
 *
 * Current weather is defined by the value of the_world->w_weather.
 * States are:
 *	0 Sunny   1 Rain   2 Stormy   3 Snowing   4 Blizzard   5 Hailing
 */


/* If it's cold, make it snow/blizzard instead of rain/storm.
 */
static int modifwthr(int loc, int n)
{
    return (ltstflg(loc, LFL_COLD)) ? (n == 1 || n == 2 ? n + 2 : n)
                                    : (n == 3 || n == 4 ? n - 2 : n);
}


/* Determine if a player is supposed to receive a weather-message,
 * and if so, return the proper message.
 */
static char *test_func(int plr, int arg, char *msg)
{
  /* We neither use arg nor msg here but they are required by send_g_msg */

    if (!ltstflg(ploc(plr), LFL_OUTDOORS) || ststflg(plr, SFL_QUIET)) {
        return NULL;
    }

    switch (modifwthr(ploc(plr), the_world->w_weather)) {
    case sunny:    return "The sun comes out of the clouds.\n";
    case rainy:    return "It has started to rain.\n";
    case stormy:   return "Dark clouds boil across the sky "
                          "as a heavy storm breaks.\n";
    case snowing:  return "It has started to snow.\n";
    case blizzard: return "You are half blinded by drifting snow, "
                          "as a white, icy blizzard sweeps across\n"
                          "the land.\n";
    case hailing:
      if (plev(plr) >= LVL_WIZARD) {
 	           return "You watch mortals run for cover as "
	                  "golf-ball sized hailstones begin to fall.\n";
      } else {
	           return "You run for cover as golf-ball sized "
	                  "hailstones begin to fall.\n";
      }
    }
}


/* Change the world's weather-setting.
 */
static void adjust_weather(weather_type new_setting)
{
    if (the_world->w_weather != new_setting) {

        the_world->w_weather = new_setting;

        send_g_msg(DEST_ALL, test_func, 0, NULL);
    }
}



/* The sun/rain/etc... commands.
 */
void set_weather(weather_type new_setting)
{
    if (!ptstflg(mynum,PFL_WEATHER)) {
        erreval();
        return;
    }
    adjust_weather(new_setting);
}



/* Change the weather-setting regularly to a random value.
 */
void longwthr()
{
  int a;

  if (randperc() < 85 || randperc() < 95)
    return;

  a = randperc();
  if (a < 10)
      adjust_weather(hailing);
  else if (a < 50)
    adjust_weather(rainy);
  else if (a > 90)
    adjust_weather(stormy);
  else
    adjust_weather(sunny);
  return;
}


/* Return the adress of a static buffer containing "rainy" etc, for
 * a weather type.
 */ 
char *wthr_type(weather_type type)
{
  switch(type) {
  case sunny:    return "Sunny";
  case rainy:    return "Rainy";
  case stormy:   return "Stormy";
  case snowing:  return "Snowing";
  case blizzard: return "Blizzard";
  case hailing:  return "Hailing";
  default:       return "Unknown";
  }
}



/* Give a description of the weather when walking into a room.
 */
void show_weather()
{
    if (!ltstflg(ploc(mynum), LFL_OUTDOORS)) {
        return;
    }

    switch (modifwthr(ploc(mynum), the_world->w_weather)) {
    case rainy:
      if (ploc(mynum) >= LOC_BLIZZARD_STREAM &&
	  ploc(mynum) < LOC_BLIZZARD_S_PASS) {
	  bprintf("It is raining, a gentle mist of rain, which sticks to "
		  "everything around\nyou making it glisten and shine.  "
		  "High in the skies above you is a rainbow.\n");
      } else
          bprintf("It is raining.\n");
      break;
    case stormy:    bprintf("The skies are dark and stormy.\n");  break;
    case snowing:   bprintf("It is snowing.\n");  break;
    case blizzard:  bprintf("A blizzard is howling around you.\n");  break;
    case hailing:   bprintf("It is hailing.\n");
  }
}







