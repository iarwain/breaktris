//! Includes
#include "TvB.h"


//! Code
void TvBTetromino::OnCreate()
{
  orxVECTOR vOffset, vPos, vOrigin;
  orxCOLOR stColor;
  
  // Gets brick offset
  orxConfig_GetVector("TetroBrickSize", &vOffset);
  
  // Goes to the T screen
  orxObject_SetParent(GetOrxObject(), orxCamera_Get("TCamera"));

  // Sets our start pos
  orxConfig_PushSection("T");
  orxConfig_GetVector("GridOrigin", &vOrigin);  
  orxConfig_PopSection();
  orxConfig_GetVector("SpawnOffset", &vPos);
  orxVector_Mul(&vPos, &vPos, &vOffset);
  orxVector_Add(&vPos, &vPos, &vOrigin);
  SetPosition(vPos);
  
  // Gets color
  orxConfig_GetVector("Color", &stColor.vRGB);
  orxVector_Mulf(&stColor.vRGB, &stColor.vRGB, orxCOLOR_NORMALIZER);
  stColor.fAlpha = orxFLOAT_1;

  // For all blocks
  for(orxS32 i = 0; i < orxConfig_GetListCounter("TetroPosList"); i++)
  {
    orxVECTOR vPos;
    TvBBrick *poBrick;
    
    // Creates it
    poBrick = TvB::GetInstance().CreateObject<TvBBrick>("BrickTetro");
    
    // Sets its color
    poBrick->SetColor(stColor, orxFALSE);
    
    // Sets as parent
    orxObject_SetParent(poBrick->GetOrxObject(), GetOrxObject());

    // Gets its position
    orxConfig_GetListVector("TetroPosList", i, &vPos);
    orxVector_Mul(&vPos, &vPos, &vOffset);

    // Updates its position
    poBrick->SetPosition(vPos, orxFALSE);
  }

  // Sets rotation
  s32Rotation = 0;
  
  // Moves
  Move(orxVECTOR_0, 0);
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

orxBOOL TvBTetromino::IsValid(const orxVECTOR &_rvPos, orxS32 _s32Rotation) const
{
  orxBOOL bResult = orxTRUE;

  // Done!
  return bResult;
}

orxBOOL TvBTetromino::Move(const orxVECTOR &_rvPos, orxS32 _s32Rotation)
{
  orxBOOL bResult;

  bResult = IsValid(_rvPos, _s32Rotation);
  
  // Valid?
  if(bResult != orxFALSE)
  {
    orxVECTOR vPos, vOffset;
    
    // Gets brick offset
    PushConfigSection();
    orxConfig_GetVector("TetroBrickSize", &vOffset);

    GetPosition(vPos);
    orxVector_Mul(&vOffset, &vOffset, &_rvPos);
    orxVector_Add(&vPos, &vPos, &vOffset);
    SetPosition(vPos);

    // Gets rotation
    s32Rotation += _s32Rotation;
    while(s32Rotation < 0)
    {
      s32Rotation += orxConfig_GetListCounter("RotationList");
    }
    s32Rotation = s32Rotation % orxConfig_GetListCounter("RotationList");
    SetRotation(orxConfig_GetListFloat("RotationList", s32Rotation) * orxMATH_KF_DEG_TO_RAD);
    
    PopConfigSection();
  }
  
  // Done!
  return bResult;
}
