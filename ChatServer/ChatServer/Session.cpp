#include "Session.h"

Session::Session()
{
}

Session::~Session()
{
}

void Session::Init(SOCKET _socket, SOCKADDR_IN _sockAddr)
{
	socket = _socket;
	sockAddr = _sockAddr;
}

bool Session::ConnectSuccess()
{
	if (isConnect.load() == true) {
		Disconnect();
		return false;
	}

	isConnect.store(true);
	
	return true;
}

void Session::RegiteraRecv()
{
	assert(isConnect.load() != false);

	recvOverlappedEx.Init();
	recvOverlappedEx.type = IocpEventType::Recv;
	recvOverlappedEx.wsaBuf.len = 1024;
	recvOverlappedEx.wsaBuf.buf = recvOverlappedEx.dataBuffer;
	recvOverlappedEx.SetSession(shared_from_this());

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	
	if (SOCKET_ERROR == ::WSARecv(socket, &recvOverlappedEx.wsaBuf, 1, OUT & numOfBytes, OUT & flags, &recvOverlappedEx, nullptr))
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			recvOverlappedEx.session = nullptr;
			Disconnect();
		}
	}
}

void Session::ProsessRecv(OverlappedEx* _overlapped)
{
	if (_overlapped->dataBuffer == nullptr) {
		Disconnect();
		return;
	}

	cout << "Client Session ID = " << sessionID << "  Recv Data = " << _overlapped->dataBuffer << endl;

	RegiteraSend(_overlapped->dataBuffer);

	recvOverlappedEx.Clear();

	RegiteraRecv();
}

void Session::RegiteraSend(char* sendBuffer)
{
	assert(isConnect.load() != false);

	sendOverlappedEx.Init();
	::CopyMemory(sendOverlappedEx.dataBuffer, sendBuffer, 1024);
	sendOverlappedEx.type = IocpEventType::Send;
	sendOverlappedEx.wsaBuf.len = 1024;
	sendOverlappedEx.wsaBuf.buf = sendOverlappedEx.dataBuffer;	
	sendOverlappedEx.SetSession(shared_from_this());


	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(socket, &sendOverlappedEx.wsaBuf, 1, OUT & numOfBytes, 0, &sendOverlappedEx, nullptr))
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			sendOverlappedEx.session = nullptr;
			Disconnect();
		}
	}
}

void Session::ProsessSend(OverlappedEx* _overlapped)
{
	cout << "Client Session ID = " << sessionID << "  송신 데이터 = " << _overlapped->dataBuffer << endl;
}

void Session::Disconnect()
{
	if (isConnect.exchange(false) == false) {
		return;
	}

	RegisterDisconnect();
}

bool Session::RegisterDisconnect()
{
	disconnectOverlappedEx.Init();
	disconnectOverlappedEx.type = IocpEventType::Disconnect;
	disconnectOverlappedEx.SetSession(shared_from_this());

	if (false == ServerUtile::DisconnectEx(socket, &disconnectOverlappedEx, TF_REUSE_SOCKET, 0))
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			disconnectOverlappedEx.session = nullptr;
			return false;
		}
	}

	return true;
}

void Session::ProsessDisconnect()
{
	recvOverlappedEx.Init();
	recvOverlappedEx.session = nullptr;

	cout << "disconnect Session ID =  " << sessionID << endl;
}


