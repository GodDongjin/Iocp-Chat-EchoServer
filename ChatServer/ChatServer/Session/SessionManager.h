#pragma once
#include <iostream>
#include <assert.h>
#include "Core/CorePch.h"
#include "Session/Session.h"

class SessionManager
{
public:
	SessionManager();
	~SessionManager();

	void CreateSession(SOCKET _socket, SOCKADDR_IN _sockAddr);

	void ConnectAddSession(SessionRef session);
	
	list<shared_ptr<Session>> GetSessionList() { return sessions; }

private:

	std::mutex lock;
	int sessionIDX = 1;
	list<shared_ptr<Session>> sessions;

};

