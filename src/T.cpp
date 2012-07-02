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
  fLeftTime = fRightTime = orxFLOAT_0;

  // Adds event handler
  orxEvent_AddHandler(orxEVENT_TYPE_USER_DEFINED, EventHandler);

  // Done!
  return eResult;
}

void TvB::ExitT()
{
  // Removes event handler
  orxEvent_RemoveHandler(orxEVENT_TYPE_USER_DEFINED, EventHandler);

  // Has grid?
  if(au64Grid != orxNULL)
  {
    // Deletes it
    orxMemory_Free(au64Grid);
    au64Grid = orxNULL;
  }
}

void TvB::UpdateT(const orxCLOCK_INFO &_rstInfo)
{
  orxFLOAT  fFallDelay;
  orxVECTOR vOffset;
  orxS32    s32Width, s32Height;
  orxS32    s32CleanedLines = 0;

  // Pushes config section
  orxConfig_PushSection("T");

  // Has selection?
  if(poSelection != orxNULL)
  {
    // No child?
    if(orxObject_GetOwnedChild(poSelection->GetOrxObject()) == orxNULL)
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

  // Left?
  if(orxInput_IsActive("MoveLeft"))
  {
    // Updates delay
    fLeftTime -= _rstInfo.fDT;

    // Can move?
    if(fLeftTime <= orxFLOAT_0)
    {
      orxVECTOR vMove;

      // Moves selection left
      orxVector_Set(&vMove, -orxFLOAT_1, orxFLOAT_0, orxFLOAT_0);

      // Moves it
      poSelection->Move(vMove, 0);

      // Updates delay
      fLeftTime = orxConfig_GetListFloat("MoveDelayList", orxInput_HasNewStatus("MoveLeft") ? 0 : 1);
    }

    // Resets right delay
    fRightTime = orxFLOAT_0;
  }
  else
  {
    // Resets left delay
    fLeftTime = orxFLOAT_0;

    // Right?
    if(orxInput_IsActive("MoveRight"))
    {
      // Updates delay
      fRightTime -= _rstInfo.fDT;

      // Can move?
      if(fRightTime <= orxFLOAT_0)
      {
        orxVECTOR vMove;

        // Moves selection right
        orxVector_Set(&vMove, orxFLOAT_1, orxFLOAT_0, orxFLOAT_0);

        // Moves it
        poSelection->Move(vMove, 0);

        // Updates delay
        fRightTime = orxConfig_GetListFloat("MoveDelayList", orxInput_HasNewStatus("MoveRight") ? 0 : 1);
      }
    }
    else
    {
      // Resets right delay
      fRightTime = orxFLOAT_0;
    }
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

  // Should fall one step?
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

  // Gets grid size
  TvB::GetInstance().GetGridSize(s32Width, s32Height);

  // Gets brick size
  orxConfig_GetVector("TetroBrickSize", &vOffset);

  // For all lines
  for(orxS32 i = s32Height - 1; i >= 0; i--)
  {
    orxBOOL bClean = orxTRUE;

    // For all columns
    for(orxS32 j = 0; j < s32Width; j++)
    {
      orxU64 u64GUID;

      // Gets GUID stored in cell
      u64GUID = (poSelection) ? poSelection->GetGUID() : 0;

      // Empty or selection?
      if((TvB::GetInstance().GetGridValue(j, i) == 0) || (TvB::GetInstance().GetGridValue(j, i) == u64GUID))
      {
        // Don't clean line
        bClean = orxFALSE;

        break;
      }
    }

    // Should clean line?
    if(bClean != orxFALSE)
    {
      // Updates line cleaned counter
      s32CleanedLines++;

      // Cleans grid line
      TvB::GetInstance().CleanGridLine(i);

      // For all bricks
      for(TvBBrick *poBrick = TvB::GetInstance().GetNextObject<TvBBrick>();
          poBrick;
          poBrick = TvB::GetInstance().GetNextObject<TvBBrick>(poBrick))
      {
        // Pushes its config section
        poBrick->PushConfigSection();

        // Is a tetromino brick and not a blockout one?
        if(orxConfig_GetBool("IsBBrick") == orxFALSE)
        {
          orxVECTOR vPos;
          orxS32 s32X, s32Y;

          // Gets its position
          poBrick->GetPosition(vPos, orxTRUE);

          // Gets its grid position
          if(TvB::GetInstance().GetGridPosition(vPos, s32X, s32Y) != orxSTATUS_FAILURE)
          {
            // Is on cleaned line?
            if(s32Y == i)
            {
              // Asks for deletion
              orxObject_SetLifeTime(poBrick->GetOrxObject(), orxFLOAT_0);
            }
            // Is above it?
            else if(s32Y < i)
            {
              // Moves it downward
              vPos.fY += vOffset.fY;
              poBrick->SetPosition(vPos, orxTRUE);
            }
          }
        }

        // Pops its config section
        poBrick->PopConfigSection();
      }

      // Updates line index due to cleaned line
      i++;
    }
  }

  // Any line cleaned?
  if(s32CleanedLines > 0)
  {
    TvB::EventPayload stPayload;
    orxEVENT          stEvent;

    // Inits event
    orxMemory_Zero(&stPayload, sizeof(TvB::EventPayload));
    stPayload.stAddLine.eType = (TvB::LineType)(s32CleanedLines - 1);

    // Sends add line event
    orxEVENT_INIT(stEvent, orxEVENT_TYPE_USER_DEFINED, TvB::EventIDAddLine, orxNULL, orxNULL, &stPayload);
    orxEvent_Send(&stEvent);

    // Adds sound on paddle
    TvB::GetInstance().GetNextObject<TvBPaddle>()->AddSound("LineCleared");
  }

  // Pops config section
  orxConfig_PopSection();
}
