#ifndef PROPERTY_H_INCLUDED
#define PROPERTY_H_INCLUDED

#include "../StdH.h"
#include "../Sprite.h"

// a "Property" sprite is a sprite that is owned by a particular team, and can therefore produce fog visibility.
class CPropertySprite : public CSprite
{
public:
  int spr_iVisibilityDistance;
  ETeam spr_etTeam;

public:
  CPropertySprite();
  virtual ~CPropertySprite();

  virtual void StartTurn();
  virtual void EndTurn();

  virtual void CalculateVisibleTiles();
};

#endif
