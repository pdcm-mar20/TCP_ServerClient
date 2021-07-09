#pragma once
#include <iostream>
#include <sstream>
#include <thread>
#include <list>
#include <WS2tcpip.h>
#pragma comment(lib,"WS2_32")

class ServerHandle {
private:
	SOCKET mySocket;
	std::list<SOCKET>* socketList;

	int byteSize;
	char* data;

	int Id;

	std::thread myThread;
	void Handle();

public:
	ServerHandle(SOCKET mySocket, std::list<SOCKET>* socketList, int Id);

	void StartHandle();

	void JoinThread();
};