/* Simple program to demonstrate a UDP-based server.
 * Loops forever.
 * Receives a word from the client. 
 * Sends back the length of that word to the client.
 * 
 * Compile using "cc -o wordlen-UDPserver wordlen-UDPserver.c"
 *
 * Yours: Design your own simple data transformation that is different from those above,
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
#include <ctype.h>
#include "../const.h"

/* Verbose debugging */
#define DEBUG 1


/*
 * Encodes in leet
 *
 * Translation based on:
 * http://www.robertecker.com/hp/research/leet-converter.php?lang=en
 * */
void leet(char *messagein){
    int lengthString = strlen(messagein);
    for (int i = 0; i < lengthString; ++i) {
        if (isalpha(messagein[i]))
            messagein[i] = tolower(messagein[i]);

        switch (messagein[i]){
            case 'a':
                messagein[i] = '4';
                break;
            case '4':
                messagein[i] = 'a';
                break;
            case 'b':
                messagein[i] = '8';
                break;
            case '8':
                messagein[i] = 'b';
                break;
            case 'c':
                messagein[i] = '<';
                break;
            case '<':
                messagein[i] = 'c';
                break;
            case 'd':
                messagein[i] = ']';
                break;
            case ']':
                messagein[i] = 'd';
                break;
            case 'e':
                messagein[i] = '3';
                break;
            case '3':
                messagein[i] = 'e';
                break;
            case 'i':
                messagein[i] = '1';
                break;
            case '1':
                messagein[i] = 'i';
                break;
            case 'o':
                messagein[i] = '0';
                break;
            case '0':
                messagein[i] = 'o';
                break;
            case 's':
                messagein[i] = '5';
                break;
            case '5':
                messagein[i] = 's';
                break;
            case 't':
                messagein[i] = '7';
                break;
            case '7':
                messagein[i] = 't';
                break;
            case 'x':
                messagein[i] = '%';
                break;
            case '%':
                messagein[i] = 'x';
                break;
            case 'z':
                messagein[i] = '2';
                break;
            case '2':
                messagein[i] = 'z';
                break;
            default:
                break;
        }
    }
}


/*
 *
 * Printing binary of char: https://stackoverflow.com/questions/18327439/printing-binary-representation-of-a-char-in-c
 * */
/*
void oneTime(char *messageIn) {
	char keyString[] = "This is the secret key!";
	int keyLength = strlen(keyString);
//	time_t t;
//	int byte;
//	srand((unsigned) time(&t));
//	for (int j = 0; j < MAX_MESSAGE_LENGTH; ++j) {
//		byte = rand() % 256;
//		keyString[j] = byte;
//	}

	printf("ORIGINAL XOR   KEY    = ENCRYPTED:\n");
	for (int i = 0; messageIn[i] != '\0'; ++i) {
	    int keyPos = i % keyLength;
	    printf("%d ",keyPos);
		for (int j = 0; j < 8; ++j) {
			printf("%d", !!((messageIn[i] << j) & 0x80));

		}

		printf(" XOR ");
            for (int j = 0; j < 8; ++j) {
                printf("%d", !!((keyString[keyPos] << j) & 0x80));

		}
		printf(" = ");

        messageIn[i] = messageIn[i] ^ keyString[keyPos];
        for (int j = 0; j < 8; ++j) {
			printf("%d", !!((messageIn[i] << j) & 0x80));
		}
		printf(" \n");


	}
}
*/


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
	si_server.sin_port = htons(LEETSPEAK_PORT);
	si_server.sin_addr.s_addr = htonl(INADDR_ANY);
	server = (struct sockaddr *) &si_server;
	client = (struct sockaddr *) &si_client;

	if (bind(s, server, sizeof(si_server)) == -1) {
		printf("Could not bind to port %d!\n", LEETSPEAK_PORT);
		return 1;
	}
	fprintf(stderr, "Welcome! I am the one time pad server!!\n");
	printf("server now listening on UDP port %d...\n", LEETSPEAK_PORT);

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

		/* create the outgoing message (as an ASCII string) */
		leet(messagein);

#ifdef DEBUG
		printf("Server sending back the message: \"%s\"\n", messagein);
#endif

		/* send the result message back to the client */
		sendto(s, messagein, strlen(messagein), 0, client, len);
	}

	close(s);
	return 0;
}
