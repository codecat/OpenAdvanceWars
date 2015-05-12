#include "StdH.h"
#include "Game.h"
#include "Tile.h"
#include "Sprite.h"

CSprite::CSprite()
{
  spr_iLayer = 0;
  spr_bSolid = false;
  spr_bNoClip = false;
  spr_bVisible = true;
  spr_iMoveSpeed = 4;
  spr_bMoving = false;
  spr_fRotation = 0.0f;
  spr_bNavigating = false;
  spr_pOnTile = 0;
}

CSprite::~CSprite()
{
}

CAnimation* CSprite::GetDefaultAnimation()
{
  return 0;
}

void CSprite::StartTurn()
{
}

void CSprite::EndTurn()
{
}

bool CSprite::CanCollideWith(CSprite* pOther)
{
  return true;
}

bool CSprite::CanNavigateInto(CSprite* pOther)
{
  return true;
}

Vector2i CSprite::GetRenderPosition()
{
  return Vector2i(
    spr_vGridPosition(1) * 32 + spr_vMoveAdd(1),
    spr_vGridPosition(2) * 32 + spr_vMoveAdd(2));
}

void CSprite::Action(int iType)
{
}

void CSprite::ActionSelected(int iType)
{
}

void CSprite::ActionAiming(int iType)
{
}

void CSprite::StartAnimation(const CString &strName)
{
  spr_strAnimation = strName;
  spr_aniState.Reset();
}

void CSprite::StartMovement(const Vector2i &vDirection)
{
  if(spr_bMoving) {
    return;
  }

  Vector2i vNewPos = spr_vGridPosition + vDirection;
  if(vNewPos(1) < 0 || vNewPos(2) < 0) {
    // out of bounds of the world, can't move here
    return;
  }

  CTile* pTileAt = _pGame->gam_World.GetTileAt(vNewPos);

  if(pTileAt == 0) {
    // no tile here, can't move here
    return;
  }

  if(pTileAt->IsSolid() && !spr_bNoClip) {
    // tile is solid, can't move here
    return;
  }

  CSprite* pSpriteAt = _pGame->gam_World.GetSpriteAt(vNewPos);

  if(pSpriteAt != 0 && pSpriteAt->spr_bSolid && !spr_bNoClip && !pSpriteAt->spr_bNoClip) {
    // sprite is solid, can't move here
    return;
  }

  spr_bMoving = true;
  spr_vMoveTo = vDirection;
}

void CSprite::FinishMovement()
{
  spr_bMoving = false;
  spr_vGridPosition += spr_vMoveTo;
  spr_vMoveAdd.Clear();
  spr_vMoveTo.Clear();
}

void CSprite::PositionChanged()
{
  spr_pOnTile = GetWorld()->GetTileAt(spr_vGridPosition);
}

void CSprite::NavigateTo(const Vector2i &vDestination)
{
  if(spr_bNavigating) {
    return;
  }

  spr_vPreNavigation = spr_vGridPosition;

  CPathFinding pf = _pGame->gam_World.CreatePathfindingObject();

  spr_saNavigationPath.Clear();
  if(!pf.FindPath(spr_saNavigationPath, spr_vGridPosition, vDestination)) {
    return;
  }

  spr_bNavigating = true;
}

void CSprite::NavigationFinished()
{
}

void CSprite::Update()
{
  if(spr_bNavigating && !spr_bMoving) {
    if(spr_saNavigationPath.Count() == 0) {
      // you have reached your destination.
      spr_bNavigating = false;
      NavigationFinished();
    } else {
      Vector2i &vNextPos = *spr_saNavigationPath.PopAt(0);

      const int iDirX = vNextPos(1) - spr_vGridPosition(1);
      const int iDirY = vNextPos(2) - spr_vGridPosition(2);
      ASSERT((iDirX == 0 && iDirY != 0) || (iDirX != 0 && iDirY == 0));

      CSprite* pHittingSprite = GetWorld()->GetSpriteAt(vNextPos);
      if(pHittingSprite != 0) {
        if(!CanNavigateInto(pHittingSprite)) {
          // this sprite isn't allowed to navigate into this other sprite, so abandon our navigation entirely
          spr_bNavigating = false;
          FinishMovement();
          NavigationFinished();
        } else {
          StartMovement(Vector2i(iDirX, iDirY));
        }
      } else {
        StartMovement(Vector2i(iDirX, iDirY));
      }

      delete &vNextPos;
    }
  }

  if(spr_bMoving) {
    int iDirX = spr_vMoveTo(1);
    int iDirY = spr_vMoveTo(2);

    ASSERT((iDirX == 0 && iDirY != 0) || (iDirX != 0 && iDirY == 0));

    if(iDirX > 0) {
      spr_vMoveAdd(1) += spr_iMoveSpeed;
      if(spr_vMoveAdd(1) >= 32) {
        FinishMovement();
      }
    } else if(iDirX < 0) {
      spr_vMoveAdd(1) -= spr_iMoveSpeed;
      if(spr_vMoveAdd(1) <= -32) {
        FinishMovement();
      }
    } else if(iDirY > 0) {
      spr_vMoveAdd(2) += spr_iMoveSpeed;
      if(spr_vMoveAdd(2) >= 32) {
        FinishMovement();
      }
    } else if(iDirY < 0) {
      spr_vMoveAdd(2) -= spr_iMoveSpeed;
      if(spr_vMoveAdd(2) <= -32) {
        FinishMovement();
      }
    }
  }

  if(spr_vGridPosition != spr_vGridPositionPrev) {
    spr_vGridPositionPrev = spr_vGridPosition;
    PositionChanged();
  }
}

void CSprite::Render()
{
  if(!spr_bVisible) {
    return;
  }
  _pGame->gam_Sheets.Render(spr_strSheet, spr_strAnimation, GetRenderPosition().GetAs<2,float>(), spr_aniState, spr_fRotation);
}

void CSprite::RenderInfoBox(int &x, int &y)
{
}

CWorldSelection* CSprite::GetWorldSelection()
{
  return _pGame->gam_World.wo_psprSelection;
}

CWorld* CSprite::GetWorld()
{
  return &_pGame->gam_World;
}
