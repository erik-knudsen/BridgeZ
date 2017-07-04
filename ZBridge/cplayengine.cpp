/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CPlayEngine.cpp
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Play engine.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the play engine.
 */

#include <QDebug>
#include <cassert>

#include "dll.h"
#include "mt19937ar.h"
#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cplayengine.h"

const int NO_HANDS_DD = 10;
const int MAX_ITER = 1000;

CPlayEngine::CPlayEngine()
{
}

/**
 * @brief Determine the next card to play.
 *
 * @param seat The players seat (dummys cards are played by declarer).
 * @param dummySeat The dummys seat.
 * @param ownCards The players cards.
 * @param dummyCards The dummys cards.
 * @param bidHistory The bid history.
 * @param playHistory The play history.
 * @return The next card to play.
 */
int CPlayEngine::getNextPlay(Seat seat, Seat dummySeat, int ownCards[], int dummyCards[],
                             CBidHistory &bidHistory, CPlayHistory &playHistory)
{
    //Determine remaining known cards (own and dummy).
    bool firstPlay = playHistory.isFirstPlay();
    int ownRemaining[13];
    int dummyRemaining[13];
    int j = 0;
    Seat ownSeat = (Seat)(!firstPlay && ((seat == dummySeat)) ? ((seat + 2) % 4) : (seat));
    for (int i = 0; i < 13; i++)
        ownRemaining[j++] = !playHistory.isCardPlayed(ownSeat, ownCards[i]) ? (ownCards[i]) : (-1);
    if (!firstPlay)
    {
        j = 0;
        for (int i = 0; i < 13; i++)
            dummyRemaining[j++] = !playHistory.isCardPlayed(dummySeat, dummyCards[i]) ? (dummyCards[i]) : (-1);
    }

    //Calculate a number of possible hands and double dummy solutions to these hands.
    futureTricks fut[NO_HANDS_DD];
    int hands[NO_HANDS_DD][4][13];
    int handNo = 0;
    int iter = 0;
    int maxFailures = 0;
    while (handNo < NO_HANDS_DD)
    {
        //Initialize cards.
        int cardValues[52];
        for (int i = 0; i < 52; i++)
            cardValues[i] = i;

        //Own cards are always known, therefore remove own cards.
        for (int i = 0; i < 13; i++)
            cardValues[ownCards[i]] = -1;

        //Dummys cards are known except for the first play, therefore remove dummy cards in other cases.
        if (!firstPlay)
            for (int i = 0; i < 13; i++)
                cardValues[dummyCards[i]] = -1;

        //Also remove cards that have already been played.
        int cards[52];
        int noCards = playHistory.getPlayed(cards);
        for (int i = 0; i < noCards; i++)
            cardValues[cards[i]] = -1;

        //Sort cards, so that remaining (still not played and unknown) cards are first.
        noCards = 0;
        for (int i = 0; i < 52; i++)
            if (cardValues[i] != -1)
                cardValues[noCards++] = cardValues[i];
        for (int i = noCards; i < 52; i++)
            cardValues[i] = -1;

        //Deal unknown cards randomly.
        for (int i = noCards - 1; i >= 0; i--)
        {
            int inx = genrand_int32() % (i + 1);
            int card = cardValues[inx];
            for (int j = inx; j < i; j++)
                cardValues[j] = cardValues[j + 1];
            cardValues[i] = card;
        }

        //In the hand to play own cards are always known (declarer plays dummys cards).
        for (int i = 0; i < 13; i++)
            hands[handNo][ownSeat][i] = ownRemaining[i];

        //In the hand to play dummy cards are known except for the first play.
        if (!firstPlay)
            for (int i = 0; i < 13; i++)
                hands[handNo][dummySeat][i] = dummyRemaining[i];

        //Distribute properly remaining cards to the hand to play.
        Seat seat_0 = seat;
        Seat seat_1 = (Seat)((seat + 1) % 4);
        Seat seat_2 = (Seat)((seat + 2) % 4);
        Seat seat_3 = (Seat)((seat + 3) % 4);
        int j = 0;
        for (int i = 0; i < 13; i++)
        {
            if ((seat_0 !=dummySeat) && (seat_0 != ownSeat) || (firstPlay && (seat_0 == dummySeat)))
                hands[handNo][seat_0][i] = cardValues[j++];
            if ((seat_1 !=dummySeat) && (seat_1 != ownSeat) || (firstPlay && (seat_1 == dummySeat)))
                hands[handNo][seat_1][i] = cardValues[j++];
            if ((seat_2 !=dummySeat) && (seat_2 != ownSeat) || (firstPlay && (seat_2 == dummySeat)))
                hands[handNo][seat_2][i] = cardValues[j++];
            if ((seat_3 !=dummySeat) && (seat_3 != ownSeat) || (firstPlay && (seat_3 == dummySeat)))
                hands[handNo][seat_3][i] = cardValues[j++];
        }

        //Check features.
        int noFailures = 0;
        int i;
        for (i = 0; i < 4; i++)
        {
            if (((i != dummySeat) && (i != ownSeat)) || (firstPlay && (i == dummySeat)))
            {
                int cards[13];
                int newNo;
                for (newNo = 0; (newNo < 13) && (hands[handNo][i][newNo] != -1); newNo++)
                    cards[newNo] = hands[handNo][i][newNo];

                if (newNo < 13)
                {
                    int played[13];
                    int  noPlayed = playHistory.getPlayed((Seat)i, played);

                    assert((noPlayed + newNo) == 13);

                    for (int k = 0; k < noPlayed; k++)
                        cards[newNo++] = played[k];
                }
                CFeatures features;
                features.setCardFeatures(cards);
                int res = features.featureIsOk(bidHistory.getHighFeatures((Seat)i), bidHistory.getLowFeatures((Seat)i));
                if (res != 0)
                    noFailures++;
                if ((res != 0) && (noFailures > maxFailures))
                    break;
            }
        }

        iter++;
        if (( i < 4) && (iter >= MAX_ITER))
        {
            maxFailures++;
            iter = 0;
        }

        //Hand ok so far?
        else if (i == 4)
        {
            //Prepare for double dummy solver.
            deal dl;
            int target = -1;        //Find the maximum number of tricks.
            int solutions = 2;      //Return all optimum cards and scores.
            int mode = 0;           //Do not search to find score if only one card can be played.
            int threadIndex = 0;    //For parallel execution (not here).
            int res;

            //Get trump suit for the hand.
            Suit suit = BID_SUIT(playHistory.getContract());
            dl.trump = (suit == NOTRUMP) ? (NOTRUMP) : (3 - suit);      //Double dummy solver format.

            //Get current leader.
            Seat currentLeader = playHistory.getCurrentLeader();
            dl.first = (currentLeader + 3) % 4;                         //Double dummy solver format.

            //In the current trick, 3 cards can be played.
            seat_0 = currentLeader;
            seat_1 = (Seat)((currentLeader + 1) % 4);
            seat_2 = (Seat)((currentLeader + 2) % 4);
            int trick[4];
            playHistory.getTrick(playHistory.getNoTrick(), trick);
            dl.currentTrickSuit[0] = (trick[seat_0] != -1) ? (3 - CARD_SUIT(trick[seat_0])) : (0);   //Double dummy solver format.
            dl.currentTrickSuit[1] = (trick[seat_1] != -1) ? (3 - CARD_SUIT(trick[seat_1])) : (0);   //Double dummy solver format.
            dl.currentTrickSuit[2] = (trick[seat_2] != -1) ? (3 - CARD_SUIT(trick[seat_2])) : (0);   //Double dummy solver format.                                                                                                                                                          dl.currentTrickRank[0] = (trick[seat_0] != -1) ? (CARD_FACE(trick[seat_0]) + 2) : (0);
            dl.currentTrickRank[0] = (trick[seat_0] != -1) ? (CARD_FACE(trick[seat_0]) + 2) : (0);   //Double dummy solver format.
            dl.currentTrickRank[1] = (trick[seat_1] != -1) ? (CARD_FACE(trick[seat_1]) + 2) : (0);   //Double dummy solver format.
            dl.currentTrickRank[2] = (trick[seat_2] != -1) ? (CARD_FACE(trick[seat_2]) + 2) : (0);   //Double dummy solver format.

            //Remaining cards.
            for (int i = 0; i < 4; i++)
                for (j = 0; j < 4; j++)
                    dl.remainCards[i][j] = 0;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 13; j++)
                    if (hands[handNo][i][j] != -1)
                    {
                        int hand = (i + 3) % 4;
                        int card = hands[handNo][i][j];
                        Suit suit = CARD_SUIT(card);
                        int face = CARD_FACE(card);
                        dl.remainCards[hand][3 - suit] |= (1 << (face + 2));            //Double dummy solver format.
                    }

            //Double dummy solver.
            res = SolveBoard(dl, target, solutions, mode, &fut[handNo], threadIndex);

            //Next hand.
            handNo++; iter = 0; maxFailures = 0;
        }
    }

    //Get the best to play (highest sum of scores).
    int cards[52];
    for (int i = 0; i < 52; i++)
        cards[i] = 0;

    for (int i = 0; i < NO_HANDS_DD; i++)
    for (int j = 0; j < fut[i].cards; j++)
    {
        Suit suit = (Suit)(3 - fut[i].suit[j]);
        int face = fut[i].rank[j] - 2;
        int card = MAKE_CARD(suit, face);
        cards[card] += fut[i].score[j];
    }

    int card = -1;
    int max = 0;
    for (int i = 0; i < 52; i++)
        if (cards[i] > max)
        {
            card  = i;
            max = cards[i];
        }

    //If none is found, just take one that is allowable.
    if (card == -1)
    {
        int *crds = (seat == dummySeat) ? dummyCards : ownCards;
        int i;
        for (i = 0; i < 13; i++)
            if (playHistory.cardOk(crds[i], seat, crds))
                    break;

        assert(i < 13);

        card = crds[i];
    }
    else
    {
        //Get trump suit for the hand.
        Suit suit = BID_SUIT(playHistory.getContract());

        //Get current leader.
        Seat currentLeader = playHistory.getCurrentLeader();
        Seat declarer = playHistory.getDeclarer();

        //In the current trick, 3 cards can be played.
        Seat seat_0 = currentLeader;
        Seat seat_1 = (Seat)((currentLeader + 1) % 4);
        Seat seat_2 = (Seat)((currentLeader + 2) % 4);
        int trick[4];
        playHistory.getTrick(playHistory.getNoTrick(), trick);
        if (((declarer == currentLeader) || (((declarer  + 2) & 3) == currentLeader)) && (suit != NOTRUMP) &&
                (trick[seat_0] == -1) && (trick[seat_1] == -1) && (trick[seat_2] == -1))
        {
            //Declarer or dummy is leading a trump play.
            CFeatures declarerFeatures;
            CFeatures dummyFeatures;
            declarerFeatures.setCardFeatures(ownCards);
            dummyFeatures.setCardFeatures(dummyCards);
            //Should we play trump?
            int no = declarerFeatures.getSuitLen(suit) + dummyFeatures.getSuitLen(suit);
            if (no >= 7)
            {
                int noOwn, noOpp;
                playHistory.getNoPlayed(declarer, suit, &noOwn, &noOpp);
                if ((no + noOpp) < 13)
                {
                    //Declarer or dummy leads trump if this is one of the best plays.
                    int i;
                    for (i = 0; i < 52; i++)
                    {
                        if ((cards[i] == max) && (CARD_SUIT(i) == suit))
                            break;
                    }
                    if (i < 52)
                        card = i;
                }
                else
                {
                    //Declarer or dummy leads non trump if this is one of the best plays.
                    int i;
                    for (i = 0; i < 52; i++)
                    {
                        if ((cards[i] == max) && (CARD_SUIT(i) != suit))
                            break;
                    }
                    if (i < 52)
                        card = i;
                }
            }
        }
    }

    return card;
}

