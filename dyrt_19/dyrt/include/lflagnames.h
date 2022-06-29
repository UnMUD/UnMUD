#ifndef __LFLAGNAMES_H__ 
#define __LFLAGNAMES_H__ 


/* Light values */
char           *L_Light[] =
{
   "Light",
   "Dark",
   "Real",
   TABLE_END
};

/* Temparature values */
char           *L_Temp[] =
{
   "TempOrdinary",
   "Hot",
   "Cold",
   "TempReal",
   TABLE_END
};

char           *Lflags[] =
{
	NULL,		NULL,		NULL,		NULL,
	"Death",	"CantSummon",	"NoSummon",	"NoQuit",
	"NoSnoop",	"NoMobiles",	"NoMagic",	"Peaceful",
	"SoundProof",	"OnePerson",	"Party",	"Private",
	"OnWater",	"UnderWater",	"Outdoors",	"TempExtreme",
	"NegRegen",	"RealTime",	"Maze",		"WizOnly",
	"AwizOnly",	"DGodOnly",	"GodOnly",	"Jail",
   TABLE_END
};

#endif /* Add nothing past this line... */
