PATTERN

Some commands support the *, ? and \ wildcards.

        * = match 0 or more characters
        ? = match exactly one character
        \ = match the next character literally.

examples:

        *               = match anything, this never fails.
        d*k             = match anything beginning with d and ending in k
                                (could be "datebook", "dork", etc..)
        si??            = match any 4-letter word starting with "si".
                                (could be "sigh", "sign", but not "sip".
        Where\?         = matches to the string "Where?"
^
[!PPflagEdit]PFLAGS <ME> <privilege> <true/false>.
[!PPflagEdit]or CHANGE PFLAGS <ME> <privilege> <true/false>.
[PPflagEdit]PFLAGS <player> <privilege> <True/False>
[PPflagEdit]or CHANGE PFLAGS [player] [privilege] <True/False>
Set/check player's privileges.  If the third argument is missing (True/False)
the current status of that privilege will be shown.  A TRUE in the third
argument means "turn pflag on", FALSE means  "turn pflag off".  Privilege
is one of those names that shows up when you do:
>MASK ME (see the help on that command).

Example:

PFLAGS kantele nozap false
PFLAGS me zap true
zap kantele
[!Y]
[!Y]  Available pflags:
[MNoExorcise]NoExorcise:        Protects you from being exorcised.
[MNoSnoop]NoSnoop:              Protects you from being snooped on.
[MNoHassle]NoHassle:            Protects you from being attacked.
[MNoAlias]NoAlias:              Protects you from being aliased into.
[MNoZap]NoZap:                  Protects you from being zapped.
[MNoSummon]NoSummon:            Protects you from being summoned.
[MNoTrace]NoTrace:              Protects you from being traced.
[MNoSteal]NoSteal:              Protects you from being robbed.
[MNoMagic]NoMagic:              Protects you from being blinded et.c.
[MNoForce]NoForce:              Protects you from being forced.
[MFrob & MChScore & MChLevel]\
Frob and ChScore and ChLevel:           You may use the frob command.
[MChScore & MFrob]ChScore and Frob:             You may change score.
[MChScore & !MFrob]ChScore:             You may change your own score.
[MChLevel & MFrob]\
ChLevel and Frob:               You may change other people's level.
[MChMobData]\
ChMobData:              You may change strength, damage and aggression
[MChMobData]                    of mobiles.
[MPflagEdit]PflagEdit:          You may change other people's pflags.
[MMaskEdit]MaskEdit:            You may do anything you want oh mighty god.
[MRoomEdit]RoomEdit:            You may change lflags and exits on locations.
[MMflagEdit]MflagEdit:          You may change mflags of mobiles.
[MObjectEdit]ObjectEdit:        You may change oflags of objects.
[MTitleEdit]TitleEdit:          You may change title and setins permanently.
[MUAFInfo]UAFInfo:              Stats, frob, pflags, et.c work on people who
[MUAFINfo]                      are not in the game at the moment.
[MExorcise]Exorcise:            You may exorcise players from the game.
[MReset]Reset:                  You may reset the game at will.
[MSnoop]Snoop:                  You may snoop on players.
[MHeal]Heal:                    You may heal players.
[MAliasMob]AliasMob:            You may alias mobiles.
[MAliasPlayers]AliasPlayers:    You may alias other players.
[MRaw]Raw:                      You may use RAW and ECHOALL commands.
[MEmote]Emote:                  You may use EMOTE and EMOTETO commands.
[MEcho]Echo:                    You may use ECHO and ECHOTO commands.
[MObjectEdit]ObjectEdit:        You may set an object's state.
[MZap]Zap:                      You may zap other players....
[MResurrect]Resurrect:          You may RESSURECT things...
[MShowUsers]ShowUsers:          Account name or host name shows up...
[MStats]Stats:                  You may use STATS and SHOW commands.
[MGoto]Goto:                    You may use GOTO and IN/AT...
[MSummonObj]SummonObj:          You may summon objects...
[MWeather]Weather:              You may change the weather at will...
[MLock]Lock:                    You may WIZLOCK the game.
[MWreckDoors]WreckDoors:        You may wreck the doors.
[MPeace]Peace:                  You may use PEACE and WAR commands.
[MSyslog]MSyslog:               You may use the SYSLOG command.
[MStartInvis]\
StartInvis:     You will enter the game with your saved visibility level.
[MTrace]Trace:                  You may trace players and items.

Important: You can only use names that shows up on MASK ME command.
[!PPflagEdit]You may only do MASK ME.
^
[PPeace]PEACE
[PPeace]Prevents fighting and stops current fights.  See WAR.
[PPeace]^
[X]POSE <fireball | hamster | sizzle | gestures | crackle>
[X]Or: POSE <1 | 2 | 3 | 4 | 5>
[!X & LMagician]POSE
[X]Abbreviations may be used as long as they are unique, example: POSE f
[LMagician]^
PN
Display the current pronoun values.  Valid pronouns are ME, IT, HIM, HER,
and THEM (THEM is the last character mentioned, regardless of sex).
^
PLAY [item]
Plays a musical instrument.
^
POWERS
Lists the higher powers on the MUD.
^
PRAY
Pray to the gods.  Perhaps they'll listen.  Use WISH when you have
something particular in mind.
^
PROMPT [text]
Changes your prompt.
[X]Use the SAVESET command if you want to save it.
^
[X]PUNT <player/mobile>
[X]Punts the player or mobile across the mud.
[X]^
PUSH [item]
Pushes an item.
^
PUT [item] IN [item]
Puts an item in another item.        See also OBJECT
^
PARTY
Party is a grouping system which allows multiple mortals to share experience
points and assist each other in fights.  Party uses a sharing system which
automatically gives each player a number of shares equal to his or her level,
but a player's shares can be changed by the group leader.  A player can only
be a member of one party at a time.

There are several party commands:
- &+WPCREATE&* : This command will create a new party for you. 
- &+WPLEAVE&*  : Leave the party you are currently in. 
- &+WPADD&*    : Add a player to the party. 
- &+WPLEADER&* : Change leader of the group.
- &+WPSHARE&*  : Change the shares for a player.
- &+WPSAY&*    : Tell all players in the party something.
- &+WPFOLLOW&* : Start following the leader.
- &+WPNAME&*   : Change the name of the party.

See also: pcreate, pleave, padd, pleader, pshare, psay, pfollow, pname
^
PADD <player>
Add a new player to the party. This can only be done by the leader of the
party. To make sure the player wants to join he should first follow the
leader. The amount of shares of the player will be equal to the level of the
player.

See also: party, pleave
^
PLEAVE
Leave a party. If the leader leaves the group will cease to exsist.

See also: party, padd
^
PCREATE
Creates a new party and automatically makes you the leader of that party. If 
you are already in a party this commands will fail. A player can only be 
member of one party at a time.

See also: party
^
PLEADER <player>
Change the leader of the group. Only the leader of the group can do this. 

See also: party
^
PSHARE <player> <new shares>
With this command you can change the amount of shares of a player. This
command is only available to the leader of the party. The default amount of
shares is the same as the level of the player.

See also: party
^
PSAY <message>
Tell everybody in the same group a message. This is an internal
communication channel like chat, only difference is you can't shut it off.

See also: party
^
PFOLLOW
Follow the leader of the party. If you are the leader this command will have
no effect whatsoever.

See also: party
^
PNAME <new name>
The leader of the party can enter a group name.  The name will show up on the
who list.  The name may only consist of one word (16 characters max).  It has
no real value, but it's a nice feature.

See also: party
^
