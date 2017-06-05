/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidHistory.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid history.

  Platforms: Qt.

*/

/**
 * \file
 * Bid history (definition).
 */

#include <cassert>

#include "cbidhistory.h"

CBidHistory::CBidHistory()
{
    ownSeat = partnerSeat = rhSeat = lhSeat = NO_SEAT;
}

/**
 * @brief Append a bid to the bid history.
 * @param bid The bid to append.
 */
void CBidHistory::appendBid(CBid &bid)
{
    bidList.append(bid);
    bidList.last().delRules = true;     //Delete non db rules on destroy.
    if (ownSeat != NO_SEAT)
    {
        int inx = bidList.size() - 1;

        if (bid.bidder == partnerSeat)
            CalculateBidRuleRange(inx, lowPartnerFeatures, highPartnerFeatures);
        else if (bid.bidder == ownSeat)
            CalculateBidRuleRange(inx, lowOwnFeatures, highOwnFeatures);
        else if (bid.bidder == rhSeat)
            CalculateBidRuleRange(inx, lowRHFeatures, highRHFeatures);
        else if (bid.bidder == lhSeat)
            CalculateBidRuleRange(inx, lowLHFeatures, highLHFeatures);
    }
}

/**
 * @brief Remove the last bid appended to the bid history.
 */
void CBidHistory::removeBid()
{
    if (!bidList.isEmpty())
        bidList.pop_back();
}

/**
 * @brief Reset (clear) the bid history.
 */
void CBidHistory::resetBidHistory()
{
    bidList.clear();
    if (ownSeat != NO_SEAT)
        setSeat(ownSeat);
}

/**
 * @brief Undo some of the bid history.
 * @param bid Last regular bidders bid (not double, redouble or pass) after one round (4) bids have been popped.
 * @return One less than number of bids given until (and including) last regular bidder or REBID if bid history gets reset.
 */
int CBidHistory::undo(Bids *bid)
{
    if (bidList.size() <= 4)
    {
        resetBidHistory();
        return REBID;
    }
    removeBid();
    removeBid();
    removeBid();
    removeBid();
    while (!bidList.empty() && ((bidList.last().bid == BID_PASS) || (bidList.last().bid == BID_DOUBLE) ||
                                (bidList.last().bid == BID_REDOUBLE)))
        removeBid();

    if (bidList.empty())
    {
        if (ownSeat != NO_SEAT)
            setSeat((ownSeat));

        return REBID;
    }

    if (ownSeat != NO_SEAT)
    {
        calculateRange(partnerSeat, lowPartnerFeatures, highPartnerFeatures);
        calculateRange(ownSeat, lowOwnFeatures, highOwnFeatures);
        calculateRange(rhSeat, lowRHFeatures, highRHFeatures);
        calculateRange(lhSeat, lowLHFeatures, highLHFeatures);
    }

    *bid = bidList.last().bid;
    return bidList.size() - 1;
}

bool CBidHistory::passedOut()
{
    return ((bidList.size() == 4) && (bidList[0].bid == BID_PASS) &&
                                     (bidList[1].bid == BID_PASS) &&
                                     (bidList[2].bid == BID_PASS) &&
            (bidList[3].bid == BID_PASS));
}

void CBidHistory::setSeat(Seat seat)
{
    partnerSeat = (Seat)((seat + 2) % 4);
    ownSeat = seat;
    rhSeat = (Seat)((seat + 3) % 4);
    lhSeat = (Seat)((seat + 1) % 4);

    highPartnerFeatures.setMaxFeatures();
    lowPartnerFeatures.setMinFeatures();
    highOwnFeatures.setMaxFeatures();
    lowOwnFeatures.setMinFeatures();
    highRHFeatures.setMaxFeatures();
    lowRHFeatures.setMinFeatures();
    highLHFeatures.setMaxFeatures();
    lowLHFeatures.setMinFeatures();
}

void CBidHistory::setFeatures(int cards[13])
{
    features.setCardFeatures(cards);
}

CFeatures &CBidHistory::getLowPartnerFeatures(Seat seat)
{
    if (seat == ownSeat)
        return lowPartnerFeatures;
    if (seat == partnerSeat)
        return lowOwnFeatures;
    if (seat == rhSeat)
        return lowLHFeatures;

    return lowRHFeatures;
}

CFeatures &CBidHistory::getHighPartnerFeatures(Seat seat)
{
    if (seat == ownSeat)
        return highPartnerFeatures;
    if (seat == partnerSeat)
        return highOwnFeatures;
    if (seat == rhSeat)
        return highLHFeatures;

    return highRHFeatures;
}

CFeatures &CBidHistory::getLowOwnFeatures(Seat seat)
{
    if (seat == ownSeat)
        return lowOwnFeatures;
    if (seat == partnerSeat)
        return lowPartnerFeatures;
    if (seat == rhSeat)
        return lowRHFeatures;

    return lowLHFeatures;
}

CFeatures &CBidHistory::getHighOwnFeatures(Seat seat)
{
    if (seat == ownSeat)
        return highOwnFeatures;
    if (seat == partnerSeat)
        return highPartnerFeatures;
    if (seat == rhSeat)
        return highRHFeatures;

    return highLHFeatures;
}

CFeatures &CBidHistory::getLowRHFeatures(Seat seat)
{
    if (seat == ownSeat)
        return lowRHFeatures;
    if (seat == partnerSeat)
        return lowLHFeatures;
    if (seat == rhSeat)
        return lowPartnerFeatures;

    return lowOwnFeatures;
}

CFeatures &CBidHistory::getHighRHFeatures(Seat seat)
{
    if (seat == ownSeat)
        return highRHFeatures;
    if (seat == partnerSeat)
        return highLHFeatures;
    if (seat == rhSeat)
        return highPartnerFeatures;

    return highOwnFeatures;
}

CFeatures &CBidHistory::getLowLHFeatures(Seat seat)
{
    if (seat == ownSeat)
        return lowLHFeatures;
    if (seat == partnerSeat)
        return lowRHFeatures;
    if (seat == rhSeat)
        return lowOwnFeatures;

    return lowPartnerFeatures;
}

CFeatures &CBidHistory::getHighLHFeatures(Seat seat)
{
    if (seat == ownSeat)
        return highLHFeatures;
    if (seat == partnerSeat)
        return highRHFeatures;
    if (seat == rhSeat)
        return highOwnFeatures;

    return highPartnerFeatures;
}

//Calculate range of feature attributes for the bid history of a given seat with rules for the bids.
void CBidHistory::calculateRange(Seat seat, CFeatures &lowFeatures, CFeatures &highFeatures)
{
    highFeatures.setMaxFeatures();
    lowFeatures.setMinFeatures();

    int size = bidList.size();

    int last;
    for (last = size - 1; last >= 0; last--)
        if (bidList[last].bidder == seat)
            break;
    if (last < 0)
        return;

    for (int i = last; i >= 0; i -= 4)
        CalculateBidRuleRange(i, lowFeatures, highFeatures);
}

//Calculate range of feature attributes for a given bid in the bid history.
void CBidHistory::CalculateBidRuleRange(int inx, CFeatures &lowFeatures, CFeatures &highFeatures)
{
    assert ((inx >= 0) && (inx < bidList.size()));

    //For the rules of a bid get the widest range for all features.
    CFeatures lowRuleFeatures;
    CFeatures highRuleFeatures;
    lowRuleFeatures.setMaxFeatures();
    highRuleFeatures.setMinFeatures();

    for (int j = 0; j < bidList[inx].rules.size(); j++)
    {
        CFeatures low;
        CFeatures high;
        bidList[inx].rules[j]->getFeatures(&low, &high);

        //Check for points.
        if ((low.getHcp(ANY) > 0) || (high.getHcp(ANY) < high.getMaxHcp(ANY)) &&
                (low.getPoints(NOTRUMP) == 0) && (high.getPoints(NOTRUMP) == high.getMaxPoints()))
        {
            low.setPoints(NOTRUMP, low.getHcp(ANY));
            high.setPoints(NOTRUMP, high.getHcp(ANY));
        }
        lowRuleFeatures.delimitFeatures(low, false);
        highRuleFeatures.delimitFeatures(high, true);
    }

    //Get the most narrow range.
    lowFeatures.delimitFeatures(lowRuleFeatures, true);
    highFeatures.delimitFeatures(highRuleFeatures, false);

    //Check for NT.
    if (isNT(inx))
    {
        lowFeatures.setDp(NOTRUMP, 0);
        highFeatures.setDp(NOTRUMP, 1);
    }
}

//Check if a given bid in the bid history is a NT bid?
bool CBidHistory::isNT(int inx)
{
    if ((bidList.size() <= inx) || (inx < 0) ||
        (BID_SUIT(bidList[inx].bid) != NOTRUMP))
        return false;

    CFeatures lowFeatures;
    CFeatures highFeatures;

    bidList[inx].rules[0]->getFeatures(&lowFeatures, &highFeatures);

    return !(((bidList[inx].bid == BID_1NT) && (highFeatures.getDp(NOTRUMP) == highFeatures.getMaxDp())) ||
             (bidList[inx].alert > 0));
}
