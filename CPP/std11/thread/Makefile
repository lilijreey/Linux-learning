all:

OBJECTS = thread

thread: thread.cpp
	g++ -Wall -std=c++0x -o $@ $^ -lpthread

mutex: mutex.cpp
	g++ -Wall -std=c++0x -o $@ $^ -lpthread

once: call_once.cpp
	g++ -Wall -std=c++0x -o $@ $^ -lpthread

clean:
	@rm -rf *.o  ${OBJECTS}
