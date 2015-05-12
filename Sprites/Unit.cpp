#include "StdH.h"
#include "../Game.h"
#include "Unit.h"
#include "WorldSelection.h"
#include <Mragpp/Pathfinding/Pathfinding.h>

CUnitSprite::CUnitSprite()
{
  spr_iHealth = 10;
  spr_iAmmo = 10;
  spr_iMoveDistance = 3;
  spr_iVisibilityDistance = 2;
  spr_etTeam = T_NEUTRAL;
  spr_iLastMovementX = 0;
  spr_iLayer = 1;
  spr_bSolid = false;
  spr_bUsed = false;
  spr_bStopNavigating = false;
}

CUnitSprite::~CUnitSprite()
{
}

CAnimation* CUnitSprite::GetDefaultAnimation()
{
  return _pGame->gam_Sheets.GetSheet(spr_strSheet)->GetAnimation("Idle_Right");
}

bool CUnitSprite::CanCollideWith(CSprite* pOther)
{
  return dynamic_cast<CUnitSprite*>(pOther) == 0;
}

bool CUnitSprite::CanNavigateInto(CSprite* pOther)
{
  CUnitSprite* pUnit = dynamic_cast<CUnitSprite*>(pOther);
  if(pUnit != 0 && pUnit->spr_pOnTile != 0) {
    // navigating into an enemy on a foggy tile? ambushed!
    if(pUnit->spr_etTeam != spr_etTeam && pUnit->spr_pOnTile->IsFoggy()) {
      spr_bStopNavigating = true;
      return false;
    }
  }

  return true;
}

void CUnitSprite::Select()
{
  CalculateWalkableTiles();

  CWorldSelection* pws = GetWorldSelection();
  pws->spr_bSelected = true;
  pws->spr_bWalkableVisible = true;
  pws->spr_psprSelected = this;

  CWorld* pw = GetWorld();
  pw->wo_psprArrowSource = this;
  pw->wo_saArrowPath.Clear();
}

void CUnitSprite::Unselect()
{
  CWorld* pw = GetWorld();
  pw->wo_psprArrowSource = 0;
  pw->EmptyWalkable();
  pw->CalculateVisibility();

  CWorldSelection* pws = GetWorldSelection();
  pws->spr_bAiming = false;
  pws->spr_bSelected = false;
  pws->spr_bWalkableVisible = false;
  pws->spr_psprSelected = 0;
}

void CUnitSprite::Action(int iType)
{
  if(spr_bUsed) {
    return;
  }

  if(spr_etTeam != _pGame->gam_etPlayingTeam) {
    return;
  }

  Select();
}

void CUnitSprite::ActionSelected(int iType)
{
  if(iType == 0) {
    // primary, navigate to destination
    CWorldSelection* pws = GetWorldSelection();

    const int x = pws->spr_vGridPosition(1);
    const int y = pws->spr_vGridPosition(2);

    if(GetWorld()->wo_pubWalkable[y][x] == 0) {
      return;
    }

    Vector2i vDestination = Vector2i(x, y);

    CSprite* pSpriteAt = GetWorld()->GetSpriteAt(vDestination);
    if(pSpriteAt != 0 && !pSpriteAt->CanCollideWith(this)) {
      // we can move to this spot only if the tile is foggy
      // the unit will then proceed to get ambushed by the enemy as it hits it while navigating
      if(pSpriteAt->spr_pOnTile != 0 && !pSpriteAt->spr_pOnTile->IsFoggy()) {
        return;
      }
    }

    GetWorld()->wo_psprArrowSource = 0;
    pws->spr_bWalkableVisible = false;
    NavigateTo(vDestination);
  } else if(iType == 1) {
    // secondary, cancel
    Unselect();
  }
}

void CUnitSprite::ActionAiming(int iType)
{
  printf("Starting a fight\n");
  Unselect();
}

void CUnitSprite::CalculateWalkableTiles()
{
  GetWorld()->EmptyWalkable();

  ASSERT(spr_saWalkableNodes.Count() == 0);
  ASSERT(spr_saWalkableNodesClosed.Count() == 0);

  SGridNode &rootNode = spr_saWalkableNodes.Push();
  rootNode.x = spr_vGridPosition(1);
  rootNode.y = spr_vGridPosition(2);

  while(spr_saWalkableNodes.Count() > 0) {
    SGridNode* pNode = spr_saWalkableNodes.Pop();

    if(pNode->dist < spr_iMoveDistance) {
      AddNode(*pNode, -1, 0);
      AddNode(*pNode, 1, 0);
      AddNode(*pNode, 0, -1);
      AddNode(*pNode, 0, 1);
    }

    spr_saWalkableNodesClosed.Push(pNode);
  }

  int ctClosedNodes = spr_saWalkableNodesClosed.Count();
  if(ctClosedNodes > 500) {
    printf("Performance warning: %d closed nodes in walkable path finding\n", ctClosedNodes);
  }

  while(spr_saWalkableNodesClosed.Count() > 0) {
    SGridNode* pNode = spr_saWalkableNodesClosed.Pop();
    GetWorld()->wo_pubWalkable[pNode->y][pNode->x] = pNode->dist;
    delete pNode;
  }
}

void CUnitSprite::AddNode(const SGridNode &parent, int x, int y)
{
  ASSERT(parent.dist < spr_iMoveDistance);

  const int w = _pGame->gam_World.wo_iWidth;
  const int h = _pGame->gam_World.wo_iHeight;
  const int newX = parent.x + x;
  const int newY = parent.y + y;

  // world boundaries
  if(newX < 0 || newX >= w) { return; }
  if(newY < 0 || newY >= h) { return; }

  // solid tiles
  if(_pGame->gam_World.wo_Tiles[newY][newX].IsSolid()) {
    return;
  }

  // if already queued for walking
  if(spr_saWalkableNodes.FindAny([=](SGridNode &node) { return node.x == newX && node.y == newY; }) != -1) {
    return;
  }

  // trying to move to a tile that this path already moved on
  const SGridNode* pParent = parent.parent;
  while(pParent != 0) {
    if(pParent->x == newX && pParent->y == newY) {
      return;
    }
    pParent = pParent->parent;
  }

  SGridNode &newNode = spr_saWalkableNodes.Push();
  newNode.parent = &parent;
  newNode.x = newX;
  newNode.y = newY;
  newNode.dist = parent.dist + 1;
}

void CUnitSprite::SetCharacter(const CString &strType, ETeam et)
{
  spr_strType = strType;
  SetTeam(et);
}

void CUnitSprite::SetTeam(ETeam et)
{
  spr_etTeam = et;

  spr_strSheet = spr_strType + "_" + GetTeamName(spr_etTeam);
  StartAnimation("Idle_Right");
}

int CUnitSprite::GetMoveDistance()
{
  return spr_iMoveDistance;
}

CString CUnitSprite::GetLookDirectionString()
{
  if(spr_bMoving) {
    int iToX = spr_vMoveTo(1);
    int iToY = spr_vMoveTo(2);

    if(iToX == 0 && iToY != 0) {
      if(iToY > 0) {
        return "Down";
      } else {
        return "Up";
      }
    } else if(iToX != 0 && iToY == 0) {
      if(iToX > 0) {
        return "Right";
      } else{
        return "Left";
      }
    } else {
      ASSERT(FALSE);
    }
  } else {
    if(spr_iLastMovementX > 0) {
      return "Right";
    } else {
      return "Left";
    }
  }
  return "Left";
}

void CUnitSprite::StartMovement(const Vector2i &vDirection)
{
  if(spr_bMoving) {
    return;
  }

  if(vDirection(1) != 0) {
    spr_iLastMovementX = vDirection(1);
  }

  CSprite::StartMovement(vDirection);

  if(spr_bMoving) {
    StartAnimation("Move_" + GetLookDirectionString());
  } else {
    StartAnimation("Idle_" + GetLookDirectionString());
  }
}

void CUnitSprite::FinishMovement()
{
  CSprite::FinishMovement();

  if(!spr_bNavigating) {
    StartAnimation("Idle_" + GetLookDirectionString());
  }
}

void CUnitSprite::NavigationFinished()
{
  CSprite::NavigationFinished();

  if(spr_bStopNavigating) {
    // don't show the action menu. for example, if we got ambushed, this unit's turn is over
    spr_bStopNavigating = false;
    spr_bUsed = true;
    Unselect();
    return;
  }

  CWorld* pw = GetWorld();
  CWorldSelection* pws = GetWorldSelection();

  if(pws->spr_bSelected) {
    CMenu &menu = _pGame->gam_Menu;
    menu.Begin([=]() {
      if(menu.men_strLastAction == "Fire") {
        //TODO
      } else {
        StartAnimation("Idle_" + GetLookDirectionString());
      }
    });

    // get units neighbouring this unit
    CUnitSprite* pNeighbours[4];
    pNeighbours[0] = pw->GetUnitAt(spr_vGridPosition + Vector2i(0, -1));
    pNeighbours[1] = pw->GetUnitAt(spr_vGridPosition + Vector2i(0, 1));
    pNeighbours[2] = pw->GetUnitAt(spr_vGridPosition + Vector2i(-1, 0));
    pNeighbours[3] = pw->GetUnitAt(spr_vGridPosition + Vector2i(1, 0));

    bool bFireMenu = false;
    for(int i=0; i<4; i++) {
      if(pNeighbours[i] != 0 && pNeighbours[i]->spr_etTeam != spr_etTeam) {
        bFireMenu = true;
      }
    }

    if(bFireMenu) {
      menu.Add("Fire", "Fire", true, [=]{
        pws->spr_bAiming = true;
      });
    }

    menu.Add("Wait", "Wait", true, [=]{
      spr_bUsed = true;
      Unselect();
    });

    menu.Add("Cancel", "EndTurn", true, [=]{
      spr_vGridPosition = spr_vPreNavigation;
      pws->spr_bWalkableVisible = true;
      pw->wo_psprArrowSource = this;
    });

    menu.Show();
  }
}

void CUnitSprite::Update()
{


  CSprite::Update();
}

void CUnitSprite::Render()
{
  ASSERT(spr_pOnTile != 0);
  if(spr_pOnTile->IsFoggy() && spr_etTeam != _pGame->gam_etPlayingTeam) {
    return;
  }

  if(spr_bUsed) {
    _pGame->gam_Sheets.SetColorMult(spr_strSheet, spr_strAnimation, MRAG_COLOR_RGB(128, 128, 128));
  } else {
    _pGame->gam_Sheets.SetColorMult(spr_strSheet, spr_strAnimation, COL_WHITE);
  }

  CSprite::Render();
}

void CUnitSprite::RenderInfoBox(int &x, int &y)
{
  _pGame->gam_Sheets.Render("MenuSprites", "Icon_Heart", Vector2f((float)x, (float)y));

  CString strHealth;
  strHealth.SetF("%d", spr_iHealth);
  _pGame->Renderer.PutText(strHealth, (float)x + 20, (float)y);

  y += 16;
}
