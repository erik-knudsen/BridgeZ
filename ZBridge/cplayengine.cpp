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
#include "cddslock.h"
#include "cplayengine.h"

const int BATCH_SIZE = 8;       //Batch size for double dummy solver.
const int MAX_NO_HANDS = 100;   //MSVC does not support dynamic arrays (newer versions of gcc does).
const int MAX_ITER = 1000;      //Maximum number of iterations for finding a hand.

CPlayEngine::CPlayEngine(int computerLevel, CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc)
{
    noHandsDD = (computerLevel == 0) ? (8) : (computerLevel == 1) ? (16) : (computerLevel == 2) ? (32) : (64);
    assert(noHandsDD <= MAX_NO_HANDS);

    nsBidOptions = nsBidOptionDoc;
    ewBidOptions = ewBidOptionDoc;
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
    futureTricks fut[MAX_NO_HANDS];
    boards batchBoards;
    solvedBoards batchSolvedBoards;
    int weight[MAX_NO_HANDS];
    int hands[MAX_NO_HANDS][4][13];
    int handNo = 0;
    int iter = 0;
    int maxFailures = 0;
    int curBoard = 0;
    int noHands = noHandsDD;
    if (playHistory.getNoTrick() >= 2)
        noHands += 8;
    if (playHistory.getNoTrick() >= 5)
        noHands += 8;
    if (playHistory.getNoTrick() >= 8)
        noHands += 8;
    if (playHistory.getNoTrick() >= 11)
        noHands = noHandsDD;
    if (noHands > MAX_NO_HANDS)
        noHands = MAX_NO_HANDS;
    while (handNo < noHands)
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
            if (((seat_0 !=dummySeat) && (seat_0 != ownSeat)) || (firstPlay && (seat_0 == dummySeat)))
                hands[handNo][seat_0][i] = cardValues[j++];
            if (((seat_1 !=dummySeat) && (seat_1 != ownSeat)) || (firstPlay && (seat_1 == dummySeat)))
                hands[handNo][seat_1][i] = cardValues[j++];
            if (((seat_2 !=dummySeat) && (seat_2 != ownSeat)) || (firstPlay && (seat_2 == dummySeat)))
                hands[handNo][seat_2][i] = cardValues[j++];
            if (((seat_3 !=dummySeat) && (seat_3 != ownSeat)) || (firstPlay && (seat_3 == dummySeat)))
                hands[handNo][seat_3][i] = cardValues[j++];
        }

        //Check features.
        int noFailures = 0;
        int i;
        for (i = 0; i < 4; i++)
        {
            //Only check for hands with unknown cards.
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
                bool res = bidHistory.checkCardFeatures(cards, (Seat)i);
                if (res)
                    noFailures++;
                if ((res) && (noFailures > maxFailures))
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

            //Calculate weight.
            weight[handNo] = calcWeight(hands[handNo], seat, dummySeat, bidHistory, playHistory, nsBidOptions, ewBidOptions);

            //Double dummy solver batch parameters.
            batchBoards.deals[curBoard] = dl;
            batchBoards.target[curBoard] = target;
            batchBoards.solutions[curBoard] = solutions;
            batchBoards.mode[curBoard] = mode;

            //Next hand.
            handNo++; iter = 0; maxFailures = 0;
            curBoard++;

            //Solve with double dummy solver?
            if ((curBoard == BATCH_SIZE) || (handNo == noHands))
            {
                batchBoards.noOfBoards = curBoard;
                CddsLock::mutex.lock();         //Static lock to protect dds static data.
                int res = SolveAllChunksBin(&batchBoards, &batchSolvedBoards, 2);
                CddsLock::mutex.unlock();
                for (int i = 1; i <= curBoard; i++)
                    fut[handNo - i] = batchSolvedBoards.solvedBoard[curBoard - i];
                curBoard = 0;
            }
        }
    }

    //Get the best to play (highest sum of scores).
    int cards[52];
    for (int i = 0; i < 52; i++)
        cards[i] = 0;

    for (int i = 0; i < noHands; i++)
    for (int j = 0; j < fut[i].cards; j++)
    {
        //Highest ranking card.
        Suit suit = (Suit)(3 - fut[i].suit[j]);
        int face = fut[i].rank[j] - 2;
        int card = MAKE_CARD(suit, face);
        cards[card] += fut[i].score[j] * weight[j];

        //Lower ranking equals.
        for (int k = 2; k < 15; k++)            //From deuce to ace (DDS format)
        if (((1 << k) & fut[i].equals[j]) != 0)
        {
            card = MAKE_CARD(suit, (k - 2));    //Own format.
            cards[card] += fut[i].score[j] * weight[j];
        }
    }

    return getBestCard(cards, ownCards, dummyCards, seat, dummySeat, bidHistory, playHistory, nsBidOptions, ewBidOptions);
}


/**
 * @brief Calculate weight to use for a given hand of cards.
 *
 * The weight is calculated in %. A figure from 0 to 100. The weight is calculated based on the
 * bid history and the play history and on rules implicit assumed and rules explicit stated in
 * the options. It is meant to give a propability measure of the likelihood of the hand could actually
 * be the right hand.
 *
 * @param hands Remaining cards to be played for the four hands. Already played cards are in the play history.
 * @param seat The seat to play the next card (declarer plays dummys cards).
 * @param dummySeat Dummys seat.
 * @param bidHistory The bid history.
 * @param playHistory The play history.
 * @param nsBidOptions Bid and play options with rules for the N/S pair.
 * @param ewBidOptions Bid and play options with rules for the E/W pair.
 * @return The weight.
 */
int CPlayEngine::calcWeight(int hands[4][13], Seat seat, Seat dummySeat, CBidHistory &bidHistory, CPlayHistory &playHistory,
                            CBidOptionDoc &nsBidOptions, CBidOptionDoc &ewBidOptions)
{
    int cards[4][13];
    CFeatures features[4];

    //Only calculate for opponent and not known hands.
    for (int i = 0; i < 4; i++)
    if ((i != dummySeat) && (i != ((dummySeat + 2) & 3)) && (i != seat))
    {
        int newNo;
        for (newNo = 0; (newNo < 13) && (hands[i][newNo] != -1); newNo++)
            cards[i][newNo] = hands[i][newNo];

        if (newNo < 13)
        {
            int played[13];
            int  noPlayed = playHistory.getPlayed((Seat)i, played);

            assert((noPlayed + newNo) == 13);

            for (int k = 0; k < noPlayed; k++)
                cards[i][newNo++] = played[k];
        }
        features[i].setCardFeatures(cards[i]);
    }

    CBidOptionDoc &bidOptions = ((dummySeat == WEST_SEAT) || (dummySeat == EAST_SEAT)) ?
                ewBidOptions : nsBidOptions;
    Bids contract = playHistory.getContract();
    float p = 1.0;
    for (int i = 0; i <= playHistory.getNoTrick(); i++)
    {
        Seat nextLeader = playHistory.getNextLeader(i);
        int openCard = playHistory.getCard(nextLeader, i);

        for (int j = 0; j < 4; j++)
        {
            Seat seatIterator = (Seat)((j + nextLeader) & 3);
            if ((seatIterator != dummySeat) && (seatIterator != ((dummySeat + 2) & 3)) && (seatIterator != seat))
            {
                int card = playHistory.getCard(seatIterator, i);
                if (card == -1)
                    break;
                Suit suit = CARD_SUIT(card);
                if (playHistory.isFirstTimeSuitPlayed(suit))
                {
                    //Opponent is opener?
                    if (j == 0)
                        p *= oppIsOpener(card, contract, cards[seatIterator], features[seatIterator], bidOptions);

                    //Opponent is openers partner?
                    else if (j == 2)
                        p *= oppIsOpenersPartner(card, openCard, contract, cards[seatIterator], features[seatIterator],
                                                 bidOptions);

                    //Declarer or dummy is opener.
                    else
                        p *= declarerOrDummyIsOpener(card, openCard, contract, cards[seatIterator],
                                                     features[seatIterator], bidOptions);
                }
            }
        }
    }

    return (int)(p*100);
}

/**
 * @brief Get the best card to play.
 *
 * The best card to play is calculated based on the double dummy calculated and weighed result scores and
 * on rules implicit assumed and rules explicit stated in the options. Rules for leads and for signals are
 * used for this.
 *
 * @param cards Double dummy calculated and weighed result scores
 * @param ownCards Players own cards (all 13 cards).
 * @param dummyCards Dummy own cards (all 13 cards)
 * @param seat Players seat (declarer plays dummys cards).
 * @param dummySeat The dummys seat.
 * @param bidHistory The bid history.
 * @param playHistory The play history.
 * @param nsBidOptions Bid and play options with rules for the N/S pair.
 * @param ewBidOptions Bid and play options with rules for the E/W pair.
 * @return The best card to play.
 */
int CPlayEngine::getBestCard(int cards[], int ownCards[], int dummyCards[], Seat seat, Seat dummySeat, CBidHistory &bidHistory,
                             CPlayHistory &playHistory, CBidOptionDoc &nsBidOptions, CBidOptionDoc &ewBidOptions)
{
    int cardC = -1;

    int max = 0;
    for (int i = 0; i < 52; i++)
    if (cards[i] > max)
        max = cards[i];

    int cardL = -1;
    int cardH = -1;
    int cardsLH[13];
    for (int i = 0; i < 13; i++)
        cardsLH[i] = -1;
    int numBest = 0;

    //DD found preferences?
    if (max > 0)
    {
        for (int i = 0; i < 52; i++)
            if (cards[i] == max)
                cardsLH[numBest++] = i;
        if (numBest > 0)
        {
            cardL = cardsLH[0];
            cardH = cardsLH[numBest - 1];
        }
        QString txt = QString("%1 equals:").arg(SEAT_NAMES[seat]);
        for (int i = 0; i < 52;i++)
        if (cards[i] == max)
        {
            txt += QString("  %1%2").arg(SUIT_NAMES[CARD_SUIT(i)]).arg(FACE_NAMES[CARD_FACE(i)]);
        }

//        qDebug() << txt;

        //Get contract suit for the hand.
        Suit contractSuit = BID_SUIT(playHistory.getContract());

        //Get current leader and declarer.
        Seat currentLeader = playHistory.getCurrentLeader();
        Seat declarer = playHistory.getDeclarer();

        //In the current trick, 3 cards can be played.
        Seat seat_0 = currentLeader;
        Seat seat_1 = (Seat)((currentLeader + 1) % 4);
        Seat seat_2 = (Seat)((currentLeader + 2) % 4);
        int trick[4];
        playHistory.getTrick(playHistory.getNoTrick(), trick);

        int cardLeadFace = CARD_FACE(playHistory.getCard(seat_0));
        Suit cardLeadSuit = CARD_SUIT(playHistory.getCard(seat_0));

        CBidOptionDoc &bidOptions = ((seat == WEST_SEAT) || (seat == EAST_SEAT)) ?
                    (ewBidOptions) : (nsBidOptions);

        //First hand (leader)?
        if (trick[seat_0] == -1)
        {
            //Declarer or dummy leading a trump play?
            if (((declarer == currentLeader) || (((declarer + 2) & 3) == currentLeader)) &&
                    (contractSuit != NOTRUMP))
            {
                CFeatures ownFeatures;
                ownFeatures.setCardFeatures(ownCards);
                CFeatures dummyFeatures;
                dummyFeatures.setCardFeatures(dummyCards);

                //Should we play trump?
                int no = ownFeatures.getSuitLen(contractSuit) + dummyFeatures.getSuitLen(contractSuit);
                int cardLS = -1;
                int cardHS = -1;
                if (no >= 7)
                {
                    int noOwn, noOpp;
                    playHistory.getNoPlayed(declarer, contractSuit, &noOwn, &noOpp);
                    if ((no + noOpp) < 13)
                    {
                        //Declarer or dummy leads trump if this is one of the best plays.
                        for (int i = 0; i < 52; i++)
                        if ((cards[i] == max) && (CARD_SUIT(i) == contractSuit))
                        {
                            if (cardLS < 0)
                                cardLS = i;
                            cardHS = i;
                        }
                        if (cardLS >= 0)
                            cardC = (CARD_FACE(cardHS) >= JACK) ? (cardHS) : (cardLS);
                    }
                }
                if (cardC == -1)
                {
                    //Declarer or dummy leads non trump if this is one of the best plays.
                    for (int i = 0; i < 52; i++)
                    if ((cards[i] == max) && (CARD_SUIT(i) != contractSuit))
                    {
                        if (cardLS < 0)
                            cardLS = i;
                        cardHS = i;
                    }
                    if (cardLS >= 0)
                        cardC = (CARD_FACE(cardHS) >= JACK) ? (cardHS) : (cardLS);
                }
            }
            //Declarer or dummy leading a non trump play?
            else if ((declarer == currentLeader) || (((declarer + 2) & 3) == currentLeader))
            {
                cardC = (CARD_FACE(cardH) >= JACK) ? (cardH) : (cardL);
            }
            //Opponent leading.
            else
            {
                cardC = getOppLead(seat, contractSuit, cardsLH, numBest, ownCards, playHistory, bidOptions);
            }
        }

        //Second hand?
        else if (trick[seat_1] == -1)
        {
            //Declarer lead i.e. opponent plays second hand?
            if ((declarer == currentLeader) || (((declarer + 2) & 3) == currentLeader))
            {
                //Follow suit to first time lead (no signal for trump lead)?
                if ((cardLeadSuit != contractSuit) && (cardLeadSuit == CARD_SUIT(cardsLH[0])) &&
                        playHistory.isFirstTimeSuitPlayed(cardLeadSuit))
                {
                    //Find largest small and smallest small card in the lead suit.
                    int signalLead = (contractSuit != NOTRUMP) ? bidOptions.declarerLeadSuit : bidOptions.declarerLeadNT;
                    cardC = getFollow(cardLeadSuit, cardsLH, numBest, signalLead, ownCards, playHistory);
                }

                //Discard?
                else if ((cardLeadSuit != CARD_SUIT(cardsLH[0])))
                {
                    int signalDiscard = (contractSuit != NOTRUMP) ? bidOptions.discardingSuit : bidOptions.discardingNT;
                    cardC = getDiscard(cardLeadSuit, cardsLH, numBest, signalDiscard, ownCards, playHistory);
                }
            }
            //Select default card?
            if (cardC == -1)
                cardC = cardL;
        }

        //Third hand?
        else if (trick[seat_2] == -1)
        {
            //Is a non trump card lead by opponent?
            if ((declarer != currentLeader) && (((declarer + 2) & 3) != currentLeader) && (cardLeadSuit != contractSuit))
            {
                //Follow suit to first time lead?
                if ((cardLeadSuit == CARD_SUIT(cardsLH[0])) && playHistory.isFirstTimeSuitPlayed(cardLeadSuit) &&
                        (cardLeadFace > TEN))
                {
                    //Find largest small and smallest small card in the lead suit.
                    int signalLead = (contractSuit != NOTRUMP) ? bidOptions.partnerLeadSuit : bidOptions.partnerLeadNT;
                    cardC = getFollow(cardLeadSuit, cardsLH, numBest, signalLead, ownCards, playHistory);
                }

                //Discard?
                else if ((cardLeadSuit != CARD_SUIT(cardsLH[0])))
                {
                    int signalDiscard = (contractSuit != NOTRUMP) ? bidOptions.discardingSuit : bidOptions.discardingNT;
                    cardC = getDiscard(cardLeadSuit, cardsLH, numBest, signalDiscard, ownCards, playHistory);
                }
            }
            //Select default card?
            if (cardC == -1)
                cardC = (((playHistory.getLeader() == seat_0) && (cardLeadFace >= JACK)) ||
                         (playHistory.takeTrick(cardL) && (CARD_SUIT(cardL) == contractSuit))) ? (cardL) :
                                                                 playHistory.takeTrick(cardH) ? (cardH) : (cardL);
        }

        //Fourth hand.
        else
        {
            //Is declarer leading?
            if ((declarer == currentLeader) || (((declarer + 2) & 3) == currentLeader))
            {
                //Follow suit to first time lead (no signal for trump lead)?
                if ((cardLeadSuit != contractSuit) && (cardLeadSuit == CARD_SUIT(cardsLH[0])) &&
                        playHistory.isFirstTimeSuitPlayed(cardLeadSuit))
                {
                    //Find largest small and smallest small card in the lead suit.
                    int signalLead = (contractSuit != NOTRUMP) ? bidOptions.declarerLeadSuit : bidOptions.declarerLeadNT;
                    cardC = getFollow(cardLeadSuit, cardsLH, numBest, signalLead, ownCards, playHistory);
                }

                //Discard?
                else if ((cardLeadSuit != CARD_SUIT(cardsLH[0])))
                {
                    int signalDiscard = (contractSuit != NOTRUMP) ? bidOptions.discardingSuit : bidOptions.discardingNT;
                    cardC = getDiscard(cardLeadSuit, cardsLH, numBest, signalDiscard, ownCards, playHistory);
                }
            }
            //Select default card?
            if (cardC == -1)
                cardC = (playHistory.getLeader() == seat_1) ? (cardL) : playHistory.takeTrick(cardL) ? (cardL) : (playHistory.takeTrick(cardH)) ? (cardH) : (cardL);
        }
    }

    //If none is found, just take the smallest that is allowable.
    if (cardC == -1)
    {
        if (max > 0)
            cardC = cardL;
        else
        {
            int *crds = (seat == dummySeat) ? dummyCards : ownCards;
            int j = 0;
            int face = 15;
            for (int i = 0; i < 13; i++)
            {
                if (playHistory.cardOk(crds[i], seat, crds) && (face > CARD_FACE(crds[i])))
                {
                    j = i;
                    face = CARD_FACE(crds[i]);
                }
            }

            assert(face != 15);

            cardC = crds[j];
        }
    }

//    qDebug() << QString("  %1%2").arg(SUIT_NAMES[CARD_SUIT(cardC)]).arg(FACE_NAMES[CARD_FACE(cardC)]);

    return cardC;
}

/**
 * @brief Get best lead card for opponent.
 *
 * Only used the first time a suit is played.
 *
 * @param seat Opponents seat.
 * @param contractSuit Contract suit.
 * @param cardsLH Best cards to play. Determined by double dummy analysis.
 * @param numBest Number of best cards to play.
 * @param ownCards Own cards.
 * @param playHistory Play history.
 * @param bidOptions Bid and play options for the opponent.
 * @return The best card to play.
 */
int CPlayEngine::getOppLead(Seat seat, Suit contractSuit, int cardsLH[], int numBest, int ownCards[], CPlayHistory &playHistory,
                            CBidOptionDoc &bidOptions)
{
    int cardC = -1;

    int noCrdsLH[4];
    for (int i = 0; i < 4; i++)
        noCrdsLH[i] = 0;
    for (int i = 0; i < numBest; i++)
        noCrdsLH[CARD_SUIT(cardsLH[i])]++;

    int noOwnCrds[4];
    for (int i = 0; i < 4; i++)
        noOwnCrds[i] = 0;
    for (int i = 0; i < 13; i++)
        noOwnCrds[CARD_SUIT(ownCards[i])]++;

    bool crdsLH[4][13];
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 13; j++)
        crdsLH[i][j] = false;
    for (int i = 0; i < numBest; i++)
        crdsLH[CARD_SUIT(cardsLH[i])][CARD_FACE(cardsLH[i])] = true;

    bool ownCrds[4][13];
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 13; j++)
        ownCrds[i][j] = false;
    for (int i = 0; i < 13; i++)
        ownCrds[CARD_SUIT(ownCards[i])][CARD_FACE(ownCards[i])] = true;

    if (contractSuit == NOTRUMP)
    {
        //Ace, King doubleton --------------------------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][ACE] && crdsLH[i][KING] && (noOwnCrds[i] == 2))
        {
            int crd = MAKE_CARD(i, KING);
            cardC = (bidOptions.openingLead[OPEN_NT][AK_INX] == aK_VAL) ? (crd) : (crd + 1);
            return cardC;
        }

        //Sequence --------------------------------------------------------------------------
        //Ace, King, Queen.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][ACE] && ownCrds[i][KING] && ownCrds[i][QUEEN])
        {
            cardC = MAKE_CARD(i, ACE);
            return cardC;
        }

        //King, Queen, Jack.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][KING] && ownCrds[i][QUEEN] && ownCrds[i][JACK])
        {
            cardC = MAKE_CARD(i, KING);
            return cardC;
        }

        //Queen, Jack, Ten.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][QUEEN] && ownCrds[i][JACK] && ownCrds[i][TEN])
        {
            cardC = MAKE_CARD(i, QUEEN);
            return cardC;
        }

        //Ace, King, not Queen, Jack.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][ACE] && ownCrds[i][KING] &&
                !ownCrds[i][QUEEN] && ownCrds[i][JACK])
        {
            cardC = MAKE_CARD(i, ACE);
            return cardC;
        }

        //King, Queen, not Jack, Ten.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][KING] && ownCrds[i][QUEEN] &&
                !ownCrds[i][JACK] && ownCrds[i][TEN])
        {
            cardC = MAKE_CARD(i, KING);
            return cardC;
        }

        //Ace, not King, Queen, Jack.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && ownCrds[i][ACE] && !ownCrds[i][KING] &&
                crdsLH[i][QUEEN] && ownCrds[i][JACK])
        {
            cardC = MAKE_CARD(i, QUEEN);
            return cardC;
        }

        //King, not Queen, Jack, Ten.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && ownCrds[i][KING] && !ownCrds[i][QUEEN] &&
                crdsLH[i][JACK] && ownCrds[i][TEN])
        {
            cardC = MAKE_CARD(i, JACK);
            return cardC;
        }

        //Fourth best, third/fifth best or low encouraging --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                (ownCrds[i][ACE] || ownCrds[i][KING] || ownCrds[i][QUEEN] || ownCrds[i][JACK]) &&
                (noOwnCrds[i] >= 4))
        {
            if (bidOptions.lengthLead == FOURTH_BEST)
            {
                int cnt = 0;
                int j;
                for (j = 12; (j >= 0) && (cnt != 4); j--)
                    if (ownCrds[i][j])
                        cnt++;
                if (crdsLH[i][j])
                {
                    cardC = MAKE_CARD(i, j);
                    return cardC;
                }
            }
            else if (bidOptions.lengthLead == THIRD_FIFTH_BEST)
            {
                int lead = (noOwnCrds[i] > 4) ? (5) : (3);

                int cnt = 0;
                int j;
                for (j = 12; (j >= 0) && (cnt != lead); j--)
                    if (ownCrds[i][j])
                        cnt++;
                if (crdsLH[i][j])
                {
                    cardC = MAKE_CARD(i, j);
                    return cardC;
                }
            }
            else //if (bidOptions.lengthLead == LOW_ENCOURAGING)
            {
                int j;
                for (j = 0; j < 4; j++)
                if (ownCrds[i][j])
                    break;
                if ((j < 4) && crdsLH[i][j])
                {
                    cardC = MAKE_CARD(i, j);
                    return cardC;
                }
            }
        }

        //Tripleton with honour --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                (ownCrds[i][ACE] || ownCrds[i][KING] || ownCrds[i][QUEEN] || ownCrds[i][JACK]) &&
                (noOwnCrds[i] == 3))
        {
            for (int j = 0; j < 13; j++)
            if (crdsLH[i][j] && (j < TEN))
            {
                cardC = MAKE_CARD(i, j);
                return cardC;
            }
        }

        //Only low with 4 card or more --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                !ownCrds[i][ACE] && !ownCrds[i][KING] && !ownCrds[i][QUEEN] && !ownCrds[i][JACK] &&
                (noOwnCrds[i] >= 4))
        {
            for (int j = 12; j >= 0; j--)
            if (crdsLH[i][j])
            {
                cardC = MAKE_CARD(i, j);
                return cardC;
            }
        }

        //Tripleton with only low --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                !crdsLH[i][ACE] && !crdsLH[i][KING] && !crdsLH[i][QUEEN] && !crdsLH[i][JACK] &&
                (noCrdsLH[i] == 3) && (noOwnCrds[i] == 3))
        {
            int tri[3];
            int n = 0;
            for (int j = 0; j < 13; j++)
            if (crdsLH[i][j])
                tri[n++] = j;
            if (bidOptions.openingLead[OPEN_NT][XXX_INX] == Xxx_VAL)
                cardC = MAKE_CARD(i, tri[2]);
            else if (bidOptions.openingLead[OPEN_NT][XXX_INX] == xXx_VAL)
                cardC = MAKE_CARD(i, tri[1]);
            else //if (bidOptions.openingLead[OPEN_NT][XXX_INX] == xxX_VAL)
                cardC = MAKE_CARD(i, tri[0]);
            return cardC;
        }

        //Doubleton --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && (noCrdsLH[i] == 2) && (noOwnCrds[i] == 2))
        {
            for (int j = 12; j >= 0; j--)
            if (crdsLH[i][j])
            {
                cardC = MAKE_CARD(i, j);
                return cardC;
            }
        }
    }
    else
    {
        //Ace, King doubleton --------------------------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][ACE] && crdsLH[i][KING] && (noOwnCrds[i] == 2))
        {
            int crd = MAKE_CARD(i, KING);
            cardC = (bidOptions.openingLead[OPEN_SUIT][AK_INX] == aK_VAL) ? (crd) : (crd + 1);
            return cardC;
        }

        //Sequence --------------------------------------------------------------------------
        //Ace, King.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][ACE] && ownCrds[i][KING])
        {
            cardC = MAKE_CARD(i, ACE);
            return cardC;
        }

        //King, Queen.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][KING] && ownCrds[i][QUEEN])
        {
            cardC = MAKE_CARD(i, KING);
            return cardC;
        }

        //Queen, Jack.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][QUEEN] && ownCrds[i][JACK])
        {
            cardC = MAKE_CARD(i, QUEEN);
            return cardC;
        }

        //Ace, King, not Queen, Jack.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][ACE] && ownCrds[i][KING] &&
                !ownCrds[i][QUEEN] && ownCrds[i][JACK])
        {
            cardC = MAKE_CARD(i, ACE);
            return cardC;
        }

        //King, Queen, not Jack, Ten.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && crdsLH[i][KING] && ownCrds[i][QUEEN] &&
                !ownCrds[i][JACK] && ownCrds[i][TEN])
        {
            cardC = MAKE_CARD(i, KING);
            return cardC;
        }

        //Ace, not King, Queen, Jack.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && ownCrds[i][ACE] && !ownCrds[i][KING] &&
                crdsLH[i][QUEEN] && ownCrds[i][JACK])
        {
            cardC = MAKE_CARD(i, QUEEN);
            return cardC;
        }

        //King, not Queen, Jack, Ten.
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && ownCrds[i][KING] && !ownCrds[i][QUEEN] &&
                crdsLH[i][JACK] && ownCrds[i][TEN])
        {
            cardC = MAKE_CARD(i, JACK);
            return cardC;
        }

        //Fourth best, third/fifth best or low encouraging --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                (ownCrds[i][ACE] || ownCrds[i][KING] || ownCrds[i][QUEEN] || ownCrds[i][JACK]) &&
                (noOwnCrds[i] >= 4))
        {
            if (bidOptions.lengthLead == FOURTH_BEST)
            {
                int cnt = 0;
                int j;
                for (j = 12; (j >= 0) && (cnt != 4); j--)
                    if (ownCrds[i][j])
                        cnt++;
                if (crdsLH[i][j])
                {
                    cardC = MAKE_CARD(i, j);
                    return cardC;
                }
            }
            else if (bidOptions.lengthLead == THIRD_FIFTH_BEST)
            {
                int lead = (noOwnCrds[i] > 4) ? (5) : (3);

                int cnt = 0;
                int j;
                for (j = 12; (j >= 0) && (cnt != lead); j--)
                    if (ownCrds[i][j])
                        cnt++;
                if (crdsLH[i][j])
                {
                    cardC = MAKE_CARD(i, j);
                    return cardC;
                }
            }
            else //if (bidOptions.lengthLead == LOW_ENCOURAGING)
            {
                int j;
                for (j = 0; j < 4; j++)
                if (ownCrds[i][j])
                    break;
                if ((j < 4) && crdsLH[i][j])
                {
                    cardC = MAKE_CARD(i, j);
                    return cardC;
                }
            }
        }

        //Tripleton with honour --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                (ownCrds[i][ACE] || ownCrds[i][KING] || ownCrds[i][QUEEN] || ownCrds[i][JACK]) &&
                (noOwnCrds[i] == 3))
        {
            for (int j = 0; j < 13; j++)
            if (crdsLH[i][j] && (j < TEN))
            {
                cardC = MAKE_CARD(i, j);
                return cardC;
            }
        }

        //Only low with 4 card or more --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                !ownCrds[i][ACE] && !ownCrds[i][KING] && !ownCrds[i][QUEEN] && !ownCrds[i][JACK] &&
                (noOwnCrds[i] >= 4))
        {
            for (int j = 12; j >= 0; j--)
            if (crdsLH[i][j])
            {
                cardC = MAKE_CARD(i, j);
                return cardC;
            }
        }

        //Tripleton with only low --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                !crdsLH[i][ACE] && !crdsLH[i][KING] && !crdsLH[i][QUEEN] && !crdsLH[i][JACK] &&
                (noCrdsLH[i] == 3) && (noOwnCrds[i] == 3))
        {
            int tri[3];
            int n = 0;
            for (int j = 0; j < 13; j++)
            if (crdsLH[i][j])
                tri[n++] = j;
            if (bidOptions.openingLead[OPEN_SUIT][XXX_INX] == Xxx_VAL)
                cardC = MAKE_CARD(i, tri[2]);
            else if (bidOptions.openingLead[OPEN_SUIT][XXX_INX] == xXx_VAL)
                cardC = MAKE_CARD(i, tri[1]);
            else //if (bidOptions.openingLead[OPEN_SUIT][XXX_INX] == xxX_VAL)
                cardC = MAKE_CARD(i, tri[0]);
            return cardC;
        }

        //Doubleton --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) && (noCrdsLH[i] == 2) && (noOwnCrds[i] == 2))
        {
            for (int j = 12; j >= 0; j--)
            if (crdsLH[i][j])
            {
                cardC = MAKE_CARD(i, j);
                return cardC;
            }
        }
    }

    return cardC;
}

/**
 * @brief Get the best card to play for opponent not leading, but following suit.
 *
 * The card is determined according to signal options. Only used the first time a suit
 * is played.
 *
 * @param cardLeadSuit The suit of the leading card.
 * @param cardsLH Best cards to play. Determined by double dummy analysis.
 * @param numBest Number of best cards to play.
 * @param signalLead Signal options.
 * @param ownCards Own cards.
 * @param playHistory Play history.
 * @return The best card to play.
 */
int CPlayEngine::getFollow(Suit cardLeadSuit, int cardsLH[], int numBest, int signalLead, int ownCards[],
                               CPlayHistory &playHistory)
{
    int cardC = -1;

    CFeatures ownFeatures;
    ownFeatures.setCardFeatures(ownCards);

    //Find largest small and smallest small card in the lead suit.
    int lowCard = 60;
    int highCard = -1;
    for (int i = 0; i < numBest; i++)
    if ((CARD_FACE(cardsLH[i]) <= NINE) && (CARD_SUIT(cardsLH[i]) == cardLeadSuit))
    {
        if (cardsLH[i] > highCard)
            highCard = cardsLH[i];
        if (cardsLH[i] < lowCard)
            lowCard = cardsLH[i];
    }
    if (highCard == -1)
        return cardC;

    //Select card according to signal options.
    if (signalLead == SIGNAL_COUNT_HIGH)
        cardC = ((ownFeatures.getSuitLen(cardLeadSuit) & 1) == 0) ? highCard : lowCard;
    else if (signalLead == SIGNAL_COUNT_LOW)
        cardC = ((ownFeatures.getSuitLen(cardLeadSuit) & 1) == 0) ? lowCard : highCard;
    else if (signalLead == SIGNAL_ATTITUDE_HIGH)
        cardC = (ownFeatures.getHcp(cardLeadSuit) > 1) ? highCard : lowCard;
    else if (signalLead == SIGNAL_ATTITUDE_LOW)
        cardC = (ownFeatures.getHcp(cardLeadSuit) > 1) ? lowCard : highCard;

    return cardC;
}

/**
 * @brief Get the best card to play for opponent not leading and discarding.
 *
 * The card is determined according to signal options. Only used the first time a suit
 * is played.
 *
 * @param cardLeadSuit The suit of the leading card.
 * @param cardsLH Best cards to play. Determined by double dummy analysis.
 * @param numBest Number of best cards to play.
 * @param signalDiscard Signal options.
 * @param ownCards Own cards.
 * @param playHistory Play history.
 * @return The best card to play.
 */
int CPlayEngine::getDiscard(Suit cardLeadSuit, int cardsLH[], int numBest, int signalDiscard, int ownCards[],
                                CPlayHistory &playHistory)
{
    int cardC = -1;

    CFeatures ownFeatures;
    ownFeatures.setCardFeatures(ownCards);

    //Find longest suit among best dd plays.
    int k = -1;
    int countMax = 0;
    for (int i = 0; i < 4; i++)
    if (i != cardLeadSuit)
    {
        int count = 0;
        for (int j = 0; j < numBest; j++)
        if (i == CARD_SUIT(cardsLH[j]))
            count++;
        if ((count > countMax) && playHistory.isFirstTimeSuitPlayed((Suit)i))
        {
            k = i;
            countMax = count;
        }
    }

    //Find largest small and smallest small card in the longest suit among best dd plays.
    if (k != -1)
    {
        int lowCard = 60;
        int highCard = -1;
        for (int i = 0; i < numBest; i++)
        if ((CARD_FACE(cardsLH[i]) <= NINE) && (CARD_SUIT(cardsLH[i]) == k))
        {
            if (cardsLH[i] > highCard)
                highCard = cardsLH[i];
            if (cardsLH[i] < lowCard)
                lowCard = cardsLH[i];
        }
        if (highCard == -1)
            return cardC;

        //Select card according to signal options.
        if (signalDiscard == SIGNAL_COUNT_HIGH)
            cardC = ((ownFeatures.getSuitLen((Suit)k) & 1) == 0) ? highCard : lowCard;
        else if (signalDiscard == SIGNAL_COUNT_LOW)
            cardC = ((ownFeatures.getSuitLen((Suit)k) & 1) == 0) ? lowCard : highCard;
        else if (signalDiscard == SIGNAL_ATTITUDE_HIGH)
            cardC = (ownFeatures.getHcp((Suit)k) > 1) ? highCard : lowCard;
        else if (signalDiscard == SIGNAL_ATTITUDE_LOW)
            cardC = (ownFeatures.getHcp((Suit)k) > 1) ? lowCard : highCard;
    }
    return cardC;
}

float CPlayEngine::oppIsOpener(int card, Bids contract, int cards[],
                               CFeatures &features, CBidOptionDoc &bidOptions)
{
    int face = CARD_FACE(card);
    Suit suit = CARD_SUIT(card);

    int contractLevel = BID_LEVEL(contract);
    Suit contractSuit = BID_SUIT(contract);

    //Opponent plays trump?
    if (suit == contractSuit)
        return (features.getSuitLen(suit) <= 1) ? (1.0) : (0.5);

    bool ace = (features.getCountCard(suit, ACE) == 1);
    bool king = (features.getCountCard(suit, KING) == 1);
    bool queen = (features.getCountCard(suit, QUEEN) == 1);
    bool jack = (features.getCountCard(suit, JACK) == 1);
    bool ten = (features.getCountCard(suit, TEN) == 1);

    //Ace (not trump) played?
    if (face == ACE)
    {
        //At high contract levels assure you get your tricks.
        if (contractLevel >= 5)
            return 1.0;

        //Doubleton ace, king.
        if ((((contractSuit != NOTRUMP) && (bidOptions.openingLead[OPEN_SUIT][AK_INX] == Ak_VAL)) ||
                ((contractSuit == NOTRUMP) && (bidOptions.openingLead[OPEN_NT][AK_INX] == Ak_VAL))) &&
                (features.getSuitLen(suit) == 2) && king)
            return 1.0;

        //Doubleton.
        if (features.getSuitLen(suit) == 2)
            return 1.0;

        //NT contract and ace, king, queen or ace, king, jack.
        if ((contractSuit == NOTRUMP) && king && (queen || jack))
            return 1.0;

        //Trump contract and ace, king, a small.
        if ((contractSuit != NOTRUMP) && king && (features.getSuitLen(suit) > 2))
            return 1.0;
    }
    else if (face == KING)
    {
        //Doubleton ace, king.
        if ((((contractSuit != NOTRUMP) && (bidOptions.openingLead[OPEN_SUIT][AK_INX] == aK_VAL)) ||
                ((contractSuit == NOTRUMP) && (bidOptions.openingLead[OPEN_NT][AK_INX] == aK_VAL))) &&
                (features.getSuitLen(suit) == 2) && ace)
            return 1.0;

        //Doubleton.
        if ((features.getSuitLen(suit) == 2) && !ace)
            return 1.0;

        //NT contract and king, queen, jack or king, queen, ten.
        if ((contractSuit == NOTRUMP) && queen && (jack || ten))
            return 1.0;

        //Trump contract and king, queen, a small.
        if ((contractSuit != NOTRUMP) && queen && (features.getSuitLen(suit) > 2))
            return 1.0;
    }
    else if (face == QUEEN)
    {
        //Doubleton.
        if ((features.getSuitLen(suit) == 2) && !ace && !king)
            return 1.0;

        //NT contract and queen, jack, ten or ace, queen, jack.
        if ((contractSuit == NOTRUMP) && jack && (ten || ace))
            return 1.0;

        //Trump contract and queen, jack, a small.
        if ((contractSuit != NOTRUMP) && jack && (features.getSuitLen(suit) > 2))
            return 1.0;
    }
    else if (face == JACK)
    {
        //Doubleton.
        if ((features.getSuitLen(suit) == 2) && !ace && !king && !queen)
            return 1.0;

        //NT contract and king, jack, ten.
        if ((contractSuit == NOTRUMP) && ten && king)
            return 1.0;

        //Trump contract and jack, ten, a small.
        if ((contractSuit != NOTRUMP) && ten && (features.getSuitLen(suit) > 2))
            return 1.0;
    }
    else
    {
        int lowest = getLowest(suit, cards);
        int highest = getHighest(suit, cards);

        //Doubleton.
        if (features.getSuitLen(suit) == 2)
            return (lowest != face) ? 1.0 : 0.5;

        if (ace || king || queen || jack)
        {
            if (features.getSuitLen(suit) == 3)
                return (lowest == face) ? (1.0) : (0.5);

            int fifthBest = getBest(suit, cards, 5);
            int fourthBest = getBest(suit, cards, 4);
            int thirdBest = getBest(suit, cards, 3);
            if ((bidOptions.lengthLead == THIRD_FIFTH_BEST) && (features.getSuitLen(suit) >= 5) &&
                    (fifthBest == face))
                return 1.0;
            if ((bidOptions.lengthLead == FOURTH_BEST) && (features.getSuitLen(suit) >= 4) &&
                    (fourthBest == face))
                return 1.0;
            if ((bidOptions.lengthLead == THIRD_FIFTH_BEST) && (features.getSuitLen(suit) >= 3) &&
                    (thirdBest == face))
                return 1.0;
            if ((bidOptions.lengthLead == LOW_ENCOURAGING) && (lowest == face))
                return 1.0;
        }
        else
        {
            if (features.getSuitLen(suit) == 3)
            {
                if ((bidOptions.openingLead[OPEN_SUIT][XXX_INX] == Xxx_VAL) && (highest == face))
                return (1.0);
                if ((bidOptions.openingLead[OPEN_SUIT][XXX_INX] == xXx_VAL) && (highest != face) && (lowest != face))
                return (1.0);
                if ((bidOptions.openingLead[OPEN_SUIT][XXX_INX] == xxX_VAL) && (lowest == face))
                return (1.0);
            }
            else if (highest == face)
                return 1.0;
        }
    }

    return 0.5;
}

float CPlayEngine::oppIsOpenersPartner(int card, int openCard, Bids contract, int cards[],
                                       CFeatures &features, CBidOptionDoc &bidOptions)
{
    int face = CARD_FACE(card);
    Suit suit = CARD_SUIT(card);

    Suit openSuit = CARD_SUIT(openCard);
    Suit contractSuit = BID_SUIT(contract);

    //Discarding opening play?
    if (suit != openSuit)
    {
        int signalDiscard = (contractSuit != NOTRUMP) ? bidOptions.discardingSuit : bidOptions.discardingNT;
        if ((signalDiscard == SIGNAL_COUNT_HIGH) &&
                ((((features.getSuitLen(suit) & 1) == 0) && (face > 3)) || (((features.getSuitLen(suit) & 1) != 0) && (face <= 3))))
            return 1.0;
        if ((signalDiscard == SIGNAL_COUNT_LOW) &&
                ((((features.getSuitLen(suit) & 1) == 0) && (face <= 3)) || (((features.getSuitLen(suit) & 1) != 0) && (face > 3))))
            return 1.0;
    }

    //Opponent plays trump and is followed by partner?
    else if (openSuit == contractSuit)
        return 1.0;

    //Opponent playes non trump and is followed by partner.
    else if ((face < TEN) && (CARD_FACE(openCard) > TEN))
    {
        int signalLead = (contractSuit != NOTRUMP) ? bidOptions.partnerLeadSuit : bidOptions.partnerLeadNT;
        if ((signalLead == SIGNAL_ATTITUDE_HIGH) &&
                (((features.getHcp(suit) >= 1) && (face > 3)) || ((features.getHcp(suit) == 0) && (face <= 3))))
            return 1.0;
        if ((signalLead == SIGNAL_ATTITUDE_LOW) &&
                (((features.getHcp(suit) >= 1) && (face <= 3)) || (((features.getHcp(suit) == 0) && (face > 3)))))
            return 1.0;
        if ((signalLead == SIGNAL_COUNT_HIGH) &&
                ((((features.getSuitLen(suit) & 1) == 0) && (face > 3)) || (((features.getSuitLen(suit) & 1) != 0) && (face <= 3))))
            return 1.0;
        if ((signalLead == SIGNAL_COUNT_LOW) &&
                ((((features.getSuitLen(suit) & 1) == 0) && (face <= 3)) || (((features.getSuitLen(suit) & 1) != 0) && (face > 3))))
            return 1.0;
    }
    else
        return 1.0;

    return 0.5;
}

float CPlayEngine::declarerOrDummyIsOpener(int card, int openCard, Bids contract, int cards[],
                                           CFeatures &features, CBidOptionDoc &bidOptions)
{
    int face = CARD_FACE(card);
    Suit suit = CARD_SUIT(card);

    Suit openSuit = CARD_SUIT(openCard);
    Suit contractSuit = BID_SUIT(contract);

    //Discarding opening play?
    if (suit != openSuit)
    {
        int signalDiscard = (contractSuit != NOTRUMP) ? bidOptions.discardingSuit : bidOptions.discardingNT;
        if ((signalDiscard == SIGNAL_COUNT_HIGH) &&
                ((((features.getSuitLen(suit) & 1) == 0) && (face > 3)) || (((features.getSuitLen(suit) & 1) != 0) && (face <= 3))))
            return 1.0;
        if ((signalDiscard == SIGNAL_COUNT_LOW) &&
                ((((features.getSuitLen(suit) & 1) == 0) && (face <= 3)) || (((features.getSuitLen(suit) & 1) != 0) && (face > 3))))
            return 1.0;
    }

    //Declarer/dummy plays trump and is followed by opponent?
    else if (openSuit == contractSuit)
        return 1.0;

    //Declarer/dummy plays non trump and is followed by opponent.
    else if (face < TEN)
    {
        int signalLead = (contractSuit != NOTRUMP) ? bidOptions.declarerLeadSuit : bidOptions.declarerLeadNT;
        if ((signalLead == SIGNAL_ATTITUDE_HIGH) &&
                (((features.getHcp(suit) >= 1) && (face > 3)) || ((features.getHcp(suit) == 0) && (face <= 3))))
            return 1.0;
        if ((signalLead == SIGNAL_ATTITUDE_LOW) &&
                (((features.getHcp(suit) >= 1) && (face <= 3)) || (((features.getHcp(suit) == 0) && (face > 3)))))
            return 1.0;
        if ((signalLead == SIGNAL_COUNT_HIGH) &&
                ((((features.getSuitLen(suit) & 1) == 0) && (face > 3)) || (((features.getSuitLen(suit) & 1) != 0) && (face <= 3))))
            return 1.0;
        if ((signalLead == SIGNAL_COUNT_LOW) &&
                ((((features.getSuitLen(suit) & 1) == 0) && (face <= 3)) || (((features.getSuitLen(suit) & 1) != 0) && (face > 3))))
            return 1.0;
    }
    else
        return 1.0;

    return 0.5;
}

int CPlayEngine::getLowest(Suit suit, int cards[])
{
    int lowest = 13;
    for (int i = 0; i < 13; i++)
        if ((CARD_SUIT(cards[i]) == suit) && (CARD_FACE(cards[i]) < lowest))
            lowest = CARD_FACE(cards[i]);

    return lowest;
}

int CPlayEngine::getHighest(Suit suit, int cards[])
{
    int highest = -1;
    for (int i = 0; i < 13; i++)
        if ((CARD_SUIT(cards[i]) == suit) && (CARD_FACE(cards[i]) > highest))
            highest = CARD_FACE(cards[i]);

    return highest;
}

int CPlayEngine::getBest(Suit suit, int cards[], int noBest)
{
    QVector<int> faces;

    for (int i = 0; i < 13; i++)
        if (CARD_SUIT(cards[i]) == suit)
            faces.append(CARD_FACE(cards[i]));

    qSort(faces.begin(), faces.end(), qGreater<int>());
    if (faces.size() < noBest)
        return -1;

    return (faces.at(noBest - 1));
}
