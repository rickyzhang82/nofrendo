INCLUDES = -Icpu -Ilibsnss -Isndhrdw -Imappers -Ines -Isdl -I$(SYSTEM)

SUBDIRS = cpu libsnss sndhrdw mappers nes sdl $(SYSTEM)

bin_PROGRAMS = nofrendo

nofrendo_SOURCES = bitmap.c \
	config.c \
	event.c \
	gui.c \
	gui_elem.c \
	intro.c \
	log.c \
	memguard.c \
	nofrendo.c \
	pcx.c \
	vid_drv.c

nofrendo_LDADD = $(SDL_LIBS) nes/libnes.a  mappers/libmappers.a sndhrdw/libsndhrdw.a libsnss/libsnss.a cpu/libcpu.a $(SYSTEM)/lib$(SYSTEM).a sdl/libsdl.a 
