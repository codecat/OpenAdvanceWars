#include "../StdH.h"
#include "Recon.h"

CReconUnit::CReconUnit()
{
  spr_iMoveDistance = 8;
  spr_iVisibilityDistance = 5;
  spr_etTeam = T_NEUTRAL;
  SetCharacter("Recon", T_NEUTRAL);
}

CReconUnit::CReconUnit(ETeam et)
{
  spr_etTeam = et;
  SetCharacter("Recon", et);
}

CReconUnit::~CReconUnit()
{
}

void CReconUnit::Update()
{


  CUnitSprite::Update();
}

void CReconUnit::Render()
{


  CUnitSprite::Render();
}
