//! Includes
#include "TvB.h"


//! Code
void TvBBall::OnCreate()
{
  orxObject_GetSpeed(GetOrxObject(), &vSpeed);
}

void TvBBall::Update(const orxCLOCK_INFO &_rstInfo)
{
  orxObject_SetSpeed(GetOrxObject(), &vSpeed);
}

void TvBBall::OnDelete()
{
}

orxBOOL TvBBall::OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
  // Vertical?
  if(orxMath_Abs(_rvNormal.fX) > orxMath_Abs(_rvNormal.fY))
  {
    // Updates speed
    vSpeed.fX *= -orxFLOAT_1;
  }
  else
  {
    // Updates speed
    vSpeed.fY *= -orxFLOAT_1;
  }

  return orxTRUE;
}
