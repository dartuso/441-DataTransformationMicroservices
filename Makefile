build: server client caeasr identity leetspeak lower reverse upper

clean:
	rm -f *.out

server: masterServer.cpp const.h
	g++ masterServer.cpp const.h -o masterServer.out

client: masterClient.cpp const.h
	g++ masterClient.cpp const.h -o masterClient.out

caeasr: microServices/caesar-UDPserver.c const.h
	g++ microServices/caesar-UDPserver.c const.h -o caesar.out

identity: microServices/identity-UDPserver.c const.h
	g++ microServices/identity-UDPserver.c const.h -o identity.out

leetspeak: microServices/leetspeak-UDPserver.c const.h
	g++ microServices/leetspeak-UDPserver.c const.h -o leetspeak.out

lower: microServices/lower-UDPserver.c const.h
	g++ microServices/lower-UDPserver.c const.h -o lower.out

reverse: microServices/reverse-UDPserver.c const.h
	g++ microServices/reverse-UDPserver.c const.h -o reverse.out

upper: microServices/upper-UDPserver.c const.h
	g++ microServices/upper-UDPserver.c const.h -o upper.out