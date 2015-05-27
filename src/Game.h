#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "StdH.h"
#include "World.h"
#include "Menu.h"
#include "PlayerInfo.h"

#include <Mragpp/Game.h>

class CAWGame : public CGame
{
public:
  CWorld gam_World;
  CAnimationSheetCollection gam_Sheets;

  CMenu gam_Menu;
  CStackArray<CPlayerInfo> gam_saPlayers;
  ETeam gam_etPlayingTeam;

public:
  CAWGame();
  ~CAWGame();

  void Initialize();

  void Update();
  void Render();
};

#endif
