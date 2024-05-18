#pragma once
#include <iostream>
#include <assert.h>
#include "CorePch.h"
#include "Session.h"

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

