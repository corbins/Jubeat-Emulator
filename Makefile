COMPILER = g++
COMPILER_FLAGS = -c -g -Wall -Werror -std=c++11 -D__LINUX_ALSA__
LINKER = g++

jubeat: display.o audio.o input.o song.o note.o parser.o game.o jubeat.o
	$(LINKER) jubeat.o game.o parser.o note.o song.o input.o audio.o display.o -o jubeat -lSDL2 -lSDL2_image -lSDL2_mixer ./lib/rtmidi/tests/Release/RtMidi.o -lasound -lpthread

jubeat.o: jubeat.cpp
	$(COMPILER) $(COMPILER_FLAGS) jubeat.cpp

game.o: src/game.cpp src/game.h src/audio.h src/input.h src/display.h src/song.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/game.cpp

song.o: src/song.cpp src/song.h src/note.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/song.cpp

parser.o: src/parser.cpp src/parser.h src/song.h lib/utf8.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/parser.cpp

display.o: src/display.cpp src/display.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/display.cpp

audio.o: src/audio.cpp src/audio.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/audio.cpp

input.o: src/input.cpp src/audio.h src/song.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/input.cpp

note.o: src/note.cpp src/note.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/note.cpp

clean:
	-rm -f *.o jubeat
