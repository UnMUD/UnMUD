#-----------------------------------------------------------------------------
# Makefile
#-----------------------------------------------------------------------------

HOME       = ../
BINDIR     = $(HOME)/bin/
DATADIR    = $(HOME)/data/
ZONEDIR    = $(DATADIR)/WORLD/
HDIR       = $(HOME)/include/
# GDBMDIR path will need to be set to absolute path on your system UNLESS it is 
# installed on the main system rather than your own directory
# GDBMDIR    = /u/kmowry/bin/gdbm-aix/
MUDD       = aberd
GEN        = generate
PFILTER    = pfilter
DUMP       = Dump
ECHO       = /bin/echo
DEPEND     = $(BINDIR)/mkdep-x
KLEENEX    = $(BINDIR)/clean
LIBS       = -lgdbm -lcrypt
CO         = co -q

# uncomment one appropriate line in the group below:
# LINUX, AIX, SUN, MIPS
CC         = gcc
# CONCENTRIX
# CC       = scc

# uncomment one appropriate line in the group below:
ARCH       = -DLINUX
# ARCH       = -DRS6000
# ARCH       = -DSUN
# ARCH       = -DCONCENTRIX
# ARCH       = -DMIPS

# uncomment one appropriate line in the group below:
# NOTE: if you are using SunOS, define BROKEN_GCC if you have gcc 1.40 or 
# another version which mysteriously dumps core.  (see code)
# NOTE: if you must install gdbm in your own directory tree and not on the main
# system, you must add -I$(GDBMDIR) (see example below) to your CFLAGS.  If 
# gdbm is on the main system, you will not need to add this, and it will look
# like the rest of the machine-specific examples below.
# example using gdbm from your own directory tree:
#CFLAGS       = -DDEBUG -g -O4 -I$(HDIR) $(ARCH) -DREBOOT -I$(GDBMDIR)
# Linux
CFLAGS       = -DDEBUG -g -O4 -I$(HDIR) $(ARCH) -DREBOOT
# AIX
# CFLAGS       = -DDEBUG -g -O -I$(HDIR) $(ARCH) -DREBOOT
# SunOS
# CFLAGS       = -DDEBUG -g -O4 -I$(HDIR) $(ARCH) -DREBOOT
# Concentrix
# CFLAGS       = -UDEBUG -localcache -O -I$(HDIR) -$(ARCH) -DREBOOT -i
# MIPS-EP/IX
# CFLAGS       = -DDEBUG -g -O -fno-builtin -I$(HDIR) -I/usr/include/bsd -DMIPS -DREBOOT

#-----------------------------------------------------------------------------
# OBJECTS
#-----------------------------------------------------------------------------

GENOBS       = generate.o

PFOBS        = pfilter.o misc.o

DOBS         = Dump.o

MUDDOBJS     = actions.o admin.o board.o bootstrap.o bprintf.o calendar.o \
               change.o clone.o commands.o communicate.o condition.o \
               fight.o flags.o frob.o game.o hate.o log.o magic.o mail.o \
               main.o misc.o mobile.o move.o mud.o objsys.o parse.o party.o \
               rooms.o s_socket.o sendsys.o sflag.o timing.o uaf.o utils.o \
               wizard.o wizlist.o writer.o zones.o
  
ZONES        = $(ZONEDIR)/files 	  $(ZONEDIR)/undef.h \
               $(ZONEDIR)/abyss.zone 	  $(ZONEDIR)/ancient.zone \
               $(ZONEDIR)/blizzard.zone   $(ZONEDIR)/castle.zone \
               $(ZONEDIR)/catacomb.zone	  $(ZONEDIR)/cave.zone \
               $(ZONEDIR)/church.zone	  $(ZONEDIR)/dead.zone \
               $(ZONEDIR)/doom.zone	  $(ZONEDIR)/eastcoast.zone \
               $(ZONEDIR)/eforest.zone    $(ZONEDIR)/femnaz.zone \
               $(ZONEDIR)/forest.zone	  $(ZONEDIR)/frobozz.zone \
               $(ZONEDIR)/haven.zone	  $(ZONEDIR)/heaven.zone \
               $(ZONEDIR)/home.zone	  $(ZONEDIR)/icecave.zone \
               $(ZONEDIR)/island.zone	  $(ZONEDIR)/jail.zone \
               $(ZONEDIR)/jbs.zone	  $(ZONEDIR)/kastle.zone \
               $(ZONEDIR)/labyrinth.zone  $(ZONEDIR)/ledge.zone \
               $(ZONEDIR)/limbo.zone	  $(ZONEDIR)/narnia.zone \
               $(ZONEDIR)/mithdan.zone	  $(ZONEDIR)/monopoly.zone \
               $(ZONEDIR)/moor.zone	  $(ZONEDIR)/mountain.zone \
               $(ZONEDIR)/newbie.zone	  $(ZONEDIR)/newvillage.zone \
               $(ZONEDIR)/nibelung.zone	  $(ZONEDIR)/oaktree.zone \
               $(ZONEDIR)/orchold.zone	  $(ZONEDIR)/party.zone \
               $(ZONEDIR)/pirate.zone     $(ZONEDIR)/window.zone \
               $(ZONEDIR)/pit.zone	  $(ZONEDIR)/pocket.zone \
               $(ZONEDIR)/powers.zone	  $(ZONEDIR)/quarry.zone \
               $(ZONEDIR)/rhome.zone	  $(ZONEDIR)/sea.zone \
               $(ZONEDIR)/sherwood.zone	  $(ZONEDIR)/start.zone \
               $(ZONEDIR)/stjames.zone	  $(ZONEDIR)/tower.zone \
               $(ZONEDIR)/treehouse.zone  $(ZONEDIR)/valley.zone \
               $(ZONEDIR)/village.zone	  $(ZONEDIR)/waste.zone \
               generate.c

#-----------------------------------------------------------------------------
# RULES
#-----------------------------------------------------------------------------

all: WORLD $(BINDIR)$(MUDD) $(BINDIR)$(PFILTER) $(BINDIR)$(DUMP)

# removes all dependencies
depend:
	$(DEPEND) -f Makefile -c $(CC) $(CFLAGS) *.c

clean:
	-@$(KLEENEX)

WORLD:	$(BINDIR)$(GEN) $(HDIR)/verbs.h \
	$(DATADIR)/locations \
	$(DATADIR)/objects \
	$(DATADIR)/mobiles \
	$(DATADIR)/zones \
	$(HDIR)/locations.h \
	$(HDIR)/mobiles.h \
	$(HDIR)/objects.h \

#-----------------------------------------------------------------------------

$(BINDIR)$(GEN): $(GENOBS)
	$(CC) -o $(BINDIR)$(GEN) $(CFLAGS) $(GENOBS) $(LIBS)

#-----------------------------------------------------------------------------

# generate world, if certain files don't exist, or if a zone has been modified

$(DATADIR)/locations \
$(DATADIR)/objects \
$(DATADIR)/mobiles \
$(DATADIR)/zones \
$(HDIR)/locations.h \
$(HDIR)/mobiles.h \
$(HDIR)/objects.h \ : $(ZONES)
	@echo Regenerating World....
	$(BINDIR)$(GEN) data $(HOME)

#-----------------------------------------------------------------------------

# generate verbs, if certain files don't exist or if a file has been modified
$(HDIR)/verbs.h $(DATADIR)verbs: $(DATADIR)verbs.src
# $(HDIR)/verbs.h: $(DATADIR)verbs.src
	@echo Regenerating Verbs...
	$(BINDIR)$(GEN) verbs $(HOME)

#-----------------------------------------------------------------------------

$(BINDIR)$(MUDD): $(MUDDOBJS)
	@$(ECHO) "Linking..."
	@-mv ../bin/aberd ../bin/aberd.old
	@-rm ./aberd
	@$(ECHO) "Moving ../bin/aberd to ../bin/aberd.old"
	$(CC) -o $(MUDD) $(CFLAGS) $(MUDDOBJS) $(LDFLAGS) $(LIBS)
	@-mv aberd ../bin/aberd
	@$(ECHO) "Installing aberd into the ../bin/ directory"
	@ln -s ../bin/aberd ../src/aberd
	@$(ECHO) "Forming symbolic link..."
	@$(ECHO) "Done."

#-----------------------------------------------------------------------------

$(BINDIR)$(PFILTER):	$(HDIR)kernel.h $(HDIR)macros.h $(HDIR)pflags.h \
		$(HDIR)pflagnames.h $(HDIR)levelnames.h $(PFOBS)
	$(CC) $(CFLAGS) -o $(BINDIR)$(PFILTER) $(PFOBS) $(LIBS)

#-----------------------------------------------------------------------------

$(BINDIR)$(DUMP): $(DOBS)
	$(CC) $(CFLAGS) -o $(BINDIR)$(DUMP) $(DOBS) $(LIBS)

#-----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

Dump.o: Dump.c /usr/include/stdlib.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/sys/stat.h \
 /usr/include/bits/stat.h /usr/include/bits/time.h \
 /usr/include/errno.h /usr/include/bits/errno.h \
 /usr/include/linux/errno.h /usr/include/asm/errno.h \
 ../include/kernel.h ../include/_VERSION_ /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h ../include/machines.h \
 ../include/machine/linux.h ../include/config.h ../include/levels.h \
 ../include/exits.h ../include/types.h ../include/utils.h \
 ../include/mudtypes.h /usr/include/sys/time.h ../include/mudmacros.h \
 ../include/macros.h ../include/extern.h ../include/files.h \
 ../include/sflags.h ../include/pflags.h ../include/pflagnames.h \
 /usr/include/gdbm.h
actions.o: actions.c /usr/include/malloc.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h \
 /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h \
 /usr/include/linux/param.h /usr/include/asm/param.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 ../include/kernel.h ../include/_VERSION_ /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h ../include/mobiles.h
admin.o: admin.c ../include/verbs.h ../include/kernel.h \
 ../include/_VERSION_ /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h ../include/locations.h
board.o: board.c /usr/include/unistd.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h ../include/machines.h \
 ../include/machine/linux.h ../include/config.h ../include/levels.h \
 ../include/exits.h ../include/types.h ../include/utils.h \
 ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/objects.h \
 /usr/include/malloc.h ../include/stdinc.h ../include/actions.h \
 ../include/board.h ../include/bootstrap.h ../include/bprintf.h \
 ../include/calendar.h ../include/cflags.h ../include/change.h \
 ../include/clone.h ../include/commands.h ../include/debug.h \
 ../include/exec.h ../include/fight.h ../include/flags.h \
 ../include/frob.h ../include/jmp.h ../include/lflags.h \
 ../include/log.h ../include/magic.h ../include/mflags.h \
 ../include/misc.h ../include/mobile.h ../include/move.h \
 ../include/mud.h ../include/objsys.h ../include/oflags.h \
 ../include/parse.h ../include/pflags.h ../include/quests.h \
 ../include/rooms.h ../include/s_socket.h ../include/sendsys.h \
 ../include/sflags.h ../include/timing.h ../include/uaf.h \
 ../include/weather.h ../include/wizard.h ../include/wizlist.h \
 ../include/writer.h ../include/zones.h ../include/admin.h \
 ../include/verbs.h ../include/communicate.h ../include/sflag.h \
 ../include/game.h ../include/main.h ../include/hate.h
bootstrap.o: bootstrap.c /usr/include/malloc.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/stdlib.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/kernel.h ../include/_VERSION_ /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/ctype.h \
 /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h ../include/machines.h \
 ../include/machine/linux.h ../include/config.h ../include/levels.h \
 ../include/exits.h ../include/types.h ../include/utils.h \
 ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
bprintf.o: bprintf.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
calendar.o: calendar.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/locations.h \
 ../include/stdinc.h /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
change.o: change.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
clone.o: clone.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/bootstrap.h \
 ../include/mobile.h ../include/pflags.h ../include/cflags.h \
 ../include/sendsys.h ../include/objsys.h ../include/oflags.h \
 ../include/zones.h ../include/parse.h ../include/fight.h \
 ../include/clone.h ../include/locations.h ../include/mobiles.h \
 ../include/stdinc.h /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bprintf.h ../include/calendar.h ../include/change.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/move.h \
 ../include/mud.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/admin.h \
 ../include/verbs.h ../include/communicate.h ../include/sflag.h \
 ../include/game.h ../include/main.h ../include/hate.h
commands.o: commands.c /usr/include/unistd.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/kernel.h ../include/_VERSION_ /usr/include/sys/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/locations.h \
 ../include/objects.h ../include/mobiles.h ../include/stdinc.h \
 /usr/include/alloca.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
communicate.o: communicate.c ../include/mobiles.h ../include/verbs.h \
 ../include/kernel.h ../include/_VERSION_ /usr/include/sys/types.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/gnu/stubs.h /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h ../include/colors.h
condition.o: condition.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
darray.o: darray.c /usr/include/unistd.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 ../include/darray.h
fight.o: fight.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/verbs.h \
 ../include/mobiles.h ../include/objects.h ../include/locations.h \
 ../include/stdinc.h /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
flags.o: flags.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/mflagnames.h \
 ../include/pflagnames.h ../include/sflagnames.h \
 ../include/lflagnames.h ../include/levelnames.h \
 ../include/questnames.h ../include/stdinc.h /usr/include/alloca.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
frob.o: frob.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
game.o: game.c ../include/verbs.h ../include/kernel.h \
 ../include/_VERSION_ /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h ../include/objects.h ../include/mobiles.h \
 ../include/locations.h
generate.o: generate.c /usr/include/stdlib.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/errno.h /usr/include/bits/errno.h \
 /usr/include/linux/errno.h /usr/include/asm/errno.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/kernel.h ../include/_VERSION_ /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/string.h \
 /usr/include/bits/string.h /usr/include/bits/string2.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/cflags.h \
 ../include/mflagnames.h ../include/pflagnames.h \
 ../include/sflagnames.h ../include/lflagnames.h \
 ../include/oflagnames.h ../include/exitnames.h ../include/lflags.h \
 ../include/generate.h
hate.o: hate.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/bprintf.h \
 ../include/stdinc.h /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
intermud.o: intermud.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h /usr/include/sys/socket.h \
 /usr/include/arpa/inet.h /usr/include/netdb.h \
 /usr/include/rpc/netdb.h ../include/stdinc.h /usr/include/alloca.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h ../include/intermud.h ../include/intermudP.h \
 ../include/darray.h
intermudComm.o: intermudComm.c ../include/kernel.h \
 ../include/_VERSION_ /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h ../include/intermud.h \
 ../include/intermudP.h ../include/darray.h
log.o: log.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h /usr/include/sys/file.h \
 /usr/include/fcntl.h /usr/include/bits/fcntl.h \
 /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h ../include/stdinc.h /usr/include/alloca.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
magic.o: magic.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/objects.h \
 ../include/locations.h ../include/stdinc.h /usr/include/alloca.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
mail.o: mail.c /usr/include/stdlib.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/ctype.h \
 /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h ../include/machines.h \
 ../include/machine/linux.h ../include/config.h ../include/levels.h \
 ../include/exits.h ../include/types.h ../include/utils.h \
 ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/objects.h \
 ../include/mobiles.h ../include/mobile.h ../include/locations.h \
 ../include/sflags.h ../include/pflags.h ../include/mflags.h \
 ../include/cflags.h ../include/quests.h ../include/sendsys.h \
 ../include/parse.h ../include/exec.h ../include/rooms.h \
 ../include/mail.h ../include/bprintf.h ../include/stdinc.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/calendar.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/misc.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/s_socket.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
main.o: main.c /usr/include/sys/file.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/fcntl.h /usr/include/bits/fcntl.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 ../include/kernel.h ../include/_VERSION_ /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/sys/stat.h \
 /usr/include/bits/stat.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/sys/ioctl.h \
 /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h \
 /usr/include/asm/ioctl.h /usr/include/bits/ioctl-types.h \
 /usr/include/sys/ttydefaults.h /usr/include/signal.h \
 /usr/include/bits/signum.h /usr/include/bits/siginfo.h \
 /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h \
 /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h \
 /usr/include/sys/socket.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h ../include/locations.h \
 ../include/stdinc.h /usr/include/alloca.h ../include/actions.h \
 ../include/board.h ../include/bootstrap.h ../include/bprintf.h \
 ../include/calendar.h ../include/cflags.h ../include/change.h \
 ../include/clone.h ../include/commands.h ../include/debug.h \
 ../include/exec.h ../include/fight.h ../include/flags.h \
 ../include/frob.h ../include/jmp.h ../include/lflags.h \
 ../include/log.h ../include/magic.h ../include/mflags.h \
 ../include/misc.h ../include/mobile.h ../include/move.h \
 ../include/mud.h ../include/objsys.h ../include/oflags.h \
 ../include/parse.h ../include/pflags.h ../include/quests.h \
 ../include/rooms.h ../include/s_socket.h ../include/sendsys.h \
 ../include/sflags.h ../include/timing.h ../include/uaf.h \
 ../include/weather.h ../include/wizard.h ../include/wizlist.h \
 ../include/writer.h ../include/zones.h ../include/admin.h \
 ../include/verbs.h ../include/communicate.h ../include/sflag.h \
 ../include/game.h ../include/main.h ../include/hate.h
misc.o: misc.c /usr/include/stdlib.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/ctype.h \
 /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h ../include/machines.h \
 ../include/machine/linux.h ../include/config.h ../include/levels.h \
 ../include/exits.h ../include/types.h ../include/utils.h \
 ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h
mobile.o: mobile.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/locations.h \
 ../include/objects.h ../include/mobiles.h ../include/objsys.h \
 ../include/stdinc.h /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h ../include/party.h
move.o: move.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/objects.h \
 ../include/mobiles.h ../include/locations.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
mud.o: mud.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h /usr/include/sys/stat.h \
 /usr/include/bits/stat.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/stdinc.h /usr/include/alloca.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h ../include/locations.h ../include/global.h
objsys.o: objsys.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/objects.h \
 ../include/mobiles.h ../include/locations.h ../include/oflagnames.h \
 ../include/stdinc.h /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
parse.o: parse.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/verbs.h \
 ../include/locations.h ../include/objects.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h ../include/colors.h
party.o: party.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/bprintf.h \
 ../include/sendsys.h ../include/parse.h ../include/log.h \
 ../include/sflags.h ../include/communicate.h /usr/include/alloca.h \
 ../include/colors.h
pfilter.o: pfilter.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/pflagnames.h \
 ../include/levelnames.h ../include/stdinc.h /usr/include/alloca.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
rooms.o: rooms.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/locations.h \
 ../include/objects.h ../include/exitnames.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
s_socket.o: s_socket.c /usr/include/sys/types.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/gnu/stubs.h /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/netinet/in.h /usr/include/stdint.h \
 /usr/include/bits/socket.h /usr/include/bits/sockaddr.h \
 /usr/include/asm/socket.h /usr/include/asm/sockios.h \
 /usr/include/bits/in.h /usr/include/bits/byteswap.h \
 /usr/include/sys/socket.h /usr/include/netdb.h \
 /usr/include/rpc/netdb.h /usr/include/fcntl.h \
 /usr/include/bits/fcntl.h ../include/s_socket.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/string.h \
 /usr/include/bits/string.h /usr/include/bits/string2.h \
 /usr/include/stdlib.h
sendsys.o: sendsys.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
sflag.o: sflag.c ../include/verbs.h ../include/kernel.h \
 ../include/_VERSION_ /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h ../include/colors.h
timing.o: timing.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h /usr/include/sys/signal.h \
 /usr/include/signal.h /usr/include/bits/signum.h \
 /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
 /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
 /usr/include/bits/sigstack.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h ../include/stdinc.h /usr/include/alloca.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
uaf.o: uaf.c /usr/include/sys/file.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/fcntl.h /usr/include/bits/fcntl.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/bits/time.h /usr/include/sys/stat.h \
 /usr/include/bits/stat.h ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/ctype.h \
 /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/sys/time.h \
 ../include/mudmacros.h ../include/macros.h ../include/extern.h \
 ../include/files.h /usr/include/errno.h /usr/include/bits/errno.h \
 /usr/include/linux/errno.h /usr/include/asm/errno.h \
 ../include/stdinc.h /usr/include/alloca.h ../include/actions.h \
 ../include/board.h ../include/bootstrap.h ../include/bprintf.h \
 ../include/calendar.h ../include/cflags.h ../include/change.h \
 ../include/clone.h ../include/commands.h ../include/debug.h \
 ../include/exec.h ../include/fight.h ../include/flags.h \
 ../include/frob.h ../include/jmp.h ../include/lflags.h \
 ../include/log.h ../include/magic.h ../include/mflags.h \
 ../include/misc.h ../include/mobile.h ../include/move.h \
 ../include/mud.h ../include/objsys.h ../include/oflags.h \
 ../include/parse.h ../include/pflags.h ../include/quests.h \
 ../include/rooms.h ../include/s_socket.h ../include/sendsys.h \
 ../include/sflags.h ../include/timing.h ../include/uaf.h \
 ../include/weather.h ../include/wizard.h ../include/wizlist.h \
 ../include/writer.h ../include/zones.h ../include/admin.h \
 ../include/verbs.h ../include/communicate.h ../include/sflag.h \
 ../include/game.h ../include/main.h ../include/hate.h \
 /usr/include/gdbm.h
utils.o: utils.c /usr/include/stdlib.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h ../include/machines.h \
 ../include/machine/linux.h ../include/config.h ../include/levels.h \
 ../include/exits.h ../include/types.h ../include/utils.h \
 ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/verbs.h ../include/communicate.h \
 ../include/sflag.h ../include/game.h ../include/main.h \
 ../include/hate.h
wizard.o: wizard.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/verbs.h \
 ../include/locations.h ../include/stdinc.h /usr/include/alloca.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 ../include/actions.h ../include/board.h ../include/bootstrap.h \
 ../include/bprintf.h ../include/calendar.h ../include/cflags.h \
 ../include/change.h ../include/clone.h ../include/commands.h \
 ../include/debug.h ../include/exec.h ../include/fight.h \
 ../include/flags.h ../include/frob.h ../include/jmp.h \
 ../include/lflags.h ../include/log.h ../include/magic.h \
 ../include/mflags.h ../include/misc.h ../include/mobile.h \
 ../include/move.h ../include/mud.h ../include/objsys.h \
 ../include/oflags.h ../include/parse.h ../include/pflags.h \
 ../include/quests.h ../include/rooms.h ../include/s_socket.h \
 ../include/sendsys.h ../include/sflags.h ../include/timing.h \
 ../include/uaf.h ../include/weather.h ../include/wizard.h \
 ../include/wizlist.h ../include/writer.h ../include/zones.h \
 ../include/admin.h ../include/communicate.h ../include/sflag.h \
 ../include/game.h ../include/main.h ../include/hate.h \
 ../include/objects.h ../include/mobiles.h
wizlist.o: wizlist.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
writer.o: writer.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h
zones.o: zones.c ../include/kernel.h ../include/_VERSION_ \
 /usr/include/sys/types.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/limits.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
 /usr/include/asm/param.h /usr/include/setjmp.h \
 /usr/include/bits/setjmp.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
 /usr/include/ctype.h /usr/include/string.h /usr/include/bits/string.h \
 /usr/include/bits/string2.h /usr/include/stdlib.h \
 ../include/machines.h ../include/machine/linux.h ../include/config.h \
 ../include/levels.h ../include/exits.h ../include/types.h \
 ../include/utils.h ../include/mudtypes.h /usr/include/bits/time.h \
 /usr/include/sys/time.h ../include/mudmacros.h ../include/macros.h \
 ../include/extern.h ../include/files.h ../include/stdinc.h \
 /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h ../include/actions.h ../include/board.h \
 ../include/bootstrap.h ../include/bprintf.h ../include/calendar.h \
 ../include/cflags.h ../include/change.h ../include/clone.h \
 ../include/commands.h ../include/debug.h ../include/exec.h \
 ../include/fight.h ../include/flags.h ../include/frob.h \
 ../include/jmp.h ../include/lflags.h ../include/log.h \
 ../include/magic.h ../include/mflags.h ../include/misc.h \
 ../include/mobile.h ../include/move.h ../include/mud.h \
 ../include/objsys.h ../include/oflags.h ../include/parse.h \
 ../include/pflags.h ../include/quests.h ../include/rooms.h \
 ../include/s_socket.h ../include/sendsys.h ../include/sflags.h \
 ../include/timing.h ../include/uaf.h ../include/weather.h \
 ../include/wizard.h ../include/wizlist.h ../include/writer.h \
 ../include/zones.h ../include/admin.h ../include/verbs.h \
 ../include/communicate.h ../include/sflag.h ../include/game.h \
 ../include/main.h ../include/hate.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
