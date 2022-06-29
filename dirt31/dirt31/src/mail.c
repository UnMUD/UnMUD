#include <unistd.h>
#include "kernel.h"
#include "timing.h"
#include "sflags.h"
#include "sendsys.h"
#include "writer.h"
#include "mobile.h"



static void    mail_handler(void *w, void *ad, int adlen);


/* Send and receive mail.
 */
void mailcom()
{
    PERSONA        p;
    char           file[256];

    int            unlink(char *file);

    if (cur_player->aliased || cur_player->polymorphed != -1) {
	bprintf("Not while aliased.\n");
	return;
    }

    if (brkword() != -1) {

	if (!getuaf(wordbuf, &p)) {
	    bprintf("No such player in system.\n");
	    return;
	}

        start_writer( "End the message with ** on the beginning of a new line",
                       "MAIL>",
                       p.p_name,
                       PNAME_LEN + 1,
                       mail_handler,
                       WR_CMD|'*',
                       200);
        return;

    } else { 

	sprintf(file, MAIL_DIR"/%s", pname(mynum));

	if (access(file,R_OK) == -1) {
	    bprintf("No mail for %s.\n", pname(mynum));
	    sclrflg(mynum, SFL_MAIL);
	} 

	disp_file(file, NULL);

        unlink(file);
        sclrflg(mynum, SFL_MAIL);
    }
}


static void mail_handler(void *w,void *ad, int adlen)
{
     PERSONA p;
     int     y;
     FILE   *f;
     char    b[100];
 
     strcpy(b, MAIL_DIR "/");
     strcat(b, (char *)ad);

     if ((f = fopen(b,"a")) == NULL) {
         progerror(b);
         terminate_writer(w);
         return;
     } else {
	 fprintf(f, "Message from %s at %s\n",
		 pname(mynum), time2ascii(TIME_CURRENT)); 

         while (wgets(b,sizeof(b),w) != NULL) {
             fputs(b, f);
         }

	 fclose(f);

	 if ((y = fpbns((char *)ad)) != -1) {
	     ssetflg(y, SFL_MAIL);
	     sendf(y, "**\a You have received new mail from %s **\n",
		   pname(mynum));
	 }
	 else if (getuaf((char *)ad, &p)) {
	     xsetbit(p.p_sflags, SFL_MAIL);
	     putuaf(&p);
	 } else 
	     mudlog("mail_handler: %s didn't exist");
     }
}   


