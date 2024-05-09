#include <iostream>
#include <assert.h>
#include "CorePch.h"
#include "ThreadManager.h"
#include "Server.h"

void test()
{

}

int main()
{
	std::shared_ptr<Server> server = make_shared<Server>();

	if (!server->Init()) {
		return 0;
	}

	for (int i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					server->WorkTheradMain();
				}
			});
	}


	GThreadManager->Join();

	::WSACleanup();
}

