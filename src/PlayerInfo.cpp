#include "StdH.h"
#include "PlayerInfo.h"
#include "Game.h"

CTexture CPlayerInfo::pi_texBlock;

CPlayerInfo::CPlayerInfo()
{
  pi_iGold = 10000;
  pi_etTeam = T_RED;
  pi_bPlayable = true;

  if(!pi_texBlock.tex_bLoaded) {
    pi_texBlock.Load(_pGame->Renderer, "Content/Textures/PlayerInfo.png");
  }
}

CPlayerInfo::~CPlayerInfo()
{
}

void CPlayerInfo::Update(bool bPlaying)
{
}

void CPlayerInfo::Render(bool bPlaying)
{
  if(!bPlaying) {
    return;
  }

  const int iOffsetX = 20;
  const int iOffsetY = 10;

  CRenderer &rend = _pGame->Renderer;
  rend.PutTexture(pi_texBlock, (float)iOffsetX, (float)iOffsetY);

  CString strGoldText;
  strGoldText.SetF("%d", pi_iGold);
  rend.SetColor(COL_WHITE|MRAG_ALPHA_OPAQUE);
  rend.PutTextRight(strGoldText, (float)(iOffsetX + pi_texBlock.tex_iWidth - 6), (float)(iOffsetY + 6));
}
