#ifndef CITY_H_INCLUDED
#define CITY_H_INCLUDED

#include "../../StdH.h"
#include "../Building.h"

class CCityBuilding : public CBuildingSprite
{
public:
  CCityBuilding();
  CCityBuilding(ETeam et);
  ~CCityBuilding();

  virtual void Update();
  virtual void Render();
};

#endif
