#include "../StdH.h"
#include "Tank.h"

CTankUnit::CTankUnit()
{
  spr_iMoveDistance = 6;
  spr_iVisibilityDistance = 3;
  spr_etTeam = T_NEUTRAL;
  SetCharacter("Tank", T_NEUTRAL);
}

CTankUnit::CTankUnit(ETeam et)
{
  spr_etTeam = et;
  SetCharacter("Tank", et);
}

CTankUnit::~CTankUnit()
{
}

void CTankUnit::Update()
{


  CUnitSprite::Update();
}

void CTankUnit::Render()
{


  CUnitSprite::Render();
}
