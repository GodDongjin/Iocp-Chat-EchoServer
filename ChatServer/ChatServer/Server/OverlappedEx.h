#pragma once
#include "CorePch.h"

class OverlappedEx : public OVERLAPPED
{
public:
	OverlappedEx();
	~OverlappedEx();

	void Init();
	
	void SetSession(SessionRef _session) { session = _session; }
	void Clear();

	IocpEventType type;
	SessionRef session;

	WSABUF wsaBuf;
	char dataBuffer[1024];
};

