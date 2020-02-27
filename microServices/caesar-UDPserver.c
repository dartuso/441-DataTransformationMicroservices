/* Simple program to demonstrate a UDP-based server.
 * Loops forever.
 * Receives a word from the client. 
 * Sends back the length of that word to the client.
 * 
 * Compile using "cc -o wordlen-UDPserver wordlen-UDPserver.c"
 *
 * Caesar: This transformation applies a simple Caesar cipher to all alphabetic symbols (i.e., a-zA-Z)
 * in a message. Recall that a Caesar cipher adds a fixed offset to each letter (with wraparound).
 * Please use a fixed offset of 13, and preserve the case of each letter.
 * Anything that is not a letter of the alphabet remains unchanged.
 * For example, the message "I love cats!" would become "V ybir pngf!".
 */
 
/* Include files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../const.h"
#include <ctype.h>
/* Manifest constants */

/* Verbose debugging */
#define DEBUG 1
#define ROTATION 13
#define ALPHABET 26

int rotation(int c)
{

	if (isalpha(c)) {
		char alpha = islower(c) ? 'a' : 'A';
		return (c - alpha + ROTATION) % ALPHABET + alpha;
	}
	return c;
}

void caesar (char * messageIn){
	unsigned int length = strlen(messageIn);
	for(int j = 0; j < length; ++j){
		messageIn[j] = rotation(messageIn[j]);
	}
}

/* Main program */
int main()
  {
    struct sockaddr_in si_server, si_client;
    struct sockaddr *server, *client;
    int s, len=sizeof(si_server);
    char messagein[MAX_MESSAGE_LENGTH];
    int readBytes;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not setup a socket!\n");
	return 1;
      }
    
    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(CAESAR_PORT);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);
    server = (struct sockaddr *) &si_server;
    client = (struct sockaddr *) &si_client;

    if (bind(s, server, sizeof(si_server))==-1)
      {
	printf("Could not bind to port %d!\n", CAESAR_PORT);
	return 1;
      }
    fprintf(stderr, "Welcome! I am the caesar cipher server!!\n");
    printf("server now listening on UDP port %d...\n", CAESAR_PORT);
	
    /* big loop, looking for incoming messages from clients */
//    for( ; ; )      {
	/* clear out message buffers to be safe */
	bzero(messagein, MAX_MESSAGE_LENGTH);

	/* see what comes in from a client, if anything */
	if ((readBytes=recvfrom(s, messagein, MAX_MESSAGE_LENGTH, 0, client, (socklen_t *) &len)) < 0)
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
	caesar(messagein);

#ifdef DEBUG
	printf("Server sending back the message: \"%s\"\n", messagein);
#endif

	/* send the result message back to the client */
	sendto(s, messagein, strlen(messagein), 0, client, len);
//      }
	close(s);
	return 0;
  }
