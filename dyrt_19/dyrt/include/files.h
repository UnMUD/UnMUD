#ifndef __FILES_H__ 
#define __FILES_H__ 


#define DATA_DIR        "../data/"      /* Directory where the files are */
#define ILLEGAL_FILE    "illegal"       /* Illegal character names */
#define LOCAL_HOST	"local_hosts"   /* list of hosts not shown in tcps */
#define EXTERNS		"actions"	/* Hug, kiss, tickle, ... */
#define OBJECTS		"objects"	/* Object names and desc. */
#define GWIZ		"wiz.txt"	/* "Welcome to Wizard" msg. */
#define BOOTSTRAP	"bootstrap"	/* Points to text files */

#define FULLHELP	"HELP/fullhelp"	        /* Detailed command desc. */
#define HELP            "HELP/help1"            /* brief commands menu */

#define CREDITS		"INFO/credits.i"	/* Author credits */
#define COUPLES		"INFO/couples.i"	/* Who's been married? */
#define INFO		"INFO/info.i"		/* Brief information on game */
#define NEWS            "INFO/news.i"           /* latest news */
#define ENTERMUD	"entermud"		/* message enters you into the 
						mud like AberIV's used to. */
#define BULLETIN1	"INFO/bulletin1.i"	/* Bulletin for mortals */
#define BULLETIN2	"INFO/bulletin2.i"	/* Bulletin for Wizards */
#define BULLETIN3	"INFO/bulletin3.i"	/* Bulletin for Arch-Wizards */

#define SETIN_FILE	"setins"	/* Player's travel messages */
#define LOG_FILE	"syslog"        /* The system's log-file */
#define MOTD		"motd"		/* Message of the day */
#define NOLOGIN		"nologin"	/* If exists, no logins */
#define DESC_DIR	"DESC"		/* Character descriptions */
#define MAIL_DIR	"MAIL"		/* Directory to keep mail */
#define UAF_RAND	"uaf_rand"      /* Holds score/privs et.c */
#define PRIVED_FILE     "prived"        /* Priveliged players (gods)*/
#define MONITOR_FILE    "monitor"       /* Players to monitor */
#define PID_FILE        "pid"           /* to tell other muds we're running */
#define WIZ_ZONES       "WIZ_ZONES"     /* wizard's personal creations */
#define ID_COUNTER      WIZ_ZONES"/ID_COUNTER"
#define BAN_CHARS	"banned_chars"	/* list of banned characters */
#define BAN_HOSTS	"banned_hosts"	/* list of banned hosts*/
#define BAN_ACCTS	"banned_accts"  /* list of banned accounts */



#endif /* Add nothing past this line... */
