#include "../StdH.h"
#include "Port.h"

CPortBuilding::CPortBuilding()
{
  SetBuilding("Port", T_NEUTRAL);
}

CPortBuilding::CPortBuilding(ETeam et)
{
  SetBuilding("Port", et);
}

CPortBuilding::~CPortBuilding()
{
}

void CPortBuilding::Update()
{


  CBuildingSprite::Update();
}

void CPortBuilding::Render()
{


  CBuildingSprite::Render();
}
