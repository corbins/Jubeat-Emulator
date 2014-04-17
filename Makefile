COMPILER = g++
COMPILER_FLAGS = -c -g -O2 -Wall -Werror -std=c++11
LINKER = g++

jubeat: song.o parser.o jubeat.o
	$(LINKER) jubeat.o parser.o song.o -o jubeat ./lib/fmod/api/lib/libfmodex64.so

jubeat.o: jubeat.cpp
	$(COMPILER) $(COMPILER_FLAGS) jubeat.cpp

song.o: src/song.cpp src/song.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/song.cpp

parser.o: src/parser.cpp src/parser.h src/song.h lib/utf8.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/parser.cpp

clean:
	-rm -f *.o jubeat
