BINDIR = ../bin/
DATADIR = ../data/
ZONEDIR = $(DATADIR)ZONES
HDIR = ../include/
MUDD    = aberd
GEN     = generate
DUMP    = Dump
PFILTER = pfilter

CC     = gcc
CFLAGS = -DDEBUG -O -g -I$(HDIR)

ZONES = $(ZONEDIR)/xlimbo.zone $(ZONEDIR)/blizzard.zone \
        $(ZONEDIR)/cave.zone $(ZONEDIR)/labyrinth.zone \
        $(ZONEDIR)/forest.zone  $(ZONEDIR)/valley.zone \
        $(ZONEDIR)/moor.zone $(ZONEDIR)/island.zone \
        $(ZONEDIR)/sea.zone $(ZONEDIR)/castle.zone \
        $(ZONEDIR)/tower.zone $(ZONEDIR)/quarry.zone $(ZONEDIR)/home.zone \
        $(ZONEDIR)/icecave.zone $(ZONEDIR)/church.zone \
        $(ZONEDIR)/village.zone $(ZONEDIR)/treehouse.zone \
        $(ZONEDIR)/ledge.zone $(ZONEDIR)/oaktree.zone \
        $(ZONEDIR)/catacomb.zone $(ZONEDIR)/waste.zone \
        $(ZONEDIR)/orchold.zone $(ZONEDIR)/eforest.zone \
        $(ZONEDIR)/frobozz.zone $(ZONEDIR)/ancient.zone \
        $(ZONEDIR)/mountain.zone $(ZONEDIR)/files


MUDDOBJS = actions.o bootstrap.o bprintf.o change.o clone.o commands.o \
           condition.o database.o exec.o fight.o flags.o frob.o \
	   log.o mail.o magic.o \
	   main.o mobile.o move.o mud.o objsys.o parse.o rooms.o sendsys.o \
           s_socket.o timing.o uaf.o utils.o weather.o wizard.o \
           wizlist.o writer.o zones.o

GENOBS   = generate.o mkdata.o utils.o


all:	g d $(BINDIR)$(DUMP) $(BINDIR)$(PFILTER)

g:	$(BINDIR)$(GEN)

d:	$(BINDIR)$(MUDD)


$(BINDIR)$(GEN):  $(GENOBS)
	$(CC) -o $(BINDIR)$(GEN) $(CFLAGS) $(GENOBS)
	$(BINDIR)$(GEN) data $(ZONEDIR)/files
	$(BINDIR)$(GEN) verbs \
                        $(DATADIR)verbs.src $(DATADIR)verbs $(HDIR)verbs.h

$(BINDIR)$(MUDD): $(MUDDOBJS)
	$(CC) -o $(MUDD) $(CFLAGS) $(MUDDOBJS) $(LDFLAGS)
	mv $(MUDD) $(BINDIR)$(MUDD)

.c.o:
	$(CC) -c $(CFLAGS) $*.c



$(HDIR)verbs.h $(DATADIR)verbs:  $(BINDIR)$(GEN) $(DATADIR)verbs.src
	$(BINDIR)$(GEN) verbs $(DATADIR)/verbs.src $(DATADIR)/verbs \
	$(HDIR)verbs.h

$(HDIR)locations.h $(HDIR)mobiles.h $(HDIR)objects.h \
$(DATADIR)/locations $(DATADIR)/mobiles $(DATADIR)/objects \
$(DATADIR)/reset_data: $(BINDIR)$(GEN) $(ZONES)
	$(BINDIR)$(GEN) data $(ZONEDIR)/files


$(BINDIR)$(DUMP):	Dump.c kernel.h macros.h $(HDIR)sflags.h \
                $(HDIR)pflags.h pflagnames.h
	$(CC) $(CFLAGS) -o $(BINDIR)$(DUMP) $(DUMP).c

$(BINDIR)$(PFILTER):	kernel.h macros.h $(HDIR)pflags.h \
			pflagnames.h levelnames.h
	$(CC) $(CFLAGS) -o $(BINDIR)$(PFILTER) $(PFILTER).c


actions.o:	actions.c $(HDIR)actions.h kernel.h mudtypes.h mudmacros.h \
                $(HDIR)bprintf.h $(HDIR)sflags.h $(HDIR)sendsys.h \
                $(HDIR)verbs.h

bootstrap.o:	bootstrap.c $(HDIR)bootstrap.h kernel.h $(HDIR)extern.h \
                $(HDIR)utils.h mudtypes.h

bprintf.o:	bprintf.c $(HDIR)bprintf.h kernel.h $(HDIR)extern.h \
                mudmacros.h macros.h $(HDIR)sflags.h \
                $(HDIR)pflags.h mudtypes.h

change.o:	change.c $(HDIR)change.h $(HDIR)bprintf.h mudmacros.h \
                $(HDIR)pflags.h kernel.h mudtypes.h mudmacros.h \
                $(HDIR)extern.h $(HDIR)levels.h $(HDIR)sflags.h \
                $(HDIR)uaf.h $(HDIR)mobile.h $(HDIR)flags.h \
                $(HDIR)sendsys.h $(HDIR)writer.h

clone.o:	clone.c kernel.h mudtypes.h mudmacros.h $(HDIR)mobile.h \
                $(HDIR)pflags.h $(HDIR)cflags.h $(HDIR)oflags.h\
                $(HDIR)sendsys.h $(HDIR)objsys.h $(HDIR)utils.h \
                $(HDIR)clone.h

commands.o:     commands.c $(HDIR)commands.h kernel.h mudmacros.h \
                $(HDIR)flags.h $(HDIR)locations.h mudtypes.h \
                $(HDIR)objects.h $(HDIR)mobiles.h $(HDIR)sflags.h \
                $(HDIR)pflags.h $(HDIR)uaf.h \

                $(HDIR)oflags.h $(HDIR)lflags.h $(HDIR)cflags.h \
                $(HDIR)quests.h $(HDIR)sendsys.h $(HDIR)levels.h \
                $(HDIR)jmp.h $(HDIR)rooms.h $(HDIR)objsys.h \
                $(HDIR)mobile.h $(HDIR)bprintf.h $(HDIR)extern.h

condition.o:    condition.c $(HDIR)condition.h $(HDIR)condact.h \
                $(HDIR)oflags.h $(HDIR)levels.h $(HDIR)pflags.h \
                $(HDIR)cflags.h mudmacros.h mudtypes.h $(HDIR)sflags.h \
                $(HDIR)mobile.h $(HDIR)extern.h $(HDIR)bprintf.h

database.o:	database.c $(HDIR)condact.h $(HDIR)objects.h \
                $(HDIR)locations.h $(HDIR)oflags.h $(HDIR)mobiles.h \
                $(HDIR)verbs.h $(HDIR)extern.h

exec.o:         exec.c kernel.h $(HDIR)exec.h mudtypes.h $(HDIR)condact.h

fight.o:	fight.c $(HDIR)fight.h kernel.h mudmacros.h \
                $(HDIR)oflags.h $(HDIR)lflags.h \
                $(HDIR)mobiles.h $(HDIR)mobile.h $(HDIR)cflags.h \
                $(HDIR)mflags.h $(HDIR)sflags.h $(HDIR)pflags.h \
                $(HDIR)quests.h $(HDIR)sendsys.h $(HDIR)objects.h \
                $(HDIR)locations.h $(HDIR)objsys.h $(HDIR)extern.h \
                $(HDIR)bprintf.h $(HDIR)verbs.h

flags.o:	flags.c $(HDIR)flags.h kernel.h mudmacros.h mudtypes.h \
                $(HDIR)pflags.h $(HDIR)lflags.h pflagnames.h mflagnames.h \
                sflagnames.h lflagnames.h levelnames.h questnames.h \
                $(HDIR)extern.h $(HDIR)bprintf.h

frob.o:		frob.c $(HDIR)frob.h kernel.h $(HDIR)levels.h mudmacros.h \
		$(HDIR)sendsys.h $(HDIR)pflags.h $(HDIR)sflags.h \
		$(HDIR)uaf.h $(HDIR)mobile.h mudtypes.h

generate.o:	generate.c mkdata.c kernel.h mudtypes.h

log.o:	        log.c $(HDIR)log.h

magic.o:	kernel.h mudmacros.h $(HDIR)sendsys.h $(HDIR)magic.h \
                $(HDIR)pflags.h $(HDIR)sflags.h $(HDIR)cflags.h \
                $(HDIR)lflags.h $(HDIR)oflags.h $(HDIR)objects.h \
                $(HDIR)bprintf.h $(HDIR)parse.h $(HDIR)mobile.h \
                $(HDIR)mud.h $(HDIR)uaf.h $(HDIR)objsys.h

mail.o:         mail.c kernel.h mudtypes.h mudmacros.h $(HDIR)timing.h \
                $(HDIR)sflags.h $(HDIR)sendsys.h $(HDIR)writer.h 

main.o:	        main.c kernel.h $(HDIR)extern.h $(HDIR)log.h \
                $(HDIR)s_socket.h mudmacros.h $(HDIR)jmp.h \
                $(HDIR)mud.h $(HDIR)bootstrap.h $(HDIR)sendsys.h \
                $(HDIR)mobile.h $(HDIR)commands.h mudtypes.h \
                $(HDIR)extern.h $(HDIR)bprintf.h

mkdata.o:	mkdata.c kernel.h mudtypes.h $(HDIR)cflags.h mflagnames.h \
                pflagnames.h sflagnames.h lflagnames.h oflagnames.h \
                $(HDIR)lflags.h

mobile.o:	mobile.c $(HDIR)mobile.h kernel.h mudtypes.h mudmacros.h \
                $(HDIR)exits.h $(HDIR)pflags.h $(HDIR)mflags.h \
                $(HDIR)sflags.h $(HDIR)oflags.h $(HDIR)lflags.h \
                $(HDIR)quests.h $(HDIR)cflags.h $(HDIR)levels.h \
                $(HDIR)jmp.h $(HDIR)sendsys.h $(HDIR)objects.h \
                $(HDIR)mobiles.h $(HDIR)fight.h $(HDIR)extern.h \
                $(HDIR)bprintf.h 

move.o:         move.c $(HDIR)move.h kernel.h $(HDIR)locations.h \
                $(HDIR)mobiles.h $(HDIR)objects.h \
                $(HDIR)verbs.h $(HDIR)parse.h mudmacros.h mudtypes.h \
                $(HDIR)lflags.h $(HDIR)levels.h $(HDIR)mflags.h \
                $(HDIR)sendsys.h $(HDIR)exits.h $(HDIR)bprintf.h

mud.o:          mud.c $(HDIR)mud.h kernel.h $(HDIR)flags.h \
                $(HDIR)global.h mudmacros.h macros.h \
                $(HDIR)sendsys.h $(HDIR)pflags.h $(HDIR)sflags.h \
                $(HDIR)locations.h $(HDIR)uaf.h \
                mudtypes.h $(HDIR)mobile.h $(HDIR)timing.h \
                $(HDIR)utils.h $(HDIR)extern.h $(HDIR)bprintf.h

objsys.o:       objsys.c $(HDIR)objsys.h mudmacros.h mudtypes.h \
                kernel.h $(HDIR)pflags.h $(HDIR)sflags.h $(HDIR)cflags.h \
                $(HDIR)oflags.h $(HDIR)mflags.h $(HDIR)levels.h \
                $(HDIR)extern.h $(HDIR)quests.h $(HDIR)sendsys.h \
                $(HDIR)exec.h $(HDIR)parse.h $(HDIR)mobile.h \
                $(HDIR)locations.h $(HDIR)mobiles.h $(HDIR)objects.h \
                $(HDIR)bprintf.h oflagnames.h

parse.o:	$(HDIR)parse.h kernel.h mudmacros.h $(HDIR)verbs.h \
                mudtypes.h $(HDIR)extern.h $(HDIR)bprintf.h $(HDIR)wizard.h \
                $(HDIR)commands.h $(HDIR)condact.h $(HDIR)sflags.h \
                $(HDIR)sendsys.h $(HDIR)mobile.h $(HDIR)exec.h $(HDIR)move.h \
                $(HDIR)weather.h $(HDIR)locations.h $(HDIR)objects.h \
		$(HDIR)clone.h

rooms.o:	rooms.c $(HDIR)rooms.h kernel.h $(HDIR)pflags.h \
                $(HDIR)sflags.h $(HDIR)cflags.h $(HDIR)lflags.h \
                $(HDIR)oflags.h $(HDIR)objects.h $(HDIR)locations.h \
                $(HDIR)parse.h $(HDIR)zones.h $(HDIR)levels.h mudtypes.h \
                $(HDIR)extern.h $(HDIR)bprintf.h

sendsys.o:	sendsys.c $(HDIR)sendsys.h kernel.h $(HDIR)pflags.h \
                $(HDIR)sflags.h mudtypes.h $(HDIR)extern.h $(HDIR)bprintf.h

s_socket.o:	s_socket.c $(HDIR)s_socket.h

timing.o:	timing.c $(HDIR)timing.h $(HDIR)bprintf.h kernel.h \
                mudtypes.h $(HDIR)bprintf.h 

uaf.o:          uaf.c $(HDIR)uaf.h kernel.h mudmacros.h $(HDIR)sflags.h \
                mudtypes.h $(HDIR)mobile.h $(HDIR)flags.h $(HDIR)extern.h \
                $(HDIR)bprintf.h $(HDIR)pflags.h

utils.o:	utils.c $(HDIR)utils.h types.h macros.h

weather.o:	weather.c $(HDIR)weather.h kernel.h $(HDIR)sendsys.h \
                $(HDIR)pflags.h $(HDIR)sflags.h $(HDIR)levels.h \
                mudmacros.h $(HDIR)lflags.h $(HDIR)locations.h \
                $(HDIR)bprintf.h $(HDIR)extern.h $(HDIR)bprintf.h

wizard.o:	wizard.c kernel.h $(HDIR)levels.h $(HDIR)log.h \
                $(HDIR)uaf.h $(HDIR)bprintf.h $(HDIR)verbs.h \
                $(HDIR)parse.h $(HDIR)sendsys.h $(HDIR)mobile.h \
                $(HDIR)objsys.h $(HDIR)utils.h $(HDIR)rooms.h \
                $(HDIR)flags.h $(HDIR)pflags.h $(HDIR)lflags.h \
                $(HDIR)sflags.h $(HDIR)cflags.h mudmacros.h

wizlist.o:	wizlist.c $(HDIR)wizlist.h $(HDIR)bprintf.h kernel.h \
                $(HDIR)levels.h

writer.o:	writer.c $(HDIR)writer.h $(HDIR)utils.h $(HDIR)bprintf.h

zones.o:	zones.c $(HDIR)zones.h kernel.h mudmacros.h \
                $(HDIR)cflags.h $(HDIR)levels.h mudtypes.h \
                $(HDIR)extern.h $(HDIR)bprintf.h



# Strip binaries.
#
strip:
	cd $(BINDIR); \
	strip $(MUDD) $(GEN) $(DUMP) $(PFILTER)

# Clean the directories of any redundant files, strip binaries,
# and compress directories not needed for day-to-day administration.
# ( For maximum space-saving. )
clean:	strip
	{		rm *.o *~ #*; \
	cd $(DATADIR);  rm *~ mkdata.log reset.list; \
	cd $(HDIR);     rm *~ locations.h objects.h verbs.h mobiles.h; \
	} 2> /dev/null
	tar cvf - src | compress > src.tar.Z; rm -rf src
	tar cvf - include | compress > include.tar.Z; rm -rf include
	tar cvf - $(ZONEDIR) | compress > ZONE.tar.Z; rm -rf $(ZONEDIR)


# Make the directories ready for distribution:
#
dist:
	{		rm *.o *~ #*; \
	cd $(DATADIR);  rm *~ DESC/* MAIL/* $(ZONEDIR)/*~ \
			   locations mkdata.log mobiles objects pid \
			   reset.list reset_data syslog uaf_rand \
			   verbs wizlist zones; \
	cd $(BINDIR);   rm *~ $(MUDD) $(GEN) $(DUMP) $(PFILTER); \
	cd $(HDIR);     rm *~ locations.h verbs.h mobiles.h objects.h; \
	cd ../doc;      rm *~; \
	} 2> /dev/null

