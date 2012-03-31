//! Includes
#include "TvB.h"


//! Constants


//! Code
orxSTATUS TvB::InitT()
{
  orxVECTOR vSize;
  orxBITMAP *pstBitmap;
  orxTEXTURE *pstTexture;
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Pushes T config
  orxConfig_PushSection("T");

  // Creates background texture
  orxConfig_GetVector("TextureSize", &vSize);
  pstBitmap  = orxDisplay_CreateBitmap(vSize.fX, vSize.fY);
  pstTexture = orxTexture_Create();
  orxTexture_LinkBitmap(pstTexture, pstBitmap, "TTexture");

  // Pops config section
  orxConfig_PopSection();

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
