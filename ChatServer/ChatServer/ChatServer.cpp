#include <iostream>
#include <assert.h>
#include "Core/CorePch.h"
#include "Utiles/ThreadManager.h"
#include "Server/Server.h"

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

