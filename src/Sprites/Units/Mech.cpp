#include "StdH.h"
#include "Mech.h"

CMechUnit::CMechUnit()
{
  spr_iMoveDistance = 2;
  spr_iVisibilityDistance = 2;
  spr_etTeam = T_NEUTRAL;
  SetCharacter("Mech", T_NEUTRAL);
}

CMechUnit::CMechUnit(ETeam et)
{
  spr_etTeam = et;
  SetCharacter("Mech", et);
}

CMechUnit::~CMechUnit()
{
}

void CMechUnit::Update()
{


  CUnitSprite::Update();
}

void CMechUnit::Render()
{


  CUnitSprite::Render();
}
