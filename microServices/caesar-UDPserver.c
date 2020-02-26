/* Simple program to demonstrate a UDP-based server.
 * Loops forever.
 * Receives a word from the client. 
 * Sends back the length of that word to the client.
 * 
 * Compile using "cc -o wordlen-UDPserver wordlen-UDPserver.c"
 *
 * TODO: Caesar: This transformation applies a simple Caesar cipher to all alphabetic symbols (i.e., a-zA-Z)
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

/* Manifest constants */

/* Verbose debugging */
#define DEBUG 1

char * caesar (char * messageIn){
	char character;
	static const int offset = 13;
	for(int j = 0; messageIn[j] != '\0'; ++j){

		character = messageIn[j];

		if(character >= 'a' && character <= 'z'){

			character = character + offset;

			if(character > 'z'){

				character = character - 'z' + 'a' - 1;

			}

			messageIn[j] = character;

		}

		else if(character >= 'A' && character <= 'Z'){

			character = character + offset;

			if(character > 'Z'){

				character = character - 'Z' + 'A' - 1;

			}

			messageIn[j] = character;

		}
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
	const char * caesarString = caesar(messagein);
	sprintf(messageout, "%s", caesarString );

#ifdef DEBUG
	printf("Server sending back the message: \"%s\"\n", caesarString);
#endif

	/* send the result message back to the client */
	sendto(s, messageout, strlen(messageout), 0, client, len);
      }
  }
