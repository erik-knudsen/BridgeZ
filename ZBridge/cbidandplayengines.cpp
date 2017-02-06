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

#include <cassert>

#include "cbidengine.h"
#include "cplayengine.h"
#include "cbidandplayengines.h"

CBidAndPlayEngines::CBidAndPlayEngines()
{
    bidEngine = 0;
    playEngine = 0;
}

CBidAndPlayEngines::~CBidAndPlayEngines()
{
    if (bidEngine != 0)
        delete bidEngine;
    if (playEngine != 0)
        delete playEngine;
}

void CBidAndPlayEngines::initialize(CBidDB *bidDB, CBidDesc *bidDesc, CBidOptionDoc &nsBidOptionDoc,
                                    CBidOptionDoc &ewBidOptionDoc, ScoringMethod scoringMethod)
{
    if (bidEngine != 0)
        delete bidEngine;
    if (playEngine != 0)
        delete playEngine;

    //Allocate bid and play engines.
    bidEngine = new CBidEngine(bidDB, bidDesc, nsBidOptionDoc, ewBidOptionDoc);
    playEngine = new CPlayEngine();

    //params.
    this->scoringMethod = scoringMethod;
}

/**
 * @brief Calculate the next bid
 *
 * Calculate the next bid by using the bidding database.
 *
 * @param[in] bidHistory The bid history.
 * @param[in] cards The cards for the next bidder.
 * @param[in] teamVul Team vulnerability.
 * @param[out] forcing The forcing status.
 * @param[out] alertId The alert id.
 * @return The calculated next bid. If none was found then return BID_NONE.
 */
Bids CBidAndPlayEngines::getNextBid(CBidHistory &bidHistory, int cards[],
                            Team teamVul, Forcing *forcing, int *alertId)
{
    assert(bidEngine != 0);

    //Must check something found in the bid database!!!!!!!
    return bidEngine->getNextBid(bidHistory, cards, scoringMethod, teamVul, forcing, alertId);
}

/**
 * @brief Get possible rules for a given bid history and next bid as calculated by getNextBid.
 * @param[in] bidHistory The bid history.
 * @param bid[in] The bid calculated by getNext bid.
 * @param teamVul Team vulnerability.
 * @return returns a list with possible rules.
 */
QList<CRule *> CBidAndPlayEngines::getpRules(CBidHistory &bidHistory, Bids bid,
                                             Team teamVul)
{
    assert(bidEngine != 0);

    //Must check something found in the bid database!!!!!!!
    return bidEngine->getpRules(bidHistory, bid, scoringMethod, teamVul);
}
