#include "StdAfx.h"
#include "../ipc/helper.h"
#include "../ipc/ipc.h"

// Static variables
char *server_addr = NULL;
__int64 stats_server_rcv_count = 0;

// Server thread
int _tmain(int argc, _TCHAR* argv[])
{
	// Create the IPC server
	osIPC::Server server("test");

	// Get the address
	server_addr = server.getAddress();

	printf("server_addr: %s\n", server_addr);

	// Declare vars
	DWORD timerOutput = GetTickCount();
	unsigned __int64 N = 0;
	unsigned __int64 lN = 0;
	unsigned __int64 received = 0;

	// Enter a loop reading data
	for (;;N ++)
	{
		// Read the data
		osIPC::Block *pBlock = server.getBlock();
		if (pBlock->Amount > 0)
		{
			received += pBlock->Amount;
			stats_server_rcv_count++;

			//Sleep(10);
		}
		server.retBlock(pBlock);

		// Check the timer
		DWORD curTime = GetTickCount();
		if (curTime - timerOutput > 1000) {
			timerOutput = curTime;

			// Print the data
			//printf("Server: %01d,  Client: %01d,  Rate: %01u,  Amount: %.1f mbps\n", stats_server_rcv_count, stats_client_snd_count, (DWORD)(N - lN), (float)received / 1024 / 1024 * 8);
			printf("Server: %I64u,  Rate: %I64u,  Amount: %.1f mbps\n", stats_server_rcv_count, (N - lN), (float)received / 1024 / 1024 * 8);
			lN = N;
			received = 0;
		}
	}

	getchar();

	// Success
	return 0;
};