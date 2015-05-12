#ifndef PLAYERINFO_H_INCLUDED
#define PLAYERINFO_H_INCLUDED

#include "StdH.h"
#include <Mragpp/Texture.h>

class CPlayerInfo
{
public:
  static CTexture pi_texBlock;

public:
  int pi_iGold;
  ETeam pi_etTeam;

  bool pi_bPlayable; // currently not used for anything useful, but will be used for network play and AI in the future

public:
  CPlayerInfo();
  ~CPlayerInfo();

  void Update(bool bPlaying);
  void Render(bool bPlaying);
};

#endif
