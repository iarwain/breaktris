//! Includes
#include "TvB.h"


//! Constants

//! Code
orxSTATUS TvB::InitT()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Creates viewport
  orxViewport_CreateFromConfig("TViewport");

  // Creates scene
  orxObject_CreateFromConfig("TScene");
  
  // Clears grid
  for(orxS32 i = 0; i < 20; i++)
  {
    for(orxS32 j = 0; j < 10; j++)
    {
      apoGrid[i][j] = orxNULL;
    }
  }

  // Clears selection
  poSelection = orxNULL;

  // Done!
  return eResult;
}

void TvB::ExitT()
{
}

void TvB::UpdateT(const orxCLOCK_INFO &_rstInfo)
{
  // No selection?
  if(poSelection == orxNULL)
  {
    // Pushes config section
    orxConfig_PushSection("T");

    // Creates a new tetromino
    poSelection = CreateObject<TvBTetromino>(orxConfig_GetString("TetrominoList"));

    // Pops config section
    orxConfig_PopSection();
  }
}
