//! Includes
#include "TvB.h"


//! Code
void TvBBrick::OnCreate()
{
  s32HP = orxConfig_GetS32("HP");
}

void TvBBrick::Update(const orxCLOCK_INFO &_rstInfo)
{
  // Dead?
  if(s32HP <= 0)
  {
    orxObject_SetLifeTime(GetOrxObject(), orxFLOAT_0);
  }
}

void TvBBrick::OnDelete()
{
}

orxBOOL TvBBrick::OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
  // Updates HP
  s32HP--;

  // Done!
  return orxTRUE;
}
