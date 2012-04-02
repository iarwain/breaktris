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
    case TvB::EventIDWarpIn:
    {
      TvB::EventPayload *pstPayload;
      TvBBall *poBall;
      
      // Gets payload
      pstPayload = (TvB::EventPayload *)_pstEvent->pstPayload;

      // Gets ball
      poBall = TvB::GetInstance().GetNextObject<TvBBall>();

      // Respawns
      poBall->Respawn(&pstPayload->stWarp.vPos, &pstPayload->stWarp.vSpeed);
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

orxSTATUS TvB::InitB()
{
  orxVECTOR vPos;
  orxCAMERA *pstCamera;
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Creates viewport
  orxViewport_CreateFromConfig("BViewport");

  pstCamera = orxCamera_Get("BCamera");

  orxConfig_PushSection("B");

  orxConfig_GetListVector("Playground", 0, &vPlayTL);
  orxConfig_GetListVector("Playground", 1, &vPlayBR);
  orxConfig_GetVector("BrickSize", &vBrickSize);

  orxVector_Copy(&vPos, &vPlayTL);

  for(orxS32 i = 0; i < orxConfig_GetListCounter("StartBricks"); i++)
  {
    const orxSTRING zLine = orxConfig_GetListString("StartBricks", i);

    vPos.fX = vPlayTL.fX;

    for(orxS32 j = 0; j < orxConfig_GetListCounter(zLine); j++)
    {
      TvBBrick *poBrick = CreateObject<TvBBrick>(orxConfig_GetListString(zLine, j));

      if(poBrick)
      {
        orxObject_SetParent(poBrick->GetOrxObject(), pstCamera);
        poBrick->SetPosition(vPos);
      }
      vPos.fX += vBrickSize.fX;
    }

    vPos.fY += vBrickSize.fY;
  }
  
  orxConfig_PopSection();
  
  // Creates scene
  CreateObject("BScene");

  // Adds event handler
  orxEvent_AddHandler(orxEVENT_TYPE_USER_DEFINED, EventHandler);

  // Done!
  return eResult;
}

void TvB::ExitB()
{
  // Removes event handler
  orxEvent_RemoveHandler(orxEVENT_TYPE_USER_DEFINED, EventHandler);
}

void TvB::UpdateB(const orxCLOCK_INFO &_rstInfo)
{
  if(orxInput_IsActive("NewLine") && orxInput_HasNewStatus("NewLine"))
  {
    AddBLine(LineTypeDefault);
  }
}

void TvB::AddBLine(LineType _eType)
{
  // For all bricks
  for(TvBBrick *poBrick = GetNextObject<TvBBrick>();
      poBrick != NULL;
      poBrick = GetNextObject<TvBBrick>(poBrick))
  {
    orxVECTOR vPos;
    orxS32    s32X, s32Y;

    // Out of grid?
    poBrick->GetPosition(vPos, orxTRUE);
    if(TvB::GetInstance().GetGridPosition(vPos, s32X, s32Y) == orxFALSE)
    {
      // Gets its position
      poBrick->GetPosition(vPos);

      // Updates it
      vPos.fY += vBrickSize.fY;

      // Updates brick
      poBrick->SetPosition(vPos);
    
      // Game over?
      if(vPos.fY > vPlayBR.fY)
      {
        //! TODO: Game over
        meGameState = GameStateEnd;
      }
    }

    //! TODO: Add new line(s)
  }
}
