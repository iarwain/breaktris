//! Includes
#include "TvB.h"


//! Constants


//! Code
orxSTATUS TvB::InitB()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

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
