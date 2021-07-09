#pragma once 
#include <iostream>
#include <sstream>
#include "ServerHandle.h"
#include <WS2tcpip.h>
#pragma comment(lib,"WS2_32")

class Server {
private:
	WSAData wsaData;
	WORD version;

	SOCKET listening;
	sockaddr_in myAddr;

	std::list<SOCKET> socketList;

	int port;

	int clintCount;
	ServerHandle* handler;

	void AcceptNewClient();

public:
	Server();
	Server(int verA, int verB, int Port);

	int Initialize();
	void Start();
};