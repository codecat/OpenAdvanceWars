#include "../StdH.h"
#include "HQ.h"

CHQBuilding::CHQBuilding()
{
  SetBuilding("HQ", T_NEUTRAL);
}

CHQBuilding::CHQBuilding(ETeam et)
{
  SetBuilding("HQ", et);
}

CHQBuilding::~CHQBuilding()
{
}

void CHQBuilding::Update()
{
  if(spr_etTeam == T_NEUTRAL) {
    // neutral HQ's shouldn't exist
    ASSERT(FALSE);
  }

  CBuildingSprite::Update();
}

void CHQBuilding::Render()
{


  CBuildingSprite::Render();
}
