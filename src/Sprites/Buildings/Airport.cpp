#include "StdH.h"
#include "Airport.h"

CAirportBuilding::CAirportBuilding()
{
  SetBuilding("Airport", T_NEUTRAL);
}

CAirportBuilding::CAirportBuilding(ETeam et)
{
  SetBuilding("Airport", et);
}

CAirportBuilding::~CAirportBuilding()
{
}

void CAirportBuilding::Update()
{


  CBuildingSprite::Update();
}

void CAirportBuilding::Render()
{


  CBuildingSprite::Render();
}
