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
 * bidders cards. Furthermore it maintains the disclosed feature limits for all bidders (players)
 */
class CBidHistory
{
public:
    CBidHistory();

    void appendBid(CBid &bid);
    void resetBidHistory();
    int undo(Bids *bid);
    bool passedOut();
    void setSeat(Seat seat);
    void setFeatures(int cards[]);
    CFeatures &getFeatures() { return features; }

    CFeatures &getLowPartnerFeatures(Seat seat);
    CFeatures &getHighPartnerFeatures(Seat seat);
    CFeatures &getLowOwnFeatures(Seat seat);
    CFeatures &getHighOwnFeatures(Seat seat);
    CFeatures &getLowRHFeatures(Seat seat);
    CFeatures &getHighRHFeatures(Seat seat);
    CFeatures &getLowLHFeatures(Seat seat);
    CFeatures &getHighLHFeatures(Seat seat);

    QList<CBid> bidList;            /**< The list of the given bids. Public for easy access. */

private:
    CFeatures features;             /**< Features of the cards for the bidder. */
    CFeatures lowPartnerFeatures;   /**< Low limits for public known partner features. */
    CFeatures highPartnerFeatures;  /**< High limits for public known partner features. */
    CFeatures lowOwnFeatures;       /**< Low limits for public known own features. */
    CFeatures highOwnFeatures;      /**< High limits for public known own features. */
    CFeatures lowRHFeatures;        /**< Low limits for public known right hand players features. */
    CFeatures highRHFeatures;       /**< High limits for public known right hand players features. */
    CFeatures lowLHFeatures;        /**< Low limits for public known left hand players features. */
    CFeatures highLHFeatures;       /**< High limits for public known left hand players features. */
    Seat partnerSeat, ownSeat, rhSeat, lhSeat;

    void removeBid();
    void calculateRange(Seat seat, CFeatures &lowFeatures, CFeatures &highFeatures);
    void CalculateBidRuleRange(int inx, CFeatures &lowFeatures, CFeatures &highFeatures);
    bool isNT(int inx);
};

#endif // CBIDHISTORY_H
