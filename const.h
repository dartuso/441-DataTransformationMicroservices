//
// Created by Daniel on 2020-02-25.
//

#ifndef INC_441ASG2_CONST_H
#define INC_441ASG2_CONST_H

static const char IDENTITY = '1',
                  REVERSE  = '2',
                  UPPER    = '3',
                  LOWER    = '4',
                  CAESAR   = '5',
                  LEET  = '6';

static const int MAX_MESSAGE_LENGTH = 2048 * 10;

static const int MAX_CHOICE_LENGTH = 7;

static const int SERVER_PORT = 9001;   /* client server port */
/*microservice port*/
static const int IDENTITY_PORT = 9002,
		REVERSE_PORT = 9003,
		UPPER_PORT = 9004,
		LOWER_PORT = 9005,
		CAESAR_PORT = 9006,
		LEETSPEAK_PORT = 9007;

//#define SERVER_IP "136.159.5.25"  /* csx.cpsc.ucalgary.ca */
#define SERVER_IP  "127.0.0.1"
#define USE_HOSTNAME 0
#define DEBUG 1

/*Client program*/
#define MAX_HOSTNAME_LENGTH 64
#endif //INC_441ASG2_CONST_H
