#ifndef PORT_H_INCLUDED
#define PORT_H_INCLUDED

#include "../../StdH.h"
#include "../Building.h"

class CPortBuilding : public CBuildingSprite
{
public:
  CPortBuilding();
  CPortBuilding(ETeam et);
  ~CPortBuilding();

  virtual void Update();
  virtual void Render();
};

#endif
