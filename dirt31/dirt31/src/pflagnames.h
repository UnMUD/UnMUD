/*
** Pflag names
*/
char *Pflags[] = {

  /* Protective: */

    "NoExorcise",	"NoSnoop",	"NoHassle",	"NoAlias",
    "NoZap",		"NoSummon",    	"NoTrace",	"NoSteal",
    "NoMagic",	       	"NoForce",	NULL,           NULL,
    NULL,		NULL,		NULL,		NULL,

  /* Priveliges: */

    "Frob",             "ChScore",      "ChLevel",      "ChMobData",
    NULL,               "Clone",        "Load/Store",   NULL,
    "PflagEdit",	"MaskEdit",     "RoomEdit",     "MflagEdit",
    "ObjectEdit",       "TitleEdit",    NULL,           "UAFInfo",

    "Exorcise",         "Reset",        "Shutdown",     "Snoop",
    "Heal",             "AliasMob",     "AliasPlayers", "Raw",
    "Emote",            "Echo",         NULL,           "Zap",
    "Resurrect",        "ShowUsers",    "Stats",        "Goto",

    NULL,               "SummonObj",    "Weather",      "Lock",
    NULL,               "WreckDoors",   NULL,           "Peace",
    "Syslog",		"StartInvis",	"Trace",	NULL,
    NULL,		NULL,		NULL,		NULL,
    TABLE_END
};
