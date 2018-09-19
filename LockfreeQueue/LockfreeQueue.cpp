// LockfreeQueue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Queue.h"
#include "simplethread.h"

using namespace lock_free;

Queue queue(1024);

unsigned long long nTest = 0;

// Client thread
DWORD WINAPI test_client(LPVOID context)
{
	for (unsigned long long j = 0; j < 1024ULL * 1024ULL * 32ULL; ++j) {
		//unpredictableDelay(500);
		//q.enqueue(j);
		Sleep(1);

		unsigned long long *pTemp = new unsigned long long;
		*pTemp = InterlockedIncrement64((long long *)&nTest);

		queue.Push(pTemp);
	}

	// Success
	return 0;
};

int _tmain(int argc, _TCHAR* argv[])
{

	// Start lots of threads
	for (int n = 0; n < 10; n++)
	{
		HANDLE hThread = CreateThread(NULL, 0, test_client, NULL, 0, NULL);
		if (!hThread) return false;
	}

	SimpleThread reader([&]() {
		unsigned long long element;
		for (unsigned long long j = 1; j < 1024ULL * 1024ULL * 32ULL;) {
			if ((j & (1024 * 1024 * 16 - 1)) == 0) {
				//log << "  ... iteration " << j << std::endl;
				//std::printf("  ... iteration %llu\n", j);
			}
			//unpredictableDelay();
			
			unsigned long long *pTemp = 0;
			if ((pTemp = (unsigned long long *)queue.Pop()) != NULL)
			{
				element = *pTemp;

				delete pTemp;

				if (element != j) {
// 					log << "  ERROR DETECTED: Expected to read " << j << " but found " << element << std::endl;
 					std::printf("  ERROR DETECTED: Expected to read %llu but found %llu", j, element);
				}
				++j;
			}
		}
	});

	getchar();

	return 0;
}

