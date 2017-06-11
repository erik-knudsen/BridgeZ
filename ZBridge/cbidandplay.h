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

#include <QCoreApplication>

#include "cbiddb.h"
#include "cbiddesc.h"
#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cfeatures.h"
#include "cbid.h"
#include "Defines.h"

class CBidAndPlayEngines;

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
    Q_DECLARE_TR_FUNCTIONS(CBidAndPlay)

public:
    CBidAndPlay();

    void setBidAndPlayEngines(CBidAndPlayEngines *bidAndPlayEngines)
    { this->bidAndPlayEngines = bidAndPlayEngines; }
    void setSeat(Seat seat);
    void setBidInfo(Bids bid, Bids bidDouble, Seat openLeader)
    { playHistory.setBidInfo(bid, bidDouble, openLeader); }
    void resetBidHistory();
    void appendBid(Seat bidder, Bids bid, Team vulnerable);
    int bidUndo(Bids *bid);
    CBid getNextBid(Seat seat, Team teamVul);
    void setActorsCards(int cards[13]);
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
    CBidHistory &getBidHistory() { return bidHistory; }
    CPlayHistory &getPlayHistory() { return playHistory; }
    QString featuresOfBid(CBid &bid);
    QString featuresOfLastBid();
    QString alertOfLastBid();

private:
    CBidHistory bidHistory;
    CPlayHistory playHistory;
    CBidAndPlayEngines *bidAndPlayEngines;
    CFeatures ownFeatures;
    Seat seat;

    int actorsCards[13];
    int dummysCards[13];
};

#endif //CBIDANDPLAY_H
