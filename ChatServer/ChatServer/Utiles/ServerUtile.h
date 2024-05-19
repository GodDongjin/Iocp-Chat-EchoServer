#pragma once
#include "../Core/CorePch.h"
#include "Types.h"

class ServerUtile
{
public:
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	static void Init();

	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);

	static bool SetLinger(SOCKET socket, int onoff, int linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);
};

template<typename T>
static inline bool SetSockOpt(SOCKET socket, int level, int optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}