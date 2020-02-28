cmake_minimum_required(VERSION 3.10)
project(441ASG2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server mainserver.cpp const.h caesar.h identity.h lower.h reverse.h upper.h yours.h)
add_executable(Client mainclient.cpp const.h)
#Microservices
add_executable(caesar caesar.c const.h caesar.h)
add_executable(identity identity.c const.h identity.h)
add_executable(lower lower.c const.h lower.h)
add_executable(leet yours.c const.h yours.h)
add_executable(reverse reverse.c const.h reverse.h)
add_executable(upper upper.c const.h upper.h)