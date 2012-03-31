#ifndef __TVB_H_
#define __TVB_H_

//! Includes
#define __NO_SCROLLED__ // Uncomment this define to prevent the embedded editor (ScrollEd) from being compiled
#include "Scroll.h"

#include "TvBObject.h"


//! TvB class
class TvB : public Scroll<TvB>
{
public:

  enum GameState
  {
    GameStateSplash = 0,
    GameStateMenu,
    GameStateRun,
    GameStatePause,
    GameStateEnd,

    GameStateNumber,

    GameStateNone = orxENUM_NONE

  };

                orxFLOAT        GetTime() const         {return mfTime;}
                orxOBJECT *     GetCameraObject() const {return mpstCameraObject;}
                GameState       GetGameState() const    {return meGameState;}
          const orxOBJECT *     GetSplashObject()       {return mpstSplashObject;}
                void            ClearSplashObject()     {mpstSplashObject = orxNULL;}

                void            LoadMenu();


private:

                orxSTATUS       Start();

                void            InitUI();
                void            InitSplash();
                void            InitScene();

                void            ExitUI();
                void            ExitScene();

                void            UpdateGame(const orxCLOCK_INFO &_rstInfo);
                void            UpdateInput(const orxCLOCK_INFO &_rstInfo);
                void            UpdateUI(const orxCLOCK_INFO &_rstInfo);

                void            Update(const orxCLOCK_INFO &_rstInfo);

                orxSTATUS       Init();
                orxSTATUS       Run();
                void            Exit();
                void            BindObjects();

                GameState       meGameState;
                orxOBJECT *     mpstCameraObject;
                orxOBJECT *     mpstSplashObject;
                orxOBJECT *     mpstPauseObject;
                ScrollObject *  mpoScene;
                orxFLOAT        mfTime;
};

#endif // __TVB_H_
