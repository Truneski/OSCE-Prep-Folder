/*
 * This is a sample plugin dll for Minalic Wevserver.
 * Register the plugin in Manilic.ini file with the lines:
 * SERVER_PLUGIN = SAMPLE:8081 and
 * SAMPLE_VAR = 4. This will make the server
 * listen on port 8081 for the plugin and it saves a parameter
 * that the plugin reads.
 * Link with ../../sdk/minalic.lib.
 * Put the sample.dll in the minalic binary directory.
 */


#include <windows.h>
#include <stdio.h>
#include "../../sdk/minalic.h"

char g_param_str[10];
char buf[100];

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		echo("Sample PLugin attached");
		get_config_value("SAMPLE_VAR",g_param_str);
		sprintf(buf,"SAMPLE_VAR=%s",g_param_str);
		echo(buf);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}



__declspec(dllexport) char *get_version()
{
	return "2.1";
}

__declspec(dllexport) char *get_build()
{
	return "PL0023";
}

__declspec(dllexport) void handle_request(SOCKET s,struct sockaddr_in addr)
{
	int		len;
	char	header[256];
	int		max;
	char	*answ="Hello world!";

	max = 0;
	while (max < 4)
	{
		len = recv(s,&header[max],1,0);
		if (len == 0)
			return ; /* No data found*/

		if (len < 0)
			return; /* Socket closed*/
		max += len;
	}
	header[max+1] = 0;
	send(s,answ,strlen(answ),0);
}
