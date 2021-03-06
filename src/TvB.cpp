//! Includes
#define __SCROLL_IMPL__
#include "TvB.h"
#undef __SCROLL_IMPL__
#include "TvBObject.h"


//! Constants
static  const orxSTRING szConfigSectionGame         = "Game";
static  const orxSTRING szConfigSectionSave         = "Save";
static  const orxSTRING szConfigSectionSplash       = "Splash";

static  const orxSTRING szInputAction               = "Action";
static  const orxSTRING szInputQuit                 = "Quit";


//! Code
static orxBOOL orxFASTCALL SaveCallback(const orxSTRING _zSectionName, const orxSTRING _zKeyName, const orxSTRING _zFileName, orxBOOL _bUseEncryption)
{
  // Done!
  return (!orxString_Compare(_zSectionName, szConfigSectionSave));
}

orxSTATUS orxFASTCALL EventHandler(const orxEVENT *_pstEvent)
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Depending on event type
  switch(_pstEvent->eType)
  {
    case orxEVENT_TYPE_OBJECT:
    {
      // Deleted?
      if(_pstEvent->eID == orxOBJECT_EVENT_DELETE)
      {
        orxOBJECT *pstObject;

        // Gets game instance
        TvB &roGame = TvB::GetInstance();

        // Gets object
        pstObject = orxOBJECT(_pstEvent->hSender);

        // Is splash screen?
        if(pstObject == roGame.GetSplashObject())
        {
          // Clears splash pointer
          roGame.ClearSplashObject();

          // Loads menu
          roGame.LoadMenu();
        }
      }

      break;
    }

    default:
    {
      break;
    }
  }

  // Done!
  return eResult;
}

orxSTATUS TvB::GetGridPosition(const orxVECTOR &_rvPos, orxS32 &_rs32X, orxS32 &_rs32Y) const
{
  orxVECTOR vOrigin, vBrickSize, vPos;
  orxSTATUS eResult = orxSTATUS_FAILURE;

  // Push T config section
  orxConfig_PushSection("T");

  // Gets grid origin
  orxConfig_GetVector("GridOrigin", &vOrigin);

  // Gets brick size
  orxConfig_GetVector("TetroBrickSize", &vBrickSize);

  // Gets relative pos
  orxVector_Sub(&vPos, &_rvPos, &vOrigin);

  // Normalizes it
  orxVector_Div(&vPos, &vPos, &vBrickSize);

  // Stores coords
  _rs32X = orxF2S(orxMath_Floor(vPos.fX));
  _rs32Y = orxF2S(orxMath_Floor(vPos.fY));

  // Updates result
  eResult = ((_rs32X >= 0) && (_rs32X < s32GridWidth) && (_rs32Y >= 0) && (_rs32Y < s32GridHeight)) ? orxSTATUS_SUCCESS : orxSTATUS_FAILURE;

  // Pops config section
  orxConfig_PopSection();

  // Done!
  return eResult;
}

orxU64 TvB::GetGridValue(orxS32 _s32X, orxS32 _s32Y) const
{
  return au64Grid[_s32X + _s32Y * s32GridWidth];
}

void TvB::SetGridValue(orxS32 _s32X, orxS32 _s32Y, orxU64 _u64Value)
{
  au64Grid[_s32X + _s32Y * s32GridWidth] = _u64Value;
}

void TvB::GetGridSize(orxS32 &_rs32Width, orxS32 &_rs32Height) const
{
  _rs32Width  = s32GridWidth;
  _rs32Height = s32GridHeight;
}

void TvB::CleanGridLine(orxS32 _s32Line)
{
  // For all lines above cleaned one
  for(orxS32 i = _s32Line; i > 0; i--)
  {
    // For all columns
    for(orxS32 j = 0; j < s32GridWidth; j++)
    {
      // Copies value from above
      SetGridValue(j, i, GetGridValue(j, i - 1));
    }
  }

  // For all columns (top line)
  for(orxS32 j = 0; j < s32GridWidth; j++)
  {
    // Clears it
    SetGridValue(j, 0, 0);
  }
}

void TvB::LoadMenu()
{
  //! TODO

  // Starts
  Start();
}

orxSTATUS TvB::Start()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Inits variables
  mfTime = orxFLOAT_0;

  // Updates game state
  meGameState = GameStateRun;

  // Inits sub-games
  InitT();
  InitB();

  // Done!
  return eResult;
}

orxSTATUS TvB::InitSplash()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Not in editor mode?
  if(!IsEditorMode())
  {
    // Creates splash screen
    mpstSplashObject = orxObject_CreateFromConfig(szConfigSectionSplash);

    // Not created?
    if(!mpstSplashObject)
    {
      // Loads menu
      LoadMenu();
    }
  }
  else
  {
    // Loads menu
    LoadMenu();
  }

  // Done!
  return eResult;
}

void TvB::UpdateGame(const orxCLOCK_INFO &_rstInfo)
{
  // Updates in game time
  mfTime += _rstInfo.fDT;

  // Updates games
  UpdateT(_rstInfo);
  UpdateB(_rstInfo);
}

void TvB::Update(const orxCLOCK_INFO &_rstInfo)
{
  // Screenshot?
  if(orxInput_IsActive("Screenshot") && orxInput_HasNewStatus("Screenshot"))
  {
    orxScreenshot_Capture();
  }

  // Depending on game state
  switch(meGameState)
  {
    case GameStateSplash:
    {
      // Action?
      if(orxInput_IsActive(szInputAction))
      {
        // Deletes splash screen
        orxObject_Delete(mpstSplashObject);
      }

      break;
    }

    case GameStateMenu:
    {
      break;
    }

    case GameStateRun:
    {
      // Updates in-game
      UpdateGame(_rstInfo);

      break;
    }

    case GameStatePause:
    {
      break;
    }

    case GameStateEnd:
    {
      // Adds game over screen
      CreateObject("GameOver");
      meGameState = GameStateNumber;
      break;
    }
  }
}

orxSTATUS TvB::Init()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Inits random seed
  orxMath_InitRandom(orxSystem_GetRealTime());

  // Init values
  meGameState       = GameStateSplash;
  mpstCameraObject  = orxNULL;
  mpstSplashObject  = orxNULL;
  mpstPauseObject   = orxNULL;
  mpoScene          = orxNULL;
  mfTime            = orxFLOAT_0;
  au64Grid          = orxNULL;

  // Inits splash screen
  InitSplash();

  // Adds event handler
  orxEvent_AddHandler(orxEVENT_TYPE_OBJECT, EventHandler);

  // Done!
  return eResult;
}

orxSTATUS TvB::Run()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Quitting?
  if(orxInput_IsActive(szInputQuit))
  {
    // Updates result
    eResult = orxSTATUS_FAILURE;
  }

  // Done!
  return eResult;
}

void TvB::Exit()
{
  meGameState = GameStateNumber;

  // Removes event handler
  orxEvent_RemoveHandler(orxEVENT_TYPE_OBJECT, EventHandler);

  // Exits from sub-games
  ExitT();
  ExitB();
}

void TvB::BindObjects()
{
  // Pushes game section
  orxConfig_PushSection(szConfigSectionGame);

  // Binds objects (C++ class <-> config section)
  ScrollBindObject<TvBPaddle>("Paddle");
  ScrollBindObject<TvBBall>("Ball");
  for(orxS32 i = 0; i < orxConfig_GetListCounter("BrickList"); i++)
  {
    ScrollBindObject<TvBBrick>(orxConfig_GetListString("BrickList", i));
  }
  for(orxS32 i = 0; i < orxConfig_GetListCounter("TetrominoList"); i++)
  {
    ScrollBindObject<TvBTetromino>(orxConfig_GetListString("TetrominoList", i));
  }

  // Pops section
  orxConfig_PopSection();
}

int main(int argc, char **argv)
{
  // Executes game
  TvB::GetInstance().Execute(argc, argv);

  // Done!
  return EXIT_SUCCESS;
}

#ifdef __orxWINDOWS__

#include "windows.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // Executes game
  TvB::GetInstance().Execute();

  // Done!
  return EXIT_SUCCESS;
}

#endif // __orxWINDOWS__
