AM_CFLAGS = -I$(top_srcdir)/include \
	    -Wall -W -Wno-unused-parameter -Wno-pointer-sign\
	    -DDATADIR=\"$(pkgdatadir)\"\
	    -DSRCDIR=\"$(top_srcdir)\"

bin_PROGRAMS = nasos

nasos_SOURCES = nasos.c gameplay.c display.c input.c timer.c
nasos_LDCONFIG = -lSDL2
