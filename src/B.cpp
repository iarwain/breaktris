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

      // Changes its space
      orxObject_SetParent(poBall->GetOrxObject(), orxCamera_Get("BCamera"));

      // Respawns
      poBall->Respawn(&pstPayload->stWarp.vPos, &pstPayload->stWarp.vSpeed);

      break;
    }

    case TvB::EventIDAddLine:
    {
      // Adds a line
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

  // Creates viewport / camera
  orxViewport_CreateFromConfig("BViewport");
  pstCamera = orxCamera_Get("BCamera");

  // Pushes config section
  orxConfig_PushSection("B");

  // Inits vars
  orxConfig_GetListVector("Playground", 0, &vPlayTL);
  orxConfig_GetListVector("Playground", 1, &vPlayBR);
  orxConfig_GetVector("BrickSize", &vBrickSize);
  orxVector_Copy(&vPos, &vPlayTL);

  // Starting bricks
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

  // Pops config section
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
  // Debug: Adds a new line?
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

    // Pushes config section
    poBrick->PushConfigSection();

    // B brick?
    if(orxConfig_GetBool("IsBBrick"))
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

        break;
      }
    }

    // Pops config section
    poBrick->PopConfigSection();
  }

  // Still running?
  if(meGameState != GameStateEnd)
  {
    orxVECTOR vPos;
    orxCAMERA *pstCamera;

    // Pushes config section
    orxConfig_PushSection("B");

    // Gets camera
    pstCamera = orxCamera_Get("BCamera");

    // Inits position at playground's corner
    orxVector_Copy(&vPos, &vPlayTL);

    // Spawns all the bricks
    for(orxS32 j = 0; j < orxConfig_GetS32("LineSize"); j++)
    {
      TvBBrick *poBrick = CreateObject<TvBBrick>(orxConfig_GetListString("BrickList", _eType));

      if(poBrick)
      {
        orxObject_SetParent(poBrick->GetOrxObject(), pstCamera);
        poBrick->SetPosition(vPos);
      }
      vPos.fX += vBrickSize.fX;
    }

    // Pops config section
    orxConfig_PopSection();
  }
}
