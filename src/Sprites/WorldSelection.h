#ifndef WORLDSELECTION_H_INCLUDED
#define WORLDSELECTION_H_INCLUDED

#include "../StdH.h"
#include "../Sprite.h"

class CWorldSelection : public CSprite
{
public:
  bool spr_bSelected;
  bool spr_bWalkableVisible;
  bool spr_bAiming;
  CSprite* spr_psprSelected;

  CTexture spr_texArrow;
  CTexture spr_texArrowBig;
  CTexture spr_texAim;

  int spr_iAnimationCount;

public:
  CWorldSelection();
  ~CWorldSelection();

  void Action(int iType);
  void ActionSelected(int iType);
  void ActionAiming(int iType);

  void PositionChanged();

  void Update();
  void Render();
};

#endif
