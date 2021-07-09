#include "Client.h"

Client::Client() {
	ipAddress = "127.0.0.1";
	port = 8888;

	byteSize = 4056;
	data = new char[byteSize];
}
Client::Client(std::string IpAddress, int Port) {
	ipAddress = IpAddress;
	port = Port;

	byteSize = 4056;
	data = new char[byteSize];
}

int Client::Initialize() {
	version = MAKEWORD(2, 2);
	int wsResult = WSAStartup(version, &wsaData);
	if (wsResult != 0)
	{
		std::cout << "Can't start Winsock! \nError : " << wsResult << "\n Exit Program!";
		return 1;
	}

	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mysocket == INVALID_SOCKET)
	{
		std::cout << "Can't create socket! \nError : " << WSAGetLastError() << "\n Exit Program!";
		WSACleanup();
		return 1;
	}

	// Fill in a myAddr structre
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &myAddr.sin_addr);

	// Connect to server
	int connResult = connect(mysocket, (sockaddr*)&myAddr, sizeof(myAddr));
	// If fail, try to connect again until connected
	int attemp = 1;
	while (connResult == SOCKET_ERROR)
	{
		system("cls");
		std::cout << "Attemp " << attemp << " ...";
		attemp++;
		int connResult = connect(mysocket, (sockaddr*)&myAddr, sizeof(myAddr));
		Sleep(1000);
	}
	// Print massage if connected
	std::cout << " >> Connected : " << mysocket << "\n";

	// If everything is fine
	return 0;
}

void Client::Start() {
	// Start thread to recieve data from server
	std::thread recieveThread(&Client::Recieve, this);
	std::thread sendThread(&Client::Send, this);
	sendThread.join();
	recieveThread.join();
}

void Client::Recieve() {
	while (true)
	{
		ZeroMemory(data, byteSize);
		int bytesReceived = recv(mysocket, data, byteSize, 0);
		if (bytesReceived > 0)
		{
			std::cout << "\n";
			std::cout << " >> " << std::string(data, 0, bytesReceived) << "\n";
			std::cout << " >> ";
		}
	}
}

void Client::Send() {
	std::string userInput;
	while (true)
	{
		std::cin >> userInput;
		std::cout << " >> ";
		send(mysocket, userInput.c_str(), userInput.size() + 1, 0);
		/*
		std::cout << userInput << "\n >> ";
		send(mysocket, userInput.c_str(), userInput.size() + 1, 0);
		Sleep(1000);
		*/
	}
}

