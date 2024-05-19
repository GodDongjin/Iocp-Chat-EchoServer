#pragma once
#include <mutex>
#include <atomic>

enum class IocpEventType
{
	Connect,
	Disconnect,
	Accept,
	Recv,
	Send
};

using SessionRef = std::shared_ptr<class Session>;


template<typename T>
using Atomic = std::atomic<T>;
using Mutex = std::mutex;
using CondVar = std::condition_variable;
using UniqueLock = std::unique_lock<std::mutex>;
using LockGuard = std::lock_guard<std::mutex>;