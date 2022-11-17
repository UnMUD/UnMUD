for (i = 0, num_me = 0; i < max_players; i++) /* should fix the mult */
      if (EQ(pname(mynum), xname(pname(i))) & /* login bug - checks    */
          is_conn(i) & !is_in_game(i))        /* for players at invis? msg */
            num_me++;
if (num_me > 1)
{
      bprintf("You shouldn't need to be on more than once.\n");
      end_connection(False);
      return;
