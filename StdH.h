#ifndef STD_H_INCLUDED
#define STD_H_INCLUDED

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <Mragpp/StdH.h>
using namespace Scratch;
using namespace Mragpp;

class CRTSGame;
extern CRTSGame* _pGame;

enum ETeam
{
  T_NEUTRAL,

  T_RED,
  T_BLUE,
  T_GREEN,
  T_YELLOW,
};

enum EDirection
{
  DIR_INVALID,

  DIR_LEFT,
  DIR_RIGHT,
  DIR_UP,
  DIR_DOWN,
};

inline EDirection GetDirectionFromOffset(const Vector2i &vOffset)
{
  int x = vOffset(1);
  int y = vOffset(2);

  EDirection dir = DIR_INVALID;
  if(x != 0) {
    dir = (x > 0 ? DIR_RIGHT : DIR_LEFT);
  } else if(y != 0) {
    dir = (y > 0 ? DIR_DOWN : DIR_UP);
  }

  return dir;
}

inline CString GetTeamName(ETeam et)
{
  switch(et) {
  case T_RED:    return "Red";
  case T_BLUE:   return "Blue";
  case T_GREEN:  return "Green";
  case T_YELLOW: return "Yellow";
  }
  return "Neutral";
}

#endif
