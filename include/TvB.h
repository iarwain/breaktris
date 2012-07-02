#ifndef __TVB_H__
#define __TVB_H__

//! Includes
#define __NO_SCROLLED__ // Uncomment this define to prevent the embedded editor (ScrollEd) from being compiled
#include "Scroll.h"

#include "TvBObject.h"


//! TvB class
class TvB : public Scroll<TvB>
{
public:

  enum EventID
  {
    EventIDWarpIn = 0,
    EventIDWarpOut,
    EventIDAddLine,
    
    EventIDNumber,
    
    EventIDNone = orxENUM_NONE
  };
  
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

  enum LineType
  {
    LineTypeDefault = 0,
    
    LineTypeNumber,
    
    LineTypeNone = orxENUM_NONE
  };

  struct EventPayload
  {
    union
    {
      struct
      {
        orxVECTOR vPos;
        orxVECTOR vSpeed;

      } stWarp;

      struct
      {
        LineType eType;

      } stAddLine;
    };
  };
  
                orxFLOAT        GetTime() const         {return mfTime;}
                orxOBJECT *     GetCameraObject() const {return mpstCameraObject;}
                GameState       GetGameState() const    {return meGameState;}
          const orxOBJECT *     GetSplashObject()       {return mpstSplashObject;}
                void            ClearSplashObject()     {mpstSplashObject = orxNULL;}

                void            LoadMenu();
  
                void            AddBLine(LineType _eType);

                orxSTATUS       GetGridPosition(const orxVECTOR &_rvPos, orxS32 &_rs32X, orxS32 &_rs32Y) const;
                orxU64          GetGridValue(orxS32 _s32X, orxS32 _s32Y) const;
                void            SetGridValue(orxS32 _s32X, orxS32 _s32Y, orxU64 _u64Value);
  
                void            GetGridSize(orxS32 &_rs32Width, orxS32 &_rs32Height) const;
  
                void            CleanGridLine(orxS32 _s32Line);

  
private:

                orxSTATUS       Start();

                orxSTATUS       InitSplash();
                orxSTATUS       InitT();
                orxSTATUS       InitB();

                void            ExitT();
                void            ExitB();

                void            UpdateGame(const orxCLOCK_INFO &_rstInfo);

                void            Update(const orxCLOCK_INFO &_rstInfo);

                void            UpdateT(const orxCLOCK_INFO &_rstInfo);
                void            UpdateB(const orxCLOCK_INFO &_rstInfo);

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
  
                orxVECTOR       vBrickSize;
                orxVECTOR       vPlayTL, vPlayBR;

                orxU64          *au64Grid;
                orxS32          s32GridWidth, s32GridHeight;
                TvBTetromino   *poSelection;
                orxFLOAT        fFallTime;
                orxFLOAT        fLeftTime, fRightTime;
};

#endif // __TVB_H__
