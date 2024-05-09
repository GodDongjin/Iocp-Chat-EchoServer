#include "OverlappedEx.h"

OverlappedEx::OverlappedEx()
{
	Init();
}

OverlappedEx::~OverlappedEx()
{
}

void OverlappedEx::Init()
{
	hEvent = 0;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
}

void OverlappedEx::Clear()
{
	hEvent = 0;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;

	session = nullptr;
	wsaBuf.buf = 0;
	wsaBuf.len = 0;
	dataBuffer[0] = { 0 };
}
