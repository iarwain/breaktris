//! Includes
#include "TvB.h"


//! Code
void TvBBall::OnCreate()
{
}

void TvBBall::Update(const orxCLOCK_INFO &_rstInfo)
{
}

void TvBBall::OnDelete()
{
}

orxBOOL TvBBall::OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
  orxVECTOR vSpeed;
  
  // Gets speed
  orxObject_GetSpeed(GetOrxObject(), &vSpeed);
  
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

  // Sets speed
  orxObject_SetSpeed(GetOrxObject(), &vSpeed);

  return orxTRUE;
}
