AM_CFLAGS = -I$(top_srcdir)/include \
	    -Wall -W -Wno-unused-parameter -Wno-pointer-sign\
	    -DDATADIR=\"$(pkgdatadir)\"\
	    -DSRCDIR=\"$(top_srcdir)\"

bin_PROGRAMS = nasos

nasos_SOURCES = nasos.c gameplay.c gameplay_init.c gameplay_enemies.c\
		gameplay_player.c gameplay_fire.c display.c input.c timer.c\
		mixer.c render_game.c menu.c
nasos_LDCONFIG = -lSDL2
