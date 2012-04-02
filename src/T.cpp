//! Includes
#include "TvB.h"


//! Constants

//! Code
static orxSTATUS orxFASTCALL EventHandler(const orxEVENT *_pstEvent)
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;
  
  // Depending on ID
  switch(_pstEvent->eID)
  {
    case TvB::EventIDWarpOut:
    {
      TvB::EventPayload *pstPayload;
      TvBBall *poBall;

      // Gets payload
      pstPayload = (TvB::EventPayload *)_pstEvent->pstPayload;
      
      // Gets ball
      poBall = TvB::GetInstance().GetNextObject<TvBBall>();
      
      // Changes its space
      orxObject_SetParent(poBall->GetOrxObject(), orxCamera_Get("TCamera"));

      // Respawns
      poBall->Respawn(&pstPayload->stWarp.vPos, &pstPayload->stWarp.vSpeed);
      
      break;
    }

    case TvB::EventIDAddLine:
    {
      // Add a line
      TvB::GetInstance().AddBLine(((TvB::EventPayload *)_pstEvent->pstPayload)->stAddLine.eType);
      
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

orxSTATUS TvB::InitT()
{
  orxVECTOR vGridSize;
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Inits grid
  orxConfig_PushSection("T");
  orxConfig_GetVector("GridSize", &vGridSize);
  s32GridWidth = orxF2S(vGridSize.fX);
  s32GridHeight = orxF2S(vGridSize.fY);
  orxConfig_PopSection();
  au64Grid = (orxU64 *)orxMemory_Allocate(s32GridWidth * s32GridHeight * sizeof(orxU64), orxMEMORY_TYPE_MAIN);
  orxMemory_Zero(au64Grid, s32GridWidth * s32GridHeight * sizeof(orxU64));

  // Creates viewport
  orxViewport_CreateFromConfig("TViewport");

  // Creates scene
  orxObject_CreateFromConfig("TScene");

  // Clears vars
  poSelection = orxNULL;
  fFallTime = GetTime();

  // Adds event handler
  orxEvent_AddHandler(orxEVENT_TYPE_USER_DEFINED, EventHandler);
  
  // Done!
  return eResult;
}

void TvB::ExitT()
{
  // Removes event handler
  orxEvent_RemoveHandler(orxEVENT_TYPE_USER_DEFINED, EventHandler);

  if(au64Grid != orxNULL)
  {
    orxMemory_Free(au64Grid);
    au64Grid = orxNULL;
  }
}

void TvB::UpdateT(const orxCLOCK_INFO &_rstInfo)
{
  orxFLOAT fFallDelay;
  
  // Pushes config section
  orxConfig_PushSection("T");

  // Has selection?
  if(poSelection != orxNULL)
  {
    // No child?
    if(orxObject_GetChild(poSelection->GetOrxObject()) == orxNULL)
    {
      // Clears it
      orxObject_SetLifeTime(poSelection->GetOrxObject(), orxFLOAT_0);
      poSelection = orxNULL;
    }
  }
  
  // No selection?
  if(poSelection == orxNULL)
  {
    // Creates a new tetromino
    poSelection = CreateObject<TvBTetromino>(orxConfig_GetString("TetrominoList"));

    // Moves it
    if(poSelection->Move(orxVECTOR_0, 0) == orxFALSE)
    {
      //! TODO: Game over
      meGameState = GameStateEnd;
    }
    
    // Updates fall time
    fFallTime = GetTime();
  }
  
  // Gets fall delay
  fFallDelay = orxConfig_GetListFloat("FallDelayList", orxInput_IsActive("SpeedUp") ? 1 : 0);
  
  // Pops config section
  orxConfig_PopSection();

  // Left?
  if(orxInput_IsActive("MoveLeft") && orxInput_HasNewStatus("MoveLeft"))
  {
    orxVECTOR vMove;
    
    // Moves selection left
    orxVector_Set(&vMove, -orxFLOAT_1, orxFLOAT_0, orxFLOAT_0);
    
    // Moves it
    poSelection->Move(vMove, 0);
  }
  // Right?
  else if(orxInput_IsActive("MoveRight") && orxInput_HasNewStatus("MoveRight"))
  {
    orxVECTOR vMove;
    
    // Moves selection right
    orxVector_Set(&vMove, orxFLOAT_1, orxFLOAT_0, orxFLOAT_0);
    
    // Moves it
    poSelection->Move(vMove, 0);
  }

  // Rotate CW?
  if(orxInput_IsActive("RotateCW") && orxInput_HasNewStatus("RotateCW"))
  {
    orxS32 s32Rotation;
    
    // Gets current rotation
    s32Rotation = poSelection->s32Rotation;
    
    // Moves it
    poSelection->Move(orxVECTOR_0, 1);
    
    // Changed?
    if(poSelection->s32Rotation != s32Rotation)
    {
      // Adds sound
      poSelection->AddSound("RotateRight");
    }
  }
  // Rotate CCW?
  else if(orxInput_IsActive("RotateCCW") && orxInput_HasNewStatus("RotateCCW"))
  {
    orxS32 s32Rotation;
    
    // Gets current rotation
    s32Rotation = poSelection->s32Rotation;
    
    // Moves it
    poSelection->Move(orxVECTOR_0, -1);
    
    // Changed?
    if(poSelection->s32Rotation != s32Rotation)
    {
      // Adds sound
      poSelection->AddSound("RotateLeft");
    }
  }
  
  // Should fall?
  if(GetTime() - fFallTime >= fFallDelay)
  {
    orxVECTOR vMove;

    // Moves selection down
    orxVector_Set(&vMove, orxFLOAT_0, orxFLOAT_1, orxFLOAT_0);
    
    // Moves it
    if(poSelection->Move(vMove, 0) == orxFALSE)
    {
      // Land!
      poSelection->Land();
      poSelection = orxNULL;
    }

    // Updates fall time
    fFallTime = GetTime();
  }

  orxS32 s32Width, s32Height;
  
  TvB::GetInstance().GetGridSize(s32Width, s32Height);
  
  orxS32 s32CleanedLines = 0;
  orxVECTOR vOffset;

  orxConfig_PushSection("T");
  orxConfig_GetVector("TetroBrickSize", &vOffset);
  orxConfig_PopSection();
  
  for(orxS32 i = s32Height - 1; i >= 0; i--)
  {
    orxBOOL bClean = orxTRUE;
    
    for(orxS32 j = 0; j < s32Width; j++)
    {
      orxU64 u64GUID;
      
      u64GUID = (poSelection) ? poSelection->GetGUID() : 0;
      // Empty or selection?
      if((TvB::GetInstance().GetGridValue(j, i) == 0) || (TvB::GetInstance().GetGridValue(j, i) == u64GUID))
      {
        bClean = orxFALSE;
        break;
      }
    }
    if(bClean != orxFALSE)
    {
      s32CleanedLines++;
      TvB::GetInstance().CleanGridLine(i);
     
      for(TvBBrick *poBrick = TvB::GetInstance().GetNextObject<TvBBrick>();
          poBrick;
          poBrick = TvB::GetInstance().GetNextObject<TvBBrick>(poBrick))
      {
        orxVECTOR vPos;
        orxS32 s32X, s32Y;
        
        poBrick->GetPosition(vPos, orxTRUE);
        
        if(TvB::GetInstance().GetGridPosition(vPos, s32X, s32Y) != orxSTATUS_FAILURE)
        {
          if(s32Y == i)
          {
            orxObject_SetLifeTime(poBrick->GetOrxObject(), orxFLOAT_0);
          }
          else
          {
            vPos.fY += vOffset.fY;
            poBrick->SetPosition(vPos, orxTRUE);
          }
        }
      }
      
      i++;
    }
  }
  
  if(s32CleanedLines > 0)
  {
    TvB::EventPayload stPayload;
    orxEVENT stEvent;
    
    orxMemory_Zero(&stPayload, sizeof(TvB::EventPayload));
    stPayload.stAddLine.eType = (TvB::LineType)s32CleanedLines;
    
    // Sends add line event
    orxEVENT_INIT(stEvent, orxEVENT_TYPE_USER_DEFINED, TvB::EventIDAddLine, orxNULL, orxNULL, &stPayload);
    orxEvent_Send(&stEvent);
    
    TvB::GetInstance().GetNextObject<TvBPaddle>()->AddSound("LineCleared");
  }
}
