/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CPlayHistory.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Play history.

  Platforms: Qt.

*/

/**
 * \file
 * Play History (definition).
 */

#include <cassert>

#include "cplayhistory.h"

CPlayHistory::CPlayHistory()
{
    openLeader = NO_SEAT;
    resetPlayHistory();
}

void CPlayHistory::resetPlayHistory()
{
    noTrick = 0;
    ewTricks = 0;
    nsTricks = 0;

    bid = BID_NONE;
    bidDouble = BID_NONE;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            play[i][j] = -1;

    currentLeader = openLeader;
}

/**
 * @brief Set bid info for the play.
 * @param bid The contract.
 * @param bidDouble Double/redouble?
 * @param openLeader The open leader.
 */
void CPlayHistory::setBidInfo(Bids bid, Bids bidDouble, Seat openLeader)
{
    this->openLeader = openLeader;

    resetPlayHistory();

    this->bid = bid;
    trumpSuit = BID_SUIT(bid);
    level = BID_LEVEL(bid);
    this->bidDouble = bidDouble;

}

/**
 * @brief Set the next played card in the history.
 * @param player The player.
 * @param trick The trick number.
 * @param cardVal The card played.
 */
void CPlayHistory::setPlay(Seat player, int trick, int cardVal)
{
    assert ((trick >= 0) && (trick < 13));
    assert ((trick == noTrick) && (play[player][trick] == -1));
    assert (!((play[WEST_SEAT][noTrick] == -1) && (play[NORTH_SEAT][noTrick] == -1) &&
                    (play[EAST_SEAT][noTrick] == -1) && (play[SOUTH_SEAT][noTrick] == -1) &&
                    (player != currentLeader)));

    play[player][trick] = cardVal;
}

/**
 * @brief Get which cards has been played until now by this seat.
 * @param seat The seat.
 * @param cards Cards played.
 * @return Number of tricks so far.
 */
int CPlayHistory::getPlayed(Seat seat, int cards[])
{
    int noCards = 0;

    for (int i = 0; i < 13; i++)
    if (play[seat][i] != -1)
    {
        cards[noCards] = play[seat][i];
        noCards++;
    }
    return noCards;
}

/**
 * @brief Get which cards have been played until now.
 * @param cards Cards played.
 * @return Number of cards played.
 */
int CPlayHistory::getPlayed(int cards[])
{
    int noCards = 0;

    for (int j = 0; j < 4; j++)
    for (int i = 0; i < 13; i++)
    if (play[j][i] != -1)
    {
        cards[noCards] = play[j][i];
        noCards++;
    }
    return noCards;
}

/**
 * @brief Check if the played card is ok.
 * @param card The played card.
 * @param seat The players seat.
 * @param cards The cards.
 * @return true if ok else false.
 */
bool CPlayHistory::cardOk(int card, Seat seat, int cards[])
{
    int i, j;
    int noPlayed, noRemaining;
    int cardsPlayed[13];
    int cardsRemaining[13];

    //Check card is allowed.
    for (i = 0; i < 13; i++)
        if (card == cards[i])
            break;
    if (i == 13)
        return false;

    noPlayed = getPlayed(seat, cardsPlayed);
    for (i = 0; i < noPlayed; i++)
        if (card == cardsPlayed[i])
            break;
    if (i < noPlayed)
        return false;

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

bool CPlayHistory::isFirstPlay()
{
    return ((play[0][0] == -1) && (play[1][0] == -1) && (play[2][0] == -1) && (play[3][0] == -1));
}

bool CPlayHistory::isFirstTimeSuitPlayed(Suit suit)
{
    for (int i = 0; i < noTrick; i++)
    for (int j = 0; j < 4; j++)
    if (CARD_SUIT(play[j][i]) == suit)
        return false;

    return true;
}

bool CPlayHistory::isCardPlayed(Seat seat, int card)
{
    for (int i = 0; i < 13; i++)
        if (card == play[seat][i])
            return true;
    return false;
}

/**
 * @brief Determine the next leader and update play stack.
 * @return The next leader.
 */
Seat CPlayHistory::getNextLeader()
{
    Seat nextLeader = NO_SEAT;
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

    playStack[noTrick].ewTricks = ewTricks;
    playStack[noTrick].nsTricks = nsTricks;
    playStack[noTrick].nextLeader = nextLeader;
    noTrick++;

    return nextLeader;
}

Seat CPlayHistory::getNextLeader(int trickNo)
{
    if (trickNo == 0)
        return openLeader;

    return playStack[trickNo - 1].nextLeader;
}

/**
 * @brief Determine leader till now (not all card has been played).
 * @return The leader untill now.
 */
Seat CPlayHistory::getLeader()
{
    Seat leader = NO_SEAT;
    int faceLeader = -1;

    if (trumpSuit != NOTRUMP)
    {
        for (int player = 0; player < 4; player++)
        if (play[player][noTrick] != -1)
        {
            int cardPlayer = play[player][noTrick];
            Suit suitPlayer = CARD_SUIT(cardPlayer);
            int facePlayer = CARD_FACE(cardPlayer);
            if ((suitPlayer == trumpSuit) && (facePlayer > faceLeader))
            {
                leader = (Seat)player;
                faceLeader = facePlayer;
            }
        }
    }
    if (faceLeader == -1)
    {
        if (play[currentLeader][noTrick] == -1)
            return currentLeader;

        Suit suitLeader = CARD_SUIT(play[currentLeader][noTrick]);

        for (int player = 0; player < 4; player++)
        if (play[player][noTrick] != -1)
        {
            int cardPlayer = play[player][noTrick];
            Suit suitPlayer = CARD_SUIT(cardPlayer);
            int facePlayer = CARD_FACE(cardPlayer);
            if ((suitPlayer == suitLeader) && (facePlayer > faceLeader))
            {
                leader = (Seat)player;
                faceLeader = facePlayer;
            }
        }
    }
    return leader;
}

/**
 * @brief Can the card given take the next trick?
 * @param card The card in question.
 * @return true if the card can take the trick, false otherwise.
 */
bool CPlayHistory::takeTrick(int card)
{
    int faceNextLeader = -1;

    Suit suitCard = CARD_SUIT(card);
    int faceCard = CARD_FACE(card);

    if (trumpSuit != NOTRUMP)
    {
        for (int player = 0; player < 4; player++)
        if (play[player][noTrick] != -1)
        {
            int cardPlayer = play[player][noTrick];
            Suit suitPlayer = CARD_SUIT(cardPlayer);
            int facePlayer = CARD_FACE(cardPlayer);
            if ((suitPlayer == trumpSuit) && (facePlayer > faceNextLeader))
                faceNextLeader = facePlayer;
        }
        if ((suitCard == trumpSuit) && (faceCard > faceNextLeader))
            return true;
    }
    if (faceNextLeader == -1)
    {
        if (play[currentLeader][noTrick] == -1)
            return true;

        Suit suitNextLeader = CARD_SUIT(play[currentLeader][noTrick]);

        for (int player = 0; player < 4; player++)
        if (play[player][noTrick] != -1)
        {
            int cardPlayer = play[player][noTrick];
            Suit suitPlayer = CARD_SUIT(cardPlayer);
            int facePlayer = CARD_FACE(cardPlayer);
            if ((suitPlayer == suitNextLeader) && (facePlayer > faceNextLeader))
                faceNextLeader = facePlayer;
        }
        if ((suitCard == suitNextLeader) && (faceCard > faceNextLeader))
            return true;
    }

    return false;
}

/**
 * @brief Get played card in this trick.
 * @return The card played.
 */
int CPlayHistory::getCard(Seat seat)
{
    return play[seat][noTrick];
}

int CPlayHistory::getCard(Seat seat, int trickNo)
{
    return play[seat][trickNo];
}

/**
 * @brief Undo trick.
 * @param undoType PT: undo partial trick. CT: undo complete(finished) trick.
 * @return REPLAY: start play from beginning otherwise the leader from the top of the play stack.
 */
int CPlayHistory::undo(int undoType)
{
    assert (((undoType == PT) && ((noTrick >= 0) && (noTrick < 13)))||
            ((undoType == CT) && ((noTrick > 0) && (noTrick <= 13))));

    if (undoType == CT)
    {
        noTrick--;
        if (noTrick > 0)
        {
            nsTricks = playStack[noTrick - 1].nsTricks;
            ewTricks = playStack[noTrick - 1].ewTricks;
            currentLeader = playStack[noTrick - 1].nextLeader;
        }
        else
        {
            nsTricks = ewTricks = 0;
            currentLeader = openLeader;
        }
    }

    play[WEST_SEAT][noTrick] = play[NORTH_SEAT][noTrick] = play[EAST_SEAT][noTrick] = play[SOUTH_SEAT][noTrick] = -1;

    return (noTrick == 0) ? (REPLAY) : (playStack[noTrick - 1].nextLeader);
}

int CPlayHistory::getResult()
{
    if (noTrick != 13)
        return -1;

    Seat declarer = (Seat)((openLeader + 3) % 4);
    if ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))
        return ewTricks;

    return nsTricks;
}

void CPlayHistory::getTrick(int trickNo, int trick[])
{
    assert ((trickNo >= 0) && (trickNo <= 12));

    trick[WEST_SEAT] = play[WEST_SEAT][trickNo];
    trick[NORTH_SEAT] = play[NORTH_SEAT][trickNo];
    trick[EAST_SEAT] = play[EAST_SEAT][trickNo];
    trick[SOUTH_SEAT] = play[SOUTH_SEAT][trickNo];
}

void CPlayHistory::getTrickInfo(int trickNo, int &ewTricks, int &nsTricks, Seat &nextLeader)
{
    assert ((trickNo >= 0) && (trickNo <= 12));

    ewTricks = playStack[trickNo].ewTricks;
    nsTricks = playStack[trickNo].nsTricks;
    nextLeader = playStack[trickNo].nextLeader;
}

void CPlayHistory::getNoPlayed(Seat seat, Suit suit, int *noOwn, int *noOpp)
{
    *noOwn = *noOpp = 0;
    for (int i = 0; i < 13; i++)
    {
        if (CARD_SUIT(play[seat][i]) == suit)
            (*noOwn)++;
        if (CARD_SUIT(play[(seat + 1) & 3][i]) == suit)
            (*noOpp)++;
        if (CARD_SUIT(play[(seat + 2) & 3][i]) == suit)
            (*noOwn)++;
        if (CARD_SUIT(play[(seat + 3) & 3][i]) == suit)
            (*noOpp)++;
    }
}
