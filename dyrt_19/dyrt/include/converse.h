#ifndef __CONVERSE_H__
#define __CONVERSE_H__

/* converse code goes after the struct for the pager */
/*} PAGER;*/

typedef struct _CONVERSE
{
  char old_prompt[PROMPT_LEN + 50];
  int talking_to;
  Boolean active;
} CONVERSE;

#endif /* Add nothing past this line... */
