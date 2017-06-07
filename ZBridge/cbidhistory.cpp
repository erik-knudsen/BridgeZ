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
 *
 * A bid is appended to the bid history. This includes not only the bid but also the
 * public known feature limits, which has been used for selecting the bid.
 *
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
 *
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

/**
 * @brief Set the bidders seat.
 *
 * The bidders seat is set. This includes initialization of public feature limits for all players.
 *
 * @param seat
 */
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

/**
 * @brief Set features for the bidder.
 *
 * The features are calculated based on the bidders cards. These are ofcourse well known to the bidder.
 *
 * @param cards
 */
void CBidHistory::setFeatures(int cards[13])
{
    features.setCardFeatures(cards);
}

/**
 * @brief Get public known low limits of partner features.
 *
 * @param seat The seat for which to get the public known low limit partner features.
 * @return Returns public known low limit partner features.
 */
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

/**
 * @brief Get public known high limits of partner features.
 *
 * @param seat The seat for which to get the public known high limit partner features.
 * @return Returns public known high limit partner features.
 */
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

/**
 * @brief Get public known low limits of own features.
 *
 * @param seat The seat for which to get the public known low limit own features.
 * @return Returns public known low limit own features.
 */
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

/**
 * @brief Get public known high limits of own features.
 *
 * @param seat The seat for which to get the public known high limit own features.
 * @return Returns public known high limit own features.
 */
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

/**
 * @brief Get public known low limits of right hand players features.
 *
 * @param seat The seat for which to get the public known low limit right hand players features.
 * @return Returns public known low limit right hand players features.
 */
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

/**
 * @brief Get public known high limits of right hand players features.
 *
 * @param seat The seat for which to get the public known high limit right hand players features.
 * @return Returns public known high limit right hand players features.
 */
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

/**
 * @brief Get public known low limits of left hand players features.
 *
 * @param seat The seat for which to get the public known low limit left hand players features.
 * @return Returns public known low limit left hand players features.
 */
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

/**
 * @brief Get public known high limits of left hand players features.
 *
 * @param seat The seat for which to get the public known high limit left hand players features.
 * @return Returns public known high limit left hand players features.
 */
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

/**
 * @brief Calculate public range of feature attributes for the bid history of a given seat.
 *
 * @param seat[in] The seat to calculate public known range of features for.
 * @param lowFeatures[out] Low limit of features.
 * @param highFeatures[out] High limit of features.
 */
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

/**
 * @brief Calculate range of feature attributes for a given bid in the bid history.
 *
 * Assumes previous bids in the bid history has been calculated.\n
 * First the rules of the bid are combined to get the widest range of feature limits. Next
 * these feature limits are combined with previous bids to get the most narrow limits.\n
 * The bid database is in some cases corrected for HCP/Points. And it is marked whether
 * the bid potentially can be a NT bid.
 *
 * @param inx[in] Index of the bid in the bid history.
 * @param[in][out] lowFeatures Low limit of features.
 * @param[in][out] highFeatures High limit of features.
 */
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
