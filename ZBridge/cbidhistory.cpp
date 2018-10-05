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
    seat = NO_SEAT;
}

void CBidHistory::setSeat(Seat seat)
{
    this->seat = seat;
    if (seat != NO_SEAT)
        initializeFeatures();
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

    if (seat != NO_SEAT)
    {
//        bidList.last().delRules = true;     //Delete non db rules on destroy.
        CalculateBidRuleRange((bidList.size() - 1), lowFeatures[bid.bidder], highFeatures[bid.bidder]);
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

    if (seat != NO_SEAT)
        initializeFeatures();
}

Seat CBidHistory::getDeclarer()
{
    int size = bidList.size();

    //Find last proper bid;
    int i;
    for (i = size - 1; i >= 0; i--)
        if (IS_BID(bidList[i].bid))
            break;
    if (i < 0)
        return NO_SEAT;

    Suit suit = BID_SUIT(bidList[i].bid);

    int first = i % 2;
    int j;
    for (j = first; j <= i; j +=2)
        if (suit == BID_SUIT(bidList[j].bid))
            break;

    return (bidList[j].bidder);
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
        if (seat != NO_SEAT)
            initializeFeatures();

        return REBID;
    }

    if (seat != NO_SEAT)
        for (int i = 0; i < 4; i++)
            calculateRange((Seat)i, lowFeatures[i], highFeatures[i]);

    *bid = bidList.last().bid;

    return (bidList.size() - 1);
}

bool CBidHistory::passedOut()
{
    return ((bidList.size() == 4) && (bidList[0].bid == BID_PASS) &&
                                     (bidList[1].bid == BID_PASS) &&
                                     (bidList[2].bid == BID_PASS) &&
            (bidList[3].bid == BID_PASS));
}

bool CBidHistory::checkCardFeatures(int cards[], Seat seat)
{
    CFeatures features;
    features.setCardFeatures(cards);

//    int res = features.featureIsOk(getHighFeatures(seat), getLowFeatures(seat));

    int size = bidList.size();
    int res = 0;

    for (int i = 0; (i < size) && (bidList[i].bidder == seat); i++)
    {
        for (int j = 0; j < bidList[i].rules.size(); j++)
        {
            CFeatures low;
            CFeatures high;
            bidList[i].rules[j]->getFeatures(&low, &high);
            res = features.featureIsOk(high, low);
            if (res == 0)
                break;
        }
        if (res != 0)
            break;
    }

    return (res == 0);
}

/**
 * @brief Set the bidders seat.
 *
 * Initialization of public feature limits for all players.
 *
 * @param seat
 */
void CBidHistory::initializeFeatures()
{
    for (int i = 0; i <4; i++)
    {
        highFeatures[i].setMaxFeatures();
        lowFeatures[i].setMinFeatures();
    }
}

/**
 * @brief Calculate public range of feature attributes for the bid history of a given seat.
 *
 * @param[in] seat The seat to calculate public known range of features for.
 * @param[out] lowFeatures Low limit of features.
 * @param[out] highFeatures High limit of features.
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
 * First the rules of the bid are combined to get the widest range of feature limits ("or"
 * combination). Next these feature limits are combined with previous bids to get the
 * most narrow limits ("and" combination). The use of this only works well if one of the
 * combined bids has the most narrow "or" range for a given feature. The bid database
 * handles (sufficiently - to be seen though) this. Example could be Stayman after 1NT.
 * This shows either 4+ cards in either hearts or spades or both. But which is revealed
 * in later bidding. [Distribution of cards in the play phase solved by double dummy play
 * to get the best next card to play does also use these combined features (to be seen
 * if this is sufficient)].\n
 *
 * @param[in] inx Index of the bid in the bid history.
 * @param[in,out] lowFeatures Low limit of features.
 * @param[in,out] highFeatures High limit of features.
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

        lowRuleFeatures.delimitFeatures(low, false);
        highRuleFeatures.delimitFeatures(high, true);
    }

    //Get the most narrow range.
    lowFeatures.delimitFeatures(lowRuleFeatures, true);
    highFeatures.delimitFeatures(highRuleFeatures, false);
}
