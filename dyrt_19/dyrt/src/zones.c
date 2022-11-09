#include "kernel.h"
#include "stdinc.h"

/* Return the zone index for a given zone name, -1 if no such zone found */
int get_zone_by_name(char *name)
{
  int x;
  int w = strlen (name);
  
  for (x = 0; x < numzon; x++)
    {
      if (strncasecmp (name, zname (x), w) == 0)
	return x;
    }
  return -1;
}


/* Return the zone index among the non-permanent zones which matches the given 
 * name _exacely_. If none exists, create one and return that index. */
int get_wizzone_by_name (char *name)
{
  int i;
  
  for (i = num_const_zon; i < numzon; i++)
    {
      if (strcasecmp (name, zname (i)) == 0)
	return i;
    }
  if (numzon == zon_array_len)
    {
      zon_array_len += 20;
      zoname = resize_array (zoname, sizeof (ZONE),
			     numzon, zon_array_len);
    }
  zname (numzon) = COPY (name);
  ztemporary (numzon) = True;
  init_intset (zlocs (numzon), 5);
  init_intset (zmobs (numzon), 3);
  init_intset (zobjs (numzon), 3);
  return numzon++;
}

/* Return the ZONE entry index for the zone that 'loc' is member of.
 * Return -1 if invalid loc number.  */
int loc2zone (int loc)
{
  return exists (loc) ? lzone (loc) : -1;
}

/* Take a 'loc' as argument and return the zone name and the offset within
 * the zone for this loc. If buff = NULL, return the name in a static buffer.*/
int findzone (int loc, char *buff)
{
  int z, x;
  
  if ((z = loc2zone (loc)) == -1)
    {
      strcpy (buff, "TCHAN");
      return 0;
    }
  strcpy (buff, zname (z));
  /* Remove mudlog later if everything seems ok: */
  if (!(x = find_int (loc, zlocs (z))))
    {
      mudlog ("Internal error: location %d was not in its zone %d.",
	      loc, z);
    }
  return x;
}

/* This function is inverse of findzone in that it from a zone index and 
 * offset number finds the loc that it makes up.  0 is returned on error. */
int getlocid (int z, int off)
{
  int a;
  
  if (z == -1 || z >= numzon)
    return 0;
  if (off == 0)
    off = 1;
  else if (off < 0)
    return 0;
  return (a = find_int_number (off - 1, zlocs (z))) == SET_END ? 0 : a;
}

/* This function is inverse of findzone in that it from a zone name and number 
 * finds the loc that it makes up.  0 is returned on error.  */
int getlocnum (char *zname, int off)
{
  return getlocid (get_zone_by_name (zname), off);
}

/* Reset a zone. If r_* != NULL, return the number of locs/objs/mobs that
 * were successfully reset. d_* = number of items destructed.
 *
 * If the zone is a wiz-made zone and the owner hasn't been on for a
 * certain time since 'now', his zone has 'expired' and will be destructed,
 * but if 'now' == NULL, proceed as if it had not expired, (ie reset it),
 * but don't kill any of the temporary (=not stored with STORE) items. */
void reset_zone (int z, time_t * now, int *d_locs, int *d_mobs, int *d_objs,
	    int *r_locs, int *r_mobs, int *r_objs)
{
  PERSONA p;
  Boolean reused;
  int i;
  int xd_locs = 0, xd_mobs = 0, xd_objs = 0, xr_locs = 0, xr_mobs = 0,
  xr_objs = 0;
  
  if (zpermanent (z)
      ||
      (!ztemporary (z) &&
       (fpbns (zname (z)) > -1
	||
	(getuaf (zname (z), &p) &&
	 (now == NULL || *now - p.p_last_on < WIZZONE_EXIST_H * 3600L)))
       )
      )
    {
      for (i = zfirst_mob (z); i != SET_END; i = znext_mob (z))
	do
	  {
	    if (!ptemporary (i))
	      {
		if (reset_mobile (i))
		  xr_mobs++;
		break;
	      }
	    else
	      {
		if (now == NULL)
		  break;
	      }
	  }
	while (destruct_mobile (i, &reused) &&
	       (xd_mobs++, reused));
      for (i = zfirst_obj (z); i != SET_END; i = znext_obj (z))
	do
	  {
	    if (!otemporary (i))
	      {
		if (reset_object (i))
		  xr_objs++;
		break;
	      }
	    else
	      {
		if (now == NULL)
		  break;
	      }
	  }
	while (destruct_object (i, &reused) &&
	       (xd_objs++, reused));
      for (i = zfirst_loc (z); i != SET_END; i = znext_loc (z))
	do
	  {
	    if (!ltemporary (i))
	      {
		if (reset_location (i))
		  xr_locs++;
		break;
	      }
	    else
	      {
		if (now == NULL)
		  break;
	      }
	  }
	while (destruct_location (i, &reused) &&
	       (xd_locs++, reused));
    }
  else
    {
      for (i = zfirst_obj (z); i != SET_END; i = znext_obj (z))
	while (destruct_object (i, &reused) && reused);
      for (i = zfirst_mob (z); i != SET_END; i = znext_mob (z))
	while (destruct_mobile (i, &reused) && reused);
      for (i = zfirst_loc (z); i != SET_END; i = znext_loc (z))
	while (destruct_location (i, &reused) && reused);
      ztemporary (z) = True;
    }
  if (d_locs != NULL)
    *d_locs = xd_locs;		/* locs destroyed */
  if (d_mobs != NULL)
    *d_mobs = xd_mobs;		/* mobs destroyed */
  if (d_objs != NULL)
    *d_objs = xd_objs;		/* objs destroyed */
  if (r_locs != NULL)
    *r_locs = xr_locs;		/* locs reset */
  if (r_mobs != NULL)
    *r_mobs = xr_mobs;		/* mobs reset */
  if (r_objs != NULL)
    *r_objs = xr_objs;		/* objs reset */
}

/* Load a zone from disk into the game.  Name, is its owner, which also 
* identifies the files it's stored on.  Place the number of items found and 
* the number actually loaded in the positions pointed to by the arguments.
 * Return memory allocated as our function value, -1 on error.  */
int load_zone (char *name, int *nlocs, int *nlocs_f, int *nmobs, 
	int *nmobs_f, int *nobjs, int *nobjs_f)
{
  Boolean locs_exist, mobs_exist, objs_exist;
  char filename[128];
  FILE *locfile, *mobfile, *objfile;
  int loc_mem = 0, mob_mem = 0, obj_mem = 0;
  int z;
  
  if (nlocs != NULL)
    *nlocs = 0;
  if (nlocs_f != NULL)
    *nlocs_f = 0;
  if (nmobs != NULL)
    *nmobs = 0;
  if (nmobs_f != NULL)
    *nmobs_f = 0;
  if (nobjs != NULL)
    *nobjs = 0;
  if (nobjs_f != NULL)
    *nobjs_f = 0;
  wiz_loc_filename (filename, name);
  locs_exist = (locfile = fopen (filename, "r")) != NULL;
  wiz_mob_filename (filename, name);
  mobs_exist = (mobfile = fopen (filename, "r")) != NULL;
  wiz_obj_filename (filename, name);
  objs_exist = (objfile = fopen (filename, "r")) != NULL;
  if (!(locs_exist || mobs_exist || objs_exist))
    {
      return 0;
    }
  z = get_wizzone_by_name (name);
  ztemporary (z) = False;
  if (locs_exist)
    {
      loc_mem = load_locations (z, locfile, nlocs, nlocs_f);
      fclose (locfile);
    }
  if (mobs_exist)
    {
      mob_mem = load_mobiles (z, mobfile, nmobs, nmobs_f);
      fclose (mobfile);
    }
  if (objs_exist)
    {
      obj_mem = load_objects (z, objfile, nobjs, nobjs_f);
      fclose (objfile);
    }
  reset_zone (z, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  return (loc_mem < 0 || mob_mem < 0 || obj_mem < 0) ? -1
    : loc_mem + mob_mem + obj_mem;
}

/* From a character name, get the filenames that will contain his zone.*/
static char *wiz_zone_filename (char *buff, char *name, char *contents)
{
  sprintf (buff, "%s/%s.%s", WIZ_ZONES, name, contents);
  return buff;
}

char *wiz_mob_filename (char *buff, char *name)
{
  return wiz_zone_filename (buff, name, "mobiles");
}

char *wiz_loc_filename (char *buff, char *name)
{
  return wiz_zone_filename (buff, name, "locations");
}

char *wiz_obj_filename (char *buff, char *name)
{
  return wiz_zone_filename (buff, name, "objects");
}

/* The ZONES command. */
void zonescom (void)
{
  int a;
  
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  bprintf ("     Zone-Name Rooms  Mobiles  Objects  "
	   "Zone-Name  Rooms  Mobiles  Objects\n\n");
  for (a = 0; a < num_const_zon; a++)
    {
      bprintf ("%14s%5d %7d %7d", zname (a), znumloc (a),
	       znumchars (a), znumobs (a));
      if (a % 2 == 1 || a == num_const_zon - 1)
	bprintf ("\n");
    }
  bprintf ("\nA total of %d permanent zones containing "
	   "%d rooms, %d mobiles, %d objects.\n\n",
	   num_const_zon, num_const_locs, num_const_chars - max_players,
	   num_const_obs);
  if (numzon != num_const_zon)
    {
      int i = 0;
      bprintf ("     Zone-Name Rooms  Mobiles  Objects  "
	       "Zone-Name  Rooms  Mobiles  Objects\n\n");
      for (a = num_const_zon; a < numzon; a++)
	{
	  int locs = znumloc (a);
	  int mobs = znumchars (a);
	  int objs = znumobs (a);
	  if (!ztemporary (a) || locs > 0 || mobs > 0 || objs > 0)
	    {
	      bprintf ("%14s%5d %7d %7d",
		       zname (a), locs, mobs, objs);
	      if (i++ % 2 == 1)
		bprintf ("\n");
	    }
	}
      bprintf ("\nA total of %d Wizard's zones containing "
	       "%d rooms, %d mobiles, %d objects.\n",
	       i,
	       numloc - num_const_locs,
	       numchars - num_const_chars,
	       numobs - num_const_obs);
    }
}
