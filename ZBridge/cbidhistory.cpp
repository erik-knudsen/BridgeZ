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

#include "cbidhistory.h"

CBidHistory::CBidHistory()
{
}

/*CBidHistory& CBidHistory::operator= (const CBidHistory &bidHistory)
{
    bidList.clear();

    for (int i = 0; i < bidHistory.bidList.size(); i++)
        bidList.append(bidHistory.bidList[i]);

    return *this;
}*/

/**
 * @brief Append a bid to the bid history.
 * @param bid The bid to append.
 */
void CBidHistory::appendBid(CBid &bid)
{
    bidList.append(bid);
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
        return REBID;

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
