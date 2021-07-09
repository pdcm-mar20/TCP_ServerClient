#include "Server.h"

Server::Server() {
	// Using version 2.2
	version = MAKEWORD(2, 2);

	// Port
	port = 8888;
}
Server::Server(int verA, int verB, int Port) {
	version = MAKEWORD(verA, verB);

	port = Port;
}

int Server::Initialize() {
	// Check Windsock
	int wsaOk = WSAStartup(version, &wsaData);
	if (wsaOk != 0)
	{
		std::cout << "Can`t Initialize Winsock! Exit Program!\n";
		return 1;
	}

	// Make socket with IPv4, TCP (SOCK_DGRAM:UDP)
	listening = socket(AF_INET, SOCK_STREAM, 0); 
	if (listening == INVALID_SOCKET)
	{
		std::cout << "Can`t create a socket! Exit Program!\n";
		return 1;
	}

	// Bind thd socket to an ip address and port to a socket
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(port);
	myAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&myAddr, sizeof(myAddr));

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Client in total
	clintCount = 0;

	// If everything is fine
	return 0;
}

void Server::Start() {
	std::cout << " >> Server Begin!\n";

	// Start accepting client
	std::thread acceptClient(&Server::AcceptNewClient, this);
	acceptClient.join();
	handler->JoinThread();

	// If accidentally exit loop
	closesocket(listening);
}

void Server::AcceptNewClient() {
	while (true) {
		clintCount++;

		// Accept new client
		SOCKET newClient = accept(listening, nullptr, nullptr);
		// Add new client to the list
		socketList.push_back(newClient);

		// Say hello to new client
		std::string temp = "Welcome to Server!";
		send(newClient, temp.c_str(), temp.size() + 1, 0);

		// Print massage in server
		std::cout << " >> Socket " << newClient << " connected to server || ID : " << clintCount << "\n";
		
		// Say hello to other client
		std::stringstream mySS;
		mySS << "Client " << newClient << " || ID : " << clintCount;
		std::string tempOut = mySS.str();
		for (SOCKET outSock : socketList)
		{
			// Send to all excep new client
			if (outSock != listening && outSock != newClient)
			{
				send(outSock, tempOut.c_str(), tempOut.size() + 1, 0);
			} 
		}
		
		// Begin handle
		handler = new ServerHandle(newClient, &socketList, clintCount); 
		handler->StartHandle();

		// Delete old socket
		newClient = NULL;
	}
}

// -> Can't get out from here