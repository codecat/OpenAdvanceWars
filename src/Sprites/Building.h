#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include "../StdH.h"
#include "../Sprite.h"
#include "Property.h"

class CBuildingSprite : public CPropertySprite
{
public:
  CString spr_strType;

public:
  CBuildingSprite();
  virtual ~CBuildingSprite();

  virtual CAnimation* GetDefaultAnimation();

  virtual void Action(int iType);

  virtual void SetBuilding(const CString &strType, ETeam et);
  virtual void SetTeam(ETeam et);

  virtual void Update();
  virtual void Render();
};

#endif
