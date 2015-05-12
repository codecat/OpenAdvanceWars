#include "StdH.h"
#include "Game.h"

CRTSGame* _pGame = 0;

int main()
{
	_pGame = new CRTSGame;
	_pGame->Run();

	return 0;
}
