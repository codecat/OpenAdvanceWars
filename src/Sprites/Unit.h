#ifndef SPRITES_CHARACTER_H_INCLUDED
#define SPRITES_CHARACTER_H_INCLUDED

#include "../StdH.h"
#include "../Sprite.h"
#include "Property.h"

// struct used for finding walkable tiles
struct SGridNode
{
  const SGridNode* parent;
  int x;
  int y;
  char dist;

  SGridNode()
  {
    parent = 0;
    x = 0;
    y = 0;
    dist = 0;
  }
};

class CUnitSprite : public CPropertySprite
{
public:
  CString spr_strType;

  int spr_iHealth;
  int spr_iAmmo;
  int spr_iMoveDistance;

  CStackArray<SGridNode> spr_saWalkableNodes;
  CStackArray<SGridNode> spr_saWalkableNodesClosed;

  int spr_iLastMovementX;
  bool spr_bUsed;
  bool spr_bStopNavigating;

public:
  CUnitSprite();
  virtual ~CUnitSprite();

  virtual CAnimation* GetDefaultAnimation();

  virtual bool CanCollideWith(CSprite* pOther);
  virtual bool CanNavigateInto(CSprite* pOther);

  virtual void Select();
  virtual void Unselect();

  virtual void Action(int iType);
  virtual void ActionSelected(int iType);
  virtual void ActionAiming(int iType);

  virtual void CalculateWalkableTiles();
  virtual void AddNode(const SGridNode &parent, int x, int y);

  virtual void SetCharacter(const CString &strType, ETeam et);
  virtual void SetTeam(ETeam et);

  virtual int GetMoveDistance();
  virtual CString GetLookDirectionString();

  virtual void StartMovement(const Vector2i &vDirection);
  virtual void FinishMovement();

  virtual void NavigationFinished();

  virtual void Update();
  virtual void Render();
  virtual void RenderInfoBox(int &x, int &y);
};

#endif
