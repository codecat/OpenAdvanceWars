#ifndef MECH_H_INCLUDED
#define MECH_H_INCLUDED

#include "../../StdH.h"
#include "../Unit.h"

class CMechUnit : public CUnitSprite
{
public:
  CMechUnit();
  CMechUnit(ETeam et);
  ~CMechUnit();

  void Update();
  void Render();
};

#endif
