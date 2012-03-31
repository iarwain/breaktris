//! Includes
#include "TvB.h"


//! Constants


//! Code
orxSTATUS TvB::InitT()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Creates viewport
  orxViewport_CreateFromConfig("TViewport");

  // Creates scene
  orxObject_CreateFromConfig("TScene");

  // Done!
  return eResult;
}

void TvB::ExitT()
{
}

void TvB::UpdateT(const orxCLOCK_INFO &_rstInfo)
{
  //! TODO
}
