/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CPlayHistory.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Play history.

  Platforms: Qt.

*/

/**
 * \file
 * Play History (declaration).
 */

#ifndef CPLAYHISTORY_H
#define CPLAYHISTORY_H

#include "Defines.h"

/**
 * @brief The CPlayHistory class describes the play history.
 */
class CPlayHistory
{
public:
    CPlayHistory();

    void resetPlayHistory();
    void setBidInfo(Bids bid, Bids bidDouble, Seat openLeader);
    void setPlay(Seat player, int trick, int cardVal);
    int getPlayed(Seat seat, int cards[13]);
    int getPlayed(int cards[52]);
    bool cardOk(int card, Seat seat, int cards[13]);
    int getEWTricks() { return ewTricks; }
    int getNSTricks() { return nsTricks; }
    bool isFirstPlay();
    bool isFirstTimeSuitPlayed(Suit suit);
    bool isCardPlayed(Seat seat, int card);

    Seat getNextLeader();
    Seat getNextLeader(int trickNo);
    Seat getLeader();
    bool takeTrick(int card);
    int getCard(Seat seat);
    int getCard(Seat seat, int trickNo);
    int undo(int undoType);
    int getNoTrick() { return noTrick; }
    Seat getOpenLeader() { return openLeader; }
    Seat getCurrentLeader() { return currentLeader; }
    Seat getDeclarer() { return (Seat)((openLeader + 3) % 4); }
    Bids getContract() { return bid; }
    Bids getContractModifier() { return bidDouble; }
    int getResult();
    void getTrick(int trickNo, int trick[]);
    void getTrickInfo(int trickNo, int &ewTricks, int &nsTricks, Seat &nextLeader);
    void getNoPlayed(Seat seat, Suit suit, int *noOwn, int *noOpp);

private:
    int noTrick;
    int ewTricks;
    int nsTricks;
    Seat openLeader;
    Seat currentLeader;
    Bids bid;
    Suit trumpSuit;
    int level;
    Bids bidDouble;
    struct playStack
    {
        int ewTricks;
        int nsTricks;
        Seat nextLeader;
    } playStack[13];

    int play[4][13];
};

#endif // CPLAYHISTORY_H
