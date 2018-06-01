/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CGameOptionDoc.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Game option data.

  Platforms: Qt.

*/

/**
 * \file
 * Game option data (declaration).
 */

#ifndef CGAMEOPTIONDOC_H
#define CGAMEOPTIONDOC_H

#include "Defines.h"

/**
 * @brief Holds the game options and methods to store/restore the options.
 */
class CGameOptionDoc
{
public:
    CGameOptionDoc();

    void WriteSettings();
    void ReadSettings();

    //Interface.
    bool insertBiddingPause;
    int biddingPause;
    bool insertPlayPause;
    int playPause;
    int computerLevel;

    //Scoring.
    ScoringMethod scoringMethod;
};

#endif // CGAMEOPTIONDOC_H
