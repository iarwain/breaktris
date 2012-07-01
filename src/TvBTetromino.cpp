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
    
    // Sets its owner
    orxObject_SetOwner(poBrick->GetOrxObject(), GetOrxObject());
    
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
  orxOBJECT *pstChild;

  // Removes all children
  while((pstChild = orxObject_GetOwnedChild(GetOrxObject())) != orxNULL)
  {
    orxObject_SetOwner(pstChild, orxNULL);
  }
  
  // Adds sound on paddle
  TvB::GetInstance().GetNextObject<TvBPaddle>()->AddSound("BlockPlaced");
  
  // Deletes
  orxObject_SetLifeTime(GetOrxObject(), orxFLOAT_0);
}

void TvBTetromino::Transform(const orxVECTOR &_rvPos, orxS32 _s32Rotation)
{
  orxVECTOR vPos, vOffset;
  
  PushConfigSection();

  // Position
  orxConfig_GetVector("TetroBrickSize", &vOffset);
  GetPosition(vPos);
  orxVector_Mul(&vOffset, &vOffset, &_rvPos);
  orxVector_Add(&vPos, &vPos, &vOffset);
  SetPosition(vPos);
  
  // Rotation
  s32Rotation += _s32Rotation;
  while(s32Rotation < 0)
  {
    s32Rotation += orxConfig_GetListCounter("RotationList");
  }
  s32Rotation = s32Rotation % orxConfig_GetListCounter("RotationList");
  SetRotation(orxConfig_GetListFloat("RotationList", s32Rotation) * orxMATH_KF_DEG_TO_RAD);

  PopConfigSection();
}

orxBOOL TvBTetromino::IsValid(const orxVECTOR &_rvPos, orxS32 _s32Rotation) const
{
  orxVECTOR vPos;
  orxU64    u64GUID;
  orxBOOL   bResult = orxTRUE;

  // Gets our GUID
  u64GUID = GetGUID();

  // Transforms
  const_cast<TvBTetromino *>(this)->Transform(_rvPos, _s32Rotation);

  orxS32 i = 0;
  // For all children
  for(orxOBJECT *pstChild = orxObject_GetOwnedChild(GetOrxObject());
      pstChild != orxNULL;
      pstChild = orxObject_GetOwnedSibling(pstChild), i++)
  {
    orxVECTOR vPos;
    orxS32    s32X, s32Y;

    // Gets its position
    orxObject_GetWorldPosition(pstChild, &vPos);
    
    // Gets its grid index
    if(TvB::GetInstance().GetGridPosition(vPos, s32X, s32Y) != orxSTATUS_FAILURE)
    {
      orxU64 u64Value;

      // Gets its value
      u64Value = TvB::GetInstance().GetGridValue(s32X, s32Y);

      // Is not us and not empty?
      if((u64Value != 0) && (u64Value != u64GUID))
      {
        // Updates result
        bResult = orxFALSE;
        
        break;
      }
    }
    else
    {
      // Updates result
      bResult = orxFALSE;
      
      break;
    }
    
//    orxLOG("Pos<%d>: %ld, %ld", i, s32X, s32Y);
  }
  
  // Transforms back
  const_cast<TvBTetromino *>(this)->Transform(*orxVector_Mulf(&vPos, &_rvPos, -orxFLOAT_1), -_s32Rotation);
  
  // Done!
  return bResult;
}

void TvBTetromino::Validate()
{
  orxS32 s32Width, s32Height;
  orxU64 u64GUID;
  
  // Gets GUID
  u64GUID = GetGUID();
  
  // Gets grid size
  TvB::GetInstance().GetGridSize(s32Width, s32Height);

  // For all grid elements
  for(orxS32 i = 0; i < s32Height; i++)
  {
    for(orxS32 j = 0; j < s32Width; j++)
    {
      // Is us?
      if(TvB::GetInstance().GetGridValue(j, i) == u64GUID)
      {
        // Clears it
        TvB::GetInstance().SetGridValue(j, i, 0);
      }
    }
  }
  
  // For all children
  for(orxOBJECT *pstChild = orxObject_GetOwnedChild(GetOrxObject());
      pstChild != orxNULL;
      pstChild = orxObject_GetOwnedSibling(pstChild))
  {
    orxVECTOR vPos;
    orxS32    s32X, s32Y;
    
    // Gets its position
    orxObject_GetWorldPosition(pstChild, &vPos);
    
    // Gets its grid position
    if(TvB::GetInstance().GetGridPosition(vPos, s32X, s32Y) != orxSTATUS_FAILURE)
    {
      // Stores GUID
      TvB::GetInstance().SetGridValue(s32X, s32Y, GetGUID());
    }
  }
}

orxBOOL TvBTetromino::Move(const orxVECTOR &_rvPos, orxS32 _s32Rotation)
{
  orxBOOL bResult;

  bResult = IsValid(_rvPos, _s32Rotation);
  
  // Valid?
  if(bResult != orxFALSE)
  {
    // Transforms
    Transform(_rvPos, _s32Rotation);

    // Validate
    Validate();
  }
  
  // Done!
  return bResult;
}
