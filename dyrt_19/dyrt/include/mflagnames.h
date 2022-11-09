#ifndef __MFLAGNAMES_H__ 
#define __MFLAGNAMES_H__ 


/* $Id: mflagnames.h,v 1.1 1993/12/18 14:37:14 vpopesc Exp $ */

/* $Log: mflagnames.h,v $
 * Revision 1.1  1993/12/18  14:37:14  vpopesc
 * Initial revision
 * */




char           *Mflags[] =
{
   "CanFireball", "CanMissile", "CanFrost",   "CanShock",
   "NegFireball", "NegMissile", "NegFrost",   "NegShock",
   "NoHeat",      "Thief",      "StealWeapon","StealWorn",
   "PitIt",       "DrainLev",   "DrainScr",    NULL,

   "BarNorth", "BarEast", "BarSouth", "BarWest",
   "BarUp",    "BarDown", "QFood",    "Blind",
   "Deaf",     "Dumb",    "NoGrab",   "GrabHostile",
   "PickStuff","NoSteal", "Block",    "Cross",

   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
   TABLE_END
};

#endif /* Add nothing past this line... */
