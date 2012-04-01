//! Includes
#include "TvB.h"


//! Code
void TvBTetromino::OnCreate()
{
  orxVECTOR vOffset;
  
  // Goes to the T screen
  orxObject_SetParent(GetOrxObject(), orxCamera_Get("TCamera"));
  
  // Sets our start pos
  
  // Gets brick offset
  orxConfig_GetVector("TetroBrickSize", &vOffset);
  
  // For all blocks
  for(orxS32 i = 0; i < orxConfig_GetListCounter("TetroPosList"); i++)
  {
    orxVECTOR vPos;
    TvBBrick *poBrick;
    
    // Creates it
    poBrick = TvB::GetInstance().CreateObject<TvBBrick>("BrickGreen");
    
    // Sets as parent
    orxObject_SetParent(poBrick->GetOrxObject(), GetOrxObject());

    // Gets its position
    orxConfig_GetListVector("TetroPosList", i, &vPos);
    orxVector_Mul(&vPos, &vPos, &vOffset);

    // Updates its position
    poBrick->SetPosition(vPos, orxFALSE);
  }
}

void TvBTetromino::Update(const orxCLOCK_INFO &_rstInfo)
{
}

void TvBTetromino::OnDelete()
{
}

void TvBTetromino::Land()
{
  // Deletes
  orxObject_SetLifeTime(GetOrxObject(), orxFLOAT_0);
}
