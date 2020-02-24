#include "defns.h"

#include <iostream>
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include "record.cpp"

#define ECHOMAX 255     /* Longest string to echo */
#define ITERATIONS	5

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

using namespace std;
int main(int argc, char *argv[])
{
    int servSock, querySock, leftSock, rightSock; /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short echoServPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char *servIP;                    /* IP address of server */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[ECHOMAX+1];      /* Buffer for receiving echoed string */
    int echoStringLen;               /* Length of string to echo */
    int respStringLen;               /* Length of received response */
    string command;
    
    if (argc < 3)    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Server IP address> <Echo Port>\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];           /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]);  /* Second arg: Use given port, if any */

    printf( "Arguments passed: server IP %s, port %d\n", servIP, echoServPort );

    /* Create a datagram/UDP socket */

    if ((servSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */

    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    echoServAddr.sin_port   = htons(echoServPort);     /* Server port */
    

    
    


    /* Pass string back and forth between server 5 times */
    
	printf( "Echoing %d iterations.\n", ITERATIONS );

	for( int i = 0; i < ITERATIONS; i++ )
	{
	  getline(cin, command);
	  int* size = new int;
	  message myMessage = createMessage(command);
	  //myMessage.printMessage();
	  //return 0;
	  //cout << sizeof(message);
	  //memcpy(echoBuffer, (char*)&myMessage, 176);
	  //message* mess2;
	  //memcpy(mess2, (message*)echoBuffer, 176);
	  //mess2->printMessage();
	  /* Send the struct to the server */
	  char* messageArray = encode(myMessage, size);
	  message decodedMsg = decode(messageArray);
	  decodedMsg.printMessage();
	  //cout << endl;

	  memcpy(echoBuffer, messageArray, *size);
	  
	  //	  for(;;){
	  if(sendto(servSock, &echoBuffer, sizeof(echoBuffer), 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != sizeof(echoBuffer))//size of 96
	    DieWithError("sendto() sent a different number of bytes than expected");
	  
	
	  cout << "Client sent message " << myMessage.msgType << " to server.\n";

    	/* Recv a response */

    	fromSize = sizeof(fromAddr);

    	if ((respStringLen = recvfrom(servSock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize)) > ECHOMAX )
        	DieWithError("recvfrom() failed");

		echoBuffer[ respStringLen] = '\0';
		
		cout << "echoServ add: " << echoServAddr.sin_addr.s_addr << endl;
		cout << "fromServ add: " << fromAddr.sin_addr.s_addr << endl;
   		if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
   		{
       		fprintf(stderr,"Error: received a packet from unknown source.\n");
       		exit(1);
   		}

   		printf("Client received same string from server: %s\n", echoBuffer );    /* Print the echoed arg */
 
	}
    
    close(servSock);
    exit(0);
}
