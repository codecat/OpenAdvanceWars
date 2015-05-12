#include "StdH.h"
#include "Game.h"
#include <SDL_main.h>

extern CRTSGame* _pGame = 0;

int main(int argc, char *argv[])
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  CRTSGame game;
  game.Run();

  return 0;
}
