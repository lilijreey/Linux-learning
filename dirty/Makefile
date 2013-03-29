all:

unit: unit.cpp
	g++ -g -Wall -std=c++0x -o $@ $^

main: main.cpp dirtyTable.cpp dirtyTable.hpp multiwayTree.hpp utf8_encode.h
#	g++ -g -Wall -std=c++0x -DDEBUG -o $@ $^
	g++ -O2 -Wall -std=c++0x -o $@ $^

clean:
	@rm -f *.o unit main
