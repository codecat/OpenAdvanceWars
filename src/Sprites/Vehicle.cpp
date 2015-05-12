#include "../StdH.h"
#include "Vehicle.h"
#include "../Game.h"

CVehicleSprite::CVehicleSprite()
{
  spr_iFuel = 20;
}

CVehicleSprite::~CVehicleSprite()
{
}

void CVehicleSprite::FinishMovement()
{
  if(spr_iFuel > 0) {
    if(--spr_iFuel == 0) {
      // no more fuel, this vehicle can't move anymore.
      spr_bStopNavigating = true;
      NavigationFinished();
    }
  }

  CUnitSprite::FinishMovement();
}

int CVehicleSprite::GetMoveDistance()
{
  return mthMin(spr_iMoveDistance, spr_iFuel);
}

void CVehicleSprite::RenderInfoBox(int &x, int &y)
{
  CUnitSprite::RenderInfoBox(x, y);

  _pGame->gam_Sheets.Render("MenuSprites", "Icon_Fuel", Vector2f((float)x, (float)y));

  CString strFuel;
  strFuel.SetF("%d", spr_iFuel);
  _pGame->Renderer.PutText(strFuel, (float)x + 20, (float)y);

  y += 16;
}
