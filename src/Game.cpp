#include "StdH.h"
#include "Game.h"

CRTSGame::CRTSGame()
{
  gam_iWidth = 640;
  gam_iHeight = 480;
  gam_etPlayingTeam = T_RED;
}

CRTSGame::~CRTSGame()
{
  CPlayerInfo::pi_texBlock.Destroy();
}

void CRTSGame::Initialize()
{
  Window.SetTitle("Advance Wars");

  //TODO: Make new Fontmap text rendering classes
  LoadSystemFont(34);
  Renderer.PushFont(gam_fntSystem);

	gam_World.Load("Content/Levels/Default.xml");
	gam_Sheets.Load(this, "Content/Textures/Sheets.xml");
  gam_Menu.Initialize();

  CPlayerInfo &infoRed = gam_saPlayers.Push();
  infoRed.pi_etTeam = T_RED;

  CPlayerInfo &infoBlue = gam_saPlayers.Push();
  infoRed.pi_etTeam = T_BLUE;
}

void CRTSGame::Update()
{
  if(Input.IsKeyDown(MKEY_ESCAPE)) {
    Exit();
    return;
  }

  gam_World.Update();
  gam_Menu.Update();
  for(int i=0; i<gam_saPlayers.Count(); i++) {
    CPlayerInfo &pi = gam_saPlayers[i];
    ASSERT(gam_World.wo_psprSelection != 0);
    pi.Update(pi.pi_etTeam == gam_etPlayingTeam);
  }
}

void CRTSGame::Render()
{
  Renderer.SetColor(COL_BLACK|MRAG_ALPHA_OPAQUE);
  Renderer.Clear();

  gam_World.Render();
  gam_Menu.Render();
  for(int i=0; i<gam_saPlayers.Count(); i++) {
    CPlayerInfo &pi = gam_saPlayers[i];
    ASSERT(gam_World.wo_psprSelection != 0);
    pi.Render(pi.pi_etTeam == gam_etPlayingTeam);
  }
}
