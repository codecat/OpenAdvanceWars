#include "../StdH.h"
#include "City.h"

CCityBuilding::CCityBuilding()
{
  SetBuilding("City", T_NEUTRAL);
}

CCityBuilding::CCityBuilding(ETeam et)
{
  SetBuilding("City", et);
}

CCityBuilding::~CCityBuilding()
{
}

void CCityBuilding::Update()
{


  CBuildingSprite::Update();
}

void CCityBuilding::Render()
{


  CBuildingSprite::Render();
}
