#ifndef VEHICLE_H_INCLUDED
#define VEHICLE_H_INCLUDED

#include "../StdH.h"
#include "../Sprite.h"
#include "Unit.h"

class CVehicleSprite : public CUnitSprite
{
public:
  int spr_iFuel;

public:
  CVehicleSprite();
  virtual ~CVehicleSprite();

  virtual void FinishMovement();

  virtual int GetMoveDistance();

  virtual void RenderInfoBox(int &x, int &y);
};

#endif
