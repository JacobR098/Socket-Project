#include "defns.h"

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include "record.cpp"
#include <iostream>
#define ECHOMAX 255     /* Longest string to echo */

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

using namespace std;
int main(int argc, char *argv[])
{  
    unsigned int c;
    unsigned short s;
    /* printf("Size of unsigned int is %d\n", sizeof(c));
       printf("Size of unsigned short is %d\n", sizeof(s));
       return 1;*/
    int sock;                        /* Socket */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    //printf("Size of echoServAddr%d\n", sizeof(echoServAddr));
    //printf("Size of echoClntAddr %d\n", sizeof(echoClntAddr));

    unsigned int cliAddrLen;         /* Length of incoming message */
    char echoBuffer[ECHOMAX];        /* Buffer for echo string */
    unsigned short echoServPort;     /* Server port */
    int recvMsgSize;                 /* Size of received message */
    int messageSize = 96;
    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */
    
    vector<struct user> stateTable;    
    
    
    
    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */
    
    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
  
    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(echoClntAddr);
	
	message m1;
	  
        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
            DieWithError("recvfrom() failed");
	
	cout << "Server received " << recvMsgSize << " bytes from client." << endl;
	echoBuffer[ recvMsgSize ] = '\0';
	
	m1 = decode(echoBuffer);
	m1.printMessage();
	if(m1.code == 1){//register request
	  struct user newUser(m1.senderName, m1.port);
	  if(userExists(stateTable, newUser)){
	     //return FAILURE
	    cout << "Not added: DUPLCIATE\n";
	    }
	    else{
	      stateTable.push_back(newUser);
	      cout << "SUCCESS\n";
	      //return SUCCESS;
	    }	    
	}
	
	
        /* Send received datagram back to the client */
        if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer))
            DieWithError("sendto() sent a different number of bytes than expected");
    }
    /* NOT REACHED */
}
