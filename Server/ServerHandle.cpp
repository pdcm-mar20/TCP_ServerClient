#include "ServerHandle.h"

ServerHandle::ServerHandle(SOCKET mySocket, std::list<SOCKET>* socketList, int Id) {
	this->mySocket = mySocket;
	this->socketList = socketList;
	this->Id = Id;

	byteSize = 4056;
	data = new char[byteSize];
}

void ServerHandle::StartHandle() {
	myThread = std::thread(&ServerHandle::Handle, this);
}
void ServerHandle::JoinThread() {
	myThread.join();
}

void ServerHandle::Handle() {
	while (true) {
		ZeroMemory(data, byteSize);

		// Receive Message
		int dataIn = recv(mySocket, data, byteSize, 0);
		if (dataIn <= 0)
		{
			// Drop the client
			closesocket(mySocket);
		}
		else
		{
			// Print massage to server
			std::cout << " >> Client-" << Id << " : " << data << "\n";

			// Send message to other client
			std::stringstream mySS;
			mySS << "Client-" << Id << " : " << data;
			std::string tempOut = mySS.str();


			for (auto it = socketList->begin(); it != socketList->end(); it++)
			{
				if (*it != mySocket)
				{
					send(*it, tempOut.c_str(), tempOut.size() + 1, 0);
				}
			}
		}

	}
}