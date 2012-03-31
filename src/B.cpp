//! Includes
#include "TvB.h"


//! Constants


//! Code
orxSTATUS TvB::InitB()
{
  orxVECTOR vSize;
  orxBITMAP *pstBitmap;
  orxTEXTURE *pstTexture;
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Pushes B config
  orxConfig_PushSection("B");

  // Creates background texture
  orxConfig_GetVector("TextureSize", &vSize);
  pstBitmap  = orxDisplay_CreateBitmap(vSize.fX, vSize.fY);
  pstTexture = orxTexture_Create();
  orxTexture_LinkBitmap(pstTexture, pstBitmap, "BTexture");

  // Pops config section
  orxConfig_PopSection();

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
  //! TODO
}
