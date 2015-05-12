#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "StdH.h"
#include <Mragpp/Texture.h>
#include <Mragpp/Function.h>
#include <Mragpp/Rectangle.h>

class CMenuItem
{
public:
  CString mi_strText;
  CString mi_strIcon;
  bool mi_bEnabled;
  CAction mi_func;

public:
  CMenuItem();
  ~CMenuItem();

  CString GetIconName();
};

class CMenu
{
public:
  CStackArray<CMenuItem> men_saItems;
  CTexture men_texArrow;
  bool men_bVisible;
  int men_iCurrentItem;

  int men_iAnimationCount;

  CString men_strLastAction;
  CAction men_onClose;

public:
  CMenu();
  ~CMenu();

  void Initialize();

  void Begin();
  void Begin(const CAction &onClose);
  CMenuItem &Add();
  void Add(const CString &strText, const CString &strIcon, bool bEnabled, CAction func);
  void Show();

  void Close();

  void Update();
  void Render();

  CRectangle GetSmallMenuRect(int ctItems);
  CRectangle GetSmallMenuRect(int ctItems, Vector2f vOrigin);

  void RenderBlock(const CRectangle &rect);
};

#endif
