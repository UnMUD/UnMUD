/*
 * Weather Routines.
 * Current weather is defined by the value of the_world->w_weather.
 * States are:
 *	0 Sunny   1 Rain   2 Stormy   3 Snowing   4 Blizzard   5 Hailing
 */
typedef enum { sunny, rainy, stormy, snowing, blizzard, hailing } weather_type;

void set_weather(weather_type new_setting);
void longwthr(void);
char *wthr_type(weather_type type);
void show_weather(void);





