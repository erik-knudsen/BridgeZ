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

/**
 * @brief Allocate bid and play engines.
 *
 * @param computerLevel Computing level for the double dummy solver.
 * @param bidDB The bid database.
 * @param bidDesc Textual descriptions related to the bid database.
 * @param nsBidOptionDoc Options for the north/south pair.
 * @param ewBidOptionDoc Options for the east/west pair.
 * @param scoringMethod The scoringmethod.
 */
void CBidAndPlayEngines::initialize(int computerLevel, CBidDB *bidDB, CBidDesc *bidDesc, CBidOptionDoc &nsBidOptionDoc,
                                    CBidOptionDoc &ewBidOptionDoc, ScoringMethod scoringMethod)
{
    if (bidEngine != 0)
        delete bidEngine;
    if (playEngine != 0)
        delete playEngine;

    //Allocate bid and play engines.
    bidEngine = new CBidEngine(bidDB, bidDesc, nsBidOptionDoc, ewBidOptionDoc);
    playEngine = new CPlayEngine(computerLevel, nsBidOptionDoc, ewBidOptionDoc);

    //params.
    this->scoringMethod = scoringMethod;
}

/**
 * @brief Calculate the next bid
 *
 * Calculate the next bid by using the bidding database.
 *
 * @param[in] seat Bidders seat.
 * @param[in] ownFeatures The features of the bidders cards.
 * @param[in] bidHistory The bid history.
 * @param[in] teamVul Team vulnerability.
 * @return The calculated next bid. If none was found then return BID_NONE.
 */
CBid CBidAndPlayEngines::getNextBid(Seat seat, CFeatures &ownFeatures, CBidHistory &bidHistory, Team teamVul)
{
    assert(bidEngine != 0);

    return bidEngine->getNextBid(seat, ownFeatures, bidHistory, scoringMethod, teamVul);
}

/**
 * @brief Get possible rules for a given bid history and next bid as calculated by getNextBid.
 *
 * @param[in] seat Bidders seat.
 * @param[in] bidHistory The bid history.
 * @param[in] bid The bid calculated by getNext bid.
 * @param[in] teamVul Team vulnerability.
 * @param[out] substitute true if a substitute auction was used.
 * @return returns a list with possible rules.
 */
QList<CRule *> CBidAndPlayEngines::getpRules(Seat seat, CBidHistory &bidHistory, Bids bid,
                                             Team teamVul, bool *substitute)
{
    assert(bidEngine != 0);

    return bidEngine->getpRules(seat, bidHistory, bid, scoringMethod, teamVul, substitute);
}

QString CBidAndPlayEngines::getAlertIdDesc(quint8 alertId)
{
    return bidEngine->getAlertIdDesc(alertId);
}

/**
 * @brief Determine the next card to play.
 *
 * @param seat The players seat (dummys cards are played by declarer).
 * @param dummySeat The dummys seat.
 * @param ownCards The players cards.
 * @param dummyCards The dummys cards.
 * @param bidHistory The bid history.
 * @param playHistory The play history.
 * @return The next card to play.
 */
int CBidAndPlayEngines::getNextPlay(Seat seat, Seat dummySeat, int ownCards[], int dummyCards[],
                                    CBidHistory &bidHistory, CPlayHistory &playHistory)
{
    assert (playEngine != 0);

    return playEngine->getNextPlay(seat, dummySeat, ownCards, dummyCards, bidHistory, playHistory);
}
