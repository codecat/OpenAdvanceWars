#include "StdH.h"
#include "Game.h"

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#include <errno.h>
#endif

CRTSGame* _pGame = 0;

int main()
{
#ifdef _MSC_VER
	DWORD dwFlags = GetFileAttributesA("Content");
	if(dwFlags == INVALID_FILE_ATTRIBUTES) {
		_chdir("../.."); // bin/Debug/astra.exe
	}
#else
	for(int i=0; i<3; i++) {
		if(access("Content/", F_OK) != 0) {
			if(errno == ENOENT || errno == ENOTDIR) {
				chdir(".."); // bin/astra.exe
			}
		} else {
			break;
		}
	}
#endif

	char szCurrentDirectory[1024];
	if(getcwd(szCurrentDirectory, 1024) != NULL) {
		printf("Current directory: %s\n", szCurrentDirectory);
	} else {
		printf("Working directory failure?\n");
		return 1;
	}

	_pGame = new CRTSGame;
	_pGame->Run();

	return 0;
}
