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
    // Adds sound
    TvB::GetInstance().GetNextObject<TvBBall>()->AddSound("BrickDestroyed");

    // Deletes it
    orxObject_SetLifeTime(GetOrxObject(), orxFLOAT_0);
  }
  else
  {
    GetPosition(vPos, orxTRUE);
  }

}

void TvBBrick::OnDelete()
{
}

orxBOOL TvBBrick::OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
  // Updates HP
  s32HP--;
  
  // Dead?
  if(s32HP <= 0)
  {    
    // On grid?
    PushConfigSection();
    if(orxConfig_GetBool("IsBBrick") == orxFALSE)
    {
      orxVECTOR vPos;
      orxS32    s32X, s32Y;
    
      GetPosition(vPos, orxTRUE);
      if(TvB::GetInstance().GetGridPosition(vPos, s32X, s32Y) != orxSTATUS_FAILURE)
      {
        // Clears it
        TvB::GetInstance().SetGridValue(s32X, s32Y, 0);
      }
    }
    PopConfigSection();
  }

  // Done!
  return orxTRUE;
}
