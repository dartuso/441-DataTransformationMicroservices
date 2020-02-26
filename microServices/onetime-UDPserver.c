/* Simple program to demonstrate a UDP-based server.
 * Loops forever.
 * Receives a word from the client. 
 * Sends back the length of that word to the client.
 * 
 * Compile using "cc -o wordlen-UDPserver wordlen-UDPserver.c"
 *
 * TODO: Yours: Design your own simple data transformation that is different from those above,
 * somewhat interesting, reasonably easy to implement and explain, and
 * applicable to one or more of the data bytes in a typical message.

 Uses one time pad for nearly perfect encryption (not perfect since rand() is pseudorandom)
 http://www6.uniovi.es/cscene/CS4/CS4-03.html
	since the key is not saved it will be difficult to decrypt however
 */
 
/* Include files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<time.h>
#include <stdlib.h>
#include "../const.h"

/* Verbose debugging */
#define DEBUG 1


char* oneTime(char * messageIn){
	char keyString[MAX_MESSAGE_LENGTH];
	time_t t;
	int key;
	int data;
	int output;
	int count=0;
	int FLAG=0;
	int byte;

	srand((unsigned) time(&t));

	for (int j = 0; j < MAX_MESSAGE_LENGTH; ++j) {
		byte=rand() % 256;
		keyString[j] = byte;
	}

	for (int i = 0; messageIn[i] != '\0'; ++i) {
		messageIn[i] = messageIn[i] ^ keyString[i];
	}

	return messageIn;
}


/* Main program */
int main()
  {
    struct sockaddr_in si_server, si_client;
    struct sockaddr *server, *client;
    int s, i, len=sizeof(si_server);
    char messagein[MAX_MESSAGE_LENGTH];
    char messageout[MAX_MESSAGE_LENGTH];
    int readBytes;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not setup a socket!\n");
	return 1;
      }
    
    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(ONETIME_PORT);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);
    server = (struct sockaddr *) &si_server;
    client = (struct sockaddr *) &si_client;

    if (bind(s, server, sizeof(si_server))==-1)
      {
	printf("Could not bind to port %d!\n", ONETIME_PORT);
	return 1;
      }
    fprintf(stderr, "Welcome! I am the one time pad server!!\n");
    printf("server now listening on UDP port %d...\n", ONETIME_PORT);
	
    /* big loop, looking for incoming messages from clients */
    for( ; ; )
      {
	/* clear out message buffers to be safe */
	bzero(messagein, MAX_MESSAGE_LENGTH);
	bzero(messageout, MAX_MESSAGE_LENGTH);

	/* see what comes in from a client, if anything */
	if ((readBytes=recvfrom(s, messagein, MAX_MESSAGE_LENGTH, 0, client, &len)) < 0)
	  {
	    printf("Read error!\n");
	    return -1;
	  }
#ifdef DEBUG
	else printf("Server received %d bytes\n", readBytes);
#endif

	printf("  server received \"%s\" from IP %s port %d\n",
	       messagein, inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));

	/* create the outgoing message (as an ASCII string) */
	char * onetime = oneTime(messagein);
	sprintf(messageout, "%s", onetime);

#ifdef DEBUG
	printf("Server sending back the message: \"%s\"\n", messageout);
#endif

	/* send the result message back to the client */
	sendto(s, messageout, strlen(messageout), 0, client, len);		
      }

    close(s);
    return 0;
  }
