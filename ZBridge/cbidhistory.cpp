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
 * @param bid The bid.
 * @return Next bidder.
 */
int CBidHistory::undo(Bids *bid)
{
    return WEST_SEAT;
}
