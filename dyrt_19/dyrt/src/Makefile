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

Dump.o: Dump.c /usr/include/stdc-predef.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h \
 /usr/include/x86_64-linux-gnu/sys/stat.h \
 /usr/include/x86_64-linux-gnu/bits/stat.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/errno.h /usr/include/x86_64-linux-gnu/bits/errno.h \
 /usr/include/linux/errno.h /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 ..//include/kernel.h ..//include/_VERSION_ \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/macros.h ..//include/sflags.h ..//include/pflags.h \
 ..//include/pflagnames.h /usr/local/include/gdbm.h
actions.o: actions.c /usr/include/stdc-predef.h /usr/include/malloc.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h /usr/include/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/mobiles.h
admin.o: admin.c /usr/include/stdc-predef.h ..//include/verbs.h \
 ..//include/kernel.h ..//include/_VERSION_ \
 /usr/include/x86_64-linux-gnu/sys/types.h /usr/include/features.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/admin.h \
 ..//include/locations.h
board.o: board.c /usr/include/stdc-predef.h /usr/include/unistd.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/objects.h /usr/include/malloc.h ..//include/stdinc.h \
 ..//include/actions.h ..//include/board.h ..//include/bootstrap.h \
 ..//include/bprintf.h ..//include/calendar.h ..//include/cflags.h \
 ..//include/change.h ..//include/clone.h ..//include/commands.h \
 ..//include/debug.h ..//include/exec.h ..//include/fight.h \
 ..//include/flags.h ..//include/frob.h ..//include/jmp.h \
 ..//include/lflags.h ..//include/log.h ..//include/magic.h \
 ..//include/mflags.h ..//include/misc.h ..//include/mobile.h \
 ..//include/move.h ..//include/mud.h ..//include/objsys.h \
 ..//include/oflags.h ..//include/parse.h ..//include/pflags.h \
 ..//include/quests.h ..//include/rooms.h ..//include/s_socket.h \
 ..//include/sendsys.h ..//include/sflags.h ..//include/timing.h \
 ..//include/uaf.h ..//include/weather.h ..//include/wizard.h \
 ..//include/wizlist.h ..//include/writer.h ..//include/zones.h \
 ..//include/admin.h ..//include/verbs.h ..//include/kernel.h \
 ..//include/stdinc.h ..//include/communicate.h ..//include/sflag.h \
 ..//include/game.h ..//include/main.h ..//include/hate.h
bootstrap.o: bootstrap.c /usr/include/stdc-predef.h /usr/include/malloc.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h /usr/include/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h /usr/include/errno.h \
 /usr/include/x86_64-linux-gnu/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 ..//include/kernel.h ..//include/_VERSION_ \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h ..//include/actions.h ..//include/board.h \
 ..//include/bootstrap.h ..//include/bprintf.h ..//include/calendar.h \
 ..//include/cflags.h ..//include/change.h ..//include/clone.h \
 ..//include/commands.h ..//include/debug.h ..//include/exec.h \
 ..//include/fight.h ..//include/flags.h ..//include/frob.h \
 ..//include/jmp.h ..//include/lflags.h ..//include/log.h \
 ..//include/magic.h ..//include/mflags.h ..//include/misc.h \
 ..//include/mobile.h ..//include/move.h ..//include/mud.h \
 ..//include/objsys.h ..//include/oflags.h ..//include/parse.h \
 ..//include/pflags.h ..//include/quests.h ..//include/rooms.h \
 ..//include/s_socket.h ..//include/sendsys.h ..//include/sflags.h \
 ..//include/timing.h ..//include/uaf.h ..//include/weather.h \
 ..//include/wizard.h ..//include/wizlist.h ..//include/writer.h \
 ..//include/zones.h ..//include/admin.h ..//include/verbs.h \
 ..//include/kernel.h ..//include/stdinc.h ..//include/communicate.h \
 ..//include/sflag.h ..//include/game.h ..//include/main.h \
 ..//include/hate.h
bprintf.o: bprintf.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/sendsys.h \
 ..//include/utils.h ..//include/mud.h
calendar.o: calendar.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/locations.h ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
change.o: change.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
clone.o: clone.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/bootstrap.h ..//include/mobile.h ..//include/pflags.h \
 ..//include/cflags.h ..//include/sendsys.h ..//include/objsys.h \
 ..//include/oflags.h ..//include/zones.h ..//include/parse.h \
 ..//include/fight.h ..//include/exits.h ..//include/clone.h \
 ..//include/locations.h ..//include/mobiles.h ..//include/stdinc.h \
 /usr/include/stdlib.h /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
commands.o: commands.c /usr/include/stdc-predef.h /usr/include/unistd.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/locations.h ..//include/objects.h ..//include/mobiles.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
communicate.o: communicate.c /usr/include/stdc-predef.h \
 ..//include/mobiles.h ..//include/verbs.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/communicate.h \
 ..//include/colors.h
condition.o: condition.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
darray.o: darray.c /usr/include/stdc-predef.h /usr/include/unistd.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h ..//include/darray.h
fight.o: fight.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/verbs.h ..//include/mobiles.h ..//include/objects.h \
 ..//include/locations.h ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
flags.o: flags.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/mflagnames.h ..//include/pflagnames.h \
 ..//include/sflagnames.h ..//include/lflagnames.h \
 ..//include/levelnames.h ..//include/questnames.h ..//include/stdinc.h \
 /usr/include/stdlib.h /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
frob.o: frob.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
game.o: game.c /usr/include/stdc-predef.h ..//include/verbs.h \
 ..//include/kernel.h ..//include/_VERSION_ \
 /usr/include/x86_64-linux-gnu/sys/types.h /usr/include/features.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/objects.h \
 ..//include/mobiles.h ..//include/locations.h ..//include/game.h
generate.o: generate.c /usr/include/stdc-predef.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/errno.h /usr/include/x86_64-linux-gnu/bits/errno.h \
 /usr/include/linux/errno.h /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 /usr/include/unistd.h /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/string.h \
 /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/utils.h ..//include/cflags.h ..//include/mflagnames.h \
 ..//include/pflagnames.h ..//include/sflagnames.h \
 ..//include/lflagnames.h ..//include/oflagnames.h \
 ..//include/exitnames.h ..//include/lflags.h ..//include/machines.h \
 ..//include/generate.h
hate.o: hate.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/config.h ..//include/bprintf.h ..//include/extern.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
intermud.o: intermud.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 /usr/include/arpa/inet.h /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/x86_64-linux-gnu/bits/netdb.h ..//include/stdinc.h \
 /usr/include/stdlib.h /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/intermud.h \
 ..//include/intermudP.h ..//include/intermud.h ..//include/darray.h
intermudComm.o: intermudComm.c /usr/include/stdc-predef.h \
 ..//include/kernel.h ..//include/_VERSION_ \
 /usr/include/x86_64-linux-gnu/sys/types.h /usr/include/features.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/intermud.h \
 ..//include/intermudP.h ..//include/intermud.h ..//include/darray.h
log.o: log.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 /usr/include/x86_64-linux-gnu/sys/file.h /usr/include/fcntl.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl-linux.h \
 /usr/include/x86_64-linux-gnu/bits/stat.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl2.h \
 /usr/include/x86_64-linux-gnu/sys/stat.h /usr/include/errno.h \
 /usr/include/x86_64-linux-gnu/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
magic.o: magic.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/objects.h ..//include/locations.h ..//include/stdinc.h \
 /usr/include/stdlib.h /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
mail.o: mail.c /usr/include/stdc-predef.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h \
 /usr/include/x86_64-linux-gnu/sys/stat.h \
 /usr/include/x86_64-linux-gnu/bits/stat.h /usr/include/errno.h \
 /usr/include/x86_64-linux-gnu/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 /usr/include/unistd.h /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/objects.h ..//include/mobiles.h ..//include/mobile.h \
 ..//include/locations.h ..//include/sflags.h ..//include/pflags.h \
 ..//include/mflags.h ..//include/cflags.h ..//include/quests.h \
 ..//include/sendsys.h ..//include/levels.h ..//include/parse.h \
 ..//include/exec.h ..//include/rooms.h ..//include/mail.h \
 ..//include/bprintf.h ..//include/stdinc.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
main.o: main.c /usr/include/stdc-predef.h \
 /usr/include/x86_64-linux-gnu/sys/file.h /usr/include/features.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h /usr/include/fcntl.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl-linux.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/stat.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl2.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 /usr/include/errno.h /usr/include/x86_64-linux-gnu/bits/errno.h \
 /usr/include/linux/errno.h /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 /usr/include/x86_64-linux-gnu/sys/stat.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h \
 /usr/include/x86_64-linux-gnu/sys/ioctl.h \
 /usr/include/x86_64-linux-gnu/bits/ioctls.h \
 /usr/include/x86_64-linux-gnu/asm/ioctls.h \
 /usr/include/asm-generic/ioctls.h /usr/include/linux/ioctl.h \
 /usr/include/x86_64-linux-gnu/asm/ioctl.h \
 /usr/include/asm-generic/ioctl.h \
 /usr/include/x86_64-linux-gnu/bits/ioctl-types.h \
 /usr/include/x86_64-linux-gnu/sys/ttydefaults.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/x86_64-linux-gnu/bits/netdb.h ..//include/config.h \
 ..//include/locations.h ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/machines.h
misc.o: misc.c /usr/include/stdc-predef.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h \
 /usr/include/x86_64-linux-gnu/sys/stat.h \
 /usr/include/x86_64-linux-gnu/bits/stat.h /usr/include/errno.h \
 /usr/include/x86_64-linux-gnu/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 ..//include/kernel.h ..//include/_VERSION_ \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h
mobile.o: mobile.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/locations.h ..//include/objects.h ..//include/mobiles.h \
 ..//include/objsys.h ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/party.h
move.o: move.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/objects.h ..//include/mobiles.h ..//include/locations.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
mud.o: mud.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 /usr/include/x86_64-linux-gnu/sys/stat.h \
 /usr/include/x86_64-linux-gnu/bits/stat.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/machines.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/locations.h \
 ..//include/mud.h ..//include/global.h
objsys.o: objsys.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/objects.h ..//include/mobiles.h ..//include/locations.h \
 ..//include/oflagnames.h ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
parse.o: parse.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/verbs.h ..//include/locations.h ..//include/objects.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/colors.h
party.o: party.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/bprintf.h ..//include/sendsys.h ..//include/parse.h \
 ..//include/log.h ..//include/sflags.h ..//include/communicate.h \
 /usr/include/stdlib.h /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h ..//include/colors.h
pfilter.o: pfilter.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/macros.h ..//include/pflagnames.h ..//include/levelnames.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
rooms.o: rooms.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/locations.h ..//include/objects.h ..//include/exitnames.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
s_socket.o: s_socket.c /usr/include/stdc-predef.h \
 /usr/include/x86_64-linux-gnu/sys/types.h /usr/include/features.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/netdb.h \
 /usr/include/rpc/netdb.h /usr/include/x86_64-linux-gnu/bits/netdb.h \
 /usr/include/fcntl.h /usr/include/x86_64-linux-gnu/bits/fcntl.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl-linux.h \
 /usr/include/x86_64-linux-gnu/bits/stat.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl2.h ..//include/s_socket.h \
 /usr/include/unistd.h /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h /usr/include/string.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h
sendsys.o: sendsys.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
sflag.o: sflag.c /usr/include/stdc-predef.h ..//include/verbs.h \
 ..//include/kernel.h ..//include/_VERSION_ \
 /usr/include/x86_64-linux-gnu/sys/types.h /usr/include/features.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/sflag.h \
 ..//include/colors.h ..//include/communicate.h
timing.o: timing.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 /usr/include/x86_64-linux-gnu/sys/signal.h /usr/include/errno.h \
 /usr/include/x86_64-linux-gnu/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
uaf.o: uaf.c /usr/include/stdc-predef.h \
 /usr/include/x86_64-linux-gnu/sys/file.h /usr/include/features.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h /usr/include/fcntl.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl-linux.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/stat.h \
 /usr/include/x86_64-linux-gnu/bits/fcntl2.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h /usr/include/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/stat.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/ctype.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 /usr/include/errno.h /usr/include/x86_64-linux-gnu/bits/errno.h \
 /usr/include/linux/errno.h /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h /usr/local/include/gdbm.h \
 ..//include/machines.h
utils.o: utils.c /usr/include/stdc-predef.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/utils.h ..//include/stdinc.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
wizard.o: wizard.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/levels.h ..//include/verbs.h ..//include/locations.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h ..//include/objects.h \
 ..//include/mobiles.h
wizlist.o: wizlist.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
writer.o: writer.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h
zones.o: zones.c /usr/include/stdc-predef.h ..//include/kernel.h \
 ..//include/_VERSION_ /usr/include/x86_64-linux-gnu/sys/types.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/select2.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/sys/param.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
 /usr/include/limits.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
 /usr/include/x86_64-linux-gnu/bits/local_lim.h \
 /usr/include/linux/limits.h \
 /usr/include/x86_64-linux-gnu/bits/posix2_lim.h /usr/include/signal.h \
 /usr/include/x86_64-linux-gnu/bits/signum.h \
 /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
 /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
 /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
 /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
 /usr/include/x86_64-linux-gnu/bits/sigaction.h \
 /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
 /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
 /usr/include/x86_64-linux-gnu/sys/ucontext.h \
 /usr/include/x86_64-linux-gnu/bits/sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
 /usr/include/x86_64-linux-gnu/bits/sigthread.h \
 /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
 /usr/include/x86_64-linux-gnu/bits/param.h /usr/include/linux/param.h \
 /usr/include/x86_64-linux-gnu/asm/param.h \
 /usr/include/asm-generic/param.h /usr/include/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp2.h /usr/include/netinet/in.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/x86_64-linux-gnu/sys/socket.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
 /usr/include/x86_64-linux-gnu/bits/socket.h \
 /usr/include/x86_64-linux-gnu/bits/socket_type.h \
 /usr/include/x86_64-linux-gnu/bits/sockaddr.h \
 /usr/include/x86_64-linux-gnu/asm/socket.h \
 /usr/include/asm-generic/socket.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
 /usr/include/asm-generic/posix_types.h \
 /usr/include/x86_64-linux-gnu/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h \
 /usr/include/x86_64-linux-gnu/asm/sockios.h \
 /usr/include/asm-generic/sockios.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h \
 /usr/include/x86_64-linux-gnu/bits/socket2.h \
 /usr/include/x86_64-linux-gnu/bits/in.h /usr/include/stdio.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
 /usr/include/x86_64-linux-gnu/bits/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/stdio2.h /usr/include/ctype.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/string.h /usr/include/strings.h \
 /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
 /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
 ..//include/machines.h ..//include/machine/linux.h ..//include/config.h \
 ..//include/levels.h ..//include/exits.h ..//include/types.h \
 ..//include/utils.h ..//include/mudtypes.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/sys/time.h ..//include/mudmacros.h \
 ..//include/macros.h ..//include/extern.h ..//include/files.h \
 ..//include/stdinc.h /usr/include/stdlib.h \
 /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h /usr/include/alloca.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/x86_64-linux-gnu/bits/stdlib.h /usr/include/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
 /usr/include/x86_64-linux-gnu/bits/environments.h \
 /usr/include/x86_64-linux-gnu/bits/confname.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
 /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
 /usr/include/x86_64-linux-gnu/bits/unistd.h \
 /usr/include/x86_64-linux-gnu/bits/unistd_ext.h ..//include/actions.h \
 ..//include/board.h ..//include/bootstrap.h ..//include/bprintf.h \
 ..//include/calendar.h ..//include/cflags.h ..//include/change.h \
 ..//include/clone.h ..//include/commands.h ..//include/debug.h \
 ..//include/exec.h ..//include/fight.h ..//include/flags.h \
 ..//include/frob.h ..//include/jmp.h ..//include/lflags.h \
 ..//include/log.h ..//include/magic.h ..//include/mflags.h \
 ..//include/misc.h ..//include/mobile.h ..//include/move.h \
 ..//include/mud.h ..//include/objsys.h ..//include/oflags.h \
 ..//include/parse.h ..//include/pflags.h ..//include/quests.h \
 ..//include/rooms.h ..//include/s_socket.h ..//include/sendsys.h \
 ..//include/sflags.h ..//include/timing.h ..//include/uaf.h \
 ..//include/weather.h ..//include/wizard.h ..//include/wizlist.h \
 ..//include/writer.h ..//include/zones.h ..//include/admin.h \
 ..//include/verbs.h ..//include/kernel.h ..//include/stdinc.h \
 ..//include/communicate.h ..//include/sflag.h ..//include/game.h \
 ..//include/main.h ..//include/hate.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
