/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidAndPlay.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid and play.

  Platforms: Qt.

*/

/**
 * @file Bid and play.
 * Handling of bids and play.
 */

#include "cbidoptiondoc.h"
#include "cbidandplay.h"

CBidAndPlay::CBidAndPlay()
{
}

/**
 * @brief Generate bid and play engines.
 * @param bidOptionDocOwn Options for own bidding.
 * @param bidOptionDocOpp Options for opponents bidding.
 */
void CBidAndPlay::generateEngines(CBidOptionDoc &bidOptionDocOwn, CBidOptionDoc &bidOptionDocOpp)
{
}

/**
 * @brief Based on bid engine get next bid.
 * @param seat The seat for the bidder.
 * @return The automatically calculated bid.
 */
Bids CBidAndPlay::getNextBid(Seat seat)
{
    if (bidHistory.bidList.isEmpty())
        return BID_1C;

    if (bidHistory.bidList.last().bid == BID_PASS)
        return BID_PASS;

    if (bidHistory.bidList.last().bid == BID_7NT)
        return BID_PASS;

    return (Bids)(bidHistory.bidList.last().bid + 1);
}

/**
 * @brief Based on play engine get next play.
 * @param player The player.
 * @param dummySeat Dummy.
 * @return The automatically calculated play.
 */
int CBidAndPlay::getNextPlay(Seat player, Seat dummySeat)
{
    int i;

    int *cards = (player == dummySeat) ? dummysCards : actorsCards;

    for (i = 0; i < 52; i++)
        if (playHistory.cardOk(i, player, cards))
            break;

    return i;
}
