#include "StdH.h"
#include "Base.h"

CBaseBuilding::CBaseBuilding()
{
  SetBuilding("Base", T_NEUTRAL);
}

CBaseBuilding::CBaseBuilding(ETeam et)
{
  SetBuilding("Base", et);
}

CBaseBuilding::~CBaseBuilding()
{
}

void CBaseBuilding::Update()
{


  CBuildingSprite::Update();
}

void CBaseBuilding::Render()
{


  CBuildingSprite::Render();
}
