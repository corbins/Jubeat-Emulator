COMPILER = g++
COMPILER_FLAGS = -c -g -O2 -Wall -Werror -std=c++11
LINKER = g++

jubeat: song.o parser.o jubeat.o
	$(LINKER) jubeat.o parser.o song.o -o jubeat ./lib/fmod/api/lib/libfmodex64.so

jubeat.o: jubeat.cpp
	$(COMPILER) $(COMPILER_FLAGS) jubeat.cpp

song.o: song.cpp song.h
	$(COMPILER) $(COMPILER_FLAGS) -c song.cpp

parser.o: parser.cpp parser.h song.h lib/utf8.h
	$(COMPILER) $(COMPILER_FLAGS) -c parser.cpp

clean:
	-rm -f *.o jubeat
