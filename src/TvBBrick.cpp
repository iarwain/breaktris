//! Includes
#include "TvB.h"


//! Code
void TvBBrick::OnCreate()
{
  // Gets HP value from config
  s32HP = orxConfig_GetS32("HP");
}

void TvBBrick::Update(const orxCLOCK_INFO &_rstInfo)
{
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
    // Pushes config section
    PushConfigSection();

    // Is a tetromino brick and not a blockout one?
    if(orxConfig_GetBool("IsBBrick") == orxFALSE)
    {
      orxVECTOR vPos;
      orxS32    s32X, s32Y;

      // Gets its position
      GetPosition(vPos, orxTRUE);

      // Is it on grid?
      if(TvB::GetInstance().GetGridPosition(vPos, s32X, s32Y) != orxSTATUS_FAILURE)
      {
        // Clears grid
        TvB::GetInstance().SetGridValue(s32X, s32Y, 0);
      }
    }

    // Pops config section
    PopConfigSection();

    // Adds destruction track
    orxObject_AddTimeLineTrack(GetOrxObject(), "BrickDestruction");
  }

  // Done!
  return orxTRUE;
}
