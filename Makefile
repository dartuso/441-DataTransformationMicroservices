build: server client

clean:
	rm -f *.out

server:
	g++ bonus.cpp const.h caesar.c lower.c identity.c reverse.c upper.c yours.c -o mainserver.out

client:
	g++ mainclient.cpp const.h -o mainclient.out

runServer: clean server
	./bonus.out

runClient: clean client
	./mainclient.out

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