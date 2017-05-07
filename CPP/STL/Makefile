CXXFLAGS = -Wall -std=c++0x

OBJECTS = forward_list vector

forward_list: forward_list.cpp
	g++ $(CXXFLAGS) -o $@ $^

vector: vector.cpp
	g++ $(CXXFLAGS) -o $@ $^

map: map.cpp
	g++ $(CXXFLAGS) -o $@ $^

clear:
	@rm -f *.o $(OBJECTS)

