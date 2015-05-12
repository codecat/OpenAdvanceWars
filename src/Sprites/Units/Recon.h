#ifndef RECON_H_INCLUDED
#define RECON_H_INCLUDED

#include "../../StdH.h"
#include "../Unit.h"
#include "../Vehicle.h"

class CReconUnit : public CVehicleSprite
{
public:
  CReconUnit();
  CReconUnit(ETeam et);
  ~CReconUnit();

  void Update();
  void Render();
};

#endif
