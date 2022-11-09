#ifndef __CALENDAR_H__ 
#define __CALENDAR_H__ 



#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif


/* calendar.c */
void init_calendar P_((void));
void update_calendar P_((void));
void compute_environment P_((void));
void calendarcom P_((void));
void broad_realtime P_((char *txt));
void weather P_((void));
char test_temp P_((void));
Boolean is_real_dark P_((void));
void show_weather P_((void));
char *short_weather P_((void));
char *real_light_state(int loc);
char *wthr_type (int type);
char *real_temp_state(int loc);
#undef P_


#define CALENDAR_COUNTDOWN 120

/* time periods within a day */

#define MORNING      0
#define DAY          1
#define AFTERNOON    2
#define EVENING      3
#define NIGHT        4


/* how many days in a month? (one season = three months) */

#define MONTH_DAYCOUNT 13

/* season periods */

#define EARLY_SEASON 0
#define MID_SEASON   1
#define LATE_SEASON  2

/* seasons */

#define SPRING       0
#define SUMMER       1
#define AUTUMN       2
#define WINTER       3
#define FALL         AUTUMN

#define WINTER_E_SEASON    10
#define WINTER_M_SEASON    4
#define WINTER_L_SEASON    10
#define SPRING_E_SEASON    17
#define SPRING_M_SEASON    20
#define SPRING_L_SEASON    22
#define SUMMER_E_SEASON    25
#define SUMMER_M_SEASON    29
#define SUMMER_L_SEASON    25
#define AUTUMN_E_SEASON    23
#define AUTUMN_M_SEASON    22
#define AUTUMN_L_SEASON    17

#define T_NIGHT     -= 8
#define T_DAY       += 1
#define T_AFTERNOON += 4
#define T_MORNING   -= 4

#define T_RAIN      -= 4

#endif /* Add nothing past this line... */
