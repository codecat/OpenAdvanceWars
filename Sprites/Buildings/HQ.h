#ifndef HQ_H_INCLUDED
#define HQ_H_INCLUDED

#include "../../StdH.h"
#include "../Building.h"

class CHQBuilding : public CBuildingSprite
{
public:
  CHQBuilding();
  CHQBuilding(ETeam et);
  ~CHQBuilding();

  virtual void Update();
  virtual void Render();
};

#endif
