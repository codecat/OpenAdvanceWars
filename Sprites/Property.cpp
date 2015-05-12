#include "StdH.h"
#include "../Game.h"
#include "Property.h"

CPropertySprite::CPropertySprite()
{
  spr_iVisibilityDistance = 5;
  spr_etTeam = T_RED;
}

CPropertySprite::~CPropertySprite()
{
}

void CPropertySprite::StartTurn()
{
}

void CPropertySprite::EndTurn()
{
}

void CPropertySprite::CalculateVisibleTiles()
{
  if(_pGame->gam_etPlayingTeam != spr_etTeam) {
    return;
  }

  CWorld* pw = GetWorld();

  const int w = _pGame->gam_World.wo_iWidth;
  const int h = _pGame->gam_World.wo_iHeight;

  const int my_x = spr_vGridPosition(1);
  const int my_y = spr_vGridPosition(2);

  const int dist = spr_iVisibilityDistance + 1; // +1 because we also want the tile we're standing on to be visible

  // +1 below to even it out
  for(int x=-dist+1; x<dist; x++) {
    int xx = mthAbs(x);
    for(int y=-dist+1+xx; y<dist-xx; y++) {
      const int new_x = my_x + x;
      const int new_y = my_y + y;

      if(new_x < 0 || new_x >= w) { continue; }
      if(new_y < 0 || new_y >= h) { continue; }

      pw->wo_pubVisible[new_y][new_x] = 1;
    }
  }
}
