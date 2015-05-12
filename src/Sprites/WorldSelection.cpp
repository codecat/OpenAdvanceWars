#include "StdH.h"
#include "WorldSelection.h"
#include "../Game.h"

CWorldSelection::CWorldSelection()
{
  spr_bSelected = false;
  spr_bWalkableVisible = false;
  spr_bAiming = false;
  spr_psprSelected = 0;
  spr_iAnimationCount = 0;

  spr_texArrow.Load(_pGame->Renderer, "Content/Textures/WS_Arrow.png");
  spr_texArrowBig.Load(_pGame->Renderer, "Content/Textures/WS_ArrowBig.png");
  spr_texAim.Load(_pGame->Renderer, "Content/Textures/WS_Aim.png");

  spr_bNoClip = true;
  spr_iMoveSpeed = 8;
  spr_iLayer = 2;

  spr_vGridPosition = Vector2i(3, 3);
}

CWorldSelection::~CWorldSelection()
{
}

void CWorldSelection::Action(int iType)
{
  CSprite* pSprite = _pGame->gam_World.GetSpriteAt(spr_vGridPosition);
  if(pSprite == 0) {
    return;
  }

  pSprite->Action(iType);
}

void CWorldSelection::ActionSelected(int iType)
{
  if(spr_psprSelected == 0) {
    ASSERT(FALSE);
    spr_bSelected = false;
    spr_bWalkableVisible = false;
    return;
  }

  spr_psprSelected->ActionSelected(iType);
}

void CWorldSelection::ActionAiming(int iType)
{
  if(spr_psprSelected == 0) {
    ASSERT(FALSE);
    spr_bSelected = false;
    return;
  }

  CSprite* pSprite = _pGame->gam_World.GetSpriteAt(spr_vGridPosition);
  if(pSprite == 0) {
    return;
  }
  pSprite->ActionAiming(iType);
}

void CWorldSelection::PositionChanged()
{
  CSprite::PositionChanged();

  if(!spr_bSelected) {
    return;
  }

  CWorld &world = *GetWorld();

  int x = spr_vGridPosition(1);
  int y = spr_vGridPosition(2);

  if(spr_vGridPosition == spr_psprSelected->spr_vGridPosition) {
    world.wo_saArrowPath.Clear();
    return;
  }

  if(world.wo_pubWalkable[y][x] != 0) {
    CPathFinding pf = world.CreatePathfindingObject();
    world.wo_saArrowPath.Clear();
    pf.FindPath(world.wo_saArrowPath, spr_psprSelected->spr_vGridPosition, spr_vGridPosition);
  }
}

void CWorldSelection::Update()
{
  static bool bUseKeyboardControls = false;
  static int iLastMouseX = 0;
  static int iLastMouseY = 0;

  if(spr_bVisible && !_pGame->gam_Menu.men_bVisible) {
    CInput &input = _pGame->Input;

    if(input.IsKeyDown(MKEY_RIGHT)) {
      bUseKeyboardControls = true;
      StartMovement(Vector2i(1, 0));
    }
    if(input.IsKeyDown(MKEY_LEFT)) {
      bUseKeyboardControls = true;
      StartMovement(Vector2i(-1, 0));
    }
    if(input.IsKeyDown(MKEY_DOWN)) {
      bUseKeyboardControls = true;
      StartMovement(Vector2i(0, 1));
    }
    if(input.IsKeyDown(MKEY_UP)) {
      bUseKeyboardControls = true;
      StartMovement(Vector2i(0, -1));
    }
    if(input.IsKeyPressed(MKEY_RETURN)) {
      bUseKeyboardControls = true;
      if(spr_bAiming) {
        ActionAiming(0);
      } else if(spr_bSelected) {
        ActionSelected(0);
      } else {
        Action(0);
      }
    }
    if(input.IsKeyPressed(MKEY_BACKSPACE)) {
      bUseKeyboardControls = true;
      if(spr_bAiming) {
        ActionAiming(1);
      } else if(spr_bSelected) {
        ActionSelected(1);
      } else {
        Action(1);
      }
    }

    int iMouseX = input.GetMouseX();
    int iMouseY = input.GetMouseY();

    if(iMouseX != iLastMouseX || iMouseY != iLastMouseY) {
      bUseKeyboardControls = false;
      iLastMouseX = iMouseX;
      iLastMouseY = iMouseY;
    }

    if(!bUseKeyboardControls) {
      Vector2i vPos(iMouseX / 32, iMouseY / 32);
      if((vPos(1) >= 0 && vPos(1) < _pGame->gam_World.wo_iWidth) &&
         (vPos(2) >= 0 && vPos(2) < _pGame->gam_World.wo_iHeight)) {
        spr_vGridPosition = vPos;
        spr_vMoveAdd = Vector2i(0, 0);
        spr_vMoveTo = Vector2i(0, 0);
        spr_bMoving = false;
      }

      if(input.IsMousePressed(MBUTTON_LEFT)) {
        if(spr_bSelected) {
          ActionSelected(0);
        } else {
          Action(0);
        }
      }

      if(input.IsMousePressed(MBUTTON_RIGHT)) {
        if(spr_bSelected) {
          ActionSelected(1);
        } else {
          Action(1);
        }
      }
    }
  }

  CSprite::Update();
}

void CWorldSelection::Render()
{
  Vector2i vPos = GetRenderPosition();
  const float x = (float)vPos(1);
  const float y = (float)vPos(2);

  if(spr_bAiming) {
    // aiming selection
    if(_pGame->gam_World.GetSpriteAt(spr_vGridPosition) != 0) {
      _pGame->Renderer.PutTexture(spr_texAim, x - 12, y - 12);
    }
  } else {
    const float w = 32;
    const float h = 32;
    const float ww = (float)spr_texArrow.tex_iWidth;
    const float hh = (float)spr_texArrow.tex_iHeight;
    const float wwb = (float)spr_texArrowBig.tex_iWidth;
    const float hhb = (float)spr_texArrowBig.tex_iHeight;

    float offset = 10;

    if(spr_iAnimationCount >= 20 && spr_iAnimationCount < 25) {
      offset = 8;
    } else if(spr_iAnimationCount >= 25 && spr_iAnimationCount < 30) {
      offset = 6;
    } else if(spr_iAnimationCount >= 30 && spr_iAnimationCount < 35) {
      offset = 8;
    }
    if(++spr_iAnimationCount >= 40) {
      spr_iAnimationCount = 0;
    }

    // top left
    _pGame->Renderer.PutTexture(spr_texArrow, x - offset, y - offset);
    // top right
    _pGame->Renderer.PutTexture(spr_texArrow, x + (w-ww) + offset - 1, y - offset, ww, hh, 90);
    // bottom left
    _pGame->Renderer.PutTexture(spr_texArrow, x - offset, y + (h-hh) + offset + 1, ww, hh, 270);
    // bottom right
    _pGame->Renderer.PutTexture(spr_texArrowBig, x + (w-ww) + offset + 2, y + (h-hh) + offset + 2);
  }
}
