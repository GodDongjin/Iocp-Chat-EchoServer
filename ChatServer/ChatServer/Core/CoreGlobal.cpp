#include "CorePch.h"
#include "CoreGlobal.h"
#include "../Utiles/ThreadManager.h"

unique_ptr<ThreadManager> GThreadManager = make_unique<ThreadManager>();

