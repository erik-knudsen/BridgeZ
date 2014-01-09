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
    bool cardOk(int card, Seat seat, int cards[13]);
    int getEWTricks() { return ewTricks; }
    int getNSTricks() { return nsTricks; }

    Seat getNextLeader();
    int undo();

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
    int play[4][13];
};

#endif // CPLAYHISTORY_H
