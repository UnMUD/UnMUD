int     clone_object(int obj, int new_zone, char *new_name);
Boolean destruct_object(int obj, Boolean *index_reused);
int     clone_mobile(int mob, int new_zone, char *new_name);
Boolean destruct_mobile(int mob, Boolean *index_reused);
int     clone_location(int l, int new_zone, char *new_name);
Boolean destruct_location(int l, Boolean *index_reused);
void    clonecom(Boolean do_brkword);
void    destructcom(char *args);
