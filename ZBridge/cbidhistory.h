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

    QList<CBid> bidList;

private:
    CFeatures features;
    CFeatures lowPartnerFeatures;
    CFeatures highPartnerFeatures;
    CFeatures lowOwnFeatures;
    CFeatures highOwnFeatures;
    CFeatures lowRHFeatures;
    CFeatures highRHFeatures;
    CFeatures lowLHFeatures;
    CFeatures highLHFeatures;
    Seat partnerSeat, ownSeat, rhSeat, lhSeat;

    void removeBid();
    void calculateRange(Seat seat, CFeatures &lowFeatures, CFeatures &highFeatures);
    void CalculateBidRuleRange(int inx, CFeatures &lowFeatures, CFeatures &highFeatures);
    bool isNT(int inx);
};

#endif // CBIDHISTORY_H
