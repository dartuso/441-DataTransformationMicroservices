/* Simple program to demonstrate a UDP-based server.
 * Loops forever.
 * Receives a word from the client. 
 * Sends back the length of that word to the client.
 * 
 * Compile using "cc -o wordlen-UDPserver wordlen-UDPserver.c"
 *
 * TODO:Lower: This transformation changes all upper-case alphabetic symbols (i.e., A-Z)
 * in a message into lower case (i.e., a-z). Anything that is already lower case remains unchanged,
 * and anything that is not a letter of the alphabet remains unchanged.
 * For example, the message "Canada 4 Russia 3" would become "canada 4 russia 3".
 */

/* Include files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "../const.h"

/* Verbose debugging */
#define DEBUG 1

/* Main program */
int main() {
	struct sockaddr_in si_server, si_client;
	struct sockaddr *server, *client;
	int s, i, len = sizeof(si_server);
	char messagein[MAX_MESSAGE_LENGTH];
	char messageout[MAX_MESSAGE_LENGTH];
	int readBytes;

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		printf("Could not setup a socket!\n");
		return 1;
	}

	memset((char *) &si_server, 0, sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(LOWER_PORT);
	si_server.sin_addr.s_addr = htonl(INADDR_ANY);
	server = (struct sockaddr *) &si_server;
	client = (struct sockaddr *) &si_client;

	if (bind(s, server, sizeof(si_server)) == -1) {
		printf("Could not bind to port %d!\n", LOWER_PORT);
		return 1;
	}
	fprintf(stderr, "Welcome! I am the lower case server!!\n");
	printf("server now listening on UDP port %d...\n", LOWER_PORT);

	/* big loop, looking for incoming messages from clients */
	for (;;) {
		/* clear out message buffers to be safe */
		bzero(messagein, MAX_MESSAGE_LENGTH);
		bzero(messageout, MAX_MESSAGE_LENGTH);

		/* see what comes in from a client, if anything */
		if ((readBytes = recvfrom(s, messagein, MAX_MESSAGE_LENGTH, 0, client, &len)) < 0) {
			printf("Read error!\n");
			return -1;
		}
#ifdef DEBUG
		else printf("Server received %d bytes\n", readBytes);
#endif

		printf("  server received \"%s\" from IP %s port %d\n",
		       messagein, inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));

		for (int j = 0; j < strlen(messagein); ++j) {
			messageout[j] = tolower(messagein[j]);
		}

#ifdef DEBUG
		printf("Server sending back the message: \"%s\"\n", messageout);
#endif

		/* send the result message back to the client */
		sendto(s, messageout, strlen(messageout), 0, client, len);
	}

	close(s);
	return 0;
}
