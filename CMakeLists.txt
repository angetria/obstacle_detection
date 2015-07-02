cmake_minimum_required(VERSION 2.8)
project( hard_obstacles )

find_package( OpenCV REQUIRED )

add_executable( detector detector.cpp )

target_link_libraries( detector ${OpenCV_LIBS})
