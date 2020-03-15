#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <vector>
#include "record.cpp"

#define ECHOMAX 500     /* Longest string to echo */
#define CHARLENGTH 21

void DieWithError(const char* errorMessage) {
	perror(errorMessage);
	exit(1);
}


struct test {
	int first;
	char msg[CHARLENGTH];
};
//int indexOfUser(vector<user>& vec, char* userName);
//void chooseNRandom(vector<user>& vec, int num, message& msg, char* leaderName);
//const char* findLeader(const vector<user>& vec);
//bool isFree(vector<user> vec, char* name);
using namespace std;

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	unsigned short echoServPort;
	int recvMsgSize;
	struct message receivedMessage, replyMessage;
	vector<struct user> stateTable;
	int valueChecker;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <PORT NUMBER>\n", argv[0]);
		exit(1);
	}

	echoServPort = atoi(argv[1]); //Set first arg as local port

	//Create socket 
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		DieWithError("socket() failed");

	//Construct local address structure
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET; 
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	echoServAddr.sin_port = htons(echoServPort); 

	//Bind to the local address
	if (bind(sock, (struct sockaddr*) & echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("bind() failed\n");
	for (;;) {
		//Set size of in-out parameter
		cliAddrLen = sizeof(echoClntAddr);

		//Block until receive message from a client
		if ((recvMsgSize = recvfrom(sock, &receivedMessage, sizeof(receivedMessage), 0, (struct sockaddr*) &echoClntAddr, &cliAddrLen)) < 0)
			DieWithError("recvfrom() failed\n");
		cout << "Received message\n--------------\n";
		receivedMessage.printMessage();

		if (receivedMessage.code == 1) {
			struct user newUser(receivedMessage.senderName, receivedMessage.inAddress, receivedMessage.port);
			if (userExists(stateTable, newUser, echoServPort)) {
				cout << "FAILURE\n";
				replyMessage.code = -1;
				if (valueChecker = sendto(sock, &replyMessage, sizeof(replyMessage), 0, (struct sockaddr*) & echoClntAddr, cliAddrLen) == -1)
					DieWithError("Error sending message\n");


			}
			else {
				stateTable.push_back(newUser);
				cout << "SUCCESS\n";
				replyMessage.code = 0;
				if(valueChecker = sendto(sock, &replyMessage, sizeof(replyMessage), 0, (struct sockaddr*) & echoClntAddr, cliAddrLen) == -1)
					DieWithError("Error sending message\n");
			}
		}
	}
}
