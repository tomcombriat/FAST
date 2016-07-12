
cmake_minimum_required(VERSION 2.8)
project( Fast )
find_package( OpenCV REQUIRED )
find_package(Boost REQUIRED serialization)

add_executable( FAST sources/main.cpp sources/point.cpp sources/find_particules.cpp sources/track.cpp sources/link_particules.cpp )
target_link_libraries( FAST ${OpenCV_LIBS} )
target_link_libraries( FAST ${Boost_LIBRARIES} )
