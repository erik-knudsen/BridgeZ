/* Erik Aagaard Knudsen.
  Copyright © 2017 - All Rights Reserved

  Project: ZBridge
  File: CBidAndPlayEngines.h
  Developers: eak

  Revision History:
  25-jan-2017 eak: Original

  Abstract: Bid and play engines.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bid and the play engines.
 */

#ifndef CBIDANDPLAYENGINES_H
#define CBIDANDPLAYENGINES_H

#include "cfeatures.h"
#include "cbidhistory.h"

class CBidDB;
class CBidDesc;
class CBidOptionDoc;
class CBidEngine;
class CPlayEngine;
class CRule;

class CBidAndPlayEngines
{
public:
    CBidAndPlayEngines();
    virtual ~CBidAndPlayEngines();

    void initialize(CBidDB *bidDB, CBidDesc *bidDesc,
                    CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc,
                    ScoringMethod scoringMethod);
    void setScoringMethod(ScoringMethod scoringMethod) { this->scoringMethod = scoringMethod; }
    Bids getNextBid(CBidHistory &bidHistory, int cards[], Team teamVul,
                    Forcing *forcing, int *alertId);
    QList<CRule *> getpRules(CBidHistory &bidHistory, Bids bid, Team teamVul);

private:
    CBidEngine *bidEngine;
    CPlayEngine *playEngine;

    ScoringMethod scoringMethod;
};

#endif // CBIDANDPLAYENGINES_H
