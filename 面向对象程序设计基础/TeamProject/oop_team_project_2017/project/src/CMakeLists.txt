# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.


project( src_ )

cmake_minimum_required(VERSION 2.8.12)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
    add_definitions(-O2)
endif()

find_package( OpenCV REQUIRED )
find_package( Boost COMPONENTS program_options REQUIRED )

include_directories( ${Boost_INCLUDE_DIRS} )

add_executable( main 
  main.cpp
  path.cpp
  route.cpp
  route_impl/route_network_flow.cpp
  route_impl/route_rule_based.cpp
  route_impl/route_input_adapter.cpp
  visualization.cpp
)

target_link_libraries( main ${OpenCV_LIBS} )
target_link_libraries( main ${Boost_LIBRARIES} )
