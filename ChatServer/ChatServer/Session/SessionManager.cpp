#include "SessionManager.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

void SessionManager::CreateSession(SOCKET _socket, SOCKADDR_IN _sockAddr)
{
	shared_ptr<Session> session;
	session->Init(_socket, _sockAddr);

	sessions.push_back(session);
}

void SessionManager::ConnectAddSession(SessionRef session)
{
	lock_guard<std::mutex> Lock_guard(lock);
	assert(session->ConnectSuccess());
	session->SetsessionID(sessionIDX);
	sessions.push_back(session);
	sessionIDX++;
}
