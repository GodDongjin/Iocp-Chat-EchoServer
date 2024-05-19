#include "Server.h"
#include "../Session/SessionManager.h"

bool Server::Init()
{
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return false;
	}

	ServerUtile::Init();

	iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	assert(iocpHandle != INVALID_HANDLE_VALUE);

	listenSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET) {
		return false;
	}

	iocpHandle = ::CreateIoCompletionPort((HANDLE)listenSocket, iocpHandle, 0, 0);

	ServerUtile::SetReuseAddress(listenSocket, true);

	ServerUtile::SetLinger(listenSocket, 0, 0);

	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	listenAddr.sin_port = htons(7777);
	if (SOCKET_ERROR == ::bind(listenSocket, (SOCKADDR*)&listenAddr, sizeof(listenAddr))) {
		return false;
	}

	if (SOCKET_ERROR == ::listen(listenSocket, SOMAXCONN)){
		return false;
	}

	cout << "Server Open Susses" << endl;

	RegistraAccept();
	
	return true;
}

void Server::RegistraAccept()
{
	SOCKET sessionSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sessionSocket == INVALID_SOCKET) {
		wprintf(L"Create accept socket failed with error: %u\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	SessionRef session = make_shared<Session>();
	session->SetSocket(sessionSocket);

	OverlappedEx overlappedEx;
	overlappedEx.type = IocpEventType::Accept;
	overlappedEx.SetSession(session);
	session->SetOverlapped(overlappedEx);

	HANDLE sessionHandle = ::CreateIoCompletionPort((HANDLE)session->GetSocket(), iocpHandle, 0, 0);
	if (sessionHandle == NULL) {
		int errocode = ::WSAGetLastError();
	}

	DWORD bytesReceived = 0;
	if (false == ServerUtile::AcceptEx(listenSocket, session->GetSocket(), session->GetOverlapped()->dataBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytesReceived, static_cast<LPOVERLAPPED>(session->GetOverlapped()))) {
		const int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			RegistraAccept();
		}
	}
}

void Server::ProsessAccept(OverlappedEx* overlapped)
{
	SessionRef session = overlapped->session;

	if (false == ServerUtile::SetUpdateAcceptSocket(session->GetSocket(), listenSocket))
	{
		RegistraAccept();
		return;
	}

	SOCKADDR_IN sockAddress;
	int sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT (SOCKADDR*)(&sockAddress), &sizeOfSockAddr))
	{
		int erroCode = ::WSAGetLastError();
		RegistraAccept();
		return;
	}

	session->SetSockAddr(sockAddress);
	sessionManager.ConnectAddSession(session);

	cout << "Successful Client Connet = " << session->GetSessionID() << endl;
	session->RegiteraRecv();

	RegistraAccept();
}

void Server::WorkTheradMain()
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	OverlappedEx* overlapped = nullptr;

	if (::GetQueuedCompletionStatus(iocpHandle, OUT &numOfBytes, OUT reinterpret_cast<PULONG_PTR>(&key), OUT reinterpret_cast<LPOVERLAPPED*>(&overlapped), INFINITE))
	{
		switch (overlapped->type)
		{
		case IocpEventType::Accept:
			ProsessAccept(overlapped);
				break;
		case IocpEventType::Connect:
			break;
		case IocpEventType::Disconnect:
			overlapped->session->ProsessDisconnect();
			break;
		case IocpEventType::Recv:
			overlapped->session->ProsessRecv(overlapped);
			break;
		case IocpEventType::Send:
			overlapped->session->ProsessSend(overlapped);
			break;
		default:
			break;
		}
	}
	else
	{
		overlapped->session->ProsessDisconnect();
		cout << "erroCode : " << ::WSAGetLastError() << endl;;
	}
}
