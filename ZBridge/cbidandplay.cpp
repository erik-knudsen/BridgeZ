#include "cbidoptiondoc.h"
#include "cbidandplay.h"

CBidAndPlay::CBidAndPlay()
{
}

void CBidAndPlay::generateEngines(CBidOptionDoc &bidOptionDocOwn, CBidOptionDoc &bidOptionDocOpp)
{
}

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

int CBidAndPlay::getNextPlay(Seat player, Seat dummySeat)
{
    int i;

    int *cards = (player == dummySeat) ? dummysCards : actorsCards;

    for (i = 0; i < 52; i++)
        if (playHistory.cardOk(i, player, cards))
            break;

    return i;
}
