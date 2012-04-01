//! Includes
#include "TvB.h"


//! Code
void TvBPaddle::OnCreate()
{
}

void TvBPaddle::Update(const orxCLOCK_INFO &_rstInfo)
{
  orxVECTOR vMove, vPos;

  vMove.fX = orxInput_GetValue("PaddleMove");
  vMove.fY = orxFLOAT_0;
  vMove.fZ = orxFLOAT_0;

  GetPosition(vPos);
  orxVector_Add(&vPos, &vPos, &vMove);

  vPos.fX = orxCLAMP(vPos.fX, orxConfig_GetListFloat("Limit", 0), orxConfig_GetListFloat("Limit", 1));

  SetPosition(vPos);
  orxMouse_SetPosition(&vPos);
}

void TvBPaddle::OnDelete()
{
}

orxBOOL TvBPaddle::OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
}
