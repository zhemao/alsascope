LDFLAGS=-lasound -lm -ldl -lncurses
CFLAGS=-Wall -O2

all: alsascope printmic

alsascope: alsascope.c alsahelpers.o

printmic: printmic.c alsahelpers.o

alsahelpers.o: alsahelpers.c alsahelpers.h

clean:
	rm -f alsascope
