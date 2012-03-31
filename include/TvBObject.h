#ifndef __TVB_OBJECT_H__
#define __TVB_OBJECT_H__

//! Includes
#include "Scroll.h"


//! Constants

//! EntObject class
class TvBPaddle : public ScrollObject
{
        void                    OnCreate();
        void                    OnDelete();
        void                    Update(const orxCLOCK_INFO &_rstInfo);
};

class TvBBall : public ScrollObject
{
  void                    OnCreate();
  void                    OnDelete();
  void                    Update(const orxCLOCK_INFO &_rstInfo);
  orxBOOL                 OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal);
  
  orxVECTOR vSpeed;
};

class TvBBrick : public ScrollObject
{
  void                    OnCreate();
  void                    OnDelete();
  void                    Update(const orxCLOCK_INFO &_rstInfo);
  orxBOOL                 OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal);
  
  orxS32                  s32HP;
};

#endif // __TVB_OBJECT_H__
