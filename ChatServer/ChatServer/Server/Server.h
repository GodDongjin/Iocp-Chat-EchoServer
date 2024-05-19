#pragma once
#include <iostream>
#include <assert.h>
#include "../Core/CorePch.h"
#include "../Utiles/ThreadManager.h"
#include "../Session/SessionManager.h"

class Server
{
public:
	bool Init();

	void RegistraAccept();
	void ProsessAccept(OverlappedEx* overlapped);

	void WorkTheradMain();

private:

	SOCKET listenSocket;
	SOCKADDR_IN listenAddr;

	HANDLE iocpHandle;

	SessionManager sessionManager;
};

