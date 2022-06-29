MAIL <user>
To send mail, type MAIL followed by the users name.
To read mail, just type MAIL.
Mail has a limit of 100 messages.  Please keep your mailbox under that
limit or the new messages coming in will be automatically deleted.
^
[!PPflagEdit]MASK
[PPflagEdit]MASK <player> <privilege> <true/false>
Show player's mask, i.e. which flags he or she is allowed to change
via the PFLAGS command.
^
MESSAGE <subject>
A bulletin board command for reading posts.  See one for more on this.
^
[X !PMflagEdit]MFLAGS [mobile]
[X !PMflagEdit]Show the Mobile-Flags (which determine the mobiles behaviour)
[X !PMflagEdit]for [mobile]
[PMflagEdit]MFLAGS [mobile] <flag> <new value>
[PMflagEdit]Show or set the Mobile-flags (which determine the mobiles
[PMflagEdit]behaviour) for the specified mobile. New value is True or False.
[PMflagEdit]Examples:
[PMflagEdit]Mflags Wraith
[PMflagEdit]Mflags Flame CanFireball True
[PMflagEdit]Some Mflags are:
[PMflagEdit]CanFireball, CanMissile, CanShock, CanFrost, PickStuff,
[PMflagEdit]NegFireball, NegMissile, NegShock, NegFrost, DrainScr,
[PMflagEdit]Thief, NoSteal, QFood, Cross, NoHeat, Blind,
[PMflagEdit]BarNorth, BarEast, BarSouth, BarWest, BarUp, BarDown  <-- Bars your way
[PMflagEdit]StealWeapon, StealWorn  <- if Thief, may also steal weapons or armor.
[X | PMflagEdit]See also MOBILE
[X | PMflagEdit]^
MOBILE
Not a command.

If a command expects a mobile as argument, it may be given as either one of:
1) <mobile-name>
2) <mobile-name><number-in-room-with-that-name>

Example, if there are 5 orcs in a room, and you want to kill the 2nd, you
could say "kill orc2".  "kill orc1" and "kill orc" means the 1st orc.

[X]3) <number>
[X]
[X]This is the unique mobile-ID number that shows up on the WHERE command.
[X]Examples:  "at 1512 kiss 1512", "heal 1611", "give biscuit to 1779"
^
[X]MOBILES <zone>
[X]Without arguments, Shows all the permanent (non wizard made) mobiles
[X]and their locations.
[X]If a zone name is present, show the mobiles in that zone.
[X]^
[X]MISSILE <player/mobile>
[X]Cast a magic missile on someone.
[X]Chance of success is (your level * (7 + N)) where N is the number of
[X]powerstones you are carrying. Damage = level * 2, or level * 3 if
[X]you carry a talisman. Max. level in these calculations is 5, so this spell
[X]is useful for lower level players.   Armors don't help.  See also MOBILE.
[X]^
[X]MLEFT
[X]Shows what mobiles are left alive.
[X]^
MUTE <player>
Make a player unable to speak.  Can be countered with the CURE spell.

See also:  CURE.
^
MOVEMENT
List of movement related commands.

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
&+WDetailed help can be obtained on any of the following by typing:&*

	help <&+Ycommand&*>

	NORTH, SOUTH, EAST, WEST, UP and DOWN 	
		These commands move you in the direction specified. They may
	      all be abreviated to their respective first letters.

	GO, CLIMB and JUMP
		In some circumstances it may be necessary to more than simply
	      walk in a given direction.

	FOLLOW and LOSE
		Allows you to follow a player, and then stop following them.

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
^
[Pintermud]MUDLIST
[Pintermud]
[Pintermud]Displays a list of muds connected via the intermud.
^
