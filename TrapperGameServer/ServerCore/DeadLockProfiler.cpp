#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	// 아이디를 찾거나 발급한다.
	int32 lockId = 0;

	auto findIt = _nameToId.find(name);

	// 만약에 찾는 ID가 없다.
	if (findIt == _nameToId.end())
	{
		// 그러면 발급
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = findIt->second;
	}

	// 잡고 있는 락이 있었다면
	if (!LLockStack.empty())
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부를 다시 확인한다.
		// 사이클이 있는지 확인
		const int32 prevId = LLockStack.top();
		// 같은 락을 잡았을 때는 재귀적으로 잡을 수 있게 허용해서
		// 락이 다를 때만 판별해준다.
		if (lockId != prevId)
		{
			// 처음 발견한다고 했을 때
			// 다른 락을 잡았다면 히스토리 참고
			set<int32>& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				// 새로운 간선을 발견한 것이라면 간선을 추가해준다.
				history.insert(lockId);
				CheckCycle();
			}
		}
	}
	// 잡고 있는 락이 없으면 락스택에 넣어주기
	LLockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (LLockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockId = _nameToId[name];
	if (LLockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	LLockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	// 지금까지 발견된 lock 카운트
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	// 싸그리 초기화, -1을 해주는 이유 = 정점이 아예 들리지 않았을 때
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	// 해당 dfs가 완벽하게 끝났는지
	_finished = vector<bool>(lockCount, false);
	// 어떤 경로에 의해 발견이 됐는지
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	// 연산이 끝났으면 정리한다.
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	// 이미 방문이 됐다면 return
	if (_discoveredOrder[here] != -1)
		return;

	_discoveredOrder[here] = _discoveredCount++;

	// 모든 인접한 정점을 순회한다.
	// lockHistory를 통해 간선으로 정점을 타고타고 들어갈 수 있다.
	auto findIt = _lockHistory.find(here);
	if (findIt == _lockHistory.end())
	{
		// 락을 잡은 상태에서 다른 락을 잡은 적이 없다면
		_finished[here] = true;
		return;
	}

	// 락을 잡은 상태에서 다른 락을 잡은 적이 있다면
	set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if (_discoveredOrder[there] == -1)
		{
			// 너는 나때문에 방문이 된겨
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// 순방향인지 역방향인지 확인하기
		// here가 there보다 먼저 발견되었다면, there는 here의 후손이다. (순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// 순방향이 아니고, Dfs(there)가 아직 종료하지 않았다면, there는 here의 선조이다. (역방향 간선)
		if (_finished[there] == false)
		{
			// 여기는 무조건 크래쉬가 나는 구간이라 한번 연결되어있는 노드 확인하기.
			printf("%s -> %s\n", _idToName[here], _idToName[there]);
			
			// 부모를 계속 타고 올라감.
			int32 now = here;

			while (true)
			{
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}

	_finished[here] = true;
}
