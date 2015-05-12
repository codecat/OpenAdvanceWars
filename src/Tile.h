#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include "StdH.h"
#include <Mragpp/Rectangle.h>
#include <Mragpp/AnimationSheet.h>

class CWorldSelection;
class CWorld;

enum ETileType : int
{
  ETT_EMPTY = 0,

  ETT_OCEAN = 1,
  ETT_GRASS = 2,
};

class CTile
{
public:
  int til_x;
  int til_y;

  ETileType til_Type;
  CWorld* til_pWorld;

  CAnimationState til_aniState;
  CAnimationState til_aniStateWalkable;
  CAnimation* til_pAnimation;

  // corners
  CAnimation* til_pAnimationTopLeft;
  CAnimation* til_pAnimationTopRight;
  CAnimation* til_pAnimationBotLeft;
  CAnimation* til_pAnimationBotRight;
  CAnimationState til_aniStateCorners[4];

public:
  CTile();
  ~CTile();

  bool IsFoggy();
  bool IsSolid();

  void Update(int x, int y);
  void Render(int x, int y);

  CWorldSelection* GetWorldSelection();
  CWorld* GetWorld();
};

#endif
