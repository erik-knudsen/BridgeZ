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
 * @param nsBbidOptionDoc Options for NS bidding.
 * @param ewBidOptionDoc Options for EW bidding.
 * @param bidDB The bid database.
 * @param bidDesc Description of the bid database.
 */
void CBidAndPlay::generateEngines(Seat seat, CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc,
                                  CBidDB &bidDB, CBidDesc &bidDesc)
{
    this->seat = seat;
}

/**
 * @brief Reset bid history.
 */
void CBidAndPlay::resetBidHistory()
{
    for (int i = 0; i < 3; i++)
    {
        features[i][0].setMinFeatures();
        features[i][1].setMaxFeatures();
    }

    bidHistory.resetBidHistory();
}

/**
 * @brief Append bid to bid history.
 * @param bid The bid to append.
 */
void CBidAndPlay::appendBid(CBid &bid)
{
    bidHistory.appendBid(bid);
}

/**
* @brief Undo some of the bid history.
* @param bid Last regular bidders bid (not double, redouble or pass) after one round (4) bids have been popped.
* @return One less than number of bids given until (and including) last regular bidder or REBID if bid history gets reset.
* */
int CBidAndPlay::bidUndo(Bids *bid)
{
    return bidHistory.undo(bid);
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

    if (bidHistory.bidList.last().bid == BID_DOUBLE)
    return BID_PASS;
//        return (Bids)(bidHistory.bidList[bidHistory.bidList.size() - 2].bid + 1);

//    if (bidHistory.bidList.last().bid == BID_REDOUBLE)
//        return BID_PASS;
//        return (Bids)(bidHistory.bidList[bidHistory.bidList.size() - 3].bid + 1);

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
