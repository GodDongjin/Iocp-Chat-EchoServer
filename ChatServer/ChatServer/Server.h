#pragma once
#include <iostream>
#include <assert.h>
#include "CorePch.h"
#include "ThreadManager.h"
#include "SessionManager.h"

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

