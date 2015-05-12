#ifndef SILO_H_INCLUDED
#define SILO_H_INCLUDED

#include "../../StdH.h"
#include "../Building.h"

class CSiloBuilding : public CBuildingSprite
{
public:
  CSiloBuilding();
  ~CSiloBuilding();

  void SetTeam(ETeam et);

  virtual void Update();
  virtual void Render();
};

#endif
