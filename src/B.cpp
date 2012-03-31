//! Includes
#include "TvB.h"


//! Constants


//! Code
orxSTATUS TvB::InitB()
{
  orxVECTOR vBrickSize, vOffset, vPos;
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  orxConfig_PushSection("B");

  orxConfig_GetListVector("Playground", 0, &vPlayTL);
  orxConfig_GetListVector("Playground", 1, &vPlayBR);
  orxConfig_GetVector("BrickSize", &vBrickSize);

  orxVector_Copy(&vOffset, &vBrickSize);

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
        poBrick->SetPosition(vPos);
      }
      vPos.fX += vOffset.fX;
    }

    vPos.fY += vOffset.fY;
  }
  
  orxConfig_PopSection();
  
  // Creates viewport
  orxViewport_CreateFromConfig("BViewport");

  // Creates scene
  CreateObject("BScene");

  // Done!
  return eResult;
}

void TvB::ExitB()
{
}

void TvB::UpdateB(const orxCLOCK_INFO &_rstInfo)
{
}
