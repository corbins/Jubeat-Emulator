COMPILER = g++
COMPILER_FLAGS = -c -g -O0 -Wall -Werror -std=c++11
LINKER = g++

jubeat: song.o parser.o jubeat.o
	$(LINKER) jubeat.o parser.o song.o -o jubeat

jubeat.o: jubeat.cpp
	$(COMPILER) $(COMPILER_FLAGS) jubeat.cpp

song.o: song.cpp song.h
	$(COMPILER) $(COMPILER_FLAGS) song.cpp

parser.o: parser.cpp parser.h song.h lib/utf8.h
	$(COMPILER) $(COMPILER_FLAGS) parser.cpp

clean:
	-rm -f *.o jubeat
