all:
	gcc src/prg.c src/paprg.c lib/libportaudio.a -I include/ -lrt -lm -lasound -ljack -pthread -o prg
