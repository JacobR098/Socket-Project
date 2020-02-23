#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <string>

#define CHARLENGTH 21
struct record {
  std::string Country_Code, Short_Name, Table_Name, Long_Name, Alpha_Code, Currency, Region, WB_Code, Latest_Census;
    record(std::istream& stream){
     
    getline(stream, Country_Code, ',');
    getline(stream, Short_Name, ',');
    getline(stream, Table_Name, ',');
    getline(stream, Long_Name, ',');
    getline(stream, Alpha_Code, ',');
    getline(stream, Currency, ',');
    getline(stream, Region, ',');
    getline(stream, WB_Code , ',');
    getline(stream, Latest_Census, '\n');
  }
  record(){
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
  void print(){
    std:: cout << Country_Code << ",";
    std:: cout << Short_Name << ",";
    std:: cout << Table_Name << ",";
    std:: cout << Long_Name << ",";
    std:: cout << Alpha_Code << ",";
    std:: cout << Currency << ",";
    std:: cout << Region << ",";
    std:: cout << WB_Code << ",";
    std:: cout << Latest_Census << "\n";

  }
};



//Size 46 bytes
struct userInfo {
  char userName[CHARLENGTH];
  char inAddress[CHARLENGTH];
  unsigned short ports[3]; //0: query port, 1: left port, 2: right port
  
};
//Size 162
struct message {
  int header; //0: query , 1: SUCCESS, 2: FAILURE
  char msgType[CHARLENGTH];
  int num;
  char senderName[CHARLENGTH];
  char inAddress[CHARLENGTH];
  unsigned short port[3];
  userInfo neighbors[2];

  message(){
    header = 0;
    memset(msgType, 0, CHARLENGTH);
    num = 0;
    memset(senderName, 0, CHARLENGTH);
    memset(inAddress, 0 , CHARLENGTH);
    memset(&port, 0, sizeof(short)*3);        
    memset(neighbors[0].userName, 0, CHARLENGTH);
    memset(neighbors[0].inAddress, 0, CHARLENGTH);
    memset(neighbors[0].ports, 0, sizeof(short)*3);
    memset(neighbors[1].userName, 0, CHARLENGTH);
    memset(neighbors[1].inAddress, 0, CHARLENGTH);
    memset(neighbors[1].ports, 0, sizeof(short)*3);
  }

  void printMessage(){
    std::cout << "header: " << header << std::endl;
    std::cout << "msgType: " << msgType << std::endl;
    std::cout << "num: " << num << std::endl;
    std::cout << "senderName " << senderName << std::endl;
    std::cout << "inAddress: " << inAddress << std::endl;
    for(int i = 0; i < 3; i++){//for each loop not working, starts printing from port[1]
      std::cout << "port[" << i << "]: " << port[i] << "\n";  
    }
    //std::cout << "Print neighbors function" << std::endl;
  }
};

std::vector<std::string> removeSpace(std::string str){
  std::vector<std::string> result;
  std::string token;
  std::stringstream ss(str);
  while(getline(ss, token, ' ')){    
    result.push_back(token);
    }
  return result;
}

message createMessage(std::string str){
  message msg; 
  std::vector<std::string> segments = removeSpace(str);
  std::size_t length;
  if(segments[0] == "register"){
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
  else if(segments[0] == "setup-dht"){    
    length = segments[0].copy(msg.msgType, CHARLENGTH - 1);
    msg.msgType[length] = '\0'; 
    msg.num = stoi(segments[1]);
    length = segments[2].copy(msg.senderName, CHARLENGTH - 1);
    msg.senderName[length] = '\0';         
  }
  else if(segments[0] == "dht-complete"){
    length = segments[0].copy(msg.msgType, CHARLENGTH - 1);
    msg.msgType[length] = '\0'; 
    length = segments[1].copy(msg.senderName, CHARLENGTH - 1);
    msg.senderName[length] = '\0';     
  }
  else if(segments[0] == "query-dht"){
    length = segments[0].copy(msg.msgType, CHARLENGTH - 1);
    msg.msgType[length] = '\0'; 
    length = segments[1].copy(msg.senderName, CHARLENGTH - 1);
    msg.senderName[length] = '\0';     
  }
  return msg;
}


/*
int main(){
  userInfo cat;
  std::cout << "Size of userInfo is: " << sizeof(struct userInfo) << std::endl;
  std::cout << "Size of message is: " << sizeof(message) << std::endl;
}
*/
