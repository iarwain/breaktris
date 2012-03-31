//! Includes
#include "TvB.h"


//! Code
void TvBPaddle::OnCreate()
{
}

void TvBPaddle::Update(const orxCLOCK_INFO &_rstInfo)
{
  orxVECTOR vMove;

  vMove.fX = orxInput_GetValue("PaddleMove");
  vMove.fY = orxFLOAT_0;
  vMove.fZ = orxFLOAT_0;

  orxObject_SetSpeed(GetOrxObject(), orxVector_Divf(&vMove, &vMove, _rstInfo.fDT));
}

void TvBPaddle::OnDelete()
{
}
