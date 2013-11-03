#ifndef CPLAYHISTORY_H
#define CPLAYHISTORY_H

#include "Defines.h"

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
