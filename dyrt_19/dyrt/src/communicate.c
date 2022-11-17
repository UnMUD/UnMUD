#include "mobiles.h"
#include "verbs.h"
#include "kernel.h"
#include "stdinc.h"
#include "communicate.h"
#include "colors.h"

typedef unsigned char uint8;
char *build_voice_text(char *name,
                       char *verb,
                       char *text,
                       char *quote,
                       color_code_type indent_color,
                       color_code_type speech_color)
{
#define LCL_SCREEN_WIDTH 80
#define LCL_MAX_WORD_LENGTH 20

  uint8 pos;
  uint8 max_chars_per_line;
  uint8 min_chars_per_line;
  uint8 max_num_lines;
  uint8 indent_chars;
  char *start;
  char *end;
  char *buffer;

  /* Hamlet say '...'  Player + Verb + Quote + Text + Quote */
  max_chars_per_line = LCL_SCREEN_WIDTH -
                       strlen(verb) -
                       strlen(name) -
                       strlen(color_strings[speech_color]) -
                       (strlen(quote) * 2);
  min_chars_per_line = max_chars_per_line - LCL_MAX_WORD_LENGTH;
  max_num_lines = (strlen(text) / min_chars_per_line) + 1;

  /* Allocate space for the text, copy in the initial text, and determine the
     indentation needed for this text. */
  buffer = (char *)malloc((LCL_SCREEN_WIDTH * max_num_lines) +
                          strlen(color_strings[indent_color]) +
                          strlen(color_strings[speech_color]));
  if (strlen(name) == 0)
  {
    sprintf(buffer, "%s%s %s%s", color_strings[indent_color],
            verb, color_strings[speech_color], quote);
    indent_chars = 1 + strlen(name) + strlen(verb) + strlen(quote);
  }
  else if (EQ(verb, ":") || EQ(verb, "*") || EQ(verb, ">") || EQ(verb, "#"))
  {
    sprintf(buffer, "%s%s%s %s%s", color_strings[indent_color],
            name, verb, color_strings[speech_color], quote);
    indent_chars = 1 + strlen(name) + strlen(verb) + strlen(quote);
  }
  else if (strlen(verb) != 1 && strlen(name) != 0)
  {
    sprintf(buffer, "%s%s %s %s%s", color_strings[indent_color],
            name, verb, color_strings[speech_color], quote);
    indent_chars = 2 + strlen(name) + strlen(verb) + strlen(quote);
  }
  start = text;
  while (strlen(start) > max_chars_per_line)
  {
    /* Search backwards up to MAX_WORD_LENGTH characters for a space. */
    for (end = start + max_chars_per_line, pos = 0;
         *end != ' ' && pos != LCL_MAX_WORD_LENGTH;
         pos++, end--)
      ;

    /* If we didn't find a space, we need to go ahead and just cut the word
       off at the maximum length */
    if (pos == LCL_MAX_WORD_LENGTH)
    {
      strncat(buffer, start, max_chars_per_line);
      strcat(buffer, "\n");
      start += max_chars_per_line;
    }
    /* Otherwise, cut the word off nicely at the space */
    else
    {
      strncat(buffer, start, end - start);
      strcat(buffer, "\n");
      start = end + 1;
    }
    /* Indent the next line to be under the previous text */
    for (pos = 0; pos < indent_chars; pos++)
      strcat(buffer, " ");
    /* Put in the color text, since a newline has passed */
    strcat(buffer, color_strings[speech_color]);
  }
  /* Tack on the last line of text, the end quote mark, and a newline */
  strcat(buffer, start);
  strcat(buffer, quote);
  strcat(buffer, "\n");
  return buffer; /* Return the created buffer to the caller */
}

void showemail(void)
{
  PERSONA d;
  int b;

  if (brkword() == -1)
  {
    bprintf("Get whose email address?\n");
    return;
  }
  if ((b = fpbns(wordbuf)) != -1 && seeplayer(b))
  {
    if (b < max_players)
    {
      player2pers(&d, NULL, b);
    }
    else
    {
      bprintf("Mobiles do not have email addresses, Silly.\n");
      return;
    }
  }
  else
  {
    bprintf("Who's that?\n");
    return;
  }
  if (b >= max_players)
  {
    bprintf("Mobiles do not have email addresses, Silly.\n");
    return;
  }
  bprintf("&+R%s's email address is:&+W  %s&*\n", d.p_name, d.p_email);
}

void nodcom(void)
{

  if (pl1 == -1)
  {
    bprintf("You nod.\n");
    sillycom("\001s%s\003%s nods.\n\003");
  }
  else
  {
    sillytp(pl1, "nods in agreement with you.");
    bprintf("You nod in agreement with %s.\n", him_or_her(pl1));
  }
}

void rawcom()
{
  char x[MAX_COM_LEN], y[MAX_COM_LEN];

  if (!ptstflg(mynum, PFL_RAW))
  {
    erreval();
    return;
  }
  getreinput(x);
  sprintf(y, "&+R** SYSTEM :&* %s\a\a\n", x);
  send_msg(DEST_ALL, MODE_QUIET | MODE_COLOR, LVL_GOD, LVL_MAX, NOBODY,
           mynum, "[%s RAW'd]\n", pname(mynum));
  broad(y);
}

void namerawcom()
{
  char x[MAX_COM_LEN], y[MAX_COM_LEN];

  if (!ptstflg(mynum, PFL_NAMERAW))
  {
    erreval();
    return;
  }
  getreinput(x);
  sprintf(y, "&+R** SYSTEM <%s> :&* %s\a\a\n", pname(mynum), x);
  broad(y);
}

void wishcom(void)
{
  char x[MAX_COM_LEN];

  if (EMPTY(item1))
  {
    bprintf("Wish for what?\n");
    return;
  }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
  {
    bprintf("I bet you do!  I would too.\n");
    return;
  }
  getreinput(x);
  send_msg(DEST_ALL, MODE_QUIET, LVL_APPREN, LVL_MAX, NOBODY, NOBODY,
           "[&+CWish from \001p%s\003&*]\n[&+C%s&*]\n", pname(mynum), x);
  sillycom("\001s%s\002%s begs and grovels to the powers that be.\n\003");
  if (!ststflg(mynum, SFL_SAYBACK))
  {
    bprintf("Ok\n");
  }
}

void conversecom(void)
{
  if (cur_player->converse.active)
  {
    bprintf("You are already in converse mode. Type ** to exit.\n");
    bprintf("To execute a command as normal start it with * '*nod'.\n");
  }
  else
  {
    cur_player->converse.active = True;
    strcpy(cur_player->converse.old_prompt, cur_player->prompt);
    if ((cur_player->converse.talking_to = pl1) == -1)
    {
      bprintf("&+WYou are now in converse mode. Type ** to exit.&*\n");
      strcpy(cur_player->prompt, ": ");
    }
    else
    {
      bprintf("You are conversing with %s.  Type ** to quit.\n", pname(pl1));
      sprintf(cur_player->prompt, "[%s]: ", pname(pl1));
    }
  }
}

void tellcom(void)
{
  int b;
  char *buffer;

  if (EMPTY(item1))
  {
    bprintf("Tell who?\n");
    return;
  }
#ifdef INTERMUD
  if (strchr(item1, '@'))
  {
    imTell();
    return;
  }
#endif
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
  {
    bprintf("I think you are the one that needs the telling.\n");
    return;
  }
  if ((b = pl1) == -1)
  {
    bprintf("No one with that name is playing.\n");
    return;
  }
  if (b == mynum)
  {
    bprintf("You talk to yourself.\n");
    return;
  }
  if (pl1 >= max_players)
  {
    bprintf("What's the use in talking to a mobile?\n");
    return;
  }
  if (EMPTY(txt2))
  {
    bprintf("What do you want to tell them?\n");
    return;
  }
  if (ststflg(mynum, SFL_GHOST))
  {
    bprintf("As hard as you try, in your ghostly form you cannot communicate.\n");
    return;
  }
  if (plev(mynum) < LVL_DEMI && ploc(pl1) != ploc(mynum) &&
      (ltstflg(ploc(mynum), LFL_SOUNDPROOF) ||
       ltstflg(ploc(pl1), LFL_SOUNDPROOF)))
  {
    bprintf("Your message can't get through the thick walls %s.\n",
            ltstflg(ploc(mynum), LFL_SOUNDPROOF) ? "here" : "there");
    return;
  }
  if (plev(mynum) < plev(pl1) && check_busy(pl1))
  {
    return;
  }
  else if (plev(mynum) >= plev(pl1) && ststflg(pl1, SFL_BUSY) &&
           (players[pl1].aliased || players[pl1].polymorphed != -1))
  {
    bprintf("%s isn't %sself right now..\n", pname(pl1), him_or_her(pl1));
    return;
  }
#ifdef SFL_AWAY
  if (ststflg(pl1, SFL_AWAY))
  {
    bprintf("%s is marked as being away, so %s may not answer immediately.\n",
            pname(pl1), psex(pl1) ? "she" : "he");
  }
  if (ststflg(mynum, SFL_AWAY))
  {
    if (plast_cmd(mynum) < 0)
    {
      bprintf("Note: You're still marked as 'idle'.\n");
    }
    else
    {
      bprintf("Note: You're still marked as 'away'.\n");
    }
  }
#endif
  if (players[pl1].inmailer)
    bprintf("%s is in the mailer, %s may not answer immediately.\n", pname(pl1),
            he_or_she(pl1));
  if (b == (max_players + MOB_CATACOMB_SERAPH))
  {
    if (strchr(txt2, '?'))
    {
      switch (my_random() % 4)
      {
      case 0:
        sprintf(txt2, "Charity");
        break;
      case 1:
        sprintf(txt2, "Faith");
        break;
      case 2:
        sprintf(txt2, "Wisdom");
        break;
      case 3:
        sprintf(txt2, "Courage");
        break;
      }
    }
    else
      sprintf(txt2, "A blessing be upon your house.");
  }
  if (ststflg(mynum, SFL_SAYBACK))
  {
    buffer = build_voice_text("You tell",
                              pname(b),
                              txt2,
                              "'",
                              COLOR_NONE,
                              COLOR_HILIGHT_CYAN);
    bprintf("%s", buffer);
  }
  buffer = build_voice_text(see_name(b, mynum),
                            "tells you",
                            txt2,
                            "'",
                            COLOR_NONE,
                            COLOR_HILIGHT_CYAN);
  sendf(b, "%s", buffer);
  if (!ststflg(mynum, SFL_SAYBACK))
    bprintf("Ok.\n");
}

void saycom(void)
{
  char foo[MAX_COM_LEN];
  char *buffer;

  getreinput(foo);
  if (EMPTY(txt1))
  {
    bprintf("What do you want to say?\n");
    return;
  }
  if (ststflg(mynum, SFL_GHOST))
  {
    bprintf("As hard as you try, in your ghostly form you cannot communicate.\n");
    return;
  }
  getreinput(foo);
  if (ststflg(mynum, SFL_SAYBACK))
  {
    buffer = build_voice_text("",
                              "You say",
                              foo,
                              "'",
                              COLOR_NONE,
                              COLOR_HILIGHT_CYAN);
    bprintf("%s", buffer);
  }
  else
  {
    bprintf("Ok.\n");
  }
  buffer = build_voice_text(pname(mynum),
                            "says",
                            foo,
                            "'",
                            COLOR_NONE,
                            COLOR_HILIGHT_CYAN);
  send_msg(ploc(mynum), MODE_NODEAF, LVL_MIN, LVL_MAX, mynum, NOBODY, "%s", buffer);
}

extern char *shout_test(int player, int sender, char *text)
{
  static char buff[MAX_COM_LEN];

  if ((player == sender) || (plev(player) >= LVL_APPREN && ststflg(player, SFL_NOSHOUT)) || (plev(player) < LVL_APPREN && ststflg(player, SFL_DEAF)) || (ltstflg(ploc(player), LFL_SOUNDPROOF) && ploc(sender) != ploc(player)))
    return NULL;

  if (plev(player) >= LVL_APPREN || plev(sender) >= LVL_APPREN || ploc(player) == ploc(sender) || (sender >= max_players))
  {
    sprintf(buff, "%s shouts '&+C%s&*'\n", see_name(player, sender), text);
  }
  else
  {
    sprintf(buff, "A voice shouts '&+C%s&*'\n", text);
  }
  return buff;
}

void shoutcom(void)
{
  char blob[MAX_COM_LEN];

  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
  {
    bprintf("Your shouting is absorbed by the walls.\n");
    return;
  }
  if (plev(mynum) < LVL_APPREN && ststflg(mynum, SFL_NOSHOUT))
  {
    bprintf("I'm sorry, you can't shout anymore.\n");
  }
  else if (ltstflg(ploc(mynum), LFL_SOUNDPROOF) && (plev(mynum) < LVL_DEMI))
  {
    bprintf("The world around you seems to absorb your cries.\n");
    return;
  }
  else if (ststflg(mynum, SFL_GHOST))
  {
    bprintf("As hard as you try, in your ghostly form you cannot communicate.\n");
    return;
  }
  else
  {
    getreinput(blob);
    if (EMPTY(blob))
    {
      bprintf("What do you want to shout?\n");
    }
    else
    {
      send_g_msg(DEST_ALL, shout_test, mynum, blob);
    }
    if (!ststflg(mynum, SFL_SAYBACK))
    {
      bprintf("Ok.\n");
    }
  }
  if (ststflg(mynum, SFL_SAYBACK))
  {
    bprintf("You shout : &+C%s&*\n", blob);
  }
}

void emoteallcom(void)
{
  char input[1024];

  if (EMPTY(item1))
  {
    bprintf("What?\n");
    return;
  }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
  {
    bprintf("Your emotions are probably what got you in here.\n");
    return;
  }
  if (ststflg(mynum, SFL_GHOST))
  {
    bprintf("You find that your ghostly form cannot communicate.\n");
    return;
  }
  if (!ptstflg(mynum, PFL_EMOTE) && (!ltstflg(ploc(mynum), LFL_PARTY)))
  {
    bprintf("You can't do that.\n");
    return;
  }
  if (plev(mynum) < LVL_APPREN)
  {
    bprintf("You are not allowed to do this.\n");
    return;
  }
  getreinput(input);
  if (ststflg(mynum, SFL_SAYBACK))
  {
    bprintf("&+CYour emoteall looked like this --&*\n");
    if (pvis(mynum) > 0)
      bprintf("(%s) %s\n", pname(mynum), input);
    else
      bprintf("%s %s\n", pname(mynum), input);
  }
  else
  {
    bprintf("Ok.\n");
  }
  if (pvis(mynum) > 0)
  {
    send_msg(DEST_ALL, 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
             "\001p(%s)\003 %s\n", pname(mynum), input);
  }
  else
  {
    send_msg(DEST_ALL, 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
             "\001p%s\003 %s\n", pname(mynum), input);
  }
}

void emotetocom(void)
{

  if (EMPTY(item1))
  {
    bprintf("What?\n");
    return;
  }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
  {
    bprintf("Your emotions are probably what got you in here.\n");
    return;
  }
  if (ststflg(mynum, SFL_GHOST))
  {
    bprintf("You find that your ghostly form cannot communicate.\n");
    return;
  }
  if (!ptstflg(mynum, PFL_EMOTE) && (!ltstflg(ploc(mynum), LFL_PARTY)))
  {
    bprintf("You can't do that.\n");
    return;
  }
  if (plev(mynum) < LVL_APPREN)
  {
    bprintf("You are not allowed to do this.\n");
    return;
  }
  if (pl1 < 0)
  {
    bprintf("No one with that name is playing.\n");
    return;
  }
  if (mynum == pl1)
  {
    bprintf("What's the point in that?\n");
    return;
  }
  if (ststflg(mynum, SFL_SAYBACK))
  {
    bprintf("&+CYour emoteto looked like this --&*\n");
    if (pvis(mynum) > 0)
      bprintf("(%s) %s\n", pname(mynum), txt2);
    else
      bprintf("%s %s\n", pname(mynum), txt2);
  }
  else
  {
    bprintf("Ok.\n");
  }
  sendf(pl1, "\001p%s\003 %s\n", pname(mynum), txt2);
}

void emotecom(void)
{
  char input[1024];

  if (EMPTY(item1))
  {
    bprintf("What?\n");
    return;
  }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
  {
    bprintf("Your emotions are probably what got you in here.\n");
    return;
  }
  if (ststflg(mynum, SFL_GHOST))
  {
    bprintf("You find that your ghostly form cannot communicate.\n");
    return;
  }
  if (!ptstflg(mynum, PFL_EMOTE) && (!ltstflg(ploc(mynum), LFL_PARTY)))
  {
    bprintf("You can't do that.\n");
    return;
  }
  getreinput(input);
  if (ststflg(mynum, SFL_SAYBACK))
  {
    bprintf("&+CYour emote looked like this --&*\n");
    if (pvis(mynum) > 0)
      bprintf("(%s) %s\n", pname(mynum), input);
    else
      bprintf("%s %s\n", pname(mynum), input);
  }
  else
  {
    bprintf("Ok.\n");
  }
  send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
           "\001p%s\003 %s\n", pname(mynum), input);
}

void echocom(void)
{
  char input[1024];

  if (!ptstflg(mynum, PFL_ECHO))
  {
    bprintf("You hear echoes.\n");
    return;
  }
  getreinput(input);
  if (ststflg(mynum, SFL_SAYBACK))
  {
    bprintf("You echo : %s\n", input);
  }
  else
  {
    bprintf("Ok.\n");
  }
  send_msg(ploc(mynum), MODE_QUIET | MODE_COLOR, LVL_GOD, LVL_MAX, NOBODY,
           mynum, "[%s has ECHOed : %s]\n", pname(mynum), input);
  sendf(ploc(mynum), "%s\n", input);
}

void echotocom(void)
{

  if (!ptstflg(mynum, PFL_ECHO))
  {
    bprintf("You hear echoes.\n");
    return;
  }
  if (pl1 < 0)
  {
    bprintf("No one with that name is playing.\n");
    return;
  }
  if (mynum == pl1)
  {
    bprintf("What's the point in that?\n");
    return;
  }
  if (ststflg(mynum, SFL_SAYBACK))
  {
    bprintf("You echoto %s : %s\n", pname(pl1), txt2);
  }
  else
  {
    bprintf("Ok.\n");
  }
  sendf(pl1, "%s\n", txt2);
  send_msg(ploc(mynum), MODE_QUIET | MODE_COLOR, LVL_GOD, LVL_MAX, NOBODY,
           mynum, "[%s has ECHOTOed %s : %s]\n", pname(mynum), pname(pl1), txt2);
}

void echoallcom(void)
{
  char input[1024];

  if (!ptstflg(mynum, PFL_ECHOALL))
  {
    bprintf("You hear echoes.\n");
    return;
  }
  getreinput(input);
  if (ststflg(mynum, SFL_SAYBACK))
  {
    bprintf("You echoall : %s\n", input);
  }
  else
  {
    bprintf("Ok.\n");
  }
  sendf(DEST_ALL, "%s\n", input);
  send_msg(DEST_ALL, MODE_QUIET | MODE_COLOR, LVL_GOD, LVL_MAX, NOBODY,
           mynum, "[%s has ECHOALLed]\n", pname(mynum));
}
