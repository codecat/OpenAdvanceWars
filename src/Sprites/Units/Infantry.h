#ifndef INFANTRY_H_INCLUDED
#define INFANTRY_H_INCLUDED

#include "../../StdH.h"
#include "../Unit.h"

class CInfantryUnit : public CUnitSprite
{
public:
  CInfantryUnit();
  CInfantryUnit(ETeam et);
  ~CInfantryUnit();

  void Update();
  void Render();
};

#endif
