cmake_minimum_required(VERSION 3.10)
project(441ASG2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server masterServer.cpp const.h)
add_executable(Client masterClient.cpp const.h)
#Microservices
add_executable(caesar microServices/caesar-UDPserver.c const.h)
add_executable(identity microServices/identity-UDPserver.c const.h)
add_executable(lower microServices/lower-UDPserver.c const.h)
add_executable(onetime microServices/onetime-UDPserver.c const.h)
add_executable(reverse microServices/reverse-UDPserver.c const.h)
add_executable(upper microServices/upper-UDPserver.c const.h)