#include "stdafx.h"

void check_for_assert(int X, char *File, unsigned int Line)
{
	if (X) return;
	char s[300];
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);
	wsprintf(s, "File %s, Line %u \n\rGetLastError = %s", File, Line, lpMsgBuf);
	LocalFree(lpMsgBuf);
	MessageBox(GetDesktopWindow(), s, "Assertion failed !", MB_OK);
	
	//_exit(1);
    TerminateProcess(GetCurrentProcess(), 1);
}

