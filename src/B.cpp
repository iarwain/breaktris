//! Includes
#include "TvB.h"


//! Constants


//! Code
orxSTATUS TvB::InitB()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  orxConfig_PushSection("B");

  orxConfig_GetListVector("Playground", 0, &vPlayTL);
  orxConfig_GetListVector("Playground", 1, &vPlayBR);

  for(orxS32 i = 0; i < orxConfig_GetListCounter("StartLines"); i++)
  {
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
