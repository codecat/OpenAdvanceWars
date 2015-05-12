#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "StdH.h"
#include "Tile.h"
#include "Sprite.h"
#include "Sprites/WorldSelection.h"
#include "Sprites/Unit.h"

#include <Mragpp/Texture.h>
#include <Mragpp/AnimationSheet.h>
#include <Mragpp/Pathfinding/Pathfinding.h>

class CWorld
{
public:
  CString wo_strName;
  CString wo_strAuthor;
  CString wo_strVersion;
  CString wo_strSheet;

  CStackArray<CStackArray<CTile>> wo_Tiles;
  CStackArray<CSprite> wo_Sprites;

  unsigned char** wo_pubWalkable; // walkable tiles by currently selected unit
  unsigned char** wo_pubVisible; // visible tiles by the current team

  CWorldSelection* wo_psprSelection;

  int wo_iWidth;
  int wo_iHeight;

  int wo_iTileSize;
  CAnimationSheet wo_aniTileset;

  bool wo_bFogEnabled;

  CSprite* wo_psprArrowSource;
  CStackArray<Vector2i> wo_saArrowPath;

private:
  static const int wo_ctMaxSprites = 500;
  CSprite** wo_pSpriteDepthSorting; // for depth sorting

public:
  CWorld();
  ~CWorld();

  void LoadTileset(const CString &strFilename);

  CPathFinding CreatePathfindingObject();

  void LoadFill(ETileType ett);
  void Load(const CString &strFilename);
  void Unload();
  void PostProcess();

  void Start();
  void StartTurn();
  void EndTurn();
  void CalculateVisibility();

  void EmptyWalkable();
  void EmptyVisible();

  CTile* GetTileAt(const Vector2i &vGridPos);
  CSprite* GetSpriteAt(const Vector2i &vGridPos);
  CUnitSprite* GetUnitAt(const Vector2i &vGridPos);

  void Update();
  void Render();
};

#endif
