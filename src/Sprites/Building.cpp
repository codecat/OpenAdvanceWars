#include "../StdH.h"
#include "../Game.h"
#include "../Tile.h"
#include "Building.h"

CBuildingSprite::CBuildingSprite()
{
  spr_strSheet = "Buildings";
  spr_iVisibilityDistance = 0; // this is 0 for Advance Wars (only its own tile) we can up it in our own game
}

CBuildingSprite::~CBuildingSprite()
{
}

CAnimation* CBuildingSprite::GetDefaultAnimation()
{
  return _pGame->gam_Sheets.GetSheet(spr_strSheet)->GetAnimation(spr_strAnimation);
}

void CBuildingSprite::Action(int iType)
{

}

void CBuildingSprite::SetBuilding(const CString &strType, ETeam et)
{
  spr_strType = strType;
  SetTeam(et);
}

void CBuildingSprite::SetTeam(ETeam et)
{
  spr_etTeam = et;

  StartAnimation(spr_strType + "_" + GetTeamName(spr_etTeam));
}

void CBuildingSprite::Update()
{


  CSprite::Update();
}

void CBuildingSprite::Render()
{
  ASSERT(spr_pOnTile != 0);
  if(spr_pOnTile->IsFoggy()) {
    _pGame->gam_Sheets.SetColorMult(spr_strSheet, spr_strAnimation, MRAG_COLOR_RGB(128, 128, 128));
  } else {
    _pGame->gam_Sheets.SetColorMult(spr_strSheet, spr_strAnimation, COL_WHITE);
  }

  CSprite::Render();
}
