#ifndef AIRPORT_H_INCLUDED
#define AIRPORT_H_INCLUDED

#include "../../StdH.h"
#include "../Building.h"

class CAirportBuilding : public CBuildingSprite
{
public:
  CAirportBuilding();
  CAirportBuilding(ETeam et);
  ~CAirportBuilding();

  virtual void Update();
  virtual void Render();
};

#endif
