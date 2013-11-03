#ifndef CBIDANDPLAY_H
#define CBIDANDPLAY_H

#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cbidengine.h"
#include "cplayengine.h"
#include "Defines.h"

class CBidOptionDoc;

class CBidAndPlay
{
public:
    CBidAndPlay();

    void generateEngines(CBidOptionDoc &bidOptionDocOwn, CBidOptionDoc &bidOptionDocOpp);

    void setBidInfo(Bids bid, Bids bidDouble, Seat openLeader)
    { playHistory.setBidInfo(bid, bidDouble, openLeader); }
    void resetBidHistory() { bidHistory.resetBidHistory(); }
    void appendBid(CBid &bid) { bidHistory.appendBid(bid); }
    int bidUndo(Bids *bid) { return bidHistory.undo(bid); }
    Bids getNextBid(Seat seat);

    void setActorsCards(int cards[13])
    { for (int i = 0; i < 13; i++) actorsCards[i] = cards[i]; }
    void setDummysCards(int cards[13])
    { for (int i = 0; i < 13; i++) dummysCards[i] = cards[i]; }
    bool cardOk(int card, Seat player, bool dummy)
    { return playHistory.cardOk(card, player, (dummy) ? dummysCards : actorsCards); }
    Seat getNextLeader() { return playHistory.getNextLeader(); }
    int getEWTricks() { return playHistory.getEWTricks(); }
    int getNSTricks() { return playHistory.getNSTricks(); }
    void setPlay(Seat player, int trick, int cardVal) { playHistory.setPlay(player, trick, cardVal); }
    void resetPlayHistory() { playHistory.resetPlayHistory(); }
    int playUndo() { return playHistory.undo(); }
    int getNextPlay(Seat player, Seat dummySeat);

private:
    CBidHistory bidHistory;
    CPlayHistory playHistory;
    CBidEngine nsBidEngine;
    CBidEngine ewBidEngine;
    CPlayEngine nsPlayEngine;
    CPlayEngine ewPlayEngine;

    int actorsCards[13];
    int dummysCards[13];
};

#endif // CHANDLEBID_H
