#include "../StdH.h"
#include "Silo.h"

CSiloBuilding::CSiloBuilding()
{
  SetBuilding("Silo", T_NEUTRAL);
}

CSiloBuilding::~CSiloBuilding()
{
}

void CSiloBuilding::SetTeam(ETeam et)
{
  // silo's can't be owned, so this is always the same
  spr_etTeam = T_NEUTRAL;
  StartAnimation(spr_strType);
}

void CSiloBuilding::Update()
{


  CBuildingSprite::Update();
}

void CSiloBuilding::Render()
{


  CBuildingSprite::Render();
}
