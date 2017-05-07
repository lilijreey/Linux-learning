CXX_INCLUDES = -isystem /usr/local/include -isystem /usr/local/include/opencv 
CXX_FLAGS=$(shell pkg-config --libs opencv)

tt: tt.cpp
	echo $(OK)
	g++ -Wall ${CXX_INCLUDES} -o tt $(CXX_FLAGS) tt.cpp
