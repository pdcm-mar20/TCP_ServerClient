#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

class Client {
private:
	std::string ipAddress;
	int port;

	WSAData wsaData;
	WORD version;

	SOCKET mysocket;
	sockaddr_in myAddr;

	int byteSize;
	char* data;

	void Receive();
	void Send();

public:
	Client();
	Client(std::string IpAddress, int Port);

	int Initialize();
	void Start();
};