/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CFeatures.cpp
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the features part of the bid database.
 */

#include <cassert>
#include <QVector>

#include "cfeatures.h"

CFeatures::CFeatures()
{
}

/**
 * @brief Set minium features.
 */
void CFeatures::setMinFeatures()
{
    hcpA = 0;
    controlsA = 0;
    hcA = 0;
    honorA = 0;
    aceA = 0;
    kingA = 0;
    queenA = 0;
    jackA = 0;
    tenA = 0;
    rkcbS = 0;
    bal = 0;
    dpA = 0;
    dif2S = 0;
    suits = 1;
    suitLenS = 0;
    suitLen2S = 0;
    lMaj = 0;
    lMin = 0;
    longestS = 0;
    longS = 0;
    interA = 0;
    losersA = 0;
    quickTricksA = 0;
    playingTricksS = 0;
    stopNTS = 0;
    qltyS = 0;
    pointsS = 0;
}

/**
 * @brief Set maximum features.
 */
void CFeatures::setMaxFeatures()
{
    hcpA = (10 << 0) + (10 << 4) + (10 << 8) + (10 << 12) + (37 << 16);
    controlsA =  (3 << 0) + (3 << 2) + (3 << 4) + (3 << 6) +(12 << 8);
    hcA =  (3 << 0) + (3 << 2) + (3 << 4) + (3 << 6) + (12 << 8);
    honorA =  (5 << 0) + (5 << 3) + (5 << 6) + (5 << 9) + (13 << 12);
    aceA =  (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (4 << 4);
    kingA =  (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (4 << 4);
    queenA =  (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (4 << 4);
    jackA =  (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (4 << 4);
    tenA =  (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (4 << 4);
    rkcbS = (5 << 0) + (5 << 3) + (5 << 6) + (5 << 9);
    bal = 9;
    dpA = (9 << 0) + (9 << 4) + (9 << 8) + (9 << 12) + (9 << 16);
    dif2S = (26 << 0) + (26 << 5) + (26 << 10) + (26 << 15) + (26 << 20) + (26 << 25);
    suits = 3;
    suitLenS = (13 << 0) + (13 << 4) + (13 << 8) + (13 << 12);
    suitLen2S = (13 << 0) + (13 << 4) + (13 << 8) + (13 << 12) + (13 << 16) + (13 << 20);
    lMaj = 13;
    lMin = 13;
    longestS = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3);
    longS = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3);
    interA = (7 << 0) + (7 << 3) + (7 << 6) + (7 << 9) + (28 << 12);
    losersA = (3 << 0) + (3 << 2) + (3 << 4) + (3 << 6) + (12 << 8);
    quickTricksA = (4 << 0) + (4 << 3) + (4 << 6) + (4 << 9) + (16 << 12);
    playingTricksS = (26 << 0) + (26 << 5) + (26 << 10) + (26 << 15) + (26 << 20);
    stopNTS = (4 << 0) + (4 << 3) + (4 << 6) + (4 << 9);
    qltyS = (6 << 0) + (6 << 4) + (6 << 8) + (6 << 12);
    pointsS = (37 << 0) + (37 << 6) + (37 << 12) + (37 << 18) + (37 << 24);
}

/**
 * @brief Calculate features for a given hand (set of 13 cards).
 * @param cards The hand to calculate the features for.
 */
void CFeatures::setCardFeatures(int cards[13])
{
    int total;
    int count[4];

    for (int suit = 0; suit < 4; suit++)
        count[suit] = calcCount((Suit)suit, 0, 12, cards);

    //High Card Points (weight of aces, kings, queens and jacks).
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int hcp = calcWeight((Suit)i, JACK, cards);
        total += hcp;
        setHcp((Suit)i, hcp);
    }
    setHcp(ANY, total);

    //Weight of aces and kings.
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int controls = calcWeight((Suit)i, KING, cards);
        total += controls;
        setControls((Suit)i, controls);
    }
    setControls(ANY, total);

    //Count of aces, kings and queens.
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int hc = calcCount((Suit)i, QUEEN, ACE, cards);
        total += hc;
        setHc((Suit)i, hc);
    }
    setHc(ANY, total);

    //Count of cards higher than 9.
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int honor = calcCount((Suit)i, TEN, ACE, cards);
        total += honor;
        setHonor((Suit)i, honor);
    }
    setHonor(ANY, total);

    //Count of aces.
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int ace = calcCount((Suit)i, ACE, ACE, cards);
        total += ace;
        setCountCard((Suit)i, ACE, ace);
    }
    setCountCard(ANY, ACE, total);

    //Count of kings.
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int king = calcCount((Suit)i, KING, KING, cards);
        total += king;
        setCountCard((Suit)i, KING, king);
    }
    setCountCard(ANY, KING, total);

    //Count of queens.
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int queen = calcCount((Suit)i, QUEEN, QUEEN, cards);
        total += queen;
        setCountCard((Suit)i, QUEEN, queen);
    }
    setCountCard(ANY, QUEEN, total);

    //Count of jacks.
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int jack = calcCount((Suit)i, JACK, JACK, cards);
        total += jack;
        setCountCard((Suit)i, JACK, jack);
    }
    setCountCard(ANY, JACK, total);

    //Count of tens.
    total = 0;
    for (int i = 0; i < 4; i++)
    {
        int ten = calcCount((Suit)i, TEN, TEN, cards);
        total += ten;
        setCountCard((Suit)i, TEN, ten);
    }
    setCountCard(ANY, TEN, total);

    //Count of aces + kings.
    for (int i = 0; i < 4; i++)
    {
        int rkcb = calcRkcb((Suit)i, cards);
        setRkcb((Suit)i, rkcb);
    }

    //How balanced?
    {
        int bal = calcBal(count);
        setBal(bal);
    }

    //Distribution points.
    {
        int dp[4];
        int dpt = 0;

        for (int suit = 0; suit < 4; suit++)
        {
            dp[suit] = (count[suit] <= 2) ? (3 - count[suit]) : 0;
            dpt += dp[suit];
        }
        setDp(NOTRUMP, dpt);

        for (int trump = 0; trump < 4; trump++)
        {
            dpt = 0;
            for (int suit = 0; suit < 4; suit++)
            {
                if (suit != trump)
                    dpt += dp[suit];
            }
            setDp((Suit)trump, dpt);
        }
    }

    //Difference in length (S-H, S-D, S-C, H-D, H-C, D-C).
    {
        for (int suit1 = 3; suit1 > 0; suit1--)
            for (int suit2 = suit1 - 1; suit2 >= 0; suit2--)
                setDif2((Suit)suit1, (Suit)suit2, count[suit1] - count[suit2]);
    }

    //Number of suits longer than 3.
    {
        int suits = 0;
        for (int i = 0; i < 4; i++)
            if (count[i] > 3)
                suits++;
        setSuits(suits);
    }

    //Length of suits.
    {
        for (int suit = 0; suit < 4; suit++)
            setSuitLen((Suit)suit, count[suit]);
    }

    //Total length of two suits (S+H, S+D, S+C, H+D, H+C, D+C).
    {
        for (int suit1 = 3; suit1 > 0; suit1--)
            for (int suit2 = suit1 - 1; suit2 >= 0; suit2--)
                setSuitLen2((Suit)suit1, (Suit)suit2, count[suit1] + count[suit2]);
    }
    //Length of longer major.
    {
        setLMaj((count[SPADES] > count[HEARTS]) ? count[SPADES] : count[HEARTS]);
    }

    //Length of longer minor.
    {
        setLMin((count[DIAMONDS] > count[CLUBS]) ? count[DIAMONDS] : count[CLUBS]);
    }

    //Is the suit the longest in the hand.
    {
        int longest = 0;
        for (int suit =0; suit < 4; suit++)
            if (count[suit] > longest)
                longest = count[suit];
        for (int suit = 0; suit < 4; suit++)
            setLongest((Suit)suit , (longest == count[suit]) ? 1 : 0);
    }

    //Is the suit not shortest in the hand.
    {
        int shortest = 20;
        for (int suit =0; suit < 4; suit++)
            if (count[suit] < shortest)
                shortest = count[suit];
        for (int suit = 0; suit < 4; suit++)
            setLong((Suit)suit, (shortest != count[suit]) ? 1 : 0);
    }

    //Weighed count of intermediate cards.
    {
        int weighedCount[4];

        for (int i= 0; i < 4; i++)
            weighedCount[i] = 0;

        for (int i = 0; i < 13; i++)
        {
            Suit suit = CARD_SUIT(cards[i]);
            int face = CARD_FACE(cards[i]);
            int w = (face == TEN) ? (4) : (face == (TEN-1)) ? (2) : (face == (TEN-2)) ? (1) : (0);
            weighedCount[suit] += w;
        }
        for (int suit = 0; suit < 4; suit++)
            setInter((Suit)suit, weighedCount[suit]);
        setInter(ANY, weighedCount[0] + weighedCount[1] + weighedCount[2] + weighedCount[3]);
    }

    //Loosers.
    {
        int tLoosers = 0;

        for (int suit = 0; suit < 4; suit++)
        {
            int loosers = count[suit];
            if (loosers > 3)
                loosers = 3;
            int offset = 3 - loosers;
            for (int j = 0; j < 13; j++)
                if ((CARD_SUIT(cards[j]) == suit) &&
                        (CARD_FACE(cards[j]) >= (QUEEN + offset)))
                    loosers--;
            tLoosers += loosers;
            setLoosers((Suit)suit, loosers);
        }
        setLoosers(ANY, tLoosers);
    }

    //Quick tricks.
    {
        int tQuickTricks = 0;

        for (int suit = 0; suit < 4; suit++)
        {
            bool ace = false;
            bool king = false;
            bool queen = false;
            for (int i = 0; i < 13; i++)
                if (CARD_SUIT(cards[i]) == suit)
                {
                    if (CARD_FACE(cards[i]) == ACE)
                        ace = true;
                    else if (CARD_FACE(cards[i]) == KING)
                        king = true;
                    else if (CARD_FACE(cards[i]) == QUEEN)
                        queen = true;
                }
            int quickTricks;
            if (ace && king)
                quickTricks = 4;
            else if (ace && queen)
                quickTricks = 3;
            else if (ace)
                quickTricks = 2;
            else if (king && queen)
                quickTricks = 2;
            else if (king && (count[suit] > 1))
                quickTricks = 1;
            else
                quickTricks = 0;

            tQuickTricks += quickTricks;
            setQuickTricks((Suit)suit, quickTricks);
        }
        setQuickTricks(ANY, tQuickTricks);
    }

    //Playing tricks.
    {
        //High card playing tricks.
        int hcpTricks = 0;
        for (int suit = 0; suit < 4; suit++)
        {
            bool ace = false;
            bool king = false;
            bool queen = false;
            bool jack = false;
            bool ten = false;
            for (int i = 0; i < 13; i++)
                if (CARD_SUIT(cards[i]) == suit)
                {
                    if (CARD_FACE(cards[i]) == ACE)
                        ace = true;
                    else if (CARD_FACE(cards[i]) == KING)
                        king = true;
                    else if (CARD_FACE(cards[i]) == QUEEN)
                        queen = true;
                    else if (CARD_FACE(cards[i]) == JACK)
                        jack = true;
                    else if (CARD_FACE(cards[i]) == TEN)
                        ten = true;
                }
            if (ace && king && queen)
                hcpTricks += 6;
            else if ((ace && king && jack) || (ace && queen && jack))
                hcpTricks += 5;
            else if ((ace && king) || (ace && queen && ten) || (ace && jack && ten) || (king && queen && jack))
                hcpTricks += 4;
            else if ((ace && queen) || (king && queen && ten) || (king && jack && ten))
                hcpTricks += 3;
            else if (ace || (king && queen) || (king && jack && (count[suit] > 2)) || (queen && jack && ten))
                hcpTricks += 2;
            else if (((king && (count[suit] > 1)) || (queen && jack && (count[suit] > 2))))
                hcpTricks += 1;
            else
                hcpTricks += 0;
        }
        //Long suit playing tricks.
        for (int suit = 0; suit < 4; suit++)
        {
            //One trick for each card over 3 in the trump suit.
            int lsptTricks = 0;
            if (count[suit] > 3)
                lsptTricks += (count[suit] - 3) * 2;

            //Half a trick for the 4. card and one trick for the 5. and
            // subsequent cards in side suits.
            int lspTricks = 0;
            for (int i = 0; i < 4; i++)
                if (i != suit)
                {
                    if (count[i] >= 4)
                    {
                        lspTricks += 1;     //Half a trick for the 4. card in a side suit.
                        for (int j = 5; j <= count[i]; j++)
                            lspTricks += 2; //One trick for the 5. and subsequent cards in a side suit.
                    }
                }
            setPlayingTricks((Suit)suit, hcpTricks + lspTricks + lsptTricks);
            if (lsptTricks == 0)
                setPlayingTricks(NOTRUMP, hcpTricks + lspTricks);
        }
    }

    //Stopper quality of the suit when playing in NT.
    {
        for (int suit = 0; suit < 4; suit++)
        {
            int stopNT;
            bool ace = false;
            bool king = false;
            bool queen = false;
            bool jack = false;
            bool ten = false;
            bool nine = false;
            for (int i = 0; i < 13; i++)
                if (CARD_SUIT(cards[i]) == suit)
                {
                    if (CARD_FACE(cards[i]) == ACE)
                        ace = true;
                    else if (CARD_FACE(cards[i]) == KING)
                        king = true;
                    else if (CARD_FACE(cards[i]) == QUEEN)
                        queen = true;
                    else if (CARD_FACE(cards[i]) == JACK)
                        jack = true;
                    else if (CARD_FACE(cards[i]) == TEN)
                        ten = true;
                    else if (CARD_FACE(cards[i]) == NINE)
                        nine = true;
                }
            int cnt = count[suit];
            if ((ace && king) || (ace && queen && (cnt > 2)) || (king && queen && jack) ||
                    (queen && jack && ten && nine))
                stopNT = 4;
            else if ((ace && jack && (cnt > 2)) || (ace && ten && (cnt > 2)) ||
                    (king && jack && (cnt > 2)) || (king && ten && nine) ||
                    (queen && jack && nine && (cnt > 3)))
                stopNT = 3;
            else if ((king && (cnt > 2)) || (queen && jack && (cnt > 2)) || (queen && ten && (cnt > 2)) ||
                     (jack && (cnt > 3)) || (ten && nine && (cnt > 3)))
                stopNT = 2;
            else if ((jack && (cnt > 2)) || (queen && (cnt > 2)) || (jack && (cnt > 1)) ||
                     (queen && (cnt > 1)) || (king && (cnt > 1)) || (ten && (cnt > 3)))
                stopNT = 1;
            else
                stopNT = 0;

            setStopNT((Suit)suit, stopNT);
        }
    }

    //General quality assessment of the suit.
    {
        for (int suit = 0; suit < 4; suit++)
        {
            int qlty;
            bool ace = false;
            bool king = false;
            bool queen = false;
            bool jack = false;
            bool ten = false;
            bool nine = false;
            for (int i = 0; i < 13; i++)
                if (CARD_SUIT(cards[i]) == suit)
                {
                    if (CARD_FACE(cards[i]) == ACE)
                        ace = true;
                    else if (CARD_FACE(cards[i]) == KING)
                        king = true;
                    else if (CARD_FACE(cards[i]) == QUEEN)
                        queen = true;
                    else if (CARD_FACE(cards[i]) == JACK)
                        jack = true;
                    else if (CARD_FACE(cards[i]) == TEN)
                        ten = true;
                    else if (CARD_FACE(cards[i]) == NINE)
                        nine = true;
                }
            int cnt = count[suit];
            if (getHcp((Suit)suit) >= 9)
                qlty = 6;
            else if ((getHcp((Suit)suit) >= 7) || (king && queen && jack))
                qlty = 5;
            else if ((ace && jack && ten) || (ace && queen && ten) || (king && queen && ten))
                qlty = 4;
            else if ((ace && queen) || (ace && ten && (cnt > 2)) || (king && jack && (cnt > 2)) ||
                     (king && ten && nine) || (queen && jack && (cnt > 2)) || (queen && ten && nine))
                qlty = 3;
            else if (ace  || (king && (cnt > 1)) || (queen && ten && (cnt > 2)) ||
                     (queen && nine && (cnt > 3)) || (jack && ten && (cnt > 3)))
                qlty = 2;
            else if ((queen && (cnt > 1)) || (jack && (cnt > 2)) || (ten && nine && (cnt > 3)))
                qlty = 1;
            else
                qlty = 0;

            setQlty((Suit)suit, qlty);
        }
    }
    //High Card Points + distribution Points + length points.
    {
        int hcp = getHcp(ANY);

        //Shortage + trump length distribution points.
        for (int trump = 0; trump < 4; trump++)
        {
            int ln = (count[trump] > 4) ? (count[trump] - 4) : 0;
            setPoints((Suit)trump, hcp + getDp((Suit)trump) + ln);
        }

        //Length distribution points.
        int ln = 0;
        for (int suit = 0; suit < 4; suit++)
            ln += (count[suit] > 4) ? (count[suit] - 4) : 0;
        setPoints(NOTRUMP, hcp + ln);
    }
}

int CFeatures::featureIsOk(CFeatures &highFeatures, CFeatures &lowFeatures)
{
    const int NO_FEATURES = 115;
    int low[NO_FEATURES], high[NO_FEATURES], cur[NO_FEATURES];
    int i;

    i = 0;
    low[i++] = lowFeatures.getHcp(SPADES);                  //1
    low[i++] = lowFeatures.getHcp(HEARTS);
    low[i++] = lowFeatures.getHcp(DIAMONDS);
    low[i++] = lowFeatures.getHcp(CLUBS);
    low[i++] = lowFeatures.getHcp(ANY);

    low[i++] = lowFeatures.getControls(SPADES);             //6
    low[i++] = lowFeatures.getControls(HEARTS);
    low[i++] = lowFeatures.getControls(DIAMONDS);
    low[i++] = lowFeatures.getControls(CLUBS);
    low[i++] = lowFeatures.getControls(ANY);

    low[i++] = lowFeatures.getHc(SPADES);                   //11
    low[i++] = lowFeatures.getHc(HEARTS);
    low[i++] = lowFeatures.getHc(DIAMONDS);
    low[i++] = lowFeatures.getHc(CLUBS);
    low[i++] = lowFeatures.getHc(ANY);

    low[i++] = lowFeatures.getHonor(SPADES);                //16
    low[i++] = lowFeatures.getHonor(HEARTS);
    low[i++] = lowFeatures.getHonor(DIAMONDS);
    low[i++] = lowFeatures.getHonor(CLUBS);
    low[i++] = lowFeatures.getHonor(ANY);

    low[i++] = lowFeatures.getCountCard(SPADES, ACE);       //21
    low[i++] = lowFeatures.getCountCard(HEARTS, ACE);
    low[i++] = lowFeatures.getCountCard(DIAMONDS, ACE);
    low[i++] = lowFeatures.getCountCard(CLUBS, ACE);
    low[i++] = lowFeatures.getCountCard(ANY, ACE);

    low[i++] = lowFeatures.getCountCard(SPADES, KING);      //26
    low[i++] = lowFeatures.getCountCard(HEARTS, KING);
    low[i++] = lowFeatures.getCountCard(DIAMONDS, KING);
    low[i++] = lowFeatures.getCountCard(CLUBS, KING);
    low[i++] = lowFeatures.getCountCard(ANY, KING);

    low[i++] = lowFeatures.getCountCard(SPADES, QUEEN);     //31
    low[i++] = lowFeatures.getCountCard(HEARTS, QUEEN);
    low[i++] = lowFeatures.getCountCard(DIAMONDS, QUEEN);
    low[i++] = lowFeatures.getCountCard(CLUBS, QUEEN);
    low[i++] = lowFeatures.getCountCard(ANY, QUEEN);

    low[i++] = lowFeatures.getCountCard(SPADES, JACK);      //36
    low[i++] = lowFeatures.getCountCard(HEARTS, JACK);
    low[i++] = lowFeatures.getCountCard(DIAMONDS, JACK);
    low[i++] = lowFeatures.getCountCard(CLUBS, JACK);
    low[i++] = lowFeatures.getCountCard(ANY, JACK);

    low[i++] = lowFeatures.getCountCard(SPADES, TEN);       //41
    low[i++] = lowFeatures.getCountCard(HEARTS, TEN);
    low[i++] = lowFeatures.getCountCard(DIAMONDS, TEN);
    low[i++] = lowFeatures.getCountCard(CLUBS, TEN);
    low[i++] = lowFeatures.getCountCard(ANY, TEN);

    low[i++] = lowFeatures.getRkcb(SPADES);                 //46
    low[i++] = lowFeatures.getRkcb(HEARTS);
    low[i++] = lowFeatures.getRkcb(DIAMONDS);
    low[i++] = lowFeatures.getRkcb(CLUBS);

    low[i++] = lowFeatures.getBal();                        //50

    low[i++] = lowFeatures.getDp(SPADES);                   //51
    low[i++] = lowFeatures.getDp(HEARTS);
    low[i++] = lowFeatures.getDp(DIAMONDS);
    low[i++] = lowFeatures.getDp(CLUBS);
    low[i++] = lowFeatures.getDp(ANY);

    low[i++] = lowFeatures.getDif2(DIAMONDS, CLUBS);        //56
    low[i++] = lowFeatures.getDif2(HEARTS, CLUBS);
    low[i++] = lowFeatures.getDif2(HEARTS, DIAMONDS);
    low[i++] = lowFeatures.getDif2(SPADES, CLUBS);
    low[i++] = lowFeatures.getDif2(SPADES, DIAMONDS);
    low[i++] = lowFeatures.getDif2(SPADES, HEARTS);

    low[i++] = lowFeatures.getSuits();                      //62

    low[i++] = lowFeatures.getSuitLen(SPADES);              //63
    low[i++] = lowFeatures.getSuitLen(HEARTS);
    low[i++] = lowFeatures.getSuitLen(DIAMONDS);
    low[i++] = lowFeatures.getSuitLen(CLUBS);

    low[i++] = lowFeatures.getSuitLen2(DIAMONDS, CLUBS);    //67
    low[i++] = lowFeatures.getSuitLen2(HEARTS, CLUBS);
    low[i++] = lowFeatures.getSuitLen2(HEARTS, DIAMONDS);
    low[i++] = lowFeatures.getSuitLen2(SPADES, CLUBS);
    low[i++] = lowFeatures.getSuitLen2(SPADES, DIAMONDS);
    low[i++] = lowFeatures.getSuitLen2(SPADES, HEARTS);

    low[i++] = lowFeatures.getLMaj();                       //73

    low[i++] = lowFeatures.getLMin();                       //74

    low[i++] = lowFeatures.getLongest(SPADES);              //75
    low[i++] = lowFeatures.getLongest(HEARTS);
    low[i++] = lowFeatures.getLongest(DIAMONDS);
    low[i++] = lowFeatures.getLongest(CLUBS);

    low[i++] = lowFeatures.getLong(SPADES);                 //79
    low[i++] = lowFeatures.getLong(HEARTS);
    low[i++] = lowFeatures.getLong(DIAMONDS);
    low[i++] = lowFeatures.getLong(CLUBS);

    low[i++] = lowFeatures.getInter(SPADES);                //83
    low[i++] = lowFeatures.getInter(HEARTS);
    low[i++] = lowFeatures.getInter(DIAMONDS);
    low[i++] = lowFeatures.getInter(CLUBS);
    low[i++] = lowFeatures.getInter(ANY);

    low[i++] = lowFeatures.getLoosers(SPADES);              //88
    low[i++] = lowFeatures.getLoosers(HEARTS);
    low[i++] = lowFeatures.getLoosers(DIAMONDS);
    low[i++] = lowFeatures.getLoosers(CLUBS);
    low[i++] = lowFeatures.getLoosers(ANY);

    low[i++] = lowFeatures.getQuickTricks(SPADES);          //93
    low[i++] = lowFeatures.getQuickTricks(HEARTS);
    low[i++] = lowFeatures.getQuickTricks(DIAMONDS);
    low[i++] = lowFeatures.getQuickTricks(CLUBS);
    low[i++] = lowFeatures.getQuickTricks(ANY);

    low[i++] = lowFeatures.getPlayingTricks(SPADES);        //98
    low[i++] = lowFeatures.getPlayingTricks(HEARTS);
    low[i++] = lowFeatures.getPlayingTricks(DIAMONDS);
    low[i++] = lowFeatures.getPlayingTricks(CLUBS);
    low[i++] = lowFeatures.getPlayingTricks(ANY);

    low[i++] = lowFeatures.getStopNT(SPADES);               //103
    low[i++] = lowFeatures.getStopNT(HEARTS);
    low[i++] = lowFeatures.getStopNT(DIAMONDS);
    low[i++] = lowFeatures.getStopNT(CLUBS);

    low[i++] = lowFeatures.getQlty(SPADES);                 //107
    low[i++] = lowFeatures.getQlty(HEARTS);
    low[i++] = lowFeatures.getQlty(DIAMONDS);
    low[i++] = lowFeatures.getQlty(CLUBS);

    low[i++] = lowFeatures.getPoints(SPADES);               //111
    low[i++] = lowFeatures.getPoints(HEARTS);
    low[i++] = lowFeatures.getPoints(DIAMONDS);
    low[i++] = lowFeatures.getPoints(CLUBS);
    low[i++] = lowFeatures.getPoints(ANY);

    assert (i == NO_FEATURES);

    i = 0;
    high[i++] = highFeatures.getHcp(SPADES);
    high[i++] = highFeatures.getHcp(HEARTS);
    high[i++] = highFeatures.getHcp(DIAMONDS);
    high[i++] = highFeatures.getHcp(CLUBS);
    high[i++] = highFeatures.getHcp(ANY);

    high[i++] = highFeatures.getControls(SPADES);
    high[i++] = highFeatures.getControls(HEARTS);
    high[i++] = highFeatures.getControls(DIAMONDS);
    high[i++] = highFeatures.getControls(CLUBS);
    high[i++] = highFeatures.getControls(ANY);

    high[i++] = highFeatures.getHc(SPADES);
    high[i++] = highFeatures.getHc(HEARTS);
    high[i++] = highFeatures.getHc(DIAMONDS);
    high[i++] = highFeatures.getHc(CLUBS);
    high[i++] = highFeatures.getHc(ANY);

    high[i++] = highFeatures.getHonor(SPADES);
    high[i++] = highFeatures.getHonor(HEARTS);
    high[i++] = highFeatures.getHonor(DIAMONDS);
    high[i++] = highFeatures.getHonor(CLUBS);
    high[i++] = highFeatures.getHonor(ANY);

    high[i++] = highFeatures.getCountCard(SPADES, ACE);
    high[i++] = highFeatures.getCountCard(HEARTS, ACE);
    high[i++] = highFeatures.getCountCard(DIAMONDS, ACE);
    high[i++] = highFeatures.getCountCard(CLUBS, ACE);
    high[i++] = highFeatures.getCountCard(ANY, ACE);

    high[i++] = highFeatures.getCountCard(SPADES, KING);
    high[i++] = highFeatures.getCountCard(HEARTS, KING);
    high[i++] = highFeatures.getCountCard(DIAMONDS, KING);
    high[i++] = highFeatures.getCountCard(CLUBS, KING);
    high[i++] = highFeatures.getCountCard(ANY, KING);

    high[i++] = highFeatures.getCountCard(SPADES, QUEEN);
    high[i++] = highFeatures.getCountCard(HEARTS, QUEEN);
    high[i++] = highFeatures.getCountCard(DIAMONDS, QUEEN);
    high[i++] = highFeatures.getCountCard(CLUBS, QUEEN);
    high[i++] = highFeatures.getCountCard(ANY, QUEEN);

    high[i++] = highFeatures.getCountCard(SPADES, JACK);
    high[i++] = highFeatures.getCountCard(HEARTS, JACK);
    high[i++] = highFeatures.getCountCard(DIAMONDS, JACK);
    high[i++] = highFeatures.getCountCard(CLUBS, JACK);
    high[i++] = highFeatures.getCountCard(ANY, JACK);

    high[i++] = highFeatures.getCountCard(SPADES, TEN);
    high[i++] = highFeatures.getCountCard(HEARTS, TEN);
    high[i++] = highFeatures.getCountCard(DIAMONDS, TEN);
    high[i++] = highFeatures.getCountCard(CLUBS, TEN);
    high[i++] = highFeatures.getCountCard(ANY, TEN);

    high[i++] = highFeatures.getRkcb(SPADES);
    high[i++] = highFeatures.getRkcb(HEARTS);
    high[i++] = highFeatures.getRkcb(DIAMONDS);
    high[i++] = highFeatures.getRkcb(CLUBS);

    high[i++] = highFeatures.getBal();

    high[i++] = highFeatures.getDp(SPADES);
    high[i++] = highFeatures.getDp(HEARTS);
    high[i++] = highFeatures.getDp(DIAMONDS);
    high[i++] = highFeatures.getDp(CLUBS);
    high[i++] = highFeatures.getDp(ANY);

    high[i++] = highFeatures.getDif2(DIAMONDS, CLUBS);
    high[i++] = highFeatures.getDif2(HEARTS, CLUBS);
    high[i++] = highFeatures.getDif2(HEARTS, DIAMONDS);
    high[i++] = highFeatures.getDif2(SPADES, CLUBS);
    high[i++] = highFeatures.getDif2(SPADES, DIAMONDS);
    high[i++] = highFeatures.getDif2(SPADES, HEARTS);

    high[i++] = highFeatures.getSuits();

    high[i++] = highFeatures.getSuitLen(SPADES);
    high[i++] = highFeatures.getSuitLen(HEARTS);
    high[i++] = highFeatures.getSuitLen(DIAMONDS);
    high[i++] = highFeatures.getSuitLen(CLUBS);

    high[i++] = highFeatures.getSuitLen2(DIAMONDS, CLUBS);
    high[i++] = highFeatures.getSuitLen2(HEARTS, CLUBS);
    high[i++] = highFeatures.getSuitLen2(HEARTS, DIAMONDS);
    high[i++] = highFeatures.getSuitLen2(SPADES, CLUBS);
    high[i++] = highFeatures.getSuitLen2(SPADES, DIAMONDS);
    high[i++] = highFeatures.getSuitLen2(SPADES, HEARTS);

    high[i++] = highFeatures.getLMaj();

    high[i++] = highFeatures.getLMin();

    high[i++] = highFeatures.getLongest(SPADES);
    high[i++] = highFeatures.getLongest(HEARTS);
    high[i++] = highFeatures.getLongest(DIAMONDS);
    high[i++] = highFeatures.getLongest(CLUBS);

    high[i++] = highFeatures.getLong(SPADES);
    high[i++] = highFeatures.getLong(HEARTS);
    high[i++] = highFeatures.getLong(DIAMONDS);
    high[i++] = highFeatures.getLong(CLUBS);

    high[i++] = highFeatures.getInter(SPADES);
    high[i++] = highFeatures.getInter(HEARTS);
    high[i++] = highFeatures.getInter(DIAMONDS);
    high[i++] = highFeatures.getInter(CLUBS);
    high[i++] = highFeatures.getInter(ANY);

    high[i++] = highFeatures.getLoosers(SPADES);
    high[i++] = highFeatures.getLoosers(HEARTS);
    high[i++] = highFeatures.getLoosers(DIAMONDS);
    high[i++] = highFeatures.getLoosers(CLUBS);
    high[i++] = highFeatures.getLoosers(ANY);

    high[i++] = highFeatures.getQuickTricks(SPADES);
    high[i++] = highFeatures.getQuickTricks(HEARTS);
    high[i++] = highFeatures.getQuickTricks(DIAMONDS);
    high[i++] = highFeatures.getQuickTricks(CLUBS);
    high[i++] = highFeatures.getQuickTricks(ANY);

    high[i++] = highFeatures.getPlayingTricks(SPADES);
    high[i++] = highFeatures.getPlayingTricks(HEARTS);
    high[i++] = highFeatures.getPlayingTricks(DIAMONDS);
    high[i++] = highFeatures.getPlayingTricks(CLUBS);
    high[i++] = highFeatures.getPlayingTricks(ANY);

    high[i++] = highFeatures.getStopNT(SPADES);
    high[i++] = highFeatures.getStopNT(HEARTS);
    high[i++] = highFeatures.getStopNT(DIAMONDS);
    high[i++] = highFeatures.getStopNT(CLUBS);

    high[i++] = highFeatures.getQlty(SPADES);
    high[i++] = highFeatures.getQlty(HEARTS);
    high[i++] = highFeatures.getQlty(DIAMONDS);
    high[i++] = highFeatures.getQlty(CLUBS);

    high[i++] = highFeatures.getPoints(SPADES);
    high[i++] = highFeatures.getPoints(HEARTS);
    high[i++] = highFeatures.getPoints(DIAMONDS);
    high[i++] = highFeatures.getPoints(CLUBS);
    high[i++] = highFeatures.getPoints(ANY);

    assert (i == NO_FEATURES);

    i = 0;
    cur[i++] = getHcp(SPADES);
    cur[i++] = getHcp(HEARTS);
    cur[i++] = getHcp(DIAMONDS);
    cur[i++] = getHcp(CLUBS);
    cur[i++] = getHcp(ANY);

    cur[i++] = getControls(SPADES);
    cur[i++] = getControls(HEARTS);
    cur[i++] = getControls(DIAMONDS);
    cur[i++] = getControls(CLUBS);
    cur[i++] = getControls(ANY);

    cur[i++] = getHc(SPADES);
    cur[i++] = getHc(HEARTS);
    cur[i++] = getHc(DIAMONDS);
    cur[i++] = getHc(CLUBS);
    cur[i++] = getHc(ANY);

    cur[i++] = getHonor(SPADES);
    cur[i++] = getHonor(HEARTS);
    cur[i++] = getHonor(DIAMONDS);
    cur[i++] = getHonor(CLUBS);
    cur[i++] = getHonor(ANY);

    cur[i++] = getCountCard(SPADES, ACE);
    cur[i++] = getCountCard(HEARTS, ACE);
    cur[i++] = getCountCard(DIAMONDS, ACE);
    cur[i++] = getCountCard(CLUBS, ACE);
    cur[i++] = getCountCard(ANY, ACE);

    cur[i++] = getCountCard(SPADES, KING);
    cur[i++] = getCountCard(HEARTS, KING);
    cur[i++] = getCountCard(DIAMONDS, KING);
    cur[i++] = getCountCard(CLUBS, KING);
    cur[i++] = getCountCard(ANY, KING);

    cur[i++] = getCountCard(SPADES, QUEEN);
    cur[i++] = getCountCard(HEARTS, QUEEN);
    cur[i++] = getCountCard(DIAMONDS, QUEEN);
    cur[i++] = getCountCard(CLUBS, QUEEN);
    cur[i++] = getCountCard(ANY, QUEEN);

    cur[i++] = getCountCard(SPADES, JACK);
    cur[i++] = getCountCard(HEARTS, JACK);
    cur[i++] = getCountCard(DIAMONDS, JACK);
    cur[i++] = getCountCard(CLUBS, JACK);
    cur[i++] = getCountCard(ANY, JACK);

    cur[i++] = getCountCard(SPADES, TEN);
    cur[i++] = getCountCard(HEARTS, TEN);
    cur[i++] = getCountCard(DIAMONDS, TEN);
    cur[i++] = getCountCard(CLUBS, TEN);
    cur[i++] = getCountCard(ANY, TEN);

    cur[i++] = getRkcb(SPADES);
    cur[i++] = getRkcb(HEARTS);
    cur[i++] = getRkcb(DIAMONDS);
    cur[i++] = getRkcb(CLUBS);

    cur[i++] = getBal();

    cur[i++] = getDp(SPADES);
    cur[i++] = getDp(HEARTS);
    cur[i++] = getDp(DIAMONDS);
    cur[i++] = getDp(CLUBS);
    cur[i++] = getDp(ANY);

    cur[i++] = getDif2(DIAMONDS, CLUBS);
    cur[i++] = getDif2(HEARTS, CLUBS);
    cur[i++] = getDif2(HEARTS, DIAMONDS);
    cur[i++] = getDif2(SPADES, CLUBS);
    cur[i++] = getDif2(SPADES, DIAMONDS);
    cur[i++] = getDif2(SPADES, HEARTS);

    cur[i++] = getSuits();

    cur[i++] = getSuitLen(SPADES);
    cur[i++] = getSuitLen(HEARTS);
    cur[i++] = getSuitLen(DIAMONDS);
    cur[i++] = getSuitLen(CLUBS);

    cur[i++] = getSuitLen2(DIAMONDS, CLUBS);
    cur[i++] = getSuitLen2(HEARTS, CLUBS);
    cur[i++] = getSuitLen2(HEARTS, DIAMONDS);
    cur[i++] = getSuitLen2(SPADES, CLUBS);
    cur[i++] = getSuitLen2(SPADES, DIAMONDS);
    cur[i++] = getSuitLen2(SPADES, HEARTS);

    cur[i++] = getLMaj();

    cur[i++] = getLMin();

    cur[i++] = getLongest(SPADES);
    cur[i++] = getLongest(HEARTS);
    cur[i++] = getLongest(DIAMONDS);
    cur[i++] = getLongest(CLUBS);

    cur[i++] = getLong(SPADES);
    cur[i++] = getLong(HEARTS);
    cur[i++] = getLong(DIAMONDS);
    cur[i++] = getLong(CLUBS);

    cur[i++] = getInter(SPADES);
    cur[i++] = getInter(HEARTS);
    cur[i++] = getInter(DIAMONDS);
    cur[i++] = getInter(CLUBS);
    cur[i++] = getInter(ANY);

    cur[i++] = getLoosers(SPADES);
    cur[i++] = getLoosers(HEARTS);
    cur[i++] = getLoosers(DIAMONDS);
    cur[i++] = getLoosers(CLUBS);
    cur[i++] = getLoosers(ANY);

    cur[i++] = getQuickTricks(SPADES);
    cur[i++] = getQuickTricks(HEARTS);
    cur[i++] = getQuickTricks(DIAMONDS);
    cur[i++] = getQuickTricks(CLUBS);
    cur[i++] = getQuickTricks(ANY);

    cur[i++] = getPlayingTricks(SPADES);
    cur[i++] = getPlayingTricks(HEARTS);
    cur[i++] = getPlayingTricks(DIAMONDS);
    cur[i++] = getPlayingTricks(CLUBS);
    cur[i++] = getPlayingTricks(ANY);

    cur[i++] = getStopNT(SPADES);
    cur[i++] = getStopNT(HEARTS);
    cur[i++] = getStopNT(DIAMONDS);
    cur[i++] = getStopNT(CLUBS);

    cur[i++] = getQlty(SPADES);
    cur[i++] = getQlty(HEARTS);
    cur[i++] = getQlty(DIAMONDS);
    cur[i++] = getQlty(CLUBS);

    cur[i++] = getPoints(SPADES);
    cur[i++] = getPoints(HEARTS);
    cur[i++] = getPoints(DIAMONDS);
    cur[i++] = getPoints(CLUBS);
    cur[i++] = getPoints(ANY);

    assert (i == NO_FEATURES);

    for (int i = 0; i < NO_FEATURES; i++)
        if ((low[i] > cur[i]) || (high[i] < cur[i]))
            return (i + 1);

    return 0;
}

void CFeatures::delimitFeatures(CFeatures &features, bool lower)
{
    //High Card Points.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getHcp((Suit)i);
        int newVal = features.getHcp((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setHcp((Suit)i, curVal);
    }

    //Weighted count of aces and kings.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getControls((Suit)i);
        int newVal = features.getControls((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setControls((Suit)i, curVal);
    }

    //Count of aces, kings and queens.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getHc((Suit)i);
        int newVal = features.getHc((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setHc((Suit)i, curVal);
    }

    //Count of cards higher than 9.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getHonor((Suit)i);
        int newVal = features.getHonor((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setHonor((Suit)i, curVal);
    }

    //Count of aces.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getCountCard((Suit)i, ACE);
        int newVal = features.getCountCard((Suit)i, ACE);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setCountCard((Suit)i, ACE, curVal);
    }

    //Count of kings.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getCountCard((Suit)i, KING);
        int newVal = features.getCountCard((Suit)i, KING);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setCountCard((Suit)i, KING, curVal);
    }

    //Count of queens.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getCountCard((Suit)i, QUEEN);
        int newVal = features.getCountCard((Suit)i, QUEEN);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setCountCard((Suit)i, QUEEN, curVal);
    }

    //Count of jacks.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getCountCard((Suit)i, JACK);
        int newVal = features.getCountCard((Suit)i, JACK);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setCountCard((Suit)i, JACK, curVal);
    }

    //Count of tens.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getCountCard((Suit)i, TEN);
        int newVal = features.getCountCard((Suit)i, TEN);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setCountCard((Suit)i, TEN, curVal);
    }

    //Count of aces in the whole hand + the count of kings in the suit.
    for (int i = 0; i < 4; i++)
    {
        int curVal = getRkcb((Suit)i);
        int newVal = features.getRkcb((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setRkcb((Suit)i, curVal);
    }

    //How balanced is the hand?
    {
        int curVal = getBal();
        int newVal = features.getBal();
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setBal(curVal);
    }

    //Distribution points.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getDp((Suit)i);
        int newVal = features.getDp((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setDp((Suit)i, curVal);
    }

    //Difference in length between 2 suits.
    for (int i = 0; i < 3; i++)
    for (int j = i + 1; j < 4; j++)
    {
        int curVal = getDif2((Suit)j, (Suit)i);
        int newVal = features.getDif2((Suit)j, (Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setDif2((Suit)j, (Suit)i, curVal);
    }

    //Number of suits that are longer than 3.
    {
        int curVal = getSuits();
        int newVal = features.getSuits();
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setSuits(curVal);
    }

    //length of a suit.
    for (int i = 0; i < 4; i++)
    {
        int curVal = getSuitLen((Suit)i);
        int newVal = features.getSuitLen((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setSuitLen((Suit)i, curVal);
    }

    //Total length of two suits.
    for (int i = 0; i < 3; i++)
    for (int j = i + 1; j < 4; j++)
    {
        int curVal = getSuitLen2((Suit)j, (Suit)i);
        int newVal = features.getSuitLen2((Suit)j, (Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setSuitLen2((Suit)j, (Suit)i, curVal);
    }

    //Length of the longer major suit.
    {
        int curVal = getLMaj();
        int newVal = features.getLMaj();
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setLMaj(curVal);
    }

    //Length of the longer minor suit.
    {
        int curVal = getLMin();
        int newVal = features.getLMin();
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setLMin(curVal);
    }

    //Is the suit the longest in the hand?
    for (int i = 0; i < 4; i++)
    {
        int curVal = getLongest((Suit)i);
        int newVal = features.getLongest((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setLongest((Suit)i, curVal);
    }

    //Is the suit not shortest in the hand?
    for (int i = 0; i < 4; i++)
    {
        int curVal = getLong((Suit)i);
        int newVal = features.getLong((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setLong((Suit)i, curVal);
    }

    //Weighted count of intermediate cards.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getInter((Suit)i);
        int newVal = features.getInter((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setInter((Suit)i, curVal);
    }

    //Expected number of losers.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getLoosers((Suit)i);
        int newVal = features.getLoosers((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setLoosers((Suit)i, curVal);
    }

    //Quick tricks.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getQuickTricks((Suit)i);
        int newVal = features.getQuickTricks((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setQuickTricks((Suit)i, curVal);
    }

    //Playing tricks.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getPlayingTricks((Suit)i);
        int newVal = features.getPlayingTricks((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setPlayingTricks((Suit)i, curVal);
    }

    //Stopper quality.
    for (int i = 0; i < 4; i++)
    {
        int curVal = getStopNT((Suit)i);
        int newVal = features.getStopNT((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setStopNT((Suit)i, curVal);
    }

    //General quality assessment.
    for (int i = 0; i < 4; i++)
    {
        int curVal = getQlty((Suit)i);
        int newVal = features.getQlty((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setQlty((Suit)i, curVal);
    }

    //High Card Points + Distribution Points.
    for (int i = -1; i < 4; i++)
    {
        int curVal = getPoints((Suit)i);
        int newVal = features.getPoints((Suit)i);
        if ((lower && (newVal > curVal)) || (!lower && newVal < curVal))
            curVal = newVal;
        setPoints((Suit)i, curVal);
    }
}

/**
 * @brief Get the High Card Points.
 */
int CFeatures::getHcp(Suit suit)
{
    assert (suit != NOTRUMP);

    int shift[] = { 16, 0, 4, 8, 12};
    int mask[] = { 0x3f, 0xf, 0xf, 0xf, 0xf};

    int val = (hcpA & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxHcp(suit))
        val = getMaxHcp(suit);

    return val;
}

/**
 * @brief Set the High Card Points.
 */
void CFeatures::setHcp(Suit suit, int hcp)
{
    assert (suit != NOTRUMP);

    assert ((hcp >= 0) && (hcp <= getMaxHcp(suit)));

    int shift[] = { 16, 0, 4, 8, 12};
    int mask[] = { 0x3f, 0xf, 0xf, 0xf, 0xf};

    hcpA = (hcpA & (~(mask[suit + 1] << shift[suit + 1]))) | (hcp << shift[suit + 1]);
}

/**
 * @brief Get the weight of aces and kings.
 */
int CFeatures::getControls(Suit suit)
{
    assert (suit != NOTRUMP);

    int shift[] = { 8, 0, 2, 4, 6};
    int mask[] = { 0xf, 0x3, 0x3, 0x3, 0x3};

    int val = (controlsA & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxControls(suit))
        val = getMaxControls(suit);

    return val;
}

/**
 * @brief Set the weight of aces and kings.
 */
void CFeatures::setControls(Suit suit, int controls)
{
    assert (suit != NOTRUMP);

    assert ((controls >= 0) && (controls <= getMaxControls(suit)));

    int shift[] = { 8, 0, 2, 4, 6};
    int mask[] = { 0xf, 0x3, 0x3, 0x3, 0x3};

    controlsA = (controlsA & (~(mask[suit + 1] << shift[suit + 1]))) | (controls << shift[suit + 1]);
}

/**
 * @brief Get count of aces and kings and queens.
 */
int CFeatures::getHc(Suit suit)
{
    assert (suit != NOTRUMP);

    int shift[] = { 8, 0, 2, 4, 6};
    int mask[] = { 0xf, 0x3, 0x3, 0x3, 0x3};

    int val = (hcA & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxHc(suit))
        val = getMaxHc(suit);

    return val;
}

/**
 * @brief Set count of aces and kings and queens.
 */
void CFeatures::setHc(Suit suit, int hc)
{
    assert (suit != NOTRUMP);

    assert ((hc >= 0) && (hc <= getMaxHc(suit)));

    int shift[] = { 8, 0, 2, 4, 6};
    int mask[] = { 0xf, 0x3, 0x3, 0x3, 0x3};

    hcA = (hcA & (~(mask[suit + 1] << shift[suit + 1]))) | (hc << shift[suit + 1]);
}

/**
 * @brief Get count of cards higher than nine.
 */
int CFeatures::getHonor(Suit suit)
{
    assert (suit != NOTRUMP);

    int shift[] = { 12, 0, 3, 6, 9};
    int mask[] = { 0xf, 0x7, 0x7, 0x7, 0x7};

    int val = (honorA & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxHonor(suit))
        val = getMaxHonor(suit);

    return val;
}

/**
 * @brief Set count of cards higher than nine.
 */
void CFeatures::setHonor(Suit suit, int honor)
{
    assert (suit != NOTRUMP);
    assert ((honor >= 0) && (honor <= getMaxHonor(suit)));

    int shift[] = { 12, 0, 3, 6, 9};
    int mask[] = { 0xf, 0x7, 0x7, 0x7, 0x7};

    honorA = (honorA & (~(mask[suit + 1] << shift[suit + 1]))) | (honor << shift[suit + 1]);
}

/**
 * @brief Get count of given card (ace, king, queen, jack or ten).
 */
int CFeatures::getCountCard(Suit suit, int card)
{
    assert (suit != NOTRUMP);
    assert ((card == ACE) || (card == KING) || (card == QUEEN) || (card == JACK) || (card == TEN));

    int shift[] = { 4, 0, 1, 2, 3};
    int mask[] = { 0x7, 0x1, 0x1, 0x1, 0x1};

    int count = (card == ACE) ? aceA : (card == KING) ? kingA : (card == QUEEN) ? queenA :
                   (card == JACK) ? jackA : tenA;

    int val = (count & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxCountCard(suit))
        val = getMaxCountCard(suit);

    return val;
}

/**
 * @brief Set count of given card (ace, king, queen, jack or ten).
 */
void CFeatures::setCountCard(Suit suit, int card, int count)
{
    assert (suit != NOTRUMP);
    assert ((card == ACE) || (card == KING) || (card == QUEEN) || (card == JACK) || (card == TEN));
    assert ((count >= 0) && (count <= getMaxCountCard(suit)));

    int shift[] = { 4, 0, 1, 2, 3};
    int mask[] = { 0x7, 0x1, 0x1, 0x1, 0x1};

    if (card == ACE)
        aceA = (aceA & (~(mask[suit + 1] << shift[suit + 1]))) | (count << shift[suit + 1]);
    else if (card == KING)
        kingA = (kingA & (~(mask[suit + 1] << shift[suit + 1]))) | (count << shift[suit + 1]);
    else if (card == QUEEN)
        queenA = (queenA & (~(mask[suit + 1] << shift[suit + 1]))) | (count << shift[suit + 1]);
    else if (card == JACK)
        jackA = (jackA & (~(mask[suit + 1] << shift[suit + 1]))) | (count << shift[suit + 1]);
    else
        tenA = (tenA & (~(mask[suit + 1] << shift[suit + 1]))) | (count << shift[suit + 1]);
}

/**
 * @brief Get count of aces + kings.
 */
int CFeatures::getRkcb(Suit suit)
{
    assert (suit != NOTRUMP);
    assert(suit != ANY);

    int shift[] = { 0, 3, 6, 9};
    int mask[] = { 0x7, 0x7, 0x7, 0x7};

    int val = (rkcbS & (mask[suit] << shift[suit])) >> shift[suit];
    if (val < 0)
        val = 0;
    if (val > getMaxRkcb())
        val = getMaxRkcb();

    return val;
}

/**
 * @brief Set count of aces + kings.
 */
void CFeatures::setRkcb(Suit suit, int rkcb)
{
    assert (suit != NOTRUMP);
    assert ((suit != ANY) && (rkcb >= 0) && (rkcb <= getMaxRkcb()));

    int shift[] = { 0, 3, 6, 9};
    int mask[] = { 0x7, 0x7, 0x7, 0x7};

    rkcbS = (rkcbS & (~(mask[suit] << shift[suit]))) | (rkcb << shift[suit]);
}

/**
 * @brief Get how balanced.
 */
int CFeatures::getBal()
{
    int val = bal;

    if (val < 0)
        val = 0;
    if (val > getMaxBal())
        val = getMaxBal();

    return val;
}

/**
 * @brief Set how balanced.
 */
void CFeatures::setBal(int bal)
{
    assert ((bal >= 0) && (bal <= getMaxBal()));

    this->bal = bal;
}

/**
 * @brief Get distribution points.
 */
int CFeatures::getDp(Suit suit)
{
    if (suit == NOTRUMP)
        suit = ANY;

    int shift[] = { 16, 0, 4, 8, 12};
    int mask[] = { 0xf, 0xf, 0xf, 0xf, 0xf};

    int val = (dpA & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxDp())
        val = getMaxDp();

    return val;
}

/**
 * @brief Set distribution points.
 */
void CFeatures::setDp(Suit suit, int dP)
{
    if (suit == NOTRUMP)
        suit = ANY;

    assert ((dP >= 0) && (dP <= getMaxDp()));

    int shift[] = { 16, 0, 4, 8, 12};
    int mask[] = { 0xf, 0xf, 0xf, 0xf, 0xf};

    dpA = (dpA & (~(mask[suit + 1] << shift[suit + 1]))) | (dP << shift[suit + 1]);
}

/**
 * @brief Get difference in length between two given suits.
 */
int CFeatures::getDif2(Suit suit1, Suit suit2)
{
    assert ((suit1 != NOTRUMP) && (suit2 != NOTRUMP));
    assert ((suit1 != ANY) && (suit2 != ANY) && (suit1 != suit2));

    int shift[] = { 0, 5, 10, 15, 20, 25};
    int mask[] = { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};

    bool signReverse = (suit1 < suit2);
    if (signReverse)
    {
        Suit a = suit1;
        suit1 = suit2;
        suit2 = a;
    }
    int inx = suit1 + suit2 - 1;
    if (suit1 == SPADES)
        inx++;

    int val = (int)((dif2S & (mask[inx] << shift[inx])) >> shift[inx]) - 13;
    if (val < -getMaxDif2())
        val = -getMaxDif2();
    if (val > getMaxDif2())
        val = getMaxDif2();

    return (signReverse) ? -val : val;
}

/**
 * @brief Set difference in length between two given suits.
 */
void CFeatures::setDif2(Suit suit1, Suit suit2, int dif)
{
    assert ((suit1 != NOTRUMP) && (suit2 != NOTRUMP));
    assert ((suit1 != ANY) && (suit2 != ANY) && (suit1 != suit2) && (dif >= -getMaxDif2()) && (dif <= getMaxDif2()));

    int shift[] = { 0, 5, 10, 15, 20, 25};
    int mask[] = { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};

    bool signReverse = (suit1 < suit2);
    if (signReverse)
    {
        Suit a = suit1;
        suit1 = suit2;
        suit2 = a;
        dif = -dif;
    }
    int inx = suit1 + suit2 - 1;
    if (suit1 == SPADES)
        inx++;

    dif2S = (dif2S & (~(mask[inx] << shift[inx]))) | ((dif + 13) << shift[inx]);
}

/**
 * @brief Get number of suits longer than 3.
 */
int CFeatures::getSuits()
{
    int val = suits;
    if (val < 0)
        val = 0;
    if (val > getMaxSuits())
        val = getMaxSuits();

    return val;
}

/**
 * @brief Set number of suits longer than 3.
 */
void CFeatures::setSuits(int suits)
{
    assert ((suits >= 0) && (suits <= getMaxSuits()));

    this->suits = suits;
}

/**
 * @brief Get the length of a given suit.
 */
int CFeatures::getSuitLen(Suit suit)
{
    assert (suit != NOTRUMP);
    assert (suit != ANY);

    int shift[] = { 0, 4, 8, 12};
    int mask[] = { 0xf, 0xf, 0xf, 0xf};

    int val = (suitLenS & (mask[suit] << shift[suit])) >> shift[suit];
    if (val < 0)
        val = 0;
    if (val > getMaxSuitLen())
        val = getMaxSuitLen();

    return val;
}

/**
 * @brief Set the length of a given suit.
 */
void CFeatures::setSuitLen(Suit suit, int suitLen)
{
    assert (suit != NOTRUMP);
    assert ((suit != ANY) && (suitLen >= 0) && (suitLen <= getMaxSuitLen()));

    int shift[] = { 0, 4, 8, 12};
    int mask[] = { 0xf, 0xf, 0xf, 0xf};

    suitLenS = (suitLenS & (~(mask[suit] << shift[suit]))) | (suitLen << shift[suit]);
}

/**
 * @brief Get the total length of two givne suits.
 */
int CFeatures::getSuitLen2(Suit suit1, Suit suit2)
{
    assert ((suit1 != NOTRUMP) && (suit2 != NOTRUMP));
    assert ((suit1 != ANY) && (suit2 != ANY) && (suit1 != suit2));

    int shift[] = { 0, 4, 8, 12, 16, 20};
    int mask[] = { 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};

    int inx = suit1 + suit2 - 1;
    if ((suit1 == SPADES) || (suit2 == SPADES))
        inx++;

    int val = (int)((suitLen2S & (mask[inx] << shift[inx])) >> shift[inx]);
    if (val < 0)
        val = 0;
    if (val > getMaxSuitLen2())
        val = getMaxSuitLen2();

    return val;
}

/**
 * @brief Set the total length of two givne suits.
 */
void CFeatures::setSuitLen2(Suit suit1, Suit suit2, int suitLen2)
{
    assert ((suit1 != NOTRUMP) && (suit2 != NOTRUMP));
    assert ((suit1 != ANY) && (suit2 != ANY) && (suit1 != suit2) && (suitLen2 >= 0) && (suitLen2 <= getMaxSuitLen2()));

    int shift[] = { 0, 4, 8, 12, 16, 20};
    int mask[] = { 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};

    int inx = suit1 + suit2 - 1;
    if ((suit1 == SPADES) || (suit2 == SPADES))
        inx++;

    suitLen2S = (suitLen2S & (~(mask[inx] << shift[inx]))) | (suitLen2 << shift[inx]);
}

/**
 * @brief Get the length of the longer major.
 */
int CFeatures::getLMaj()
{
    int val = lMaj;
    if (val < 0)
        val = 0;
    if (val > getMaxLMaj())
        val = getMaxLMaj();

    return val;
}

/**
 * @brief Set the length of the longer major.
 */
void CFeatures::setLMaj(int lMaj)
{
    assert((lMaj >= 0) && (lMaj <= getMaxLMaj()));

    this->lMaj = lMaj;
}

/**
 * @brief Get the length of the longer minor.
 */
int CFeatures::getLMin()
{
    int val = lMin;
    if (val < 0)
        val = 0;
    if (val > getMaxLMin())
        val = getMaxLMin();

    return val;
}

/**
 * @brief Set the length of the longer minor.
 */
void CFeatures::setLMin(int lMin)
{
    assert((lMin >= 0) && (lMin <= getMaxLMin()));

    this->lMin = lMin;
}

/**
 * @brief Is the given suit the longest in the hand?
 */
int CFeatures::getLongest(Suit suit)
{
    assert (suit != NOTRUMP);
    assert (suit != ANY);

    int shift[] = { 0, 1, 2, 3};
    int mask[] = { 0x1, 0x1, 0x1, 0x1};

    int val = (longestS & (mask[suit] << shift[suit])) >> shift[suit];
    if (val < 0)
        val = 0;
    if (val > getMaxLongest())
        val = getMaxLongest();

    return val;
}

/**
 * @brief Set the given suit the longest in the hand.
 */
void CFeatures::setLongest(Suit suit, int longest)
{
    assert (suit != NOTRUMP);
    assert ((suit != ANY) && (longest >= 0) && (longest <= getMaxLongest()));

    int shift[] = { 0, 1, 2, 3};
    int mask[] = { 0x1, 0x1, 0x1, 0x1};

    longestS = (longestS & (~(mask[suit] << shift[suit]))) | (longest << shift[suit]);
}

/**
 * @brief Is the given suit not shortest in the hand?
 */
int CFeatures::getLong(Suit suit)
{
    assert (suit != NOTRUMP);
    assert (suit != ANY);

    int shift[] = { 0, 1, 2, 3};
    int mask[] = { 0x1, 0x1, 0x1, 0x1};

    int val = (longS & (mask[suit] << shift[suit])) >> shift[suit];
    if (val < 0)
        val = 0;
    if (val > getMaxLong())
        val = getMaxLong();

    return val;
}

/**
 * @brief Set the given suit not shortest in the hand.
 */
void CFeatures::setLong(Suit suit, int lng)
{
    assert (suit != NOTRUMP);
    assert ((suit != ANY) && (lng >= 0) && (lng <= getMaxLong()));

    int shift[] = { 0, 1, 2, 3};
    int mask[] = { 0x1, 0x1, 0x1, 0x1};

    longS = (longS & (~(mask[suit] << shift[suit]))) | (lng << shift[suit]);
}

/**
 * @brief Get weighed count intermediate cards.
 */
int CFeatures::getInter(Suit suit)
{
    assert (suit != NOTRUMP);

    int shift[] = { 12, 0, 3, 6, 9};
    int mask[] = { 0x1f, 0x7, 0x7, 0x7, 0x7};

    int val = (interA & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxInter(suit))
        val = getMaxInter(suit);

    return val;
}

/**
 * @brief Set weighed count intermediate cards.
 */
void CFeatures::setInter(Suit suit, int inter)
{
    assert (suit != NOTRUMP);

    assert ((inter >= 0) && (inter <= getMaxInter(suit)));

    int shift[] = { 12, 0, 3, 6, 9};
    int mask[] = { 0x1f, 0x7, 0x7, 0x7, 0x7};

    interA = (interA & (~(mask[suit + 1] << shift[suit + 1]))) | (inter << shift[suit + 1]);
}

/**
 * @brief Get loosers.
 */
int CFeatures::getLoosers(Suit suit)
{
    assert (suit != NOTRUMP);

    int shift[] = { 8, 0, 2, 4, 6};
    int mask[] = { 0xf, 0x3, 0x3, 0x3, 0x3};

    int val = (losersA & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxLoosers(suit))
        val = getMaxLoosers(suit);

    return val;
}

/**
 * @brief Set loosers.
 */
void CFeatures::setLoosers(Suit suit, int loosers)
{
    assert (suit != NOTRUMP);

    assert ((loosers >= 0) && (loosers <= getMaxLoosers(suit)));

    int shift[] = { 8, 0, 2, 4, 6};
    int mask[] = { 0xf, 0x3, 0x3, 0x3, 0x3};

    losersA = (losersA & (~(mask[suit + 1] << shift[suit + 1]))) | (loosers << shift[suit + 1]);
}

/**
 * @brief Get quick tricks.
 */
int CFeatures::getQuickTricks(Suit suit)
{    
    assert (suit != NOTRUMP);

    int shift[] = { 12, 0, 3, 6, 9};
    int mask[] = { 0x1f, 0x7, 0x7, 0x7, 0x7};

    int val = (quickTricksA & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxQuickTricks(suit))
        val = getMaxQuickTricks(suit);

    return val;
}

/**
 * @brief Set quick tricks.
 */
void CFeatures::setQuickTricks(Suit suit, int quickTricks)
{
    assert (suit != NOTRUMP);

    assert ((quickTricks >= 0) && (quickTricks <= getMaxQuickTricks(suit)));

    int shift[] = { 12, 0, 3, 6, 9};
    int mask[] = { 0x1f, 0x7, 0x7, 0x7, 0x7};

    quickTricksA = (quickTricksA & (~(mask[suit + 1] << shift[suit + 1]))) | (quickTricks << shift[suit + 1]);
}

/**
 * @brief Get playing tricks.
 */
int CFeatures::getPlayingTricks(Suit suit)
{
    if (suit == NOTRUMP)
        suit = ANY;

    int shift[] = {20, 0, 5, 10, 15};
    int mask[] = { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};

    int val = (playingTricksS & (mask[suit + 1] << shift[suit + 1])) >> shift[suit + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxPlayingTricks())
        val = getMaxPlayingTricks();

    return val;
}

/**
 * @brief Set playing tricks.
 */
void CFeatures::setPlayingTricks(Suit suit, int playingTricks)
{
    if (suit == NOTRUMP)
        suit = ANY;

    assert ((playingTricks >= 0) && (playingTricks <= getMaxPlayingTricks()));

    int shift[] = {20, 0, 5, 10, 15};
    int mask[] = { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};

    playingTricksS = (playingTricksS & (~(mask[suit + 1] << shift[suit + 1]))) | (playingTricks << shift[suit + 1]);
}

/**
 * @brief Get stopper quality for a given suit.
 */
int CFeatures::getStopNT(Suit suit)
{
    assert (suit != NOTRUMP);
    assert (suit != ANY);

    int shift[] = { 0, 3, 6, 9};
    int mask[] = { 0x7, 0x7, 0x7, 0x7};

    int val =  (stopNTS & (mask[suit] << shift[suit])) >> shift[suit];
    if (val < 0)
        val = 0;
    if (val > getMaxStopNT())
        val = getMaxStopNT();

    return val;
}

/**
 * @brief Set stopper quality for a given suit.
 */
void CFeatures::setStopNT(Suit suit, int stopNT)
{
    assert (suit != NOTRUMP);
    assert ((suit != ANY) && (stopNT >= 0) && (stopNT <= getMaxStopNT()));

    int shift[] = { 0, 3, 6, 9};
    int mask[] = { 0x7, 0x7, 0x7, 0x7};

    stopNTS = (stopNTS & (~(mask[suit] << shift[suit]))) | (stopNT << shift[suit]);
}

/**
 * @brief Get general quality assesment for a given suit.
 */
int CFeatures::getQlty(Suit suit)
{
    assert (suit != NOTRUMP);
    assert (suit != ANY);

    int shift[] = { 0, 4, 8, 12};
    int mask[] = { 0xf, 0xf, 0xf, 0xf};

    int val = (qltyS & (mask[suit] << shift[suit])) >> shift[suit];
    if (val < 0)
        val = 0;
    if (val > getMaxQlty())
        val = getMaxQlty();

    return val;
}

/**
 * @brief Set general quality assesment for a given suit.
 */
void CFeatures::setQlty(Suit suit, int qlty)
{
    assert (suit != NOTRUMP);
    assert ((suit != ANY) && (qlty >= 0) && (qlty <= getMaxQlty()));

    int shift[] = { 0, 4, 8, 12};
    int mask[] = { 0xf, 0xf, 0xf, 0xf};

    qltyS = (qltyS & (~(mask[suit] << shift[suit]))) | (qlty << shift[suit]);
}

/**
 * @brief Get high card points + distribution points for a given trump suit.
 */
int CFeatures::getPoints(Suit trump)
{
    if (trump == NOTRUMP)
        trump = ANY;

    int shift[] = { 24, 0, 6, 12, 18};
    int mask[] = { 0x3f, 0x3f, 0x3f, 0x3f, 0x3f};

    int val = (pointsS & (mask[trump + 1] << shift[trump + 1])) >> shift[trump + 1];
    if (val < 0)
        val = 0;
    if (val > getMaxPoints())
        val = getMaxPoints();

    return val;
}

int CFeatures::getExtPoints(Suit trump, bool low)
{
    if (low)
    {
        if ((getPoints(trump) == 0) && (getPoints(NOTRUMP) == 0))
            return getHcp(ANY);

        else if ((getPoints(trump) == 0) && (getPoints(NOTRUMP) != 0))
            return getPoints(NOTRUMP);

        else
            return getPoints(trump);
    }
    else
    {

    if ((getPoints(trump) == getMaxPoints()) && (getPoints(NOTRUMP) == getMaxPoints()))
        return getHcp(ANY);

    else if ((getPoints(trump) == getMaxPoints()) && (getPoints(NOTRUMP) != getMaxPoints()))
        return getPoints(NOTRUMP);

    else
        return getPoints(trump);
    }
}

/**
 * @brief Set high card points + distribution points for a given trump suit.
 */
void CFeatures::setPoints(Suit trump, int points)
{
    if (trump == NOTRUMP)
        trump = ANY;

    assert ((points >= 0) && (points <= getMaxPoints()));

    int shift[] = { 24, 0, 6, 12, 18};
    int mask[] = { 0x3f, 0x3f, 0x3f, 0x3f, 0x3f};

    pointsS = (pointsS & (~(mask[trump + 1] << shift[trump + 1]))) | (points << shift[trump + 1]);
}

void CFeatures::updPoints(Suit trump, bool low)
{
    int points = getExtPoints(trump, low);
    setPoints(trump, points);
}

int CFeatures::calcWeight(Suit suit, int firstCard, int cards[])
{
    int weight = 0;

    int top = SUIT_INTERVAL[suit][1];
    int bottom = SUIT_INTERVAL[suit][0] + firstCard -1;

    for (int i = 0; i < 13; i++)
    {
        int card = cards[i];
        if ((card > bottom) && (card <= top))
            weight += (card - bottom);
    }

    return weight;
}

int CFeatures::calcCount(Suit suit, int low, int high, int cards[])
{
    int count = 0;

    int top = SUIT_INTERVAL[suit][0] + high;
    int bottom = SUIT_INTERVAL[suit][0] + low;

    assert ((top >= bottom) && (top <= SUIT_INTERVAL[suit][1]) && (bottom >= SUIT_INTERVAL[suit][0]));

    for (int i = 0; i < 13; i++)
    {
        int card = cards[i];
        if ((card >= bottom) && (card <= top))
            count++;
    }

    return count;
}

int CFeatures::calcRkcb(Suit suit, int cards[])
{
    int rkcb = 0;

    int aceC = SUIT_INTERVAL[CLUBS][0] + ACE;
    int aceD = SUIT_INTERVAL[DIAMONDS][0] + ACE;
    int aceH = SUIT_INTERVAL[HEARTS][0] + ACE;
    int aceS = SUIT_INTERVAL[SPADES][0] + ACE;
    int king = SUIT_INTERVAL[suit][0] + KING;

    for (int i = 0; i < 13; i++)
    {
        int card = cards[i];
        if ((card == aceC) || (card == aceD) || (card == aceH) || (card == aceS) || (card == king))
            rkcb++;
    }

    return rkcb;
}

int CFeatures::calcBal(int count[])
{
    QVector<int> countV;
    int bal;

    for (int i = 0; i < 4; i++)
        countV.append(count[i]);

    qSort(countV.begin(), countV.end());

    if (countV[0] == 3)
        bal = 0;
    else if ((countV[0] == 2) && (countV[3] == 4))
        bal = 1;
    else if ((countV[0] == 2) && (countV[1] == 3) && (countV[3] == 5))
        bal = 2;
    else if (countV[0] == 2)
        bal = 3;
    else if ((countV[0] == 1) && (countV[3] == 5))
        bal = 4;
    else if (countV[0] == 1)
        bal = 5;
    else if (countV[1] >= 3)
        bal = 6;
    else if (countV[1] >= 1)
        bal = 7;
    else
        bal = 8;

    return bal;
}

/**
 * @brief operator << Serialization of features.
 */
QDataStream &operator<<(QDataStream &out, const CFeatures &features)
{
    out<<features.hcpA<<features.controlsA<<features.hcA<<features.honorA<<features.aceA;
    out<<features.kingA<<features.queenA<<features.jackA<<features.tenA<<features.rkcbS<<features.bal;
    out<<features.dpA<<features.dif2S<<features.suits<<features.suitLenS;
    out<<features.suitLen2S<<features.lMaj<<features.lMin<<features.longestS;
    out<<features.longS<<features.interA<<features.losersA<<features.quickTricksA;
    out<<features.playingTricksS<<features.stopNTS<<features.qltyS<<features.pointsS;

    return out;
}

/**
 * @brief operator >> Serialization of features.
 */
QDataStream &operator>>(QDataStream &in, CFeatures &features)
{
    in>>features.hcpA>>features.controlsA>>features.hcA>>features.honorA>>features.aceA;
    in>>features.kingA>>features.queenA>>features.jackA>>features.tenA>>features.rkcbS>>features.bal;
    in>>features.dpA>>features.dif2S>>features.suits>>features.suitLenS;
    in>>features.suitLen2S>>features.lMaj>>features.lMin>>features.longestS;
    in>>features.longS>>features.interA>>features.losersA>>features.quickTricksA;
    in>>features.playingTricksS>>features.stopNTS>>features.qltyS>>features.pointsS;

    return in;
}
