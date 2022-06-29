#ifndef __CONDITION_H__ 
#define __CONDITION_H__ 


Boolean exec_match(int i, int ob, int pl);
Boolean check_conditions(CONDITION cond_arr[]);
void do_actions(ACTION act_arr[]);
int getnext(void);
int getpar(void);
Boolean condeval(CONDITION cond);
Boolean stdaction(ACTION a);

#endif /* Add nothing past this line... */
