#include "StdH.h"
#include "World.h"
#include "Game.h"

#include "Sprites/WorldSelection.h"

#include "Sprites/Unit.h"
#include "Sprites/Units/Infantry.h"
#include "Sprites/Units/Mech.h"
#include "Sprites/Units/Recon.h"
#include "Sprites/Units/Tank.h"

#include "Sprites/Building.h"
#include "Sprites/Buildings/HQ.h"
#include "Sprites/Buildings/City.h"
#include "Sprites/Buildings/Base.h"
#include "Sprites/Buildings/Port.h"
#include "Sprites/Buildings/Airport.h"
#include "Sprites/Buildings/Silo.h"

CWorld::CWorld()
{
  wo_psprSelection = 0;
  wo_pubWalkable = 0;
  wo_pubVisible = 0;
  wo_iWidth = 0;
  wo_iHeight = 0;
  wo_iTileSize = 0;
  wo_bFogEnabled = true;
  wo_psprArrowSource = 0;
  wo_pSpriteDepthSorting = new CSprite*[wo_ctMaxSprites];
}

CWorld::~CWorld()
{
  delete[] wo_pSpriteDepthSorting;
  Unload();
}

void CWorld::LoadTileset(const CString &strFilename)
{
  wo_iTileSize = 32;
  wo_aniTileset.Load(_pGame->Renderer, strFilename);
}

CPathFinding CWorld::CreatePathfindingObject()
{
  CPathFinding pf(wo_iWidth, wo_iHeight);

  for(int y=0; y<wo_iHeight; y++) {
    for(int x=0; x<wo_iWidth; x++) {
      pf.AddTile(Vector2i(x, y), wo_Tiles[y][x].IsSolid());
    }
  }

  return pf;
}

void CWorld::LoadFill(ETileType ett)
{
  Unload();
  LoadTileset("Content/Textures/Tileset.png");

  if(wo_iWidth == 0) {
    wo_iWidth = 35;
    wo_iHeight = 20;
  }

  for(int y=0; y<wo_iHeight; y++) {
    CStackArray<CTile> &row = wo_Tiles.Push();
    for(int x=0; x<wo_iWidth; x++) {
      CTile &tile = row.Push();
      tile.til_Type = ett;
    }
  }

  Start();
}

void CWorld::Load(const CString &strFilename)
{
  Unload();

  XmlFile file(strFilename);

  XmlTag* pXmlLevel = file.Query("level");
  ASSERT(pXmlLevel != 0);
  if(pXmlLevel == 0) {
    return;
  }

  if(pXmlLevel->Attributes.HasKey("name"))    { wo_strName    = pXmlLevel->Attributes["name"]; }
  if(pXmlLevel->Attributes.HasKey("author"))  { wo_strAuthor  = pXmlLevel->Attributes["author"]; }
  if(pXmlLevel->Attributes.HasKey("version")) { wo_strVersion = pXmlLevel->Attributes["version"]; }
  if(pXmlLevel->Attributes.HasKey("sheet"))   { wo_strSheet   = pXmlLevel->Attributes["sheet"]; }

  wo_aniTileset.Load(_pGame->Renderer, wo_strSheet);

  XmlTag* pXmlTiles = file.Query("level/tiles");
  ASSERT(pXmlTiles != 0);
  if(pXmlTiles == 0) {
    return;
  }

  wo_iWidth = atoi(pXmlTiles->Attributes["width"]);
  wo_iHeight = atoi(pXmlTiles->Attributes["height"]);

  const CString &strTiles = pXmlTiles->Value;
  const int iStrLen = strlen(strTiles);
  int ctTiles = 0;
  for(int i=0; i<iStrLen; i++) {
    const char c = strTiles[i];
    if(c == ' ' || c == '\n' || c == '\r') {
      continue;
    }

    const int y = (int)mthFloorF((float)ctTiles / wo_iWidth);
    const int x = ctTiles % wo_iWidth;

    CStackArray<CTile>* pRow = 0;
    if(y >= wo_Tiles.Count()) {
      pRow = &wo_Tiles.Push();
    } else {
      pRow = &wo_Tiles[y];
    }
    ASSERT(pRow != 0);
    CStackArray<CTile> &row = *pRow;

    ASSERT(x == row.Count());
    CTile &tile = row.Push();

    CAnimation* pAnim = 0;
    ETileType ett = ETT_EMPTY;
    switch((char)tolower(c)) {
    case 'w': ett = ETT_OCEAN; pAnim = wo_aniTileset.GetAnimation("Ocean"); break;
    case '.': ett = ETT_GRASS; pAnim = wo_aniTileset.GetAnimation("Grass"); break;

    default: printf("No known tile for character '%c'\n", (char)tolower(c)); ASSERT(FALSE); break;
    }

    tile.til_Type = ett;
    tile.til_pAnimation = pAnim;
    ctTiles++;
  }

  XmlTag* pXmlSprites = file.Query("level/sprites");
  ASSERT(pXmlSprites != 0);
  if(pXmlSprites == 0) {
    return;
  }

  for(int i=0; i<pXmlSprites->Children.Count(); i++) {
    XmlTag &xmlSprite = pXmlSprites->Children[i];

    if(xmlSprite.IsComment) {
      continue;
    }

    if(xmlSprite.Name == "unit") {
      CUnitSprite* pNewUnit = 0;

      ASSERT(xmlSprite.Attributes.HasKey("team"));
      ASSERT(xmlSprite.Attributes.HasKey("type"));
      ASSERT(xmlSprite.Attributes.HasKey("position"));

      const CString strTeam = xmlSprite.Attributes["team"].ToLower();
      const CString strType = xmlSprite.Attributes["type"].ToLower();
      const Vector2i vPosition = ParseVector2f(xmlSprite.Attributes["position"]).GetAs<2,int>();

           if(strType == "infantry") { pNewUnit = new CInfantryUnit; }
      else if(strType == "mech")     { pNewUnit = new CMechUnit; }
      else if(strType == "recon")    { pNewUnit = new CReconUnit; }
      else if(strType == "tank")     { pNewUnit = new CTankUnit; }
      else { printf("No known unit type for '%s'\n", strType); ASSERT(FALSE); }

      if(pNewUnit != 0) {
             if(strTeam == "red")    { pNewUnit->SetTeam(T_RED); }
        else if(strTeam == "blue")   { pNewUnit->SetTeam(T_BLUE); }
        else if(strTeam == "green")  { pNewUnit->SetTeam(T_GREEN); }
        else if(strTeam == "yellow") { pNewUnit->SetTeam(T_YELLOW); }
        else if(strTeam == "neutral") { printf("Neutral units not allowed\n"); ASSERT(FALSE); }
        else { printf("No known team type for '%s'\n", strTeam); ASSERT(FALSE); }
        pNewUnit->spr_vGridPosition = vPosition;
        wo_Sprites.Push(pNewUnit);
      }

    } else if(xmlSprite.Name == "building") {
      CBuildingSprite* pNewBuilding = 0;

      ASSERT(xmlSprite.Attributes.HasKey("team"));
      ASSERT(xmlSprite.Attributes.HasKey("type"));
      ASSERT(xmlSprite.Attributes.HasKey("position"));

      const CString strTeam = xmlSprite.Attributes["team"].ToLower();
      const CString strType = xmlSprite.Attributes["type"].ToLower();
      const Vector2i vPosition = ParseVector2f(xmlSprite.Attributes["position"]).GetAs<2,int>();

           if(strType == "hq")      { pNewBuilding = new CHQBuilding; }
      else if(strType == "city")    { pNewBuilding = new CCityBuilding; }
      else if(strType == "base")    { pNewBuilding = new CBaseBuilding; }
      else if(strType == "port")    { pNewBuilding = new CPortBuilding; }
      else if(strType == "airport") { pNewBuilding = new CAirportBuilding; }
      else if(strType == "silo")    { pNewBuilding = new CSiloBuilding; }
      else { printf("No known building type for '%s'\n", strType); ASSERT(FALSE); }

      if(pNewBuilding != 0) {
             if(strTeam == "red")     { pNewBuilding->SetTeam(T_RED); }
        else if(strTeam == "blue")    { pNewBuilding->SetTeam(T_BLUE); }
        else if(strTeam == "green")   { pNewBuilding->SetTeam(T_GREEN); }
        else if(strTeam == "yellow")  { pNewBuilding->SetTeam(T_YELLOW); }
        else if(strTeam == "neutral") { pNewBuilding->SetTeam(T_NEUTRAL); }
        else { printf("No known team type for '%s'\n", strTeam); ASSERT(FALSE); }
        pNewBuilding->spr_vGridPosition = vPosition;
        wo_Sprites.Push(pNewBuilding);
      }

    } else {
      printf("No known sprite for tag '%s'\n", xmlSprite.Name);
      ASSERT(FALSE);
    }
  }

  Start();
}

void CWorld::PostProcess()
{
  //  -1 0 1
  //-1 . . .
  // 0 . X .
  // 1 . . .
  ETileType map[3][3];

  // shorthand variables for formatting's sake
  int mx = wo_iWidth;
  int my = wo_iHeight;

  struct SPoint
  {
    int x; int y;
    SPoint(int xx, int yy) { x = xx; y = yy; }
  };
  const SPoint       left(0, 1);
  const SPoint      right(2, 1);
  const SPoint         up(1, 0);
  const SPoint    left_up(0, 0);
  const SPoint   right_up(2, 0);
  const SPoint       down(1, 2);
  const SPoint  left_down(0, 2);
  const SPoint right_down(2, 2);

  CAnimation* pAnimTopLeftIn = wo_aniTileset.GetAnimation("Ocean_TopLeft_In");
  CAnimation* pAnimTopRightIn = wo_aniTileset.GetAnimation("Ocean_TopRight_In");
  CAnimation* pAnimBotLeftIn = wo_aniTileset.GetAnimation("Ocean_BotLeft_In");
  CAnimation* pAnimBotRightIn = wo_aniTileset.GetAnimation("Ocean_BotRight_In");

  CAnimation* pAnimLeft = wo_aniTileset.GetAnimation("Ocean_Left");
  CAnimation* pAnimRight = wo_aniTileset.GetAnimation("Ocean_Right");
  CAnimation* pAnimTop = wo_aniTileset.GetAnimation("Ocean_Top");
  CAnimation* pAnimBot = wo_aniTileset.GetAnimation("Ocean_Bot");
  
  CAnimation* pAnimTopLeftOut = wo_aniTileset.GetAnimation("Ocean_TopLeft_Out");
  CAnimation* pAnimTopRightOut = wo_aniTileset.GetAnimation("Ocean_TopRight_Out");
  CAnimation* pAnimBotLeftOut = wo_aniTileset.GetAnimation("Ocean_BotLeft_Out");
  CAnimation* pAnimBotRightOut = wo_aniTileset.GetAnimation("Ocean_BotRight_Out");

  for(int y=0; y<wo_iHeight; y++) {
    for(int x=0; x<wo_iWidth; x++) {
      CTile &tileMid = wo_Tiles[y][x];

      // this can't happen
      ASSERT(tileMid.til_Type != ETT_EMPTY);

      // grass is fine and is always the same
      if(tileMid.til_Type == ETT_GRASS) {
        continue;
      }

      // generate a "map" of the surrounding neighbouring tiles
      map[0][0] = (x-1 >= 0 && y-1 >= 0) ? wo_Tiles[y-1][x-1].til_Type : ETT_OCEAN;
      map[0][1] = (            y-1 >= 0) ? wo_Tiles[y-1][x  ].til_Type : ETT_OCEAN;
      map[0][2] = (x+1 < mx && y-1 >= 0) ? wo_Tiles[y-1][x+1].til_Type : ETT_OCEAN;
      map[1][0] = (x-1 >= 0            ) ? wo_Tiles[y  ][x-1].til_Type : ETT_OCEAN;
      map[1][1] =                          wo_Tiles[y  ][x  ].til_Type            ;
      map[1][2] = (x+1 < mx            ) ? wo_Tiles[y  ][x+1].til_Type : ETT_OCEAN;
      map[2][0] = (x-1 >= 0 && y+1 < my) ? wo_Tiles[y+1][x-1].til_Type : ETT_OCEAN;
      map[2][1] = (            y+1 < my) ? wo_Tiles[y+1][x  ].til_Type : ETT_OCEAN;
      map[2][2] = (x+1 < mx && y+1 < my) ? wo_Tiles[y+1][x+1].til_Type : ETT_OCEAN;

      if(tileMid.til_Type == ETT_OCEAN) {
        //NOTE: Every cornered tile has 4 sub-tiles. Top left, top right, bottom left, bottom right.
        //      These will be set to the appropriate (possibly animated) tiles in the tileset. It
        //      will check its neighbouring tiles for non-mutual tile types, and base its animation
        //      on that information.

        // lazy macros follow..
#define Water(p) (map[p.y][p.x] == ETT_OCEAN)
#define Land(p) (map[p.y][p.x] != ETT_OCEAN)

        { // top left
          if(Land(up) && Land(left)) {
            tileMid.til_pAnimationTopLeft = pAnimTopLeftIn;
          } else if(Land(up) && Water(left)) {
            tileMid.til_pAnimationTopLeft = pAnimTop;
          } else if(Land(left) && Water(up)) {
            tileMid.til_pAnimationTopLeft = pAnimLeft;
          } else if(Land(left_up) && Water(left) && Water(up)) {
            tileMid.til_pAnimationTopLeft = pAnimTopLeftOut;
          }
        }

        { // top right
          if(Land(up) && Land(right)) {
            tileMid.til_pAnimationTopRight = pAnimTopRightIn;
          } else if(Land(up) && Water(right)) {
            tileMid.til_pAnimationTopRight = pAnimTop;
          } else if(Land(right) && Water(up)) {
            tileMid.til_pAnimationTopRight = pAnimRight;
          } else if(Land(right_up) && Water(right) && Water(up)) {
            tileMid.til_pAnimationTopRight = pAnimTopRightOut;
          }
        }

        { // bot left
          if(Land(down) && Land(left)) {
            tileMid.til_pAnimationBotLeft = pAnimBotLeftIn;
          } else if(Land(down) && Water(left)) {
            tileMid.til_pAnimationBotLeft = pAnimBot;
          } else if(Land(left) && Water(down)) {
            tileMid.til_pAnimationBotLeft = pAnimLeft;
          } else if(Land(left_down) && Water(left) && Water(down)) {
            tileMid.til_pAnimationBotLeft = pAnimBotLeftOut;
          }
        }

        { // bot right
          if(Land(down) && Land(right)) {
            tileMid.til_pAnimationBotRight = pAnimBotRightIn;
          } else if(Land(down) && Water(right)) {
            tileMid.til_pAnimationBotRight = pAnimBot;
          } else if(Land(right) && Water(down)) {
            tileMid.til_pAnimationBotRight = pAnimRight;
          } else if(Land(right_down) && Water(right) && Water(down)) {
            tileMid.til_pAnimationBotRight = pAnimBotRightOut;
          }
        }

#undef Water
#undef Land
      }
    }
  }
}

void CWorld::Unload()
{
  wo_Tiles.Clear();

  if(wo_psprSelection != 0) {
    // pop it so it doesn't try to delete it twice
    wo_Sprites.PopAt(wo_Sprites.FindPointer(wo_psprSelection));
    delete wo_psprSelection;
    wo_psprSelection = 0;
  }

  if(wo_pubWalkable != 0) {
    for(int i=0; i<wo_iHeight; i++) {
      delete[] wo_pubWalkable[i];
    }
    delete[] wo_pubWalkable;
  }

  if(wo_pubVisible != 0) {
    for(int i=0; i<wo_iHeight; i++) {
      delete[] wo_pubVisible[i];
    }
    delete[] wo_pubVisible;
  }
}

void CWorld::Start()
{
  PostProcess();

  ASSERT(wo_psprSelection == 0);
  wo_psprSelection = new CWorldSelection;
  wo_Sprites.Push((CSprite*)wo_psprSelection);

  wo_pubWalkable = new unsigned char*[wo_iHeight];
  for(int i=0; i<wo_iHeight; i++) {
    wo_pubWalkable[i] = new unsigned char[wo_iWidth];
    for(int x=0; x<wo_iWidth; x++) {
      wo_pubWalkable[i][x] = 0;
    }
  }

  wo_pubVisible = new unsigned char*[wo_iHeight];
  for(int i=0; i<wo_iHeight; i++) {
    wo_pubVisible[i] = new unsigned char[wo_iWidth];
    for(int x=0; x<wo_iWidth; x++) {
      wo_pubVisible[i][x] = 0;
    }
  }

  StartTurn();
}

void CWorld::StartTurn()
{
  EmptyWalkable();
  CalculateVisibility();

  for(int i=0; i<wo_Sprites.Count(); i++) {
    wo_Sprites[i].StartTurn();
  }
}

void CWorld::EndTurn()
{
  for(int i=0; i<wo_Sprites.Count(); i++) {
    wo_Sprites[i].EndTurn();
  }
}

void CWorld::CalculateVisibility()
{
  EmptyVisible();

  for(int i=0; i<wo_Sprites.Count(); i++) {
    CPropertySprite* sprite = dynamic_cast<CPropertySprite*>(&wo_Sprites[i]);
    if(sprite != 0) {
      sprite->CalculateVisibleTiles();
    }
  }
}

void CWorld::EmptyWalkable()
{
  ASSERT(wo_pubWalkable != 0);
  for(int i=0; i<wo_iHeight; i++) {
    for(int x=0; x<wo_iWidth; x++) {
      wo_pubWalkable[i][x] = 0;
    }
  }
}

void CWorld::EmptyVisible()
{
  ASSERT(wo_pubVisible != 0);
  for(int i=0; i<wo_iHeight; i++) {
    for(int x=0; x<wo_iWidth; x++) {
      wo_pubVisible[i][x] = 0;
    }
  }
}

CTile* CWorld::GetTileAt(const Vector2i &vGridPos)
{
  int x = vGridPos(1);
  int y = vGridPos(2);

  if(y < wo_Tiles.Count() && x < wo_Tiles[y].Count()) {
    return &wo_Tiles[y][x];
  }

  return 0;
}

CSprite* CWorld::GetSpriteAt(const Vector2i &vGridPos)
{
  for(int i=0; i<wo_Sprites.Count(); i++) {
    CSprite &sprite = wo_Sprites[i];
    // World Selection sprite can't be retrieved with this function
    if(dynamic_cast<CWorldSelection*>(&sprite) == 0) {
      if(sprite.spr_vGridPosition == vGridPos && !sprite.spr_bNoClip) {
        return &sprite;
      }
    }
  }

  return 0;
}

CUnitSprite* CWorld::GetUnitAt(const Vector2i &vGridPos)
{
  CSprite* pSprite = GetSpriteAt(vGridPos);
  if(pSprite != 0) {
    CUnitSprite* pUnit = dynamic_cast<CUnitSprite*>(pSprite);
    if(pUnit != 0) {
      return pUnit;
    }
  }
  return 0;
}

void CWorld::Update()
{
  for(int y=0; y<wo_iHeight; y++) {
    for(int x=0; x<wo_iWidth; x++) {
      wo_Tiles[y][x].Update(x, y);
    }
  }

  for(int i=0; i<wo_Sprites.Count(); i++) {
    wo_Sprites[i].Update();
  }
}

int _SpriteDepth_qsort(const void* p1, const void* p2)
{
  CSprite* pSprite1 = *(CSprite**)p1;
  CSprite* pSprite2 = *(CSprite**)p2;

  if(pSprite1->spr_iLayer < pSprite2->spr_iLayer) {
    return -1;
  } else if(pSprite1->spr_iLayer > pSprite2->spr_iLayer) {
    return 1;
  }

  int y1 = pSprite1->spr_vGridPosition(2);
  int y2 = pSprite2->spr_vGridPosition(2);
  return y1 - y2;
}

void CWorld::Render()
{
  _pGame->Renderer.SetColor(COL_WHITE|MRAG_ALPHA_OPAQUE);

  for(int y=0; y<wo_iHeight; y++) {
    for(int x=0; x<wo_iWidth; x++) {
      wo_Tiles[y][x].Render(x, y);
    }
  }

  // for arrow rendering
  Vector2i vPosPrev;

  int ctSprites = wo_Sprites.Count();
  if(ctSprites > 0) {
    // for depth sorting, we make a quick seperate list of pointers to sprites
    for(int i=0; i<ctSprites; i++) {
      wo_pSpriteDepthSorting[i] = &wo_Sprites[i];
    }
    // and then run qsort on it
    qsort((void*)wo_pSpriteDepthSorting, ctSprites, sizeof(CSprite*), _SpriteDepth_qsort);

    for(int i=0; i<ctSprites; i++) {
      CSprite* pSprite = wo_pSpriteDepthSorting[i];

      // render the start of the movement arrow
      if(wo_psprArrowSource != 0 && wo_saArrowPath.Count() > 0 && pSprite == wo_psprArrowSource) {
        Vector2i vPos = pSprite->spr_vGridPosition;
        EDirection dir = GetDirectionFromOffset(wo_saArrowPath[0] - vPos);

        vPosPrev = vPos;

        switch(dir) {
        case DIR_LEFT:  _pGame->gam_Sheets.Render("Arrow", "Start_Left",  pSprite->spr_vGridPosition.GetAs<2,float>() * 32.0f); break;
        case DIR_RIGHT: _pGame->gam_Sheets.Render("Arrow", "Start_Right", pSprite->spr_vGridPosition.GetAs<2,float>() * 32.0f); break;
        case DIR_DOWN:  _pGame->gam_Sheets.Render("Arrow", "Start_Down",  pSprite->spr_vGridPosition.GetAs<2,float>() * 32.0f); break;
        case DIR_UP:    _pGame->gam_Sheets.Render("Arrow", "Start_Up",    pSprite->spr_vGridPosition.GetAs<2,float>() * 32.0f); break;
        }
      }

      pSprite->Render();
    }
  }

  // render the parts of the movement arrow
  if(wo_psprArrowSource != 0 && wo_saArrowPath.Count() > 0) {
    for(int i=0; i<wo_saArrowPath.Count(); i++) {
      Vector2i &vPos = wo_saArrowPath[i];
      EDirection dir = GetDirectionFromOffset(vPos - vPosPrev);

      ASSERT(dir != DIR_INVALID);

      CString strAnimation;

      if(i == wo_saArrowPath.Count() - 1) {
        // this is the end of the arrow
        switch(dir) {
        case DIR_LEFT:  strAnimation = "End_Left"; break;
        case DIR_RIGHT: strAnimation = "End_Right"; break;
        case DIR_DOWN:  strAnimation = "End_Down"; break;
        case DIR_UP:    strAnimation = "End_Up"; break;
        }
        _pGame->gam_Sheets.Render("Arrow", strAnimation, vPos.GetAs<2,float>() * 32.0f);
      } else {
        // this is a middle piece
        Vector2i &vPosNext = wo_saArrowPath[i + 1];
        EDirection dirNext = GetDirectionFromOffset(vPosNext - vPos);

        if(dir == dirNext) {
          if(dir == DIR_DOWN || dir == DIR_UP) {
            strAnimation = "Vertical";
          } else {
            strAnimation = "Horizontal";
          }
        } else {
          if((dir == DIR_DOWN && dirNext == DIR_RIGHT) || (dir == DIR_LEFT && dirNext == DIR_UP)) {
            strAnimation = "BotLeft";
          } else if((dir == DIR_DOWN && dirNext == DIR_LEFT) || (dir == DIR_RIGHT && dirNext == DIR_UP)) {
            strAnimation = "BotRight";
          } else if((dir == DIR_UP && dirNext == DIR_RIGHT) || (dir == DIR_LEFT && dirNext == DIR_DOWN)) {
            strAnimation = "TopLeft";
          } else if((dir == DIR_UP && dirNext == DIR_LEFT) || (dir == DIR_RIGHT && dirNext == DIR_DOWN)) {
            strAnimation = "TopRight";
          }
        }

        //NOTE: temporary solution for animation states for multiple middle parts..
        static CAnimationState aniStates[25];
        _pGame->gam_Sheets.Render("Arrow", strAnimation, vPos.GetAs<2,float>() * 32.0f, aniStates[i]);
      }

      // remember our previous position
      vPosPrev = vPos;
    }
  }

  // render the info panel of what's below world selection
  Vector2i vCursorPos = wo_psprSelection->spr_vGridPosition;

  CTile* pTile = GetTileAt(vCursorPos);
  CSprite* pSprite = GetSpriteAt(vCursorPos);

  ASSERT(pTile != 0);

  if(pTile->IsFoggy()) {
    pSprite = 0;
  }

  const int ip_width = pSprite == 0 ? 60 : 132;
  const int ip_height = 112;
  const int ip_x = _pGame->gam_iWidth - ip_width - 4;
  const int ip_y = _pGame->gam_iHeight - ip_height - 4;

  _pGame->Renderer.SetColor(MRAG_COLOR_RGBA(0, 0, 0, 128));
  _pGame->Renderer.FillRectangle((float)ip_x, (float)ip_y, (float)ip_width, (float)ip_height);

  static CAnimationState aniStateTile;
  pTile->til_pAnimation->ani_colMultiplier = COL_WHITE; // has to be white to make sure we render with the correct color
  pTile->til_pAnimation->Render(Vector2f(ip_x + (pSprite == 0 ? 14.0f : 86.0f), ip_y + 28.0f), aniStateTile);

  if(pSprite != 0) {
    // render the sprite itself
    CAnimation* pAnim = _pGame->gam_Sheets.GetSheet(pSprite->spr_strSheet)->GetAnimation(pSprite->spr_strAnimation);
    pAnim->ani_colMultiplier = COL_WHITE; // has to be white
    static CAnimationState aniStateSprite;
    pAnim->Render(Vector2f(ip_x + 14.0f, ip_y + 28.0f), aniStateSprite);

    // render additional sprite info
    int x = ip_x + 11;
    int y = ip_y + 58;
    _pGame->Renderer.SetColor(COL_WHITE|MRAG_ALPHA_OPAQUE);
    pSprite->RenderInfoBox(x, y);
  }
}
