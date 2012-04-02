//! Includes
#include "TvB.h"


//! Code
void TvBBall::OnCreate()
{
  orxObject_GetSpeed(GetOrxObject(), &vSpeed);
  GetPosition(vInitPos);
  
  bIn = orxTRUE;
}

void TvBBall::Update(const orxCLOCK_INFO &_rstInfo)
{
  orxVECTOR vPos;

  // Game over?
  if(TvB::GetInstance().GetGameState() == TvB::GameStateEnd)
  {
    orxVector_Copy(&vSpeed, &orxVECTOR_0);
  }
  
  // Going out?
  if(GetPosition(vPos).fY > orxConfig_GetFloat("Bottom"))
  {
    // Respawns
    Respawn();
  }
  else if((vPos.fY < orxConfig_GetFloat("Top")) && (vSpeed.fY < orxFLOAT_0))
  {
    // Warps
    Warp();
  }
  else
  {
    orxObject_SetSpeed(GetOrxObject(), &vSpeed);
  }
  
  if(orxInput_IsActive("Warp") && orxInput_HasNewStatus("Warp"))
  {
    Warp();
  }
}

void TvBBall::OnDelete()
{
}

orxBOOL TvBBall::OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
  TvBPaddle *poPaddle;

  // Left?
  if(_rvNormal.fX > orx2F(0.5f))
  {
    // Updates speed
    vSpeed.fX = orxMath_Abs(vSpeed.fX);
  }
  // Right
  else if(_rvNormal.fX < orx2F(-0.5f))
  {
    // Updates speed
    vSpeed.fX = -orxMath_Abs(vSpeed.fX);
  }
  // Top?
  if(_rvNormal.fY > orx2F(0.5f))
  {
    // Updates speed
    vSpeed.fY = orxMath_Abs(vSpeed.fY);
  }
  // Bottom
  else if(_rvNormal.fY < orx2F(-0.5f))
  {
    // Updates speed
    vSpeed.fY = -orxMath_Abs(vSpeed.fY);
  }

  // Gets paddle
  poPaddle = TvB::GetInstance().GetNextObject<TvBPaddle>();
  
  // Colliding with the paddle?
  if(_poCollider == poPaddle)
  {
    orxVECTOR vOffset, vPos1, vPos2;
    
    // Adds sound
    poPaddle->AddSound("PaddleHit");
    
    // Gets offset
    orxVector_Sub(&vOffset, &GetPosition(vPos1, orxTRUE), &poPaddle->GetPosition(vPos2, orxTRUE));

    // Updates speed
    PushConfigSection();
    vSpeed.fX += orxConfig_GetFloat("ReactionScale") * vOffset.fX;
    
    orxVector_FromCartesianToSpherical(&vSpeed, &vSpeed);
    vSpeed.fRho += orxConfig_GetFloat("SpeedUp");
    orxVector_FromSphericalToCartesian(&vSpeed, &vSpeed);
    PopConfigSection();
  }
  else
  {
    // Adds sound
    AddSound("BallHit");
  }

  
  return orxTRUE;
}

void TvBBall::Respawn(const orxVECTOR *_pvPos, const orxVECTOR *_pvSpeed)
{
  // Resets ball
  Enable(orxTRUE, orxFALSE);

  // Back to B game?
  if(_pvPos == orxNULL)
  {
    // Changes its space
    orxObject_SetParent(GetOrxObject(), orxCamera_Get("BCamera"));
  }
  
  // Resets potision & speed
  SetPosition((_pvPos != orxNULL) ? *_pvPos : vInitPos);
  orxObject_SetSpeed(GetOrxObject(), (_pvSpeed != orxNULL) ? _pvSpeed : orxConfig_GetVector("Speed", &vSpeed));
}

void TvBBall::Warp()
{
  TvB::EventPayload stPayload;
  orxEVENT stEvent;
  TvB::EventID eEventID;

  orxMemory_Zero(&stPayload, sizeof(TvB::EventPayload));
  GetPosition(stPayload.stWarp.vPos);
  vSpeed.fY *= -orxFLOAT_1;
  orxVector_Copy(&stPayload.stWarp.vSpeed, &vSpeed);
  eEventID = (bIn != orxFALSE) ? TvB::EventIDWarpOut : TvB::EventIDWarpIn;

  // Sends warp out event
  orxEVENT_INIT(stEvent, orxEVENT_TYPE_USER_DEFINED, eEventID, this, orxNULL, &stPayload);
  orxEvent_Send(&stEvent);

  bIn = !bIn;

  // Disables ball
//  Enable(orxFALSE, orxTRUE);
}
