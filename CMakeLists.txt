cmake_minimum_required(VERSION 3.10)
project(441ASG2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server mainserver.cpp const.h)
add_executable(Client mainclient.cpp const.h)
#Microservices
add_executable(caesar microServices/caesar.c const.h)
add_executable(identity microServices/identity.c const.h)
add_executable(lower microServices/lower.c const.h)
add_executable(leet microServices/yours.c const.h)
add_executable(reverse microServices/reverse.c const.h)
add_executable(upper microServices/upper.c const.h)