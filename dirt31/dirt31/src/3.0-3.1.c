#include "kernel.h"
#include <errno.h>
#include "pflags.h"
#include "sflags.h"
#include "levels.h"


/* Convert a DIRT-3.0 uaf_rand file to the DIRT-3.1 format.
 * Reads from the standard input, writes to standard output.
 *
 * link with utils.o and add ../include to the list of include dirs.
 *
 * example: gcc -o 30-31 -I../include 3.0-3.1.c utils.c
 */


int convert(FILE *from, FILE *to);


main( int argc, char *argv[] )
{
	FILE *       f;
        int          errors = 0;


	if (argc == 1) {
		errors += convert(stdin, stdout);
	}
	else 
        while (*++argv != NULL) {
		if ( (f = fopen(*argv, "r")) == NULL) {
			perror("Convert");
                        ++errors;
		}
		else {
			errors += convert(f, stdout);
			pclose(f);
		}
	}

	fprintf(stderr, "Errors: %d\n", errors);

	return errors;
}


/* The old PERSONA format (make changes if you have modified yours)
 */
typedef struct {
    char        p_name[PNAME_LEN+1];
    char        p_title[TITLE_LEN+1];
    char        p_passwd[PASSWD_LEN];
    int         p_home;
    int         p_score;
    int         p_strength;
    int         p_damage;
    int         p_armor;
    SFLAGS      p_sflags;
    PFLAGS      p_pflags;
    PFLAGS      p_mask;
    QFLAGS      p_quests;
    int         p_vlevel;
    int         p_level;
    int         p_carry;
    time_t      p_last_on;
    int         p_spare[4];
} OLD_PERSONA;



int convert(FILE *from, FILE *to)
{
	PERSONA      new;
	OLD_PERSONA  old;
	time_t       now = time(NULL);

	/* players now have unique ID's so it's possible to make
	 * stuff in the game that'll be carried by them on reset.
	 */
	long int id = 0x7FFFFFFF;

	while (fread( &old, sizeof(OLD_PERSONA), 1, from) > 0) {
#if 0
		/* Clean up. Remove players who have not been on for
		 * 2 months and score <= 2020
		 */
		if (now - old.p_last_on > 5184000 && old.p_level <= 3
		    && old.p_score <= 2020) {
			fprintf(stderr, "Deleting %s....\n", old.p_name);
			continue;
		}
#endif
		strcpy(new.p_name, old.p_name);
		strcpy(new.p_title, old.p_title);
		strcpy(new.p_passwd, old.p_passwd);

		new.p_home = old.p_home;
		new.p_score = old.p_score;
		new.p_strength = old.p_strength;
		new.p_damage = old.p_damage;
		new.p_armor = old.p_armor;
		new.p_sflags = old.p_sflags;
		new.p_pflags = old.p_pflags;
		new.p_mask = old.p_mask;
		new.p_quests = old.p_quests;

		new.p_vlevel = old.p_vlevel;
		new.p_level = old.p_level;
		new.p_last_on = old.p_last_on;

		new.p_wimpy = 0;   /* wimpy is new, but not (yet) used */

		new.p_id = id--;   /* assign ID */

		clr_bit(&new.p_pflags, 54); /* PFL_AS_MORTAL is obsolete */
		clr_bit(&new.p_mask, 54);

		clr_bit(&new.p_pflags, 42); /* PFL_SET is obsolete */
		clr_bit(&new.p_mask, 42);

		/* awizes and below used to have CRASH, that has changed
		 * to PFL_SHUTDOWN, let only >= demigods have it
		 */
		if (new.p_level < LVL_DEMI) {
			clr_bit(&new.p_pflags, PFL_SHUTDOWN);
			clr_bit(&new.p_mask, PFL_SHUTDOWN);
		}


		/* Give wizards and above CLONE and awizes LOAD/STORE.
		 */
		if (new.p_level > LVL_WIZARD) {
			set_bit(&new.p_pflags, PFL_CLONE);
			set_bit(&new.p_mask, PFL_CLONE);

			/* Just in case someone doesn't have title edit */
			set_bit(&new.p_pflags, PFL_TITLES);
			set_bit(&new.p_mask, PFL_TITLES);

			/* A promotional gift: */
			set_bit(&new.p_mask, PFL_ROOM);
			set_bit(&new.p_pflags, PFL_ROOM);

		}

		if (new.p_level >= LVL_ARCHWIZARD) {
			set_bit(&new.p_pflags, PFL_LD_STORE);
			set_bit(&new.p_mask, PFL_LD_STORE);
		}

		fwrite( &new, sizeof(PERSONA), 1, to);
	}

	return ferror(from) || ferror(to);
}
