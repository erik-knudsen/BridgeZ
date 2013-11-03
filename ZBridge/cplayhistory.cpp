#include <cassert>

#include "cplayhistory.h"

CPlayHistory::CPlayHistory()
{
}

void CPlayHistory::resetPlayHistory()
{
    noTrick = 0;
    ewTricks = 0;
    nsTricks = 0;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            play[i][j] = -1;
}

void CPlayHistory::setBidInfo(Bids bid, Bids bidDouble, Seat openLeader)
{
    resetPlayHistory();

    this->bid = bid;
    trumpSuit = BID_SUIT(bid);
    level = BID_LEVEL(bid);

    this->bidDouble = bidDouble;
    this->openLeader = openLeader;
    currentLeader = openLeader;
}

void CPlayHistory::setPlay(Seat player, int trick, int cardVal)
{
    if ((play[WEST_SEAT][noTrick] != -1) && (play[NORTH_SEAT][noTrick] != -1) &&
            (play[EAST_SEAT][noTrick] != -1) && (play[SOUTH_SEAT][noTrick] != -1) &&
            (trick == (noTrick + 1)) && (trick < 13))
        noTrick++;

    if ((play[WEST_SEAT][noTrick] == -1) && (play[NORTH_SEAT][noTrick] == -1) &&
            (play[EAST_SEAT][noTrick] == -1) && (play[SOUTH_SEAT][noTrick] == -1) &&
            (player != currentLeader))
        return;

    if ((trick == noTrick) && play[player][trick] == -1)
        play[player][trick] = cardVal;
}

int CPlayHistory::getPlayed(Seat seat, int cards[])
{
    for (int i = 0; i <= noTrick; i++)
        cards[i] = play[seat][i];

    return (cards[noTrick] == -1) ? noTrick : noTrick + 1;
}

bool CPlayHistory::cardOk(int card, Seat seat, int cards[])
{
    int i, j;
    int noPlayed, noRemaining;
    int cardsPlayed[13];
    int cardsRemaining[13];

    //Leader can play whatever he wants.
    if (currentLeader == seat)
        return true;

    int leaderCard = play[currentLeader][noTrick];

    //Leader must be the first to play.
    if (leaderCard == -1)
        return false;

    Suit leaderSuit = CARD_SUIT(leaderCard);
    Suit playSuit = CARD_SUIT(card);
    if ( playSuit == leaderSuit)
        return true;

    noPlayed = getPlayed(seat, cardsPlayed);

    for (i = 0, j = 0; i < 13; i++)
    {
        int card = cards[i];
        bool found = false;
        for (int m = 0; m < noPlayed; m++)
            found = found || (card == cardsPlayed[m]);
        if (!found)
            cardsRemaining[j++] = card;
    }
    noRemaining = 13 - noPlayed;

    //Check consistency.
    if (j != noRemaining)
        return false;

    //Player must follow suit.
    for (i = 0; i < noRemaining; i++)
        if (leaderSuit == CARD_SUIT(cardsRemaining[i]))
            return false;

    return true;
}

Seat CPlayHistory::getNextLeader()
{
    Seat nextLeader;
    int faceNextLeader = -1;

    if (trumpSuit != NOTRUMP)
    {
        for (int player = 0; player < 4; player++)
        {
            int cardPlayer = play[player][noTrick];
            Suit suitPlayer = CARD_SUIT(cardPlayer);
            int facePlayer = CARD_FACE(cardPlayer);
            if ((suitPlayer == trumpSuit) && (facePlayer > faceNextLeader))
            {
                nextLeader = (Seat)player;
                faceNextLeader = facePlayer;
            }
        }
    }
    if (faceNextLeader == -1)
    {
        Suit suitNextLeader = CARD_SUIT(play[currentLeader][noTrick]);

        for (int player = 0; player < 4; player++)
        {
            int cardPlayer = play[player][noTrick];
            Suit suitPlayer = CARD_SUIT(cardPlayer);
            int facePlayer = CARD_FACE(cardPlayer);
            if ((suitPlayer == suitNextLeader) && (facePlayer > faceNextLeader))
            {
                nextLeader = (Seat)player;
                faceNextLeader = facePlayer;
            }
        }
    }

    if ((nextLeader == WEST_SEAT) || (nextLeader == EAST_SEAT))
        ewTricks++;
    else if ((nextLeader == NORTH_SEAT) || (nextLeader == SOUTH_SEAT))
        nsTricks++;

    currentLeader = nextLeader;

    return nextLeader;
}

int CPlayHistory::undo()
{
    return 0;
}
