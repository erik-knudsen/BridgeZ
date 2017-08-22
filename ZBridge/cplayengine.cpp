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

const int NO_HANDS_DD = 25;
const int MAX_ITER = 1000;

CPlayEngine::CPlayEngine(CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc)
{
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
    futureTricks fut[NO_HANDS_DD];
    int weight[NO_HANDS_DD];
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

            //Calculate weight.
            weight[handNo] = calcWeight(hands[handNo], seat, dummySeat, bidHistory, playHistory, nsBidOptions, ewBidOptions);

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
    return 100;
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

    int cardL;
    int cardH;
    int cardsLH[13];
    for (int i = 0; i < 13; i++)
        cardsLH[i] = -1;
    int numBest = 0;
    for (int i = 0; i < 52; i++)
        if (cards[i] == max)
            cardsLH[numBest++] = i;
    if (numBest > 0)
    {
        cardL = cardsLH[0];
        cardH = cardsLH[numBest - 1];
    }

    //DD found preferences?
    if (max > 0)
    {
        QString txt = QString("%1 equals:").arg(SEAT_NAMES[seat]);
        for (int i = 0; i < 52;i++)
        if (cards[i] == max)
        {
            txt += QString("  %1%2").arg(SUIT_NAMES[CARD_SUIT(i)]).arg(FACE_NAMES[CARD_FACE(i)]);
        }
        qDebug() << txt;

        //Get trump suit for the hand.
        Suit suit = BID_SUIT(playHistory.getContract());

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

        CBidOptionDoc &bidOptions = ((seat == WEST_SEAT) || (seat == EAST_SEAT)) ? (ewBidOptions) : (nsBidOptions);

        //First hand (leader)?
        if (trick[seat_0] == -1)
        {
            //Declarer or dummy leading a trump play?
            if (((declarer == currentLeader) || (((declarer + 2) & 3) == currentLeader)) && (suit != NOTRUMP))
            {
                CFeatures ownFeatures;
                ownFeatures.setCardFeatures(ownCards);
                CFeatures dummyFeatures;
                dummyFeatures.setCardFeatures(dummyCards);

                //Should we play trump?
                int no = ownFeatures.getSuitLen(suit) + dummyFeatures.getSuitLen(suit);
                int cardLS = -1;
                int cardHS;
                if (no >= 7)
                {
                    int noOwn, noOpp;
                    playHistory.getNoPlayed(declarer, suit, &noOwn, &noOpp);
                    if ((no + noOpp) < 13)
                    {
                        //Declarer or dummy leads trump if this is one of the best plays.
                        for (int i = 0; i < 52; i++)
                        if ((cards[i] == max) && (CARD_SUIT(i) == suit))
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
                    if ((cards[i] == max) && (CARD_SUIT(i) != suit))
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
                cardC = getOppLead(seat, suit, cardsLH, numBest, ownCards, playHistory, bidOptions);
            }
        }

        //Second hand?
        else if (trick[seat_1] == -1)
        {
            //Declarer lead i.e. opponent plays second hand?
            if ((declarer == currentLeader) || (((declarer + 2) & 3) == currentLeader))
            {
                //Follow suit to first time lead (no signal for trump lead)?
                if ((cardLeadSuit != suit) && (cardLeadSuit == CARD_SUIT(cardsLH[0])) &&
                        playHistory.isFirstTimeSuitPlayed(cardLeadSuit))
                {
                    //Find largest small and smallest small card in the lead suit.
                    int signalLead = (suit != NOTRUMP) ? bidOptions.declarerLeadSuit : bidOptions.declarerLeadNT;
                    cardC = getFollow(cardLeadSuit, cardsLH, numBest, signalLead, ownCards, playHistory);
                }

                //Discard?
                else if ((cardLeadSuit != CARD_SUIT(cardsLH[0])))
                {
                    int signalDiscard = (suit != NOTRUMP) ? bidOptions.discardingSuit : bidOptions.discardingNT;
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
            if ((declarer != currentLeader) && (((declarer + 2) & 3) != currentLeader) && (cardLeadSuit != suit))
            {
                //Follow suit to first time lead?
                if ((cardLeadSuit == CARD_SUIT(cardsLH[0])) && playHistory.isFirstTimeSuitPlayed(cardLeadSuit))
                {
                    //Find largest small and smallest small card in the lead suit.
                    int signalLead = (suit != NOTRUMP) ? bidOptions.partnerLeadSuit : bidOptions.partnerLeadNT;
                    cardC = getFollow(cardLeadSuit, cardsLH, numBest, signalLead, ownCards, playHistory);
                }

                //Discard?
                else if ((cardLeadSuit != CARD_SUIT(cardsLH[0])))
                {
                    int signalDiscard = (suit != NOTRUMP) ? bidOptions.discardingSuit : bidOptions.discardingNT;
                    cardC = getDiscard(cardLeadSuit, cardsLH, numBest, signalDiscard, ownCards, playHistory);
                }
            }
            //Select default card?
            if (cardC == -1)
                cardC = ((playHistory.getLeader() == seat_0) && (cardLeadFace >= JACK)) ? (cardL) : playHistory.takeTrick(cardH) ? (cardH) : (cardL);
        }

        //Fourth hand.
        else
        {
            //Is declarer leading and opponent discarding?
            if ((declarer == currentLeader) || (((declarer + 2) & 3) == currentLeader) &&
                    ((cardLeadSuit != CARD_SUIT(cardsLH[0]))))
            {
                int signalDiscard = (suit != NOTRUMP) ? bidOptions.discardingSuit : bidOptions.discardingNT;
                cardC = getDiscard(cardLeadSuit, cardsLH, numBest, signalDiscard, ownCards, playHistory);
            }
            //Select default card?
            if (cardC == -1)
                cardC = (playHistory.getLeader() == seat_1) ? (cardL) : playHistory.takeTrick(cardL) ? (cardL) : (playHistory.takeTrick(cardH)) ? (cardH) : (cardL);
        }
    }

    //If none is found, just take one that is allowable.
    if (cardC == -1)
    {
        if (max > 0)
            cardC = (CARD_FACE(cardH) >= JACK) ? (cardH) : (cardL);
        else
        {
            int *crds = (seat == dummySeat) ? dummyCards : ownCards;
            int i;
            for (i = 0; i < 13; i++)
                if (playHistory.cardOk(crds[i], seat, crds))
                    break;

            assert(i < 13);

            cardC = crds[i];
        }
    }

    return cardC;
}

/**
 * @brief Get best lead card for opponent.
 *
 * Only used the first time a suit is played.
 *
 * @param seat Opponents seat.
 * @param suit Trump suit.
 * @param cardsLH Best cards to play. Determined by double dummy analysis.
 * @param numBest Number of best cards to play.
 * @param ownCards Own cards.
 * @param playHistory Play history.
 * @param bidOptions Bid and play options for the opponent.
 * @return The best card to play.
 */
int CPlayEngine::getOppLead(Seat seat, Suit suit, int cardsLH[], int numBest, int ownCards[], CPlayHistory &playHistory,
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

    if (suit == NOTRUMP)
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
                ownCrds[i][ACE] || ownCrds[i][KING] || ownCrds[i][QUEEN] || ownCrds[i][JACK] &&
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
                for (int j = 0; j < 13; j++)
                if (crdsLH[i][j] && (j < TEN))
                {
                    cardC = MAKE_CARD(i, j);
                    return cardC;
                }
            }
        }

        //Tripleton with honour --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                ownCrds[i][ACE] || ownCrds[i][KING] || ownCrds[i][QUEEN] || ownCrds[i][JACK] &&
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
            for (int j = 0; j < 13; j++)
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
                ownCrds[i][ACE] || ownCrds[i][KING] || ownCrds[i][QUEEN] || ownCrds[i][JACK] &&
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
                for (int j = 0; j < 13; j++)
                if (crdsLH[i][j] && (j < TEN))
                {
                    cardC = MAKE_CARD(i, j);
                    return cardC;
                }
            }
        }

        //Tripleton with honour --------------------------------------------
        for (int i = 0; i < 4; i++)
        if (playHistory.isFirstTimeSuitPlayed((Suit)i) &&
                ownCrds[i][ACE] || ownCrds[i][KING] || ownCrds[i][QUEEN] || ownCrds[i][JACK] &&
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
            for (int j = 0; j < 13; j++)
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

        //Select card according to signal options.
        if (signalDiscard == SIGNAL_COUNT_HIGH)
            cardC = ((ownFeatures.getSuitLen((Suit)k) & 1) == 0) ? highCard : lowCard;
        else if (signalDiscard == SIGNAL_COUNT_LOW)
            cardC = ((ownFeatures.getSuitLen((Suit)k) & 1) == 0) ? lowCard : highCard;
    }
    return cardC;
}
