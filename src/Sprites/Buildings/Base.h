#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include "../../StdH.h"
#include "../Building.h"

class CBaseBuilding : public CBuildingSprite
{
public:
  CBaseBuilding();
  CBaseBuilding(ETeam et);
  ~CBaseBuilding();

  virtual void Update();
  virtual void Render();
};

#endif
