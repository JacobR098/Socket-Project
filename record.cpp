#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <string>

#define CHARLENGTH 21
enum State {
	Free, Leader, InDHT //0: Free, 1:Leader, 2: InDHT
};

struct user {
	char userName[CHARLENGTH];
	char inAddress[CHARLENGTH];
	State state; //0: Free, 1:Leader, 2: InDHT
	unsigned short port[3];
	user() {
		memset(userName, 0, CHARLENGTH);
		memset(inAddress, 0, CHARLENGTH);
		state = Free;
		for (short i : port) {
			i = 0;
		}
	}

	user(char* name, char* address, unsigned short* sockets) {
		memcpy(userName, name, CHARLENGTH);
		memcpy(inAddress, address, CHARLENGTH);
		state = Free;
		for (int i = 0; i < 3; i++) {
			port[i] = sockets[i];
		}
	}
};

bool userExists(std::vector<struct user> vec, struct user newUser, unsigned short serverPort) {
	bool exists = false;
	for (int i = 0; i < 3; i++) {
		if (newUser.port[i] == serverPort)
			return true;
	}
	if (newUser.port[0] == newUser.port[1] || newUser.port[0] == newUser.port[2] || newUser.port[1] == newUser.port[2])
		return true;
	for (int i = 0; i < vec.size(); i++) {
		if (strcmp(newUser.userName, vec[i].userName) == 0) {
			exists = true;
			break;
		}
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (newUser.port[j] == vec[i].port[k]) {
					exists = true;
					break;
				}
			}
		}
	}
	return exists;
}

bool userExists(std::vector<struct user> vec, char* userName) {
	bool exists = false;
	for (int i = 0; i < vec.size(); i++) {
		if (strcmp(vec[i].userName, userName) == 0) {
			exists = true;
			break;
		}
	}
	return exists;
}

struct record {
	std::string Country_Code, Short_Name, Table_Name, Long_Name, Alpha_Code, Currency, Region, WB_Code, Latest_Census;
	record(std::istream& stream) {

		getline(stream, Country_Code, ',');
		getline(stream, Short_Name, ',');
		getline(stream, Table_Name, ',');
		getline(stream, Long_Name, ',');
		getline(stream, Alpha_Code, ',');
		getline(stream, Currency, ',');
		getline(stream, Region, ',');
		getline(stream, WB_Code, ',');
		getline(stream, Latest_Census, '\n');
	}
	record() {
		Country_Code = "";
		Short_Name = "";
		Table_Name = "";
		Long_Name = "";
		Alpha_Code = "";
		Currency = "";
		Region = "";
		WB_Code = "";
		Latest_Census = "";
	}
	void print() {
		std::cout << Country_Code << ",";
		std::cout << Short_Name << ",";
		std::cout << Table_Name << ",";
		std::cout << Long_Name << ",";
		std::cout << Alpha_Code << ",";
		std::cout << Currency << ",";
		std::cout << Region << ",";
		std::cout << WB_Code << ",";
		std::cout << Latest_Census << "\n";

	}
};



//Size 60 bytes
struct userInfo {
	char userName[CHARLENGTH];
	char inAddress[CHARLENGTH];
	unsigned short ports[3]; //0: query port, 1: left port, 2: right port

};
//Size 176
struct message {
	int code; 
	int header; //0: query , 1: SUCCESS, 2: FAILURE
	char msgType[CHARLENGTH];
	int num;
	char senderName[CHARLENGTH];
	char inAddress[CHARLENGTH];
	unsigned short port[3]; //0: query port, 1: left port, 2: right port
	userInfo neighbors[2];
	userInfo* random;

	message() {
		header = 0;
		code = 0;
		memset(msgType, 0, CHARLENGTH);
		num = 0;
		memset(senderName, 0, CHARLENGTH);
		memset(inAddress, 0, CHARLENGTH);
		memset(&port, 0, sizeof(short) * 3);
		memset(neighbors[0].userName, 0, CHARLENGTH);
		memset(neighbors[0].inAddress, 0, CHARLENGTH);
		memset(neighbors[0].ports, 0, sizeof(short) * 3);
		memset(neighbors[1].userName, 0, CHARLENGTH);
		memset(neighbors[1].inAddress, 0, CHARLENGTH);
		memset(neighbors[1].ports, 0, sizeof(short) * 3);
	}

	void printMessage() {
		std::cout << "code: " << code << std::endl;
		std::cout << "header: " << header << std::endl;
		std::cout << "msgType: " << msgType << std::endl;
		std::cout << "num: " << num << std::endl;
		std::cout << "senderName " << senderName << std::endl;
		std::cout << "inAddress: " << inAddress << std::endl;
		for (int i = 0; i < 3; i++) {//for each loop not working, starts printing from port[1]
			std::cout << "port[" << i << "]: " << port[i] << "\n";
		}
		//std::cout << "Print neighbors function" << std::endl;    
	}

	void printMessage(int numUsers) {
		std::cout << "code: " << code << std::endl;
		std::cout << "header: " << header << std::endl;
		std::cout << "msgType: " << msgType << std::endl;
		std::cout << "num: " << num << std::endl;
		std::cout << "senderName " << senderName << std::endl;
		std::cout << "inAddress: " << inAddress << std::endl;
		for (int i = 0; i < 3; i++) {//for each loop not working, starts printing from port[1]
			std::cout << "port[" << i << "]: " << port[i] << "\n";
		}
		//std::cout << "Print neighbors function" << std::endl;    
		for (int i = 0; i < numUsers; i++) {
			std::cout << "user[" << i << "]: ";
			std::cout << random[i].userName << "\t";
			std::cout << random[i].inAddress << "\t";
			for (int j = 0; j < 3; j++) {
				std::cout << "ports[" << j << "] " << random[i].ports[j] << "\t";
			}
			std::cout << "\n";
		}
	}
};

std::vector<std::string> removeSpace(std::string str) {
	std::vector<std::string> result;
	std::string token;
	std::stringstream ss(str);
	while (getline(ss, token, ' ')) {
		result.push_back(token);
	}
	return result;
}

message createMessage(std::string str) {
	message msg;
	std::vector<std::string> segments = removeSpace(str);
	std::size_t length;
	if (segments[0] == "register") {
		msg.code = 1;
		length = segments[0].copy(msg.msgType, CHARLENGTH - 1);
		msg.msgType[length] = '\0';
		length = segments[1].copy(msg.senderName, CHARLENGTH - 1);
		msg.senderName[length] = '\0';
		length = segments[2].copy(msg.inAddress, CHARLENGTH - 1);
		msg.inAddress[length] = '\0';

		msg.port[0] = (short)std::stoi(segments[3]);
		// cout << "segments[3] = " << stoi(segments[3]) << endl;
		msg.port[1] = (short)std::stoi(segments[4]);
		msg.port[2] = (short)std::stoi(segments[5]);
	}
	else if (segments[0] == "setup-dht") {
		msg.code = 2;
		//length = segments[0].copy(msg.msgType, CHARLENGTH - 1);
		//msg.msgType[length] = '\0'; 
		msg.num = stoi(segments[1]);
		length = segments[2].copy(msg.senderName, CHARLENGTH - 1);
		msg.senderName[length] = '\0';
	}
	else if (segments[0] == "dht-complete") {
		msg.code = 3;
		//length = segments[0].copy(msg.msgType, CHARLENGTH - 1);
		//msg.msgType[length] = '\0'; 
		length = segments[1].copy(msg.senderName, CHARLENGTH - 1);
		msg.senderName[length] = '\0';
	}
	else if (segments[0] == "query-dht") {
		msg.code = 4;
		//length = segments[0].copy(msg.msgType, CHARLENGTH - 1);
		//msg.msgType[length] = '\0'; 
		length = segments[1].copy(msg.senderName, CHARLENGTH - 1);
		msg.senderName[length] = '\0';
	}
	return msg;
}

char* encode(message msg, int* size) {
	char* arr;//= new char[176];

	int index = 0;
	switch (msg.code) {
	case -1://FAILURE
		arr = new char[4];
		*size = 4;
		memcpy(arr, std::to_string(msg.code).c_str(), sizeof(int));
		break;
	case 0://SUCCESS
		arr = new char[4];
		*size = 4;
		memcpy(arr, std::to_string(msg.code).c_str(), sizeof(int));
		break;
	case 1://register <user-name> <ip-Address> <query-port>  <left-port> <right-port>    
		arr = new char[64];
		*size = 64;
		memcpy(arr, std::to_string(msg.code).c_str(), sizeof(int));
		index += sizeof(int);
		//memcpy(arr, msg.msgType, CHARLENGTH);    
		memcpy(arr + index, msg.senderName, CHARLENGTH);
		index += CHARLENGTH;
		memcpy(arr + index, msg.inAddress, CHARLENGTH);
		index += CHARLENGTH;
		memcpy(arr + index, std::to_string(msg.port[0]).c_str(), 6);
		/* char pp[9];
		memcpy(pp, std::to_string(msg.port[0]).c_str(), 6);
		for(int i =0; i < 10; i++){
		  std::cout << "pp[" << i << "]: " <<  pp[i] << std::endl;
		}
	   {
		  unsigned short s = (short)atoi(pp);
		  std::cout << "Short value is: " << s << std::endl;
		  }*/
		index += 6;
		memcpy(arr + index, std::to_string(msg.port[1]).c_str(), 6);
		index += 6;
		memcpy(arr + index, std::to_string(msg.port[2]).c_str(), 6);
		index += 6;
		break;
	case 2://setup-dht <n> <user-name>
		arr = new char[29];
		*size = 29;
		memcpy(arr, std::to_string(msg.code).c_str(), sizeof(int));
		index += sizeof(int);
		memcpy(arr + index, std::to_string(msg.num).c_str(), sizeof(int));
		index += sizeof(int);
		memcpy(arr + index, msg.senderName, CHARLENGTH);
		break;
	case 3: //dht-complete <user-name>
		arr = new char[25];
		*size = 25;
		memcpy(arr, std::to_string(msg.code).c_str(), sizeof(int));
		index += sizeof(int);
		memcpy(arr + index, msg.senderName, CHARLENGTH);
		break;
	case 4: // query-dht <user-name>
		arr = new char[25];
		*size = 25;
		memcpy(arr, std::to_string(msg.code).c_str(), sizeof(int));
		index += sizeof(int);
		memcpy(arr + index, msg.senderName, CHARLENGTH);
		break;
	case 20:
		arr = new char[4 + 60 * (msg.num)];//60
		*size = 4 + 60 * (msg.num);
		memcpy(arr, std::to_string(msg.code).c_str(), sizeof(int));
		index += sizeof(int);
		memcpy(arr + index, std::to_string(msg.num).c_str(), sizeof(int));
		index += sizeof(int);
		for (int i = 0; i < msg.num; i++) {
			memcpy(arr + index, msg.random[i].userName, CHARLENGTH);
			index += CHARLENGTH;
			memcpy(arr + index, msg.random[i].inAddress, CHARLENGTH);
			index += CHARLENGTH;
			for (int j = 0; j < 3; j++) {
				memcpy(arr + index, std::to_string(msg.random[i].ports[j]).c_str(), 6);
				index += 6;
			}
		}
		break;
	}
	return arr;
}

message decode(char* arr) {
	message msg;
	char buffer[CHARLENGTH]; //Create buffer to use memcpy on
	memset(buffer, 0, CHARLENGTH);//Initialize to '\0'
	memcpy(buffer, arr, sizeof(int));
	//  std::cout << "buffer: " << buffer << "\n"; 

	int index = 0;
	msg.code = atoi(buffer);
	index += sizeof(int);
	switch (msg.code) {
	case -1:
		std::cout << "Code decoded corresponds to FAILURE\n";
		break;
	case 0:
		std::cout << "Code decoded corresponds to SUCCESS\n";
		break;
	case 1:
		std::cout << "Code decoded corresponds to register\n";
		memcpy(msg.senderName, arr + index, CHARLENGTH);
		index += CHARLENGTH;
		memcpy(msg.inAddress, arr + index, CHARLENGTH);
		index += CHARLENGTH;
		for (int i = 0; i < 3; i++) {
			memcpy(buffer, arr + index, 6);
			msg.port[i] = (unsigned short)atoi(buffer);
			index += 6;
		}
		break;
	case 2:
		std::cout << "Code decoded corresponds to setup-dht\n";
		memcpy(buffer, arr + index, sizeof(int));
		msg.num = atoi(buffer);
		index += sizeof(int);
		memcpy(msg.senderName, arr + index, CHARLENGTH);
		break;
	case 3:
		std::cout << "Code decoded corresponds to dht-complete\n";
		memcpy(msg.senderName, arr + index, CHARLENGTH);
		break;
	case 4:
		std::cout << "Code decoded corresponds to query-dht\n";
		memcpy(msg.senderName, arr + index, CHARLENGTH);
		break;
	case 20:
		std::cout << "Code corresponds to SUCCESS for setup-dht\n";
		memcpy(buffer, arr + index, sizeof(int));
		msg.num = atoi(buffer);
		index += sizeof(int);
		msg.random = new userInfo[msg.num];
		for (int i = 0; i < msg.num; i++) {
			memcpy(msg.random[i].userName, arr + index, CHARLENGTH);
			index += CHARLENGTH;
			memcpy(msg.random[i].inAddress, arr + index, CHARLENGTH);
			index += CHARLENGTH;
			for (int j = 0; j < 3; j++) {
				memcpy(buffer, arr + index, 6);
				msg.random[i].ports[j] = (unsigned short)atoi(buffer);
				index += 6;
			}
		}
		return msg;
	}
}

/*
int main(){
  userInfo cat;
  std::cout << "Size of userInfo is: " << sizeof(struct userInfo) << std::endl;
  std::cout << "Size of message is: " << sizeof(message) << std::endl;
}
*/


