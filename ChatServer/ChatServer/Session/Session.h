#pragma once
#include "Core/CorePch.h"
#include "Core/OverlappedEx.h"

class Session : public enable_shared_from_this<Session>
{
public:
	Session();
	~Session();

	void Init(SOCKET _socket, SOCKADDR_IN _sockAddr);

	SOCKET GetSocket() { return socket; }
	SOCKADDR_IN GetSockAddr() { return sockAddr; }
	OverlappedEx* GetOverlapped() { return &recvOverlappedEx; }
	int GetSessionID() { return sessionID; }

	void SetSocket(SOCKET _socket) { socket = _socket; }
	void SetSockAddr(SOCKADDR_IN _sockAddr) { sockAddr = _sockAddr; }
	void SetOverlapped(OverlappedEx _overlappedEx) { recvOverlappedEx = _overlappedEx; }
	void SetOverlappedSession(SessionRef _session) { return recvOverlappedEx.SetSession(_session); }
	void SetsessionID(int _sessionID) { sessionID = _sessionID; }

	bool ConnectSuccess();

	void RegiteraRecv();
	void ProsessRecv(OverlappedEx* _overlapped);

	void RegiteraSend(char* sendBuffer);
	void ProsessSend(OverlappedEx* _overlapped);
	
	void Disconnect();
	bool RegisterDisconnect();
	void ProsessDisconnect();

private:
	std::mutex lock;

	int sessionID = 0;

	OverlappedEx recvOverlappedEx;
	OverlappedEx sendOverlappedEx;
	OverlappedEx disconnectOverlappedEx;

	SOCKET socket = INVALID_SOCKET;
	SOCKADDR_IN sockAddr = {};
	std::atomic<bool> isConnect = false;
};


