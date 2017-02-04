/* Erik Aagaard Knudsen.
  Copyright © 2017 - All Rights Reserved

  Project: ZBridge
  File: CBidAndPlayEngines.cpp
  Developers: eak

  Revision History:
  25-jan-2017 eak: Original

  Abstract: Bid and play engines.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the bid and the play engines.
 */

#include "cbidengine.h"
#include "cplayengine.h"
#include "cbidandplayengines.h"

CBidAndPlayEngines::CBidAndPlayEngines(CBidDB *bidDB, CBidDesc *bidDesc,
                 CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc,
                 ScoringMethod scoringMethod)
{
    //Allocate bid and play engines.
    bidEngine = new CBidEngine(bidDB, bidDesc, nsBidOptionDoc, ewBidOptionDoc);
    playEngine = new CPlayEngine();

    //params.
    this->scoringMethod = scoringMethod;
}

CBidAndPlayEngines::~CBidAndPlayEngines()
{
    delete bidEngine;
    delete playEngine;
}
