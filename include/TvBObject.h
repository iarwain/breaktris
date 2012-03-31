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

#endif // __TVB_OBJECT_H__
