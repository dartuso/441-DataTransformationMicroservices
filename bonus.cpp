/* Include files for C socket programming and stuff */
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <csignal>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <ctime>

#include "const.h"
#include "caesar.h"
#include "identity.h"
#include "lower.h"
#include "reverse.h"
#include "upper.h"
#include "yours.h"
/* Optional verbose debugging output */
#define DEBUG 1

using namespace std;
/* Global variable */
int childSocketFD;
char message[MAX_MESSAGE_LENGTH];

void dispatchUDP(const string &choiceString);

void callUDP(int port);

void callServer(int port);
void spinUpCall (int port);
/* This is a signal handler to do graceful exit if needed */
void catcher(int sig) {
	close(childSocketFD);
	exit(sig);
}

int main() {
	struct sockaddr_in server{};
	static struct sigaction act;
	int ParentSocketFD;
	int pid;

	/* Set up a signal handler to catch some weird termination conditions. */
	act.sa_handler = catcher;
	sigfillset(&(act.sa_mask));
	sigaction(SIGPIPE, &act, nullptr);

	/* Initialize server sockaddr structure */
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	/* set up the transport-level end point to use TCP */
	if ((ParentSocketFD = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Error: socket() call failed!\n");
		exit(1);
	}

	/* bind a specific address and port to the end point */
	if (bind(ParentSocketFD, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) {
		fprintf(stderr, "Error: bind() call failed!\n");
		exit(1);
	}

	/* start listening for incoming connections from clients */
	if (listen(ParentSocketFD, 5) == -1) {
		cerr << "Error: listen() call failed!\n";
		exit(1);
	}


	for (;;) {
		/* accept a connection */
		if ((childSocketFD = accept(ParentSocketFD, nullptr, nullptr)) == -1) {
			cerr << "Error: accept() call failed!\n";
			exit(1);
		}

		/* try to create a child process to deal with this new client */
		pid = fork();

		/* use process id (pid) returned by fork to decide what to do next */
		if (pid < 0)
		{
			cerr << "Error: fork() call failed!\n";
			exit(1);
		}
		else if (pid == 0) {
			/* the child process is the one doing the "then" part */
			/* don't need the parent listener socket that was inherited */
			close(ParentSocketFD);

			/* obtain the message from this client */
			while (recv(childSocketFD, message, MAX_MESSAGE_LENGTH, 0) > 0) {
				string choiceString;
				string messageString;
				string combinedMessage(message);
				const int beginning = 0;

				int choiceEnd = combinedMessage.find_first_of('$');

				choiceString = combinedMessage.substr(beginning, choiceEnd);
				messageString = combinedMessage.substr(choiceEnd + 1);
				strcpy(message, messageString.c_str());

				cout << "Choices are: " << choiceString << "\n";
				cout << "Message is: " << messageString << "\n";

				dispatchUDP(choiceString);

				/* send the result message back to the client */
				send(childSocketFD, message, strlen(message), 0);

				/* clear out message strings again to be safe */
				bzero(message, MAX_MESSAGE_LENGTH);
			}

			/* when client is no longer sending information to us, */
			/* the socket can be closed and the child process terminated */
			close(childSocketFD);
			exit(0);
		} /* end of then part for child */
		else {
			/* the parent process is the one doing the "else" part */
			cout << "Created child process to handle that client\n";
			cout << "Parent going back to job of listening...\n\n";

			/* parent doesn't need the childSocketFD */
			close(childSocketFD);
		}
	}
}

void dispatchUDP(const string& choiceString){
	for (char i : choiceString) {
		cerr << "Performing choice: " << i << '\n';
		switch (i) {
			case IDENTITY:
				spinUpCall(IDENTITY_PORT);
				break;
			case REVERSE:
                spinUpCall(REVERSE_PORT);
				break;
			case UPPER:
                spinUpCall(UPPER_PORT);
				break;
			case LOWER:
                spinUpCall(LOWER_PORT);
				break;
			case CAESAR:
                spinUpCall(CAESAR_PORT);
				break;
			case LEET:
                spinUpCall(LEETSPEAK_PORT);
				break;
			default:
				cerr <<"Error in selection!\n";
				bzero(message, MAX_MESSAGE_LENGTH);
				strcpy(message, "Error in number selection!\n");
				break;
		}
	}
}

void spinUpCall (const int port){
    int pid = fork();

    /* use process id (pid) returned by fork to decide what to do next */
    if (pid < 0)
    {
        cerr << "Error: fork() call failed!\n";
        exit(1);
    }
    else if (pid == 0)
    {
        cout << "Client calling server: \n";
        callServer(port);
    } else {
        sleep(1);
        cout << "Parent sending message:" << endl;
        callUDP(port);
    }
}

void callServer(const int port) {
    int returnStatus = 0;
    switch (port) {
        case IDENTITY_PORT:
            returnStatus = identityServer();
            break;
        case REVERSE_PORT:
            returnStatus = reverseServer();
            break;
        case UPPER_PORT:
            returnStatus = upperServer();
            break;
        case LOWER_PORT:
            returnStatus = lowerServer();
            break;
        case CAESAR_PORT:
            returnStatus = caesarServer();
            break;
        case LEETSPEAK_PORT:
            returnStatus = yoursServer();
            break;
        default:
            cout << "Error invalid port!\n";
            break;
    }
    exit(returnStatus);
}

void callUDP(const int port) {
	cout << "Port " << port << endl;
	struct sockaddr_in si_server{};
	struct sockaddr *server;
	int s, len = sizeof(si_server);

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		strcpy(message, "Could not set up a socket!\n");
		printf("Could not set up a socket!\n");
	}

	memset((char *) &si_server, 0, sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(port);
	server = (struct sockaddr *) &si_server;

	if (inet_pton(AF_INET, SERVER_IP, &si_server.sin_addr) == 0) {
		strcpy(message, "inet_pton() failed\n");
		printf("inet_pton() failed\n");
	}

	printf("About to send to UDP on port: %d\n", port);
	printf("About to send message: %s\n", message);

	if (sendto(s, message, strlen(message), 0, server, sizeof(si_server)) == -1) {
		strcpy(message, "inet_pton() failed\n");
		printf("sendto failed\n");

	}


	//300 Ms timeout via: https://stackoverflow.com/questions/13547721/udp-socket-set-timeout
	struct timeval tv{};
	tv.tv_sec = 0;
	tv.tv_usec = 300000;
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		perror("Error");
	}


	bzero(message, MAX_MESSAGE_LENGTH);
	if ((recvfrom(s, message, MAX_MESSAGE_LENGTH, 0, server, reinterpret_cast<socklen_t *>(&len))) == -1) {
		strcpy(message, "Read Error\n");
		printf("Read error!\n");

	}
	printf("Received back: %s\n\n", message);
}


