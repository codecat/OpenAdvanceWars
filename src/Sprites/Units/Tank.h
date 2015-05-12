#ifndef TANK_H_INCLUDED
#define TANK_H_INCLUDED

#include "../../StdH.h"
#include "../Unit.h"
#include "../Vehicle.h"

class CTankUnit : public CVehicleSprite
{
public:
  CTankUnit();
  CTankUnit(ETeam et);
  ~CTankUnit();

  void Update();
  void Render();
};

#endif
