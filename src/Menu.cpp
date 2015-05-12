#include "StdH.h"
#include "Menu.h"
#include "Game.h"

CMenuItem::CMenuItem()
{
  mi_bEnabled = true;
}

CMenuItem::~CMenuItem()
{
}

CString CMenuItem::GetIconName()
{
  return "Icon_" + mi_strIcon;
}

CMenu::CMenu()
{
  men_bVisible = false;
  men_iCurrentItem = 0;
  men_iAnimationCount = 0;
}

CMenu::~CMenu()
{
}

void CMenu::Initialize()
{
  men_texArrow.Load(_pGame->Renderer, "Content/Textures/Menu_Arrow.png");
}

void CMenu::Begin()
{
  men_bVisible = false;
  men_iCurrentItem = 0;
  men_saItems.Clear();
}

void CMenu::Begin(const CAction &onClose)
{
  Begin();
  men_onClose = onClose;
}

CMenuItem &CMenu::Add()
{
  return men_saItems.Push();
}

void CMenu::Add(const CString &strText, const CString &strIcon, bool bEnabled, CAction func)
{
  CMenuItem &item = men_saItems.Push();
  item.mi_strText = strText;
  item.mi_strIcon = strIcon;
  item.mi_bEnabled = bEnabled;
  item.mi_func = func;
}

void CMenu::Show()
{
  men_bVisible = true;
}

void CMenu::Close()
{
  // Note: Do not clear men_saItems here!
  men_bVisible = false;
  if(men_onClose) {
    men_onClose();
  }
}

void CMenu::Update()
{
  static int iLastMouseX = 0;
  static int iLastMouseY = 0;

  if(!men_bVisible) {
    return;
  }

  CInput &input = _pGame->Input;

  const int ctItems = men_saItems.Count();

  CRectangle rectMenu = GetSmallMenuRect(ctItems);

  const int iMouseX = input.GetMouseX();
  const int iMouseY = input.GetMouseY();

  static bool bMouseOver = false;
  if(iMouseX != iLastMouseX || iMouseY != iLastMouseY) {
    iLastMouseX = iMouseX;
    iLastMouseY = iMouseY;

    bMouseOver = rectMenu.Contains(Vector2i(iMouseX, iMouseY));

    if(bMouseOver) {
      const int iOffsetY = iMouseY - (int)rectMenu.y;
      men_iCurrentItem = (int)mthFloorF(iOffsetY / 32.0f);
    }
  }

  if(input.IsKeyPressed(MKEY_DOWN)) {
    if(++men_iCurrentItem >= men_saItems.Count()) {
      men_iCurrentItem = 0;
    }
  }
  if(input.IsKeyPressed(MKEY_UP)) {
    if(--men_iCurrentItem < 0) {
      men_iCurrentItem = men_saItems.Count() - 1;
    }
  }

  if(input.IsKeyPressed(MKEY_RETURN) || (bMouseOver && input.IsMousePressed(MBUTTON_LEFT))) {
    CMenuItem &item = men_saItems[men_iCurrentItem];
    men_strLastAction = item.mi_strText;

    Close();

    if(item.mi_func) {
      item.mi_func();
    }
  }
}

void CMenu::Render()
{
  _pGame->Renderer.PutTexture(men_texArrow, 0, 100);

  if(!men_bVisible) {
    return;
  }

  CRenderer &rend = _pGame->Renderer;
  CAnimationSheetCollection &sheets = _pGame->gam_Sheets;

  const int ctItems = men_saItems.Count();
  Vector2f vOrigin(_pGame->gam_iWidth - 270.0f, 30.0f);
  RenderBlock(GetSmallMenuRect(ctItems, vOrigin));

  // offset for the selection arrow
  float offset = 0;
  if(men_iAnimationCount >= 20 && men_iAnimationCount < 25) {
    offset = 2;
  } else if(men_iAnimationCount >= 25 && men_iAnimationCount < 30) {
    offset = 4;
  } else if(men_iAnimationCount >= 30 && men_iAnimationCount < 35) {
    offset = 2;
  }
  if(++men_iAnimationCount >= 40) {
    men_iAnimationCount = 0;
  }

  rend.SetColor(COL_BLACK | MRAG_ALPHA_OPAQUE);

  for(int i=0; i<ctItems; i++) {
    CMenuItem &item = men_saItems[i];

    Vector2f vIcon = vOrigin + Vector2f(10, i * 32.0f);
    Vector2i vIconSize = sheets.Render("MenuSprites", item.GetIconName(), vIcon);

    Vector2f vText = vIcon + Vector2f((float)vIconSize(1) + 4, 4);
    rend.PutText(item.mi_strText, vText(1), vText(2));

    if(men_iCurrentItem == i) {
      rend.PutTexture(men_texArrow, vIcon(1) - men_texArrow.tex_iWidth + offset, vIcon(2) + 2);
    }
  }
}

CRectangle CMenu::GetSmallMenuRect(int ctItems)
{
  return GetSmallMenuRect(ctItems, Vector2f(_pGame->gam_iWidth - 270.0f, 30.0f));
}

CRectangle CMenu::GetSmallMenuRect(int ctItems, Vector2f vOrigin)
{
  const float fContentWidth = 250.0f;
  const float fContentHeight = ctItems * 32.0f + 4;
  return CRectangle(vOrigin(1), vOrigin(2), fContentWidth, fContentHeight);
}

void CMenu::RenderBlock(const CRectangle &rect)
{
  CAnimationSheetCollection &sheets = _pGame->gam_Sheets;

  const float x = rect.x;
  const float y = rect.y;
  const float w = rect.w;
  const float h = rect.h;

  const Vector2i szTopLeft = sheets.GetSize("Menu", "TopLeft_Red");
  const Vector2i szTop = sheets.GetSize("Menu", "Top_Red");
  const Vector2i szLeft = sheets.GetSize("Menu", "Left_Red");
  const Vector2i szRight = sheets.GetSize("Menu", "Right_Red");
  const Vector2i szBotLeft = sheets.GetSize("Menu", "BotLeft_Red");
  const Vector2i szBot = sheets.GetSize("Menu", "Bot_Red");

  sheets.Render("Menu", "TopLeft_Red",  Vector2f(x - szTopLeft(1), y - szTop(2))                                              );
  sheets.Render("Menu", "Top_Red",      Vector2f(               x, y - szTop(2)), Vector2f(                w, (float)szTop(2)));
  sheets.Render("Menu", "TopRight_Red", Vector2f(           x + w, y - szTop(2))                                              );
  sheets.Render("Menu", "Left_Red",     Vector2f(   x - szLeft(1),            y), Vector2f( (float)szLeft(1),               h));
  sheets.Render("Menu", "Mid_Red",      Vector2f(               x,            y), Vector2f(                w,               h));
  sheets.Render("Menu", "Right_Red",    Vector2f(           x + w,            y), Vector2f((float)szRight(1),               h));
  sheets.Render("Menu", "BotLeft_Red",  Vector2f(x - szBotLeft(1),        y + h)                                              );
  sheets.Render("Menu", "Bot_Red",      Vector2f(               x,        y + h), Vector2f(                w, (float)szBot(2)));
  sheets.Render("Menu", "BotRight_Red", Vector2f(           x + w,        y + h)                                              );
}
