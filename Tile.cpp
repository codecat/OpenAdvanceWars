#include "StdH.h"
#include "Tile.h"
#include "Game.h"

CTile::CTile()
{
  til_x = -1;
  til_y = -1;
  til_pWorld = &_pGame->gam_World;
  til_Type = ETT_EMPTY;
  til_pAnimation = 0;
  til_pAnimationTopLeft = 0;
  til_pAnimationTopRight = 0;
  til_pAnimationBotLeft = 0;
  til_pAnimationBotRight = 0;
}

CTile::~CTile()
{
}

bool CTile::IsFoggy()
{
  CWorld &world = *GetWorld();
  if(!world.wo_bFogEnabled) {
    return false;
  }

  ASSERT(til_y != -1 && til_x != -1);
  return world.wo_pubVisible[til_y][til_x] == 0;
}

bool CTile::IsSolid()
{
  switch(til_Type) {
  case ETT_OCEAN:
    return true;
  }

  return false;
}

void CTile::Update(int x, int y)
{
  til_x = x;
  til_y = y;
}

void CTile::Render(int x, int y)
{
  ASSERT(til_pAnimation != 0);
  if(til_pAnimation == 0) {
    return;
  }

  Vector2f vPos(x * 32.0f, y * 32.0f);

  CWorld* pw = GetWorld();
  CWorldSelection* pws = GetWorldSelection();

  COLOR colFog = COL_WHITE;
  if(IsFoggy()) {
    colFog = MRAG_COLOR_RGB(128, 128, 128);
  }
  til_pAnimation->ani_colMultiplier = colFog;
  if(til_pAnimationTopLeft  != 0) {  til_pAnimationTopLeft->ani_colMultiplier = colFog; }
  if(til_pAnimationTopRight != 0) { til_pAnimationTopRight->ani_colMultiplier = colFog; }
  if(til_pAnimationBotLeft  != 0) {  til_pAnimationBotLeft->ani_colMultiplier = colFog; }
  if(til_pAnimationBotRight != 0) { til_pAnimationBotRight->ani_colMultiplier = colFog; }

  til_pAnimation->Render(vPos, til_aniState);
  if(til_pAnimationTopLeft  != 0) {  til_pAnimationTopLeft->Render(vPos                , til_aniStateCorners[0]); }
  if(til_pAnimationTopRight != 0) { til_pAnimationTopRight->Render(vPos+Vector2f(16, 0), til_aniStateCorners[1]); }
  if(til_pAnimationBotLeft  != 0) {  til_pAnimationBotLeft->Render(vPos+Vector2f( 0,16), til_aniStateCorners[2]); }
  if(til_pAnimationBotRight != 0) { til_pAnimationBotRight->Render(vPos+Vector2f(16,16), til_aniStateCorners[3]); }

  if(pws->spr_bSelected && pws->spr_bWalkableVisible && pw->wo_pubWalkable[y][x] != 0) {
    CAnimation* pWalkable = _pGame->gam_Sheets.GetSheet("Misc")->GetAnimation("Walkable");
    ASSERT(pWalkable != 0);

    pWalkable->ani_ebmBlendMode = BM_ADD;
    pWalkable->Render(vPos, til_aniStateWalkable);
  }
}

CWorldSelection* CTile::GetWorldSelection()
{
  return _pGame->gam_World.wo_psprSelection;
}

CWorld* CTile::GetWorld()
{
  return til_pWorld;
}
