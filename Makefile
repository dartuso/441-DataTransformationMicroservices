build: server client caeasr identity leetspeak lower reverse upper

clean:
	rm -f *.out

server: masterServer.cpp const.h caesar.h lower.h identity.h reverse.h upper.h yours.h
	g++ masterServer.cpp const.h caesar.h lower.h identity.h reverse.h upper.h yours.h -o masterServer.out

client: masterClient.cpp const.h
	g++ masterClient.cpp const.h -o masterClient.out

#caeasr: caesar.c const.h
#	g++ microServices/caesar-UDPserver.c const.h -o caesar.out
#
#identity: identity.c const.h
#	g++ microServices/identity-UDPserver.c const.h -o identity.out
#
#leetspeak: yours.c const.h
#	g++ microServices/leetspeak-UDPserver.c const.h -o leetspeak.out
#
#lower: lower.c const.h
#	g++ microServices/lower-UDPserver.c const.h -o lower.out
#
#reverse: reverse.c const.h
#	g++ microServices/reverse-UDPserver.c const.h -o reverse.out
#
#upper: upper.c const.h
#	g++ microServices/upper-UDPserver.c const.h -o upper.out