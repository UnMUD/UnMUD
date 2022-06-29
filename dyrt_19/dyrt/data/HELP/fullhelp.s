[PSocket]SOCINFO
[PSocket]Shows open sockets by number, user, status, and host.
[PSocket]Use in conjunction with killsoc.
[PSocket]
[PSocket]See Also: KILLSOC
^
SAYBACK
Echoes to you what you have said or done.  Especially handy when the
MUD is lagged.
^
[X!PSummonObj]SUMMON [player/mobile]
[XPsummonObj]SUMMON [player/mobile/item]
[!XPSummonObj]SUMMON [player/item]
[!X!PSummonObj]SUMMON [player]
Summons a player to your location.
[!X]For non-Wizards, this has the unpleasant effect of the player beeing
[!X]summoned dropping everything that's not worn or wielded, so most players
[!X]regard this as a hostile action.
[PSummonObj]You may also summon objects, either by name or object-number.
[X]You can specify several items or players to summon in the same command,
[X]example:   AT death1 SUMMON kantele boddiker steinar

See also MOBILE
^
SAVE
Saves character's current score and status.  The things you carry and
wear are not saved.  Remember: Save early, save often.
^
[X]STORE <player-name>
[X]If not followed by any argument, it means yourself.
[X]This command creates a zone for you, its name will be your characters name.
[X]It is not intended for making zones for mortals to adventure in,
[X]but it lets you build your own personal stuff with much more fun and ease
[X]then the traditional way of bugging your God do it for you.
[X]The STORE command does the following:
[X]
[X]- Marks all mobiles/locations/objects you have created (with CLONE) as non-
[X]  temporary, so that they will be reset on a reset instead of destructed.
[X]
[X]- Sets their reset-values to the exact state the mobiles/locations/objects
[X]  are in when the STORE command is issued. Your zone will be reset on every
[X]  reset to the exact state it was in, the last time you did a STORE.
[X]
[X]- Saves everything on disk, so that you may LOAD your zone at a later time.
[X]  Your zone will remain in the the game even after you quit,
[X]  but will go away if you don't come back within a certain period of time,
[X]  (currently 3 days). It will in any case be loaded again automatically the
[X]  next time you enter the game.     See also LOAD, CLONE, DESTRUCT, RESET
[X]^
SAVEALL
Saves all mortals.

See also:  SAVE.
^
[PObjectEdit]SET [object] [property] [new value]
[PObjectEdit]Sets properties on a object.
[PObjectEdit]These properties expect a number as its new value:
[PObjectEdit]
[PObjectEdit]       BaseValue, Size, Visibility, Damage, Armor, State
[PObjectEdit]
[PObjectEdit]These properties expect a line of text as its new value:
[PObjectEdit]
[PObjectEdit]       Desc0, Desc1, Desc2, Desc3, Name, AltName
[PObjectEdit]
[PObjectEdit]       (Descriptions for states 0-3, name and alternative name)
[PObjectEdit]
[PObjectEdit]These properties expect a truth-value (On/Off or True/False):
[PObjectEdit]
[PObjectEdit]       Weapon,    Container, Lit,   GetFlips,  Key,    Extinguish
[PObjectEdit]       Lightable, Wearable,  Armor, NoGet,     Pushable,
[PObjectEdit]       Lockable,  Openable,  Food,  Destroyed, PushToggle,
[PObjectEdit]
[PObjectEdit]All properties may be abbreviated so long as they are unique.
[PObjectEdit]See also SHOW, OFLAGS, VISIBILITY for more info.
[PObjectEdit]^
SAY [msg] or "[msg] or '[msg]
Says something to everyone  else in the room.  Assuming   they haven't
been deafened.   You may abbreviate this to  a double quote  or single
quote followed immediately by a message.

Example: "Hi folks!  What's up?
^
SCORE
Display your score, strength, and level.
Wizards may also do SCORE <player> to check the score of other players.
^
SIT
Sit  down and rest.   When  resting, you regain strength-points at a
faster rate than normal.  However,  you can't move  while sitting.  To
stand up again, type STAND.
^
STAND
Stand up after sitting or being pushed over.  See SIT and PUSH.
^
STEAL [item] FROM [player]
For all those moments when some other player or mobile has something
you fancy -- just don't get caught at it!

Example: "steal diamond from tom"       See also OBJECT and MOBILE
^
[X]START
[X]Start mobiles.  See STOP.
[X]^
[X]STOP
[X]Stop mobiles.  See START.
[X]^
[X]SETIN [msg]
[X]Sets message printed when entering a room.  See REVIEW.
[X]Put %n anywhere in the string to represent your name.
[X]^
[X]SETSIT <msg>
[X]Sets the message printed when someone enters the room and you are sitting.
[X]See REVIEW.  Put %n anywhere in the string to represent your name.
[X]^
[X]SETSTAND <msg>
[X]Sets message printed when someone enters the room and you are standing.
[X]See REVIEW.  Put %n anywhere in the string to represent your name.
[X]^
[X]SETTRENTER <msg>
[X]Sets message printed when you summon someone into the room.
[X]See REVIEW.  Put %n anywhere in the string to represent the VICTIM's name.
[X]^
[X]SETTRROOM <msg>
[X]Sets the message displayed just as you start to trans someone.
[X]See REVIEW.  Put %n anywhere in the string to represent your name.
[X]^
[X]SETTRVICT <msg>
[X]Sets the message displayed to the person you are transing.
[X]See REVIEW.  Put %n anywhere in the string to represent your  name.
[X]^
[X]SETOUT [msg]
[X]Sets message printed when exiting a room. If you want the direction you left
[X]from to be printed, put %d somewhere in your string. %n is your name which
[X]must go somewhere in the string too.
[X]
[X]Example:
[X]SETOUT %n says 'To the %d, men!' and wanders off in the wrong direction.
[X]^
[X]SETMIN [msg]
[X]Sets message when teleporting into a room.  See REVIEW.
[X]Put %n somewhere in the string to represent your name.
[X]^
[X]SETMOUT [msg]
[X]Sets message when teleporting out of a room.  See REVIEW.
X]^
[X]Put %n somewhere in the string to represent your name.
[X]^
[X]SETVIN [msg]
[X]Sets message when becoming visible.  See REVIEW.
[X]Put %n somewhere in the string to represent your name.
[X]^
[X]SETVOUT [msg]
[X]Sets message when becoming invisible.  See REVIEW.
[X]Put %n somewhere in the string to represent your name.
[X]^
[X]SETQIN <msg>
[X]Sets the message displayed when you enter the game.  See REVIEW.
[X]Put %n somewhere in the string to represent your name.
[X]^
[X]SETQOUT [msg]
[X]Sets message when quitting the game.  See REVIEW.
[X]Put %n somewhere in the string to represent your name.
[X]^
[X]SAVESET
[X]Saves all your setin/out messages and prompt.
[X]^
[X !PUaf]SETSTART <location>
[X !PUaf]Puts you in <location> when you enter the game, or go whenever you
[X !PUaf]type 'GOTO' without any arguments.
[X PUaf]SETSTART <player> <location>
[X PUaf]Sets home for player, to location. Your home is where you enter
[X PUaf]the game. If no playername is given, you are assumed to be it.
[X]If no location is given, your current home will be erased.
[X]Example: "Setstart home14"   will put you in home14 upon game entry.
[X PUaf]Example: "Setstart Shoikana home14"  sets her home to home14.
[X]Example: "Setstart" erases your home (50/50 chance betw. the church/temple)
[X PUaf]"Setstart Shoikana" does the same for her.
[X]^
[X]SHOCK <player/mobile>
[X]Cast an electric shock on someone.
[X]Chance of success is (your level * (5 + N)) where N is the number of
[X]powerstones you are carrying. Damage = level * 2, or level * 3 if
[X]you carry a lodestone.  Works right through armors.   See also MOBILE.
[X]^
[PStats]SHOW [item]
[PStats]Gives detailed information about an item.  Meanings of information
[PStats]shown are as follows:
[PStats]
[PStats]State:          Current state of object (open, closed, full, charged..)
[PStats]Max State:      The maximum state number.
[PStats]Base Value:     The maximum value this object can have.
[PStats]                Actual value is: MIN(#players, 9) / 9 * Base Value
[PStats]Damage:         If this is a weapon, how much damage does it do.
[PStats]                Total damage = random(0, 8 + weapon-damage) if player,
[PStats]                If a mobile has it: random(0, mob-dam + 1/2 * weap-dam)
[PStats]Armor Class:    If this is armor, with what percentage does it reduce
[PStats]                your opponents chance to hit you ? (starting at 57%)
[PStats]Size:           How much space does item take up in pack.
[PStats]                If it's a container, how much it can contain.
[PStats]Properties:     Wearable, weapon, edible, NoGet, etc. (See help OFLAGS)
[PStats]Descriptions:   One-line descriptions for each state.
[PStats]^
SEARCH
See also:  EXAMINE.
^
SHOUT
Sends a message to everyone on the MUD.  Using shout costs 2 hitpoints.
Overuse of shout will often upset other people; some of whom have the ability 
to zap you, so beware... >:>  If you have a conversation to hold try tell
or gossip.
[X]
[X]See also:  NOSHOUT.
^
[G]SHUTDOWN [message]
[G]God command. Kicks everyone off and disables logins. If followed by
[G]a message, it will display that message to everyone who tries to enter.
[G]^
SOUTH
Moves you one room in the southern direction.  SOUTH can be abbreviated "s".

See also:  NORTH, EAST, WEST, UP, DOWN.
^
[PSyslog]SYSLOG
[PSyslog]
[PSyslog]Displays selected portions of the syslog. Without argument, it prints
[PSyslog]the log for the current day. An argument may be one of the following:
[PSyslog]
[PSyslog] a number n - display the last n lines of the syslog
[PSyslog] a string   - display all the lines containing that string
[PSyslog] a pattern  - display all lines matching that pattern
[PSyslog]
[PSyslog] Some examples:
[PSyslog]
[PSyslog]   syslog 10                   display last ten lines of the log
[PSyslog]   syslog snoop                check see who snooped who
[PSyslog]   syslog ENTRY*mit.edu        show logins from any mit.edu machine
[PSyslog]
[PSyslog] Type HELP PATTERN for more information on patterns.
[PSyslog]^
SMASH
See also:  BREAK.
^
[PSnoop]SNOOP [user]
[PSnoop]Anything that goes to the user's screen  will be seen by  you.
[PSnoop]Please try not to spy on people.  It's impolite. You can not snoop
[PSnoop]on two people at once. Wizards can't snoop on mortals in "private"
[PSnoop]rooms.
[PSnoop]^
[X]STATS
[X]Lists various information (strength, idle time, time on et.c.)
[X]for the visible players that are on.   See also WHO and USERS.
[X]
[X]STATS [player/mobile]
[X]Lists detailed information about a player or mobile.
[X]^
SUGGEST
Works like the bug command: puts a suggestion into the syslog for the gods
to read, ponder, and perhaps even add.
^
[PRaw]SYSTEM
[PRaw]Global echo a system message.
[PRaw]^
