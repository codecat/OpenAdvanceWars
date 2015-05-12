#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include "StdH.h"
#include <Mragpp/AnimationSheet.h>

class CWorldSelection;
class CWorld;
class CTile;

class CSprite
{
public:
  CString spr_strSheet;
  CString spr_strAnimation;
  CAnimationState spr_aniState;

  int spr_iLayer;
  bool spr_bSolid;
  bool spr_bNoClip;
  bool spr_bVisible;

  int spr_iMoveSpeed;
  bool spr_bMoving;

  Vector2i spr_vGridPositionPrev;
  Vector2i spr_vGridPosition;
  Vector2i spr_vMoveAdd;
  Vector2i spr_vMoveTo;

  float spr_fRotation;

  bool spr_bNavigating;
  Vector2i spr_vPreNavigation;
  CStackArray<Vector2i> spr_saNavigationPath;

  CTile* spr_pOnTile;

public:
  CSprite();
  virtual ~CSprite();

  virtual CAnimation* GetDefaultAnimation();

  virtual void StartTurn();
  virtual void EndTurn();

  virtual bool CanCollideWith(CSprite* pOther);
  virtual bool CanNavigateInto(CSprite* pOther);

  virtual Vector2i GetRenderPosition();

  virtual void Action(int iType);
  virtual void ActionSelected(int iType);
  virtual void ActionAiming(int iType);

  virtual void StartAnimation(const CString &strName);

  virtual void StartMovement(const Vector2i &vDirection);
  virtual void FinishMovement();

  virtual void PositionChanged();

  virtual void NavigateTo(const Vector2i &vDestination);
  virtual void NavigationFinished();

  virtual void Update();
  virtual void Render();
  virtual void RenderInfoBox(int &x, int &y);

  virtual CWorldSelection* GetWorldSelection();
  virtual CWorld* GetWorld();
};

#endif
