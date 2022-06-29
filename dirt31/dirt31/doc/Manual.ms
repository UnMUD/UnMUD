\" AberMUD documentation, a real hodge-podge and mess.
\" Lightly edited and heavily formatted by Rich Salz.
\" Heavily edited and lightly formatted by Brian Preble (rassilon@eddie.mit.edu)
\"
\" Uses the -ms macro package as a lowest-common denominator.
.po 1i
.nr PO 1i
.DS CH "
.ds LF "AberMUD
.ds RF "Page %
\" -ms is kind of stupid; if you have a .IP that's exactly the width of
\" the indent, you lose bigtime.
.de Ip
.IP "\\$1"
.br
..
\" It's also a pain to have section levels and titles on different lines
.de Nh
.NH "\\$1"
\\$2
..
.TL
.UL "A\ b\ e\ r\ M\ U\ D"
.sp
A guide to the system and its use
.AU
Alan Cox
Richard Acott
Jim Finnis
Leon Thrane
Brian Preble <rassilon@eddie.mit.edu>
Rich Salz <rsalz@bbn.com>
.AB
.LP
AberMUD is a multi-user adventure system for up to 16 players, it is
designed to run on almost anything with a processor in it, and has been
successfully run originally on a Honeywell L66 under GCOS 3, as well as
SunOS3.4 (+ SunOS4.0).
Indeed the original system was written in B, and it shows in parts.
.LP
The game is currently a fantasy scenario, but could be changed to any
other idea.
.LP
Note that AberMUD is far from the complete multi-user game system, it's
a simple system that works and was intended to be such, there's huge scope
for improvement, so if you don't like something, then change it:
feel free to frob with the code!
.AE
.NP
\"
.Nh 1 "Idea of the game"
.LP
The idea of the current game is to score 140,000 points and become a
wizard.
Points are scored for dropping treasure into the sacrificial pit
at the temple, and the one at the village church.
Points can also be scored for killing mobiles, and for killing
players \(em a pastime many players like to have a go at.
.LP
The game has a complete combat system, one that considers weapons and
shields, and also allows people to flee from fights.
To complement this the game also has a magic system with several
spells of both informational, combat and utility nature.
.LP
All the scoring points is fine, but there is one important problem
with life in the game, that is dying.
There are two ways of dying in the game
dying through little accidents with puzzles is a non-fatal event that simply
kicks you off the game, while dying in a fight is fatal \(em your
character will be no more, and it's back to zero points.
Most players will
rapidly get the hang of dying; living takes a little longer to master.
\"
.Nh 1 "Commands"
.LP
The game system supports all the basic adventure type commands
like
.I "take the sword" ,
.I "drop the axe" ,
as well as a set of commands needed for a multi-user system.
Some of these commands are:
.Ip WHO
Show the players on the game, of course certain more
powerful beings may not wish to show up on the list.
.Ip SHOUT
Shout a message across the game, normally used for uttering
those nasty death threats to the person who killed you for
the fourteenth time this week.
.Ip TELL
Allows you to tell a player something.
Although really you
you ought only to be able to tell something to someone in the
same room we found the game as a whole works better with
tell as it currently is.
Similarly you could argue that
.I "who"
is unrealistic, but the results of its absence are simply many
.I "Who's playing?"
shouts.
.Ip SAY
Says something to everyone else in the room.
Assuming they haven't been deafened.
.Ip STEAL
For all those moments when some other player or mobile
has something you fancy \(em just don't get caught at it!
.Ip GIVE
If you are feeling generous you can donate goodies to other players
.Ip KILL
Your opportunity to beat mobiles and players to death.
.Ip WIELD
Select something as your weapon to hand, for when those
nasty people decide to brain you.
.Ip SAVE
Saves your characters current score and status.
The things you carry and wear are not saved.
.Ip QUIT
Exits the game, saving the character automatically.
All your belongings will be left where you quit, and should you
re-enter the game you will be carrying nothing and be at a
start position.
\"
.Nh 2 "Commands for the privileged"
.LP
Once a player achieves the status of wizard he/she gets the right to use
a great many more powers, such as
.I "ZAP" ,
which can be used to totally destroy a
person, exorcise to kick players off the game, and a vast range of
miscellaneous wonders like
.I "POSE" ,
.I "DIR" ,
.I "LOC" ,
or being able to directly go to any room.
.LP
The system assumes that wizards in return for having these powers have
a certain amount of responsibility for the way they use them.
We haven't had to turn any wizards back into mortals yet.
\"
.Nh 2 "Commands for the wizards"
\"
.Ip ZAP
Kill user.
.Ip EXORCISE
Exorcise user out of the game (not killing him).
.Ip CRASH
Crash the game kicking out all the users.
.Ip RESET
Reset mobiles and objects (for a new world_file this needs to be done
before any mobiles or objects show up).
.Ip WIZLOCK
Locks the game allowing only wizards to enter.
.Ip WIZUNLOCK
Unlocks the game for mortals.
.Ip STATS
Show stats of player or object.
.Ip DIRECTORY
Show object directory.
.Ip LOCATIONS
Show names of all the locations (format: ZONE NBR.
.Ip GOTO
Goto a specific room. The room format is: ZONE NUMBER (goto home 1).
.Ip IN
Perform a command in specified room (in home 1 look).
.Ip VIS
Become visible.
.Ip INVIS
Become invisible.
.Ip WIZLIST
Show current wizard list.
.Ip RESURRECT
Resurrect an item.
.Ip LOCNODES
Show known zones.
.Ip SHOW
Show info about an object.
.Ip STOP
Stop mobiles.
.Ip START
Start mobiles.
.Ip ALIAS
Alias yourself to some mobile.
.LP
For more commands see help page for wizards (help command in the game). 
\"
.Nh 2 "Commands for the really privileged"
.Ip SHELL
Sends all commands as if they had
.I "TSS"
in front until
.I "**"
is typed; commands beginning
.I "*"
are treated as normal commands.
.Ip TSS
The name of this command is a historical accident to do with
GCOS3, but essentially it passes the current command via
.I "system()" .
.Ip RAW
Allows superusers to make announcements.
Certain very privileged people can send totally raw messages.
.Ip INUM
Returns the internal number of an item, or \-1 if it does not exist.
Mainly used when debugging new parts of the game
.Ip DEBUGMODE
Enter debug mode, this is subject to a
.I "PFLAG"
so it can be used with mortals too.
It gives all the messages passed and information on item numbers along with
the normal game data.
.Ip PATCH
Allows DIRECT editing of the game world file.
Not to be used lightly, it just happens to be handy when debugging at times.
.Ip PFLAGS
Allows editing of a users privileges.
.Ip AWIZLOCK
Lock the game to all but archwizards.
.Ip AWIZUNLOCK
Unlock the game.
.LP
Of course all the other useful commands like
.I "SET"
and
.I "DIR"
are also available.
\"
.Nh 2 "Registering wizards"
.LP
The way we handled wizards within the system is to let them choose
a suitable name and to assign that name to a suitable spare level
in
.I "BOOT/levels" .
Finally
.I "FROB"
the player to the correct level.
The levels for wizards should be in the range 10 through 999 (many wizards!).
\"
.Nh 1 "Installation Guide"
.LP
Edit kernel.h and type: 'make install'. After this you need to set passwords for
two privileged users; root and debugger. For the first time you need to use
reset command to get all the objects in to the game.
.LP
AberMUD is a simple multi-user game system working entirely with files.
While it is slower than the sockets that are
beloved by
.SM
UNIX
.NL
gurus, it is far more portable.
.LP
The system shares game data in a single file about 150K.
It holds all the player data and some of the object data.
Each player inspects this file regularly (using a two-second alarm) and when
changing it, to keep up with the world.
In addition it also includes a message-passing mechanism that allows
messages to be sent to players or rooms or whatever.
.LP
All the user output is buffered via a routine called
.I "bprintf()"
to ensure no smart-aleck can hit CTRL/S while he has the file
locked, and also helps to improve performance.
That's about it really, the rest of the game logic and coding is no
different to a single user game.
.LP
So far as I know the only things that may cause porting problems
across different versions of UNIX is the
.I "flock()"
call and the use of
.I "getpass()" .
The latter needs a trap to check for a null return, and then do
.I "crapup(\"Not a terminal\");"
or something similar.
.LP
The file is locked using
.I "flock()" ,
hence one important restriction \(em the system cannot run across
networked hosts.
This is for one simple reason:
.I "lockf()"
on our Suns was having little accidents
like locking the entire Network Filing System so I had to resort to
.I "flock()" ,
which is both faster and avoids getting the system manager cross.
.LP
By far, the biggest problem is that this code assumes 32-bit
.I "int" 's.
This is really gross, and should be fixed.
Start with the
.I "scanf"
calls in
.I "bootstrap.c"
and the
.I "typedef" 's
in
.I "kernel.h"
and use
.I "lint"
to help you from there, converting items to the
.I "WORD"
type as necessary.
.LP
All screen and keyboard IO must go via the routines supplied and not
via the standard I/O library.
The details of these can be found from examining
.I "key.c"
and
.I "bprintf.c" .
.LP
Certain users may shell out of AberMUD, they are any Arch_Wizard (level>9999)
who is already on the game uid.
No one else is capable of shelling out, save to the bulletin board.
You may wish to improve this.
The uid defined in the configuration as
.I "GUEST"
is not permitted to use the
.I "LOG"
command, and may have a kick off time defined.
\"
.Nh 1 "Extending the system"
.LP
The system is designed to be easy to extend and patch, its whole basis
being to keep it simple but working.
.LP
The current system is far from perfect, it was written originally as a
joke, so don't blame me!
But at least this means there is plenty of scope for messing with the code.
.LP
AberMUD reads
.I "BOOT/bootstrap"
on startup.
This file contains boot data formatted:
.DS
<code>:<filename>
.DE
The codes are:
.DS
.ta \w'O    'u
O	Objects
V	Verbs
L	Locations
M	Messages
Z	Zones
.DE
These files contain the relevant boot data for the system.
The rest of this section describes the individual file formats.
.LP
Note that some of the files are named in the bootfile, and can therefore
be changed at run-time, while others have their names compiled in.
\"
.Nh 2 "Characters"
.LP
Characters are added by adding them to the boot files.
.LP
The amount of damage a monster does is controlled by the routine
.I "damof(monster)"
which returns the amount of damage maximum that creature does.
.LP
The other behaviors of a creature are in
.I "mobile.c"
via the routines
.I "on_look()"
and
.I "on_timing()" .
These also use
.I "chkfight()"
and
.I "consid_move()"
to decide motion and combat.
.LP
The routine
.I "setname(mobile)"
assigns pronouns according
to the mobile's sex, being an it etc., this should be clear when examined.
.LP
Finally change the number
.I "CHARS"
in the
.I "kernel.h"
file.
The maximum number of characters is currently 48 unless the size of
.I "ublock" ,
and the
.I "openworld()"
and
.I "closeworld()"
routines are changed.
\"
.Nh 2 "Objects"
.LP
Objects are added by giving them an entry in the object boot table.
The file starts with the number of objects, then each object, formatted:
.DS
<name> <altname> <maxstates> <value> <flannel> <size> <weight>
Desc 0^
Desc 1^
Desc 2^
Desc 3^
Examine
^
.DE
The things of note here are:
.Ip Maxstates
Each item has a state value that controls its current state
and is set with
.I "setstate(item, val)"
and tested with
.I "state(item)" .
This controls which one of the four descriptions is used.  If the description
to be used is an empty string then no printing is done at all and the item is
deemed to be in an invisible state.  Certain state uses are implied \(em see
below.  Please note that the carat (^) after Examine is on a line by itself.
If you forget this you'll wind up without a carriage return after your examine.
This may be desireable in some circumstances.  For example, a branch might be
described as:

The words "ACME Dog Bait" are stamped on the branch.^

Which you could follow with "It is lit." if this were the case.
.Ip Flannel
If this is 1, the item is scenic, and cannot be taken, it will also
be described before any normal items or and the weather.
.LP
The second table holding objects is the text file
.I "reset.text" .
The
.I "generate"
program reads this file and creates the binary file
.I "reset_data"
that is used by the program (see the
.I "Makefile" ).
The text file holds four lines for each item as follows:
.DS
Starting location value {comment}
Starting state
combat byte:special byte:Bit flags
Starting carried flag
.DE
.LP
There are two byte flags.  The first is used by the combat system to store the
effective power of a weapon, and the second is for used special cases.  At
present, the second byte is used to indicate the effective armor class of armor
or shields.
.LP
Bit flag order is highest left.
.DS
.ta \w'15    'u
15	Is Weapon
14	Is Container
13	Is Lit (state 0 is lit)
12	State 0 if taken
11	Is Key
10	Can Extinguish (state 1 is extinguished)
09	Can Light (state 0 is lit)
08	Can Wear
07	Is Armor
06	Is Food	(normal food)
05	Push toggles state
04	Push sets to state 0
03	Can lock/unlock (2=locked)
02	Can open/close (1=closed 0=open)
01	Item is paired in state with the item number
	which is its num XOR 1
00	Destroyed
.DE
.LP
The carried flag has one of 4 values:
.DS
.ta \w'15    'u
0	In room starting location value
1	Carried by character number in start loc value
2	Worn by character number in start loc value
3	Contained in object whose number is in start loc value
.DE
\"
.Nh 2 "Locations"
.LP
The file starts with the number of rooms, then each room formatted:
.DS
<room number> <exit n> <exit e> <exit s> <exit w> <exit u> <exit d>
location flags (a 16-digit binary number, see lflags.h for information)
short text^
long text
\&^
.DE
If the long text starts with a # it is a death room.
The room number file is a positive code not negative like the exits.
If the exit code is 0, you can't go that way.
Exits 1000+ equate to access only if object n\-1000 is in state 0, then
move to that object's room.
Exit codes greater than 9999 are for special cases.
\"
.Nh 2 "Mobiles"
.LP
The file starts with the number of mobiles, then each mobile formatted:
.DS
name^
<startloc> <stam> <sex> <level>
Here text^
.DE
Mobiles attack data is still stored in
.I "mobile.c"
and damage done in the
.I "damof()"
routine.  In addition, mobiles with levels between -1 and -199 will never
attack, and levels -100 to -299 never move.  Beyond that, the chance to attack
is "level MOD 100".
To have more than 48 mobiles, the system parameter
.I "MAX_MOBS"
must be changed in kernel.h.
\"
.Nh 2 "Verbs"
.LP
This file contains the number of verbs, then the verbs formatted:
.DS
word value
.DE
\"
.Nh 2 "Zones"
.LP
This file contains the zones data, with the number of zones, followed by
zone data formatted:
.DS
Zone_Name Zone_Base
.DE
\"
.Nh 2 "Levels"
.LP
The levels file holds the levels in this form:
.DS
levelnum:femalename:malename
.DE
.Nh 2 "Prompts"
.LP
The format of the
.I "promptlist"
file is explained at the top of that file.
\"
.Nh 2 "The database"
.LP
The database facilities are as yet incomplete but 3 tables are implemented:
.DS
.ta \w'interrupt    'u
event	called to analyze actions
status	called after each action is done
interrupt	called every two seconds
.DE
Each line of the table is formatted:
.DS
VERB,ITEM1,ITEM2,CONDITIONS*6,ACTIONS*4
.DE
where \-1 for the verb means table end, and 0 means any.
ITEM1 can be an object number,
.I "P(player number)" ,
.I "ANY" ,
.I "NONE" ,
.I "OB" ,
 or
.I "PL" .
The same is true for item 2.
.LP
The conditions are formatted:
.DS
condition, data
.DE
The conditions are best understood by looking at the example code, and
at
.I "condition.c" .
One special condition of note is
.I "PAR" ,
which is always true and stores its parameter for use by the actions.
The spare conditions are set to
.I "PAR, 0" .
Spare actions are coded as
.I "NULL" .
.LP
The actions are executed if all the conditions are true, and take their
parameters from
.I "PAR"
values.
Since not all actions are yet implemented 
entirely in database be careful of those big block actions like
.I "ZAP" .
In the ideal final system, all actions will be coded purely in
database code.
To see what actions do, look at
.I "action.c" .
\"
.Nh 2 "Variables"
.LP
Any parameter may be a variable instead of a constant.
The allowed values are:
.DS
.ta \w'MY_CURCH    'u
FL_CURCH	My room number
FL_MYNUM	my player number
FL_OB1	the number of the object in parameter 1
FL_OB2
FL_PL1
FL_PL2	etc;
.DE
The full set are listed in
.I "condact.h" .
.LP
Also 256 user variables are available, G(0) through G(255).
The first 127 are global, while 128 to 255 are user-only.
.LP
An action of 200\-1999 prints message actioncode\-200.
One of 2000+ prints the message actioncode\-2000 without a trailing return.
