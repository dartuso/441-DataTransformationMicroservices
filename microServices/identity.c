/* Simple program to demonstrate a UDP-based server.
 * Loops forever.
 * Receives a word from the client. 
 * Sends back the length of that word to the client.
 * 
 * Compile using "cc -o wordlen-UDPserver wordlen-UDPserver.c"
 *
 * Identity: The identity transformation does nothing to the data,
 *      but merely returns exactly what was received. It is also known as an echo server.
 */

/* Include files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../const.h"


/* Main program */
int main() {
	struct sockaddr_in si_server, si_client;
	struct sockaddr *server, *client;
	int s, len = sizeof(si_server);
	char messagein[MAX_MESSAGE_LENGTH];
	char messageout[MAX_MESSAGE_LENGTH];
	int readBytes;

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		printf("Could not setup a socket!\n");
		return 1;
	}

	memset((char *) &si_server, 0, sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(IDENTITY_PORT);
	si_server.sin_addr.s_addr = htonl(INADDR_ANY);
	server = (struct sockaddr *) &si_server;
	client = (struct sockaddr *) &si_client;

	if (bind(s, server, sizeof(si_server)) == -1) {
		printf("Could not bind to port %d!\n", IDENTITY_PORT);
		return 1;
	}
	fprintf(stderr, "Welcome! I am the identity server!!\n");
	printf("server now listening on UDP port %d...\n", IDENTITY_PORT);

	/* clear out message buffers to be safe */
	bzero(messagein, MAX_MESSAGE_LENGTH);
	bzero(messageout, MAX_MESSAGE_LENGTH);

	/* see what comes in from a client, if anything */
	if ((readBytes = recvfrom(s, messagein, MAX_MESSAGE_LENGTH, 0, client, (socklen_t *) &len)) < 0) {
		printf("Read error!\n");
		return -1;
	}
#ifdef DEBUG
	else printf("Server received %d bytes\n", readBytes);
#endif

	printf("  server received \"%s\" from IP %s port %d\n",
	       messagein, inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));

	/* create the outgoing message (as an ASCII string) */
	sprintf(messageout, "%s", messagein);

#ifdef DEBUG
	printf("Server sending back the message: \"%s\"\n", messageout);
#endif

	/* send the result message back to the client */
	sendto(s, messageout, strlen(messageout), 0, client, len);

	close(s);
	return 0;
}
