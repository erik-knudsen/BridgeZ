/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidHistory.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid history.

  Platforms: Qt.

*/

/**
 * \file
 * Bid History (declaration).
 */

#ifndef CBIDHISTORY_H
#define CBIDHISTORY_H

#include <QList>

#include "cfeatures.h"
#include "cbid.h"

/**
 * @brief The CBidHistory class describes the bid history.
 *
 * The bid history has the bids given by the players. It also has the features (HCP etc.) for the
 * bidders cards. Furthermore it maintains the disclosed feature limits for all bidders (players).
 * The bid history is also used on the server where feature limits are not known.
 */
class CBidHistory
{
public:
    CBidHistory();

    void setSeat(Seat seat);
    void appendBid(CBid &bid);
    void resetBidHistory();
    Seat getDeclarer();
    int undo(Bids *bid);
    bool passedOut();
    CFeatures &getLowFeatures(Seat seat) { return lowFeatures[seat]; }
    CFeatures &getHighFeatures(Seat seat) { return highFeatures[seat]; }

    QList<CBid> bidList;            /**< The list of the given bids. Public for easy access. */

private:
    CFeatures features;             /**< Features of the cards for the bidder. */
    CFeatures lowFeatures[4];       /**< Low limits for public known player features. */
    CFeatures highFeatures[4];      /**< High limits for public known player features. */
    Seat seat;                      /**< The bidders seat. */

    void initializeFeatures();
    void removeBid();
    void calculateRange(Seat seat, CFeatures &lowFeatures, CFeatures &highFeatures);
    void CalculateBidRuleRange(int inx, CFeatures &lowFeatures, CFeatures &highFeatures);
};

#endif // CBIDHISTORY_H
