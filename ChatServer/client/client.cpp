#include <iostream>
#include "CorePch.h"

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777); // 80 : HTTP

	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		return 0;
	}
		
	cout << "Connected To Server!" << endl;
	char sendBuffer[1024] = {};
	char recvBuffer[1024] = {};
	char exit;
	while (true)
	{
		cin >> sendBuffer;

		int resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR) {
			cout << "서버가 연결이 끊겼다. 아무키를 누르고 엔터를 눌러 주세요." << endl;
			cin >> exit;
			return 0;
		}
			
		int recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen <= 0) {
			return 0;
		}
			

		cout << "Echo Data : " << recvBuffer << endl;

		this_thread::sleep_for(1s);
	}

	::closesocket(clientSocket);
	::WSACleanup();
}