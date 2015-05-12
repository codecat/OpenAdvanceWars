#include "StdH.h"
#include "Infantry.h"

CInfantryUnit::CInfantryUnit()
{
  spr_iMoveDistance = 3;
  spr_iVisibilityDistance = 2;
  spr_etTeam = T_NEUTRAL;
  SetCharacter("Infantry", T_NEUTRAL);
}

CInfantryUnit::CInfantryUnit(ETeam et)
{
  spr_etTeam = et;
  SetCharacter("Infantry", et);
}

CInfantryUnit::~CInfantryUnit()
{
}

void CInfantryUnit::Update()
{


  CUnitSprite::Update();
}

void CInfantryUnit::Render()
{


  CUnitSprite::Render();
}
