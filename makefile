server:	myServer.cpp record.cpp
	g++ -g -std=c++11 myServer.cpp -o server
client: myClient.cpp record.cpp
	g++ -g -std=c++11 myClient.cpp -o client
