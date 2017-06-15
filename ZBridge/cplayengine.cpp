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

#include "dll.h"
#include "mt19937ar.h"
#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cplayengine.h"

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
    /*    int i;

    int *crds = (seat == dummySeat) ? dummyCards : ownCards;

    for (i = 0; i < 52; i++)
        if (playHistory.cardOk(i, seat, crds))
            break;

    return i;
*/
    //Determine remaining known cards (own and dummy).
    int ownRemaining[13];
    int dummyRemaining[13];
    int j = 0;
    for (int i = 0; i < 13; i++)
        ownRemaining[j++] = !playHistory.isCardPlayed(seat, ownCards[i]) ? (ownCards[i]) : (-1);
    j = 0;
    for (int i = 0; i < 13; i++)
        dummyRemaining[j++] = !playHistory.isCardPlayed(dummySeat, dummyCards[i]) ? (dummyCards[i]) : (-1);

    //Calculate a number of possible hands and double dummy solutions to these hands.
    int hands[100][4][13];
    int noHands = 0;
    while (noHands < 100)
    {
        //Initialize cards.
        int cardValues[52];
        for (int i = 0; i < 52; i++)
            cardValues[i] = i;

        //Own cards are always known, therefore remove own cards..
        for (int i = 0; i < 13; i++)
            cardValues[ownCards[i]] = -1;

        //Dummys cards are known except for the first play, therefore remove dummy cards in other cases.
        if (!playHistory.isFirstPlay())
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

        //Deal cards randomly.
        for (int i = noCards - 1; i >= 0; i--)
        {
            int inx = genrand_int32()%(i + 1);
            int card = cardValues[inx];
            for (int j = inx; j < i; j++)
                cardValues[j] = cardValues[j + 1];
            cardValues[j] = card;
        }

        //In the hand to play own cards are always known (declarer plays dummys cards).
        Seat ownSeat = (Seat)((seat == dummySeat) ? ((seat + 2) % 4) : (seat));
        for (int i = 0; i < 13; i++)
            hands[noHands][ownSeat][i] = ownRemaining[i];

        //In the hand to play dummy card are known except for the first play.
        bool firstPlay = playHistory.isFirstPlay();
        if (!firstPlay)
            for (int i = 0; i < 13; i++)
                hands[noHands][dummySeat][i] = dummyRemaining[i];

        //Distribute properly remaining cards to the hand to play.
        Seat seat_0 = seat;
        Seat seat_1 = (Seat)((seat + 1) % 4);
        Seat seat_2 = (Seat)((seat + 2) % 4);
        Seat seat_3 = (Seat)((seat + 3) % 4);
        int j = 0;
        for (int i = 0; i < 13; i++)
        {
            if ((seat_0 !=dummySeat) && (seat_0 != ownSeat) || (firstPlay && (seat_0 == dummySeat)))
                hands[noHands][seat_0][i] = cardValues[j++];
            if ((seat_1 !=dummySeat) && (seat_1 != ownSeat) || (firstPlay && (seat_1 == dummySeat)))
                hands[noHands][seat_1][i] = cardValues[j++];
            if ((seat_2 !=dummySeat) && (seat_2 != ownSeat) || (firstPlay && (seat_2 == dummySeat)))
                hands[noHands][seat_2][i] = cardValues[j++];
            if ((seat_3 !=dummySeat) && (seat_3 != ownSeat) || (firstPlay && (seat_3 == dummySeat)))
                hands[noHands][seat_3][i] = cardValues[j++];
        }

        //Check features.
        CFeatures features;
        for (int i = 0; i < 4; i++)
        {
            if ((i == dummySeat) && firstplay)
            {
                features.setCardFeatures(hands[noHands][i]);
                CFeatures &highFeatures = bidHistory.getHighFeatures((Seat)i);
                CFeatures &lowFeatures = bidHistory.getLowFeatures((Seat)i);
            }
        }

        //Prepare for double dummy solver.
        deal dl;
        futureTricks fut;
        int target;
        int solutions;
        int mode;
        int threadIndex = 0;
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
        if (hands[noHands][i][j] != -1)
        {
            int hand = (i + 3) % 4;
            int card = hands[noHands][i][j];
            Suit suit = CARD_SUIT(card);
            int face = CARD_FACE(card);
            dl.remainCards[hand][3 - suit] |= (1 << (face + 2));            //Double dummy solver format.
        }

        //Double dummy solver.
        target = -1;
        solutions = 2;
        mode = 0;
        res = SolveBoard(dl, target, solutions, mode, &fut, threadIndex);
    }
}

