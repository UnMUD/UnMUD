10/7/96		Marty		4921749@ibk.fnt.hvu.nl
-----------------------------------------------------------------
Removed last remainders of database code, ie exec.c and
 condition.c are almost completely down the drain...

9/25/96		Marty		4921749@ibk.fntt.hvu.nl
--------------------------------------------------------------
added 4 lines of code, but fixed an crash bug with it :)
  an aliased player killing chicken wont do any harm anymore.

9/18/96		Marty		4921749@ibk.fnt.hvu.nl
--------------------------------------------------------------
Refinded hate, removed the crash bug. And while i was at it
I removed somewarning messages during compile.

9/04/96  	Marty		4921749@ibk.fnt.hvu.nl
--------------------------------------------------------------
Added vis check to where, fixed percent bug in say and tell

2/25/96		Dragorn		dragorn@96mowry.bruske.alma.edu
--------------------------------------------------------------
Added visibility-chooser on logins. 

2/17/96		Rhadamanthus	logan@96mowry.bruske.alma.edu
--------------------------------------------------------------
Fixed mailer where you can only delete/undelete positive message numbers.


2/13/96		Rex		kmowry@96mowry.bruske.alma.edu
--------------------------------------------------------------
added jail code by Freakster so that bad mortals can be dealt with.

2/12/96		Rex		kmowry@96mowry.bruske.alma.edu
--------------------------------------------------------------
Added the PitReset flag so that items flagged with this will be reset
to their original location with a value of 0 when pitted.  This way more
people can use them.

01/20/96	Dragorn		dragorn@96mowry.bruske.alma.edu
-----------------------------------------------------
Installed rMail 1.0 mailer. Based on iDIRT mailer.
 Cleaned up routines
 Remote send (info gateway)
 Neater interface
 Commands from editing (*<command)

01/08/96	Dragorn		dragorn@96mowry.bruske.alma.edu
-----------------------------------------------------
Added Socinfo and Killsoc commands.
Fixed percent sign bugs.
Added level-up congratulations code
Fixed numerous quest bugs, mainly references to mynum as
 opposed to attacker.
Linewrapping now in immortal channels, gossip
Immortal channels, gossip now are numbered. 0 is gossip, 1 wiz, 2 awiz
 and soforth.
You can no longer become a banned player

12/10/95	Dragorn		dragorn@96mowry.bruske.alma.edu
-----------------------------------------------------
Fixed Dragon bug (player score would get sent to millions)
Fixed Talon, Chicken bugs - same prob as dragon, it referenced
MyNum, which could be invalid. Changed to attacker.
Fixed infocom to dissalow anything longer then 15 chars for a file
name - it would crash if you made it too long.

10/24/95	rex		96mowry@matt.alma.edu
-----------------------------------------------------
You may now move while aliased :)  Thanx Steven!

10/1/95		rex		96mowry@matt.alma.edu
-----------------------------------------------------
Fixed bug that allowed a player carrying any object to go to sea.
restored and fixed the uaf file that got fried somehow late last
night.  Everything seems to be fine now, tho I don't know what caused
the problem

9/24/95         dragorn	        dragorn@96mowry
----------------------------------------
Added hamlets linewrap code, tho it's not all implemented
yet.
made puff a little more mellow. (LOT more mellow.)
added Delayed reboots. (delay command toggles, reboots next reset)
added delay to globalcom.
NegRegen rooms work now.

9/21/95		Rex		96mowry@matt.alma.edu
------------------------------------------------------
wow it's been forever since I've written here.  I guess I haven't done
that much that was exciting.  I've been more concerned with moving and
getting my machine back up on the net.
One thing I fixed today was a bug that kept the game thinking you
were in mail if you lost your connection while you were in it.
The only things you couldn't do were the commands that you are not allowed
to do from within mail.

9/04/95         Dragorn      dragorn@aslan
----------------------------------------
Puff now talks to people and shouts with people's names.
(yes, you may now kill me for releasing The Annoyance. *grin*)

9/03/95         Dragorn      dragorn@aslan
----------------------------------------
Puff will now stuff it if you noshout and mute her.
noshout takes off her shouts, mute mutes her (well, thats kinda obvious. 
;) )

9/03/95         Dragorn      dragorn@aslan
----------------------------------------
made puff more interesting, now she has actions and shouts. ;)

9/02/95         Dragorn      dragorn@aslan
----------------------------------------
Added multi-level WarnZap.

9/02/95		Dragorn	     dragorn@aslan
----------------------------------------
Fixed (I think) all instances of pmaxstr(mynum)/pstr(mynum) which
died horribly with attempt to divide by 0 if the players str was such.
Thanks to gumby for helping me find what was actually crashing it!

9/01/95		Dragorn	     dragorn@aslan
----------------------------------------
Put in ghosts. Origional idea by Gumby, much work from me, Katie, Justin.


8/13/95         Rhadamanthus logan@aslan
----------------------------------------
Put in a handy little function called get_yn.  For functions where you want
to verify something (like crash, for example), you'd do it like this:
void chrashcom(int yn)
{
  if(yn == YN_INIT) { get_yn(crashcom, "Are you sure you want to crash",
			ASSUME_NO); return; }
  if(yn == YN_NO) return;
  <crash stuff here>
}
(;

8/13/95		Rex	96mowry@matt.alma.edu
---------------------------------------------
Wrote some more code for the new quests.  Fixed the ledge zone to be fully
operational.  I figure it's time people figure out that it exists. ;)

8/12/95		Rex	96mowry@matt.alma.edu
---------------------------------------------
Added in several new quests, although the code for them is not yet complete.

8/11/95         Rhadamanthus logan@aslan
----------------------------------------
Fixed the minor shout problem (a mess of parentheses in shout_test, basically).
Since misspellings irk me, I changed OFL_UNLOCATABLE and the oflag name
"Unlocatable" to OFL_UNLOCATEABLE and "Unlocateable" respectively.  All code
was changed to reflect this as well.  Got bored and successfully de-Walled
generate.c and flags.c.  flags.c was easy, generate.c wasn't. *grin*

8/7/95		&+GBelegur&* belegur@aslan.hollyfeld.org
Debugged, detypoed and recoloured abyss, ancient and blizzard
zones.

8/6/95		Rex	96mowry@matt.alma.edu
---------------------------------------------
icy dagger will now melt in hell.

8/4/95		Rex	96mowry@matt.alma.edu
----------------------------------------------
Added an autowiz command so that people can give emeritis to themselves.
Remember you'll probably want to remove this ;)

8/5/95		Dragorn Dragorn@aslan.hollyfeld.org
---------------------------------------------
2 new wizard toys. Warnzap and puntzap.

1.8.95		Rex	96mowry@matt.alma.edu
---------------------------------------------
Reinstalled the boat oflag.
Added the unlocatable oflag.
(Ok so I spelled it wrong, sue me ;)

30.7.95		Rex	96mowry@matt.alma.edu
---------------------------------------------
Updated shoutcom and shout_test to disallow shouting to or from a soundproof
room.
Changed SorcerorsTower quest to InvisWand quest, like it was under AberIV>
The object of the quest has been changed, and the crown is no longer the
quest object.
Changed FindGrail so that you no longer have to put the cup for the quest.
You get credit when you pick it up.


27/7/95		Rex	96mowry@matt.alma.edu
---------------------------------------------
Inserted an update of Thrace's to fix problems with the Makefile.
He also updated sun.h in /include/machine.  Changed newstyle to compact.
It's hardly new anymore ;)

24/7/95	Vagnerr		pjw@ukc.ac.uk
--------------------------------------
Dump now works with the new uaf_rand format, so Dump now displays
sensible information, there is also an extra flag -f if you use it
you get the p-flags, otherwise you get e-mail addresses.

23/7/95	Vagnerr		pjw@ukc.ac.uk
--------------------------------------
Fixed lockcom and unlockcom so that you can unlock doors without
being informed they are already unlocked and so you cannot lock 
a door that is already locked. (objsys.c) Working on Dump.c so it 
works with the Gnu Database Manager (in testing stage)

21/7/95		Rex	96mowry@matt.alma.edu
---------------------------------------------
Updated showplayer to show players' email addresses.  ooh ahh ;)
Changed users to show players' numeric levels.
Changed prompt in becom and change.c for password to cprompt

20/7/1995	Rex	96mowry@matt.alma.edu
---------------------------------------------
Installed Twizzly's version of trace.

18/7/1995	Rex	96mowry@matt.alma.edu
----------------------------------------------
Ran the zones through ispell to get rid of most of the errors. Found a 
lot of junk that was bad in frobozz.zone, so I took a few liberties in
fixing them.  I'm sure there's more I've done but i can't remember right
now ;)

16/7/1995  Belegur	belegur@aslan.hollyfeld.org
-------------------------------------------------
Installed the new look help system. If there is enough
demand I will make simular modifications to the wizard 
section. Please let me know if you spot any typos.

7/15/95	   Kender       kender@Hollyfeld.org
-------------------------------------------------
Modified the crash catcher so that it dumps core when
it crashes, for post mortem debugging.  It should put
the core in the data directory.  You can do pm debugging
by doing:
	gdb ../bin/aberd ../data/core
And get a stack trace by typing in where when you get a 
prompt.  Feel free to ping me if you have any gdb questions.

7/12/94    Dragorn	dragorn@aslan.hollyfeld.org
---------------------------------------------
Added code so that if you are idle for more then 3 minutes it puts
[Inactive] in your title. I had free time, couldya guess? ;)		
...
Later that day... ;)
made it so immortals are flagged inactive after 5 mins, mortals 3.
and fixed a bug i caused by not adding a %%s in to sprintf

7/12/95    Rhadamanthus loganh@freenet.hut.fi
---------------------------------------------
Ack, this stuff is what I did long ago and neglected to write down. ):
All I can remember at this point is that I put in wearall (wearcom now
calls a function called trywear for you coders out there), and I fixed
empty by having it call a dropobj which was recoded so that you can de-
cide whether you want it to use the command line or just drop a specific
object. :P
Ok, now for something that I actually did today... I changed push, pop,
and replace input handler functions to either clear the Distracted flag
if the new handler is get_command, or else set it. (;

7/4/95     Rhadamanthus loganh@freenet.hut.fi
---------------------------------------------
Well, I kinda forgot about this part, but let me think... I fixed ropes 
(at least I hope I did), put strength in your prompt when fighting, made 
mobiles talk during fights, made the tree keep score, and I believe I 
fixed the parentheses problem with the prompt when one goes invis.  
That's all I can remember for now. (;

7/4/95		rex	96mowry@matt.alma.edu
---------------------------------------------
I've been a delinquent on this recently.  ;)  lessee....  I've had to change
several prompts back to cprompts and resolve them differently than they
were before.  I added the disconnect command, which will disconnect a player
(kinda like exo but not so rude).  It's based on a pflag, and given to gods
only.   I was also thinking of adding a smite command.  *grin*
Added in the code for breathing underwater when you have the amulet or
talisman.

7/3/95		rex	96mowry@matt.alma.edu
---------------------------------------------
I have a version of the mud running with Thrace's new gdbm-based user file
now.  It seems to be reliable.

6/27/95		rex	96mowry@matt.alma.edu
---------------------------------------------
Changed all instances of cprompt to prompt.  Consistency seems best, and 
the change seems to have gotten rid of several multiple prompt problems,
as well as bringing up the player's prompt directly after exiting mail, 
where it would hang before until the person hit return.

6/26/95		kender	kender@hollyfeld.org
--------------------------------------------
Fixed leveling crash/weirdness bug

Put in reboot-on-crash-bug code


6/25/95		kender	kender@hollyfeld.org
--------------------------------------------
Added numbers for the wiz/awiz/dgod/god channels, for lazy people
like myself :)

Fixed sayback bug in bad tells.

6/25/95		kender	kender@hollyfeld.org
--------------------------------------------
the mud now creates a pid.port file instead of a pid file.  This will
enable the simultaneous running of multiple muds on various ports.
Note: this is intended _only_ for test, as the results can be
quite unpredictable when two or more muds access the same uaf file.

6/25/95		rex	96mowry@matt.alma.edu
---------------------------------------------
added cfr, insert a line to give a mortal his strength after being hit with
a spell cast by another, crashed the mud a lot, and generally had a good 
time ;)

6-24-95		&+GDragorn&* dragorn@bitsy.hollyfeld.org
---------------------------------------------
Added mithdan zone...

6/24/95		&+GBelegur&* rt2@ukc.ac.uk
---------------------------------------------
Colourised the help menus.

6/23/95		Dragorn dragorn@bitsy.hollyfeld.org
---------------------------------------------
Added more puff sayings (most of 'em written by Gestahl)

6/22/95		rex	96mowry@matt.alma.edu
---------------------------------------------
Added newstyle and made a few minor code fixes.
Fixed title so only %%s can go into it - no more %%n, etc
fixed percent doubling in the wizline

6/21/95		rex	96mowry@matt.alma.edu
---------------------------------------------
put in the fixes mailed to me by Watz after he ran the mud through
purify.

6/21/95		Dragorn (dragorn@bitsy.hollyfeld.org)
typos fixed by Rex ;)
----------------------------------------------
Improved frob display once again. :) Now it has all the levels except
god and master (do we <really> need them in? :) ) in about 1/2 a 
screen. 

6/20/95		Dragorn (dragorn@bitsy.hollyfeld.org)
-----------------------------------------------
Fixed demo levels in frob so now they make more sense and have
the right level number in it... Only changed bprintf's so this
should have no possible errors. :)
Grr damn typos. :)   (editor's note:  what typos? ;)
--Later same day :)
Added mortal levels, str, score to frob. Should be complete now

6/19/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Added an email address field into the uaf file.  Now when a player creates
his character, it will ask him to enter an email address.  That address
can be changed at any time by typing "change email <newaddress>".  Only the
player in question may change his or her email address.  Players can find
registered email addresses by typing "email <player>".  Changed Dump.c to
include the email address, though it is encoded.  Doing a Dump before failed
miserably.

6/17/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Installed level-restricting pflags, so that players of lower levels cannot
enter places marked by higher level players as being for higher level
players only. Also made a few cosmetic changes to frob.  When frobbing,
the game gives you a few sample commonly-used frobs and now saves the
target after the frob so that forcing them to save is no longer necessary.

6/16/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
installed Illusion's updated mailer.  I notice that putting %%s's into
the body of the message repeatedly will still crash the mud.  I need
to figure out why and how to stop it.  I will include all docs from
the mailer since it is newly installed so that all gods can configure
it for their own muds.

6/15/95		belegur		(rt2@ukc.ac.uk)
-----------------------------------------------
Modified mud.c so that invis immortals no longer have a problem with 
an everlengthening prompt.

4/24/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Help files are finished but I"m sure that some of them still don't
work.  I need to figure out why some of the help files weren't being
read correctly.

4/23/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
I've spent the past few days redoing the help files, which is taking some
time.  I've also included a powers.zone so that gods only have to edit it
instead of building their own if they so desire.

4/16/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
added a qdone command

4/15/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
added color (bright white) to all room names

4/11/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Made autoexits display the exits on one line. 
Added the Maze lflag so that wizards can't help mortals cheat their 
way through mazes.

4/10/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Updated the syslog system so that it will accept multiple arguments with
a | between them, so you can do something like syslog rex | entry and only
entries for the entry of rex into the game will show up.

4/8/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
First let me begin by saying I met Moses today.  (Not that this has anything
to do with dyrt 1.2, but hey, I think it's cool. ;)  Today was a day of
minor changes, prettying things up, changing eatcom to give the devour message
before the object is destroyed so that cloned objects aren't destroyed before
they are eaten ;) stuff like that.

4/6/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Sent out about 7 bug fixes to dyrt users.  Spent time fixing those
bugs that I got information about (party didn't work, couldn't pray
in wasteland, give fan viola didn't give her the fan, the rock in
oaktree was not working properly, killing the dragon with the 
dagger set your level to one, score was one round slow, and setting
idle and on times to zero and reseting isawiz in xmain_reboot.
I've been working on a new score format that i think is kinda kewl.

4/3/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
reboot now sets idle and plogged_on to 0.  I know it's kind of a clumsy
way to do it, but the only other alternative is to save those times out
to the file, and I'm not sure it's worth all that at least at this point.
Someday when I'm actually bored I might be tempted to do it, but for now,
everyone gets a fresh 0 start upon reboot.  isawiz is also set upon reboot
now so that people can unveil after doing a reboot.  (Before, people in
prived could not unveil after a reboot without quitting and logging back
on, because isawiz is set upon login and was not set at any other time
until today).  So you gods who are messing with unveil stuff while setting
up your muds lucked out.

4/2/95 		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Every time a person levels they will do a saveall.  (Tho they won't know it :))
I installed clone.c from Feldene which apparently has some bug fixes in 
destruct that I was missing.  I have the solaris patch from Watz.  Thrace
and I will be trying to port dyrt to Solaris in the near future.  *bleh*.

3/31/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Mortals can now saveall.  I figured it can't possibly hurt anything if
everyone has access to the command.  Healall will do a saveall as well
from now on so that you don't need to type both commands.

3/27/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
I fixed fight.c so that after a player gets hit, their new str is displayed
to them rather than their old str.  Being informed that your str is higher
than it actually is can be rather confusing and can lead to unnecessary death.

3/26/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
The only way mortals can kill other mortals now is if tournament mode is on.
The game catches the attack and calls peace, warns the person that their
behavior is not acceptable, and calls war again.  The catch is instantaneous.
It may bother other players if they happen to try to kill someone at that 
exact second, but they will be able to attack next round.

3/24/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Added Watz's bug fix (with a few alterations of my own) into the code to
stop whatever was causing kick_out_yn etc, to not work correctly and
form two copies of the player.

3/4/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Added busy, away, autoexits, made inventory look inside of containers,
equipment command, and some other quick yet worthwhile things.  Fixed
converse so that it will kill the converse if the person you are conversing 
with quits or loses connection.  Also added NoSlain and all that good stuff.

3/2/95 		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
whipped together a laston command so that mortals can check when another
person was last on.

2/29/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Added a High Archwizard Level, a High Demigod level and a Master level.
These all fit between Archwiz and D-God, D-God and God, and Master is
above God for those power-hungry mongers who don't want to be the same
level as the rest of their gods ;)  The level for Master is 99999.
The cutoff for God is 99998.  I wouldn't use level 100000, as there
will be problems.

2/28/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Added an emeriti level and a high wizard level, which I am calling Senior
Wizard.  The name can be easily changed.

2/27/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Thanx to Kender, the wizlist <argument> command now works so that people
can check who is in a certain wiz level or whether or not a specific person
is a wizard or power on your mud.

2/26/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
And there was much rejoicing... database.c is HISTORY!

2/24/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
After a few days of desperately working on a first draft of my thesis, I'm
back a bit for the weekend.  I added lockcom, unlockcom, opencom and closecom
to objsys.c to get those things OUT of database.c  I also added a ringcom
for the same purpose.

2/18/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
I decided to add the -Wall flag to the compile to check for all little 
problems and found TONS!!!   So I am going to dewall the code (at least
for my machine) so that it will run more smoothly.  This should really
help cut down memory usage.

2/17/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
I worked a bit on alias today and realized that it is really fubar'ed.
For example, when you alias someone, other players in the room where you
did the alias will see that the mobile is standing there, not you.  Also,
if a reboot occurrs while someone is aliased, the game will create a level
0 player of the name of the mobile, which could potentially cause problems
in the future that I don't know about yet.  I patched the holes that crash
the mud when the aliasor types who, user, or moves in a direction, but I
have a feeling that there are deeper problems in the way alias works, and
you should use it at your own risk, if not disable it all together.

2/16/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Took some more out of database.c and put it in the rest of the code.
Pondered how to fix the ropes.
Looked into alias a bit.

2/15/95 	rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Finally took the time to fix follow.
Implemented the converse command.
Added playcom
I'm planning to:
	fix the rope problem at the treehouse
	rip out database.c
	fix alias and asmortal
	add in an emeriti and high wiz level.
	add in level restricting pflags
I noticed that several zones are corrupted with extra e's being inserted
	between words.  Like "grassy knoll" has become "grassyeknoll". 
	I don't know when or where this happened, but I intend to fix it.
There are also some problems in the zone files with pnames being set to
	things like weapon and food, overriding the real name of the
	object.   Maybe this is part of the clone bug where if you clone
	something called food and try to get it the mud will crash.
I noticed that after a reboot, idle times go haywire.  Why?  I think 
	they need to be reset to 0 upon reboot and they should be OK.

2/14/95		rex	(96mowry@matt.alma.edu)
-----------------------------------------------
Added several zones and their code to the distribution. (author permission)
Added healall, saveall, gossip/nogossip 
Added reboot, acct, diagnose, compare, and punt.

7/28/96		rex	(kmowry@hollyfeld.org)
----------------------------------------------
added Marty's party code

7/31/96
-------
added suggest command
fixed immortal problems with party code

8/1/96
------
plugged become command from inside parties

9/3/96
------
added an entermud file to ../data which is read when a player enters the mud.
It is the Hallway message displayed on AberIVs.

9/3/96		Kender		kender@Hollyfeld.org	
------
	o	Fixed autoexits on 'special transes'
	o	Fixed crash on give to mobiles

9/4/96           Rex
------
added on-line banning (banchar and banhost commands)
added a wizecho command
added a new version of asmortal

9/7/96          Rex
------
added a New Player flag which will show up in the who listing throughout a 
 	player's first login
