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

void TvB::LoadMenu()
{
  //! TODO
  // Starts
  Start();
}

orxSTATUS TvB::Start()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Pushes game section
  orxConfig_PushSection(szConfigSectionGame);

  // Inits variables
  mfTime = orxFLOAT_0;

  // Updates game state
  meGameState = GameStateRun;

  // Pops config section
  orxConfig_PopSection();

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
      break;
    }
  }
}

orxSTATUS TvB::Init()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Filters logs
  orxDEBUG_ENABLE_LEVEL(orxDEBUG_LEVEL_RENDER, orxFALSE);
  
  // Pushes game section
  orxConfig_PushSection(szConfigSectionGame);

  // Init values
  meGameState       = GameStateSplash;
  mpstCameraObject  = orxNULL;
  mpstSplashObject  = orxNULL;
  mpstPauseObject   = orxNULL;
  mpoScene          = orxNULL;
  mfTime            = orxFLOAT_0;

  // Inits splash screen
  InitSplash();

  // Inits games
  InitT();
  InitB();

  // Creates scene
  mpoScene = CreateObject("Scene");
  
  // Pops config section
  orxConfig_PopSection();

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
  // Removes event handler
  orxEvent_RemoveHandler(orxEVENT_TYPE_OBJECT, EventHandler);

  // Exits from games
  ExitT();
  ExitB();
}

void TvB::BindObjects()
{
  // Pushes game section
  orxConfig_PushSection(szConfigSectionGame);

  // Binds objects
  ScrollBindObject<TvBPaddle>("Paddle");
  ScrollBindObject<TvBBall>("Ball");

  for(orxS32 i = 0; i < orxConfig_GetListCounter("BrickList"); i++)
  {
    ScrollBindObject<TvBBrick>(orxConfig_GetListString("BrickList", i));
  }

  //! TODO

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
