cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )

add_executable( tt tt.cpp )
target_link_libraries( tt ${OpenCV_LIBS} )

add_executable( t2 t2.cpp )
target_link_libraries( t2 ${OpenCV_LIBS} )
