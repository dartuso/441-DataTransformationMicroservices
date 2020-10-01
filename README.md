Data Transformation Microservices
## Compliation
g++ bonus.cpp const.h caesar.c lower.c identity.c reverse.c upper.c yours.c -o mainserver.out
g++ mainclient.cpp const.h -o mainclient.out
or
make
(on command line)

## Run
Run mainserver.out (first)
Then run mainclient.out

## Configuration
SERVERIP to use is set in const.h
can also use USE_HOSTNAME 1 to resolve the hostname set in HOSTNAME function

## Use
On the client enter a string and choice 1 or more data transformations of:
Identity: The identity transformation does nothing to the data, but merely returns exactly what was received. It is also known as an echo server.
Reverse: This transformation reverses the order of the bytes in a message, and returns the result back. For example, the message "dog" would become "god".
Upper: This transformation changes all lower-case alphabetic symbols (i.e., a-z) in a message into upper case (i.e., A-Z). Anything that is already upper case remains unchanged, and anything that is not a letter of the alphabet remains unchanged. For example, the message "Canada 4 Russia 3" would become "CANADA 4 RUSSIA 3".
Lower: This transformation changes all upper-case alphabetic symbols (i.e., A-Z) in a message into lower case (i.e., a-z). Anything that is already lower case remains unchanged, and anything that is not a letter of the alphabet remains unchanged. For example, the message "Canada 4 Russia 3" would become "canada 4 russia 3".
Caesar: This transformation applies a simple Caesar cipher to all alphabetic symbols (i.e., a-zA-Z) in a message. Recall that a Caesar cipher adds a fixed offset to each letter (with wraparound). Please use a fixed offset of 13, and preserve the case of each letter. Anything that is not a letter of the alphabet remains unchanged. For example, the message "I love cats!" would become "V ybir pngf!".
Yours: Leet speak encoder/decoder (I love cats! -> 1 l0v3 <475!)
