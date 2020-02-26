/* TCP-based client example of socket programming.    */
/* This client interacts with the word length server, */
/* which needs to be running first.                   */
/*                                                    */
/* Usage: cc -o wordlen-TCPclient wordlen-TCPclient.c */
/*        ./wordlen-TCPclient                         */
/*                                                    */
/* Written by Carey Williamson       January 13, 2012 */

/* Include files for C socket programming and stuff */
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include "const.h"


/* Manifest constants used by client program */



/* Prompt the user to enter a word */
void printMenu() {
	printf("\n");
	printf("Enter all choices desired in order of completion:\n");
	printf("  1 - Identity\n");
	printf("  2 - Reverse\n");
	printf("  3 - Upper\n");
	printf("  4 - Lower\n");
	printf("  5 - Caesar\n");
	printf("  6 - Leet Speak\n");
	printf("or 0 to exit program.\n");
	printf("Your desired menu selection(s)?: ");
}

/* Main program of client */
using namespace std;
int main() {
	int socketDescriptor;
    struct sockaddr_in server{};
	struct hostent *hp;
	string hostname;
	string message;
    string choices;
    int len, bytes;

	/* Initialization of server sockaddr data structure */
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

/*#ifdef BYNAME
	*//* use a resolver to get the IP address for a domain name *//*
	*//* I did my testing using csx1 (136.159.5.25)    Carey *//*
	strcpy(hostname, "csx1.cpsc.ucalgary.ca");
	hp = gethostbyname(hostname);
	if (hp == nullptr) {
		fprintf(stderr, "%s: unknown host\n", hostname);
		exit(1);
	}
	*//* copy the IP address into the sockaddr structure *//*
	bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
#else*/
	/* hard code the IP address so you don't need hostname resolver */
//	server.sin_addr.s_addr = inet_addr("136.159.5.25");
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    printf("Using ip: %s\n",SERVER_IP);
//#endif


	/* create the client socket for its transport-level end point */
	if ((socketDescriptor = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Error: socket() call failed!\n");
		exit(1);
	}

	/* connect the socket to the server's address using TCP */
	if (connect(socketDescriptor, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) {
		fprintf(stderr, "Error: connect() call failed!\n");
		perror(nullptr);
		exit(1);
	}

	/* Print welcome banner */
	printf("Welcome! This is Daniel's data transformation service\n");
        message.clear();
        cout <<"Enter your string: ";
        getline(cin, message);

        /* main loop: read a word, send to server, and print answer received */
        int choiceNumbers = 0;
        while (true) {
            /* get rid of newline after the (integer) menu choice given */

//		get choices and send them first
            printMenu();
            cin >> choiceNumbers;
            choices = to_string(choiceNumbers);
            if (choices.find_first_of("789") != string::npos) {
                cerr << "Invalid choice!" << endl;
                break;
            } else if (choiceNumbers == 0) {
                break;
            }

            string combinedString = choices;
            combinedString.append("$");
            combinedString.append(message);

            send(socketDescriptor, combinedString.c_str(), combinedString.size(), 0);

            combinedString.clear();
            /* see what the server sends back */
            char recvMessage[MAX_MESSAGE_LENGTH];
            bzero(recvMessage, MAX_MESSAGE_LENGTH);
            if ((bytes = recv(socketDescriptor, recvMessage, MAX_MESSAGE_LENGTH, 0)) > 0) {
                string recvString(recvMessage);
                cout << "Answer received from server: " << recvMessage << endl;
            } else {
                /* an error condition if the server dies unexpectedly */
                cerr << "Sorry, dude. Server failed!\n";
                close(socketDescriptor);
                exit(1);
            }

            std::cin.ignore();
        }

/* Program all done, so clean up and exit the client */
	close(socketDescriptor);
	exit(0);
}
