CHANGE <See below.  You may use abbreviations as long as they are unique>
CHANGE SEX <player>				Change a player's sex
CHANGE DESCRIPTION				See HELP DESCRIPTION
[PTitleEdit]CHANGE TITLE [player/mobile] <newtitle>		See HELP TITLE
[!D]CHANGE PASSWORD				Change your password
[D]CHANGE PASSWORD <player>			Change password on a player
[X]CHANGE NAME [oldname] [newname]			Change mobile/player name
[X PChLevel PFrob]CHANGE LEVEL <player> <newlevel> 		Change level of a player
[X PChLevel !PFrob]CHANGE LEVEL <me> <newlevel>			Change your level
[PHeal !PChMobData]CHANGE STRENGTH <player> <new strength>		Change player's strength
[PHeal PChMobData]CHANGE STRENGTH <player/mobile> <new strength>		Change mobile/player strength
[!PHeal PChMobData]CHANGE STRENGTH <mobile> <new strength>		Change mobile's str
[X PChScore !PFrob]CHANGE SCORE <me> <new score>		Change your score
[X PChScore PFrob]CHANGE SCORE <player> <new score>
[X !A PChMobData]CHANGE DAMAGE [me/mobile] [new damage]
[X !A !PChMobData]CHANGE DAMAGE [me] [new damage]
[A PChMobData]CHANGE DAMAGE [player/mobile] [new damage]
[A !PChMobData]CHANGE DAMAGE [player] [new damage]
[X]   The damage determines how hard a player/mobile hits when/if it hits.
[X !A PChMobData]CHANGE ARMOR [me/mobile] [new value]
[X !A !PChMobData]CHANGE ARMOR [me] [new value]
[A PChMobData]CHANGE ARMOR [player/mobile] [new value]
[X]CHANGE SPEED [mobile] [new speed]    The average number of moves per minute.
[PChMobData]CHANGE AGGRESSION [moblile] <new value>
[PChMobData]   Changes the chance of a mobile attacking a player in the same room.
[X]CHANGE VIS [player/mobile] [vis-level]
[X]   Changes a mobile or player's visibility level. See help VISIBILITY
^
[PMayCode]CODE
[PMayCode]Turns the coding sflag on and off.  Informs other players that you are off
[PMayCode]coding and may not answer their tells right away.
^
COMPACT
Compacts the output to your screen so that you don't get a line of prompt
between each line of output.  Can either be toggled, or turned off by typing
EXPAND.
^
CLASS
Not a command.  A class is a type of object.  The following classes
are defined:

        all      = everything
        clothing = anything that can be worn (including shields)
        containers = anything you can put other objects in
        food     = anything edible
        keys     = anything used as a key
        weapons  = all weapons (you may be surprised)

In addition, modifiers and adjectives are allowed in all commands which deal
with objects.  For example: Suppose there are three robes in the room...

        There is a black robe here.
        There is a scarlet robe here.
        There is a green robe here.

If you want the scarlet robe, you could either say "get scarlet",
or "get robe2" (see also OBJECT)
^
CLEAR
Clears the screen.
See also CLS.
^
CLIMB [direction]
Travel in a given direction.
See also:  GO.
^
[X]CLONE <location/mobile/object> <optional new name>
[X]Create a copy of an existing location, mobile or object.
[X]Examples: "clone puff The Big Red Dragon" or "clone home2 My New Home"
[X](In case a new mobile name contains spaces, its name in the game will be
[X] whatever word comes after the last space).
[X]
[X]The intended way of creating your unique room, mobile or object, is to
[X]clone something close to what you want, and then use the SET and CHANGE
[X]commands to forge it to exactly match your needs.
[X]
[X]All new creations will normally be destructed at RESET unless you have
[X]the power, by invoking the STORE command, to mark it as non-temporary.
[X]
[X]See also DESTRUCT, STORE, RESET       (and ROOM, MOBILE, OBJECT)
[X]^
CLOSE <object>
Usually used on containers and doors.  Allows you to close them.

See also:  LOCK, OPEN.
^
COLOR
Enables ANSI color or monochrome.  Fireballs are red, flowers are colored,
your prompt stands out, etc.  You must have an ANSI compatible terminal
(an IBM PC, for example) to use this.  Color is optional, but monochrome
will only highlight a few things.

See also COLORCODES.
^
COLORCODES

&+RCO&+GLO&+YRC&+BOD&+MES&+C!

For all of you with ansi color screens.  These codes allows you to set the
colors of your says, tells, title...

 & + <color>               for foreground
 & - <color>               for background
 & = <fg color><bg color>  to set both foreground and background
 & *                       resets the colors to standard settings

(Note that you have to concatenate the code string, that is, no space
 between & and + and <color> for instance.)

code   represents      code   represents
---------------------------------------------------
 &+ll      black           &+LL      black,   highlighted
 &+bb      blue            &+BB      blue,    highlighted
 &+cc      cyan            &+CC      cyan,    highlighted
 &+gg      green           &+GG      green,   highlighted
 &+mm      magenta         &+MM      magenta, highlighted
 &+rr      red             &+RR      red,     highlighted
 &+ww      white           &+WW      white,   highlighted
 &+yy      yellow          &+YY      yellow,  highlighted

Example: &#mWHello where # is a '=' will result in the text '&=mWHello&*'.
         &#GHiya   where # is a '+' will result in the text '&+GHiya&*'.
         &#CHi     where # is a '-' will result in the text '&-CHi&*'.
^
CONVERSE
Carry on a conversation with another player.  When in converse mode, all
lines that you type will be sent to that player in the form of tells unless
you type an * in front of the command.
^
COMPARE <item1> <item2>
Compare two items for their armor or damage value.  Costs 500 points
to use.  You must have 2500 points to use it.
^
COUPLES
Lets you see who is married to whom in this land.  
^
CREDITS
Gives credit where credit is due.  This is a list of all of the people
who have helped create and maintain dyrt.

See also:  INFO ZONES.
^
[PWeather]CALENDAR <change>
[PWeather]Lets you change the weather.
[PWeather]^
CLS
Clears your screen.  Also:  CLEAR.
^
CRIPPLE <player/mobile>
Cripples the specified player or mobile if you are lucky.
Cripple can be counteracted with the CURE command.

See also:  CURE.
^
[PCrash]CRASH
[PCrash]Makes the game go BOOM.
[PCrash]^
CURE <player/mobile>
Cures a player or mobile of all of the BLIND, DEAFEN, MUTE, and CRIPPLE
spells.  Can be used on yourself.
^
COMMUNICATION
List of commands related to communication.

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
&+WDetailed help can be obtained on any of the following by typing:&*

	help <&+Ycommand&*>

	SAY 
		Speak to everybody in the same room as you.

	TELL, MAIL, CONVERSE, BEEP, ASK and FLOWERS
		Contact a specific person.

	GOSSIP, NOGOSSIP and SHOUT
		Talk to everybody on the game.

	WISH
		Sends a message to all immortals. Use with extreme caution!

	WRITE, ERASE and MESSAGE
		Use one of the bulletin boards.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
^
COMBAT
List of commands related to combat.

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
&+WDetailed help can be obtained on any of the following by typing:&*

        help <&+Ycommand&*>

	ATTACK and KILL
	    Used to start a fight.

	FLEE
	    Used to get away from a fight that you regret starting.

	DIAGNOSE and LOOK
	    Gives some idea as to how strong a mobile is.

	WIELD and WEAR
	    Used to wield weapons and wear armour.

	SIT and STAND
	    Sitting down speeds healing, but you must stand up to move.

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
^
