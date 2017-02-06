/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidEngine.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Bid engine.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bid engine.
 */

#ifndef CBIDENGINE_H
#define CBIDENGINE_H

#include <QList>

#include "Defines.h"

class CBidDB;
class CBidDesc;
class CBidOptionDoc;
class CBidDBDefine;
class CBidHistory;
class CFeatures;
class CRule;

/**
 * @brief The bid engine class handles automatic bidding.
 */
class CBidEngine
{
public:
    CBidEngine(CBidDB *bidDB, CBidDesc *bidDesc,
               CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc);
    virtual ~CBidEngine();

    Bids getNextBid(CBidHistory &bidHistory, int cards[13], ScoringMethod scoringMethod,
            Team teamVul, Forcing *forcing, int *alertId);
    QList<CRule *> getpRules(CBidHistory &bidHistory, Bids bid, ScoringMethod scoringMethod,
                             Team teamVul);

private:
    CBidDB *bidDB;
    CBidDesc *bidDesc;
    CBidDBDefine *bidDBDefine;
};

#endif // CBIDENGINE_H
