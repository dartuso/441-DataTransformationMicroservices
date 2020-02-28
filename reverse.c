/*
 * CPSC 441 Assignment 2
 * Daniel Artuso
 *
 * Reverse: This transformation reverses the order of the bytes in a message,
 *      and returns the result back. For example, the message "dog" would become "god".
 */

/* Include files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "const.h"


char *strrev(char *str);


/* Main program */
int reverseServer() {
    struct sockaddr_in si_server, si_client;
    struct sockaddr *server, *client;
    int s, len = sizeof(si_server);
    char messagein[MAX_MESSAGE_LENGTH];
    char *messageout = NULL;
    int readBytes;

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("Could not setup a socket!\n");
        return 1;
	}

	memset((char *) &si_server, 0, sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(REVERSE_PORT);
	si_server.sin_addr.s_addr = htonl(INADDR_ANY);
	server = (struct sockaddr *) &si_server;
	client = (struct sockaddr *) &si_client;

	if (bind(s, server, sizeof(si_server)) == -1) {
		printf("Could not bind to port %d!\n", REVERSE_PORT);
		return 1;
	}
	fprintf(stderr, "Welcome! I am the reverse word server!!\n");
	printf("server now listening on UDP port %d...\n", REVERSE_PORT);


	/* clear out message buffers to be safe */
	bzero(messagein, MAX_MESSAGE_LENGTH);

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

	messageout = strrev(messagein);

#ifdef DEBUG
	printf("Server sending back the message: \"%s\"\n", messageout);
#endif

	/* send the result message back to the client */
	sendto(s, messageout, strlen(messageout), 0, client, len);

	close(s);
	return 0;
}

char *strrev(char *str) {
	char *p1, *p2;

	if (!str || !*str)
		return str;
	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}