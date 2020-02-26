Assignment 2: Data Transformation Services (40 marks)
Due: Friday, February 28, 2020 (11:30pm)
Learning Objectives
The purpose of this assignment is to learn about client-server network applications, transport-layer protocols, and data representation. In particular, you will write some client-server programs (in C or C++) that implement several different data transformation services, using a combination of TCP and UDP.

Background Scenario
The modern Internet offers many different online services for users, such as language translation, currency conversion, mortgage calculators, time zone conversion, and many more. In fact, the current trend is toward service chaining of virtualized network functions, which we will see much later in the semester when we discuss software-defined networking.

In this assignment, we will tinker with some simple "micro-services" for word manipulation, and then dynamically compose them in different ways to achieve some different desired data representations.

Your Task
Your primary task in this assignment is to build a network-based application that offers several different data transformation services to users. Each of these micro-services will run as its own server, and you will use a master server to coordinate each session of data transformation services.

The master server operates on sentence-like messages entered by the user, and uses TCP as its transport-layer protocol, for reliable data transfer with the client. The client's interaction with the master server will involve connecting to the server, entering a sentence of one or more words to be used as the source data, and then entering a loop for interaction with the server. Within the loop, the client can specify what data transformations are desired on the original sentence source data, and in what order. These requests may involve one or more data transformations, to be performed in the order specified. The master server then communicates with the micro-services via UDP to perform the composed data transformations on each word, prior to returning the final result data back to the client via TCP. Additional client commands can be sent to apply new transformations to the same original sentence source data. When the client is finished with all data transformation requests, the session with the master server ends.

The micro-services can operate on a word or a sentence at a time (your choice). These data transformation services will be offered via UDP-based communication, which is simple and fast, but unreliable. There will be several different micro-services running, each on a different port. The master server needs to know which services are running where (i.e., IP address and port), and send the data to the correct place for each data transformation request.

The specific data transformation micro-services that you need to implement are:

Identity: The identity transformation does nothing to the data, but merely returns exactly what was received. It is also known as an echo server.
Reverse: This transformation reverses the order of the bytes in a message, and returns the result back. For example, the message "dog" would become "god".
Upper: This transformation changes all lower-case alphabetic symbols (i.e., a-z) in a message into upper case (i.e., A-Z). Anything that is already upper case remains unchanged, and anything that is not a letter of the alphabet remains unchanged. For example, the message "Canada 4 Russia 3" would become "CANADA 4 RUSSIA 3".
Lower: This transformation changes all upper-case alphabetic symbols (i.e., A-Z) in a message into lower case (i.e., a-z). Anything that is already lower case remains unchanged, and anything that is not a letter of the alphabet remains unchanged. For example, the message "Canada 4 Russia 3" would become "canada 4 russia 3".
Caesar: This transformation applies a simple Caesar cipher to all alphabetic symbols (i.e., a-zA-Z) in a message. Recall that a Caesar cipher adds a fixed offset to each letter (with wraparound). Please use a fixed offset of 13, and preserve the case of each letter. Anything that is not a letter of the alphabet remains unchanged. For example, the message "I love cats!" would become "V ybir pngf!".
Yours: Design your own simple data transformation that is different from those above, somewhat interesting, reasonably easy to implement and explain, and applicable to one or more of the data bytes in a typical message.
Please note (and obey) the numbering on the data transformation services above, since they will be used for the client commands sent to the master server. For example, if the source data "I love cats!" is transformed using the command "2153", then the resulting data would be "!FGNP RIBY V" if operating on a sentence at a time, or "V RIBY !FGNP" if operating on a word at a time. It is up to your master server to coordinate this, and either is acceptable. Your master server should also do something reasonable if no response is received from one of the UDP-based micro-services within a certain time limit.

Grading Scheme
The grading scheme for the assignment is as follows:

14 marks for the design and implementation of the main TCP-based client-server solution to this problem. Your implementation should include proper use of TCP and UDP socket programming in C or C++, and reasonably commented code.
12 marks for a proper implementation of each of the UDP-based data transformation services (6 of them, each worth 2 marks)
4 marks for a clear and concise user manual (at most 1 page) that describes how to compile, configure, and use your application. Also describe where and how your testing was done (e.g., home, university, office), what works, and what does not. Be honest!
10 marks for a suitable demonstration of your data transformation services to your TA in your tutorial section, or to your professor at a mutually convenient time. All demos will be done during the week of March 9 (after the midterm, and once the Brier is over).
When you are finished, please submit your assignment solution to your TA via D2L. Your submission should include your source code and user manual documentation.

Bonus (optional): Up to 4 bonus marks are available if your master server allows the dynamic creation, registration, and termination of micro-services while the master server is running, rather than having them all started in advance and/or statically assigned.

Tips
Look at Carey's "word length server" examples, which might provide some insights about how to do TCP and UDP socket programming, and how to send and receive ASCII data.
On a single machine, you can do your initial testing using the loopback interface.
For the micro-services, start with the echo server, which is the simplest one. Once it is working, you can make the small changes required to create the other micro-services. Test them individually, perhaps on a word at a time.
When debugging the master server, use just one micro-service initially. Then try it with two, and make sure to keep track of which is which. And so on.
It might be helpful to have the master server put some fixed delays (e.g., 1 second) in between the different stages of data transformation when you are doing composed micro-services.
Once you are testing on a real network interface, you may find Wireshark particularly useful to look at the network packets being sent and received by your application (i.e., ports, size, data content).
Stick with plain-text ASCII content for your testing. Make sure you know how to convert between numeric data and ASCII data, and use only the latter when sending data content over the network.
Work with very small amounts of data (e.g., 1-5 words) to start with, so that you can put verbose debugging in to test the functionality of your application. Once these are working right, you can scale up to larger datasets (e.g., 10-100 words) to tackle any new challenges that might arise.