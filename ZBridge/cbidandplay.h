/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidAndPlay.h
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

#ifndef CBIDANDPLAY_H
#define CBIDANDPLAY_H

#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cbidengine.h"
#include "cplayengine.h"
#include "Defines.h"

class CBidOptionDoc;

/**
 * @brief Bid and play handling.
 *
 * The class handles bid and play for one actor i.e. each actor has a bid and play object.
 *  - It generates initally engines for automatic bidding and for automatic playing, and uses these engines
 *    for bidding and playing.
 *  - It stores the bid history and the play history.
 *  - It also has the actors cards and the dummys cards.
 *  - It determines the next leader after a trick has been played.
 *  - It supports undo of bid and undo of play.
 */
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
    int playUndo(int val) { return playHistory.undo(val); }
    int getNoTrick() { return playHistory.getNoTrick(); }
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
