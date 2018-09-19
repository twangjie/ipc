#include "StdAfx.h"
#include "../ipc/helper.h"
#include "../ipc/ipc.h"

// Static variables
char *server_addr = NULL;
int nDataLen = 1024;

LONGLONG dwTotoalBytes = 0, dwLastCountBytes = 0;
LONGLONG dwLastCount = 0, dwSuccessCount = 0;
LONGLONG unFailedCount = 0;

// Client thread
DWORD WINAPI test_client(LPVOID context)
{
	// Create the IPC client
	osIPC::Client client(server_addr);

	// Declare vars
	char *testData = new char[nDataLen];

	for (;;)
	{
		sprintf(testData, "%I64u", dwSuccessCount);
		client.write(testData, nDataLen);

		InterlockedIncrement64(&dwSuccessCount);
		InterlockedExchangeAdd64(&dwTotoalBytes, (unsigned long long)nDataLen);
	}

	// Success
	return 0;
};

DWORD WINAPI static_CountThread( void *lParam )
{

	while (1)
	{
		printf("Success Count=%I64u, Failed Count=%I64u, %I64u pkgs/sec, %.1f mbps\n", dwSuccessCount, unFailedCount,
			dwSuccessCount - dwLastCount, (float)(dwTotoalBytes - dwLastCountBytes) / 1024.0 / 1024.0 * 8);

		InterlockedExchange64(&dwLastCount, dwSuccessCount);
		InterlockedExchange64(&dwLastCountBytes, dwTotoalBytes);

		//InterlockedExchange64(&dwTotoalBytes, 0);

		Sleep(1000);
	}


	return FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int nthreadcount = 1;
	if(argc == 2)
	{
		server_addr = argv[1];
	}
	else if(argc == 3)
	{
		server_addr = argv[1];
		nDataLen = atoi(argv[2]);
	}
	else if(argc == 4)
	{
		server_addr = argv[1];
		nDataLen = atoi(argv[2]);
		nthreadcount = atoi(argv[3]);
	}
	else
	{
		printf("IPCClient.exe server_addr [DataLen] [threadcount]");

		return 1;
	}

	unsigned int uiThreadID = 0;
	HANDLE hCountThread = (HANDLE)CreateThread(NULL, 0, static_CountThread, NULL, 0, NULL);
	if (hCountThread)
	{
	}

	// Start lots of threads
	for (int n = 0; n < nthreadcount; n++)
	{
		HANDLE hThread = CreateThread(NULL, 0, test_client, NULL, 0, NULL);
		if (!hThread) return false;
	}

	getchar();

	// Success
	return 0;
};