/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CFeatures.h
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the features part of the bid database.
 */

#ifndef CFEATURES_H
#define CFEATURES_H

#include <QtGlobal>
#include <QDataStream>

#include "defines.h"

/**
 * @brief The class handles the features for a bridge hand.
 *
 * Heavily inspired by:\n
 *
 * Learning to Bid in Bridge\n
 *  1. Authors:
 *     Asaf Amit,
 *     Shaul Markovitch
 *     Department of Computer Science
 *     Technion---Israel Institute of Technology
 *     Technion City, Haifa, Israel 32000
 *  2. published in:
 *     Journal of Machine Learning
 *     Volume 63 Issue 3, June 2006
 *     Pages 287 - 327
 *     Kluwer Academic Publishers Hingham, MA, USA
 *
 * The class list feature classes. For each feature class, we mark
 * the variations this class takes. The possible variations are:\n
 *  1. S = {♠,♥,♦, ♣}. One feature instance for each of the four suits.
 *  2. A = {♠,♥,♦,♣,♠∪♥∪♦∪♣}. Five instances – one for each suit and one for the whole hand.
 *  3. TS = {<s1, s2>|s1, s2 ∈ S}. One feature instance for each pair of suits, the first of
 *     which is the trump suit, and the second, which can be any suit (including the trump).
 *     16 feature instances in all.
 *  4. TA = {<s1, s2>|s1 ∈ S, s2 ∈ A}. One feature instance for each pair of suits, the first of
 *     which is the trump suit, and the second, which can be any suit (including the trump)
 *     or the whole hand. 20 feature instances in all.
 *  5. T∗A. Same as above but s1 can also be NT.
 *  6. 2S = {<s1, s2>|s1, s2 ∈ S, s1 != s2}. One feature instance for each combination of two
 *     different suits (ordered). 12 feature instances in all.
 */
class CFeatures
{
public:
    CFeatures();

    //Serialization.
    friend QDataStream &operator<<(QDataStream &out, const CFeatures &features);
    friend QDataStream &operator>>(QDataStream &in, CFeatures &features);

    //Set features.
    void setMinFeatures();              //Min. features.
    void setMaxFeatures();              //Max features.
    void setCardFeatures(int cards[]);  //Features for given hand.
    int featureIsOk(CFeatures &highFeatures, CFeatures &lowFeatures);
    void delimitFeatures(CFeatures &features, bool lower);

    //High card points.
    int getHcp(Suit suit);
    int getMaxHcp(Suit suit) { return (suit == ANY) ? 37 : 10;}
    void setHcp(Suit suit, int hcp);

    //Weighed  count of aces and kings.
    int getControls(Suit suit);
    int getMaxControls(Suit suit) { return (suit == ANY) ? 12 : 3;}
    void setControls(Suit suit, int controls);

    //Count of aces, kings and queens.
    int getHc(Suit suit);
    int getMaxHc(Suit suit) { return (suit == ANY) ? 12 : 3;}
    void setHc(Suit suit, int hc);

    //Count of cards higher than 9.
    int getHonor(Suit suit);
    int getMaxHonor(Suit suit) { return (suit == ANY) ? 13 : 5;}
    void setHonor(Suit suit, int honor);

    //Count of aces, kings, queens, jacks and tens.
    int getCountCard(Suit suit, int card);
    int getMaxCountCard(Suit suit) { return (suit == ANY) ? 4 : 1;}
    void setCountCard(Suit suit, int card, int count);

    //Count of aces in hand + count of kings in suit.
    int getRkcb(Suit suit);
    int getMaxRkcb() { return 5;}
    void setRkcb(Suit suit, int rkcb);

    //How balanced is the hand?
    int getBal();
    int getMaxBal() { return 9;}
    void setBal(int bal);

    //Distribution points.
    int getDp(Suit suit);
    int getMaxDp() { return 9;}
    void setDp(Suit suit, int dP);

    //Difference in length between two suits.
    int getDif2(Suit suit1, Suit suit2);
    int getMaxDif2() {return 13;}
    void setDif2(Suit suit1, Suit suit2, int dif);

    //Number of suits longer than 3.
    int getSuits();
    int getMaxSuits() { return 3;}
    void setSuits(int suits);

    //Length of suit.
    int getSuitLen(Suit suit);
    int getMaxSuitLen() { return 13;}
    void setSuitLen(Suit suit, int suitLen);

    //Total length of two suits.
    int getSuitLen2(Suit suit1, Suit suit2);
    int getMaxSuitLen2() { return 13;}
    void setSuitLen2(Suit suit1, Suit suit2, int suitLen2);

    //Length of longer major.
    int getLMaj();
    int getMaxLMaj() { return 13;}
    void setLMaj(int lMaj);

    //Length of longer minor.
    int getLMin();
    int getMaxLMin() { return 13;}
    void setLMin(int lMin);

    //Is the suit the longest in the hand.
    int getLongest(Suit suit);
    int getMaxLongest() { return 1;}
    void setLongest(Suit suit , int longest);

    //Is the suit not shortest in the hand.
    int getLong(Suit suit);
    int getMaxLong() { return 1;}
    void setLong(Suit suit , int lng);

    //Weighed count of intermediate cards.
    int getInter(Suit suit);
    int getMaxInter(Suit suit) {  return (suit == ANY) ? 28 : 7;}
    void setInter(Suit suit, int inter);

    //Expected number of loosers.
    int getLoosers(Suit suit);
    int getMaxLoosers(Suit suit) { return (suit == ANY) ? 12 : 3;}
    void setLoosers(Suit suit, int loosers);

    //Quick tricks.
    int getQuickTricks(Suit suit);
    int getMaxQuickTricks(Suit suit) { return (suit == ANY) ? 16 : 4;}
    void setQuickTricks(Suit suit, int quickTricks);

    //Playing tricks.
    int getPlayingTricks(Suit suit);
    int getMaxPlayingTricks() { return 26;}
    void setPlayingTricks(Suit suit, int playingTricks);

    //Stopper quality of suit when playing NT.
    int getStopNT(Suit suit);
    int getMaxStopNT() { return 4;}
    void setStopNT(Suit suit, int stopNT);

    //General quality assesment of the suit.
    int getQlty(Suit suit);
    int getMaxQlty() { return 6;}
    void setQlty(Suit suit, int qlty);

    //High Card Points + Distribution Points.
    int getPoints(Suit trump);
    int getExtPoints(Suit trump, bool low);
    int getMaxPoints() { return 37;}
    void setPoints(Suit trump, int points);
    void updPoints(Suit trump, bool low);

private:

    int calcWeight(Suit suit, int firstCard, int cards[]);
    int calcCount(Suit suit, int low, int high, int cards[]);
    int calcRkcb(Suit suit, int cards[]);
    int calcBal(int count[]);


//-------------Attributes Classes for Evaluating High Cards-------------------

    //The weighted count of High Card Points where A=4; K=3;
    //Q=2 & J=1. Range is [0,10] for each suit and [0,37] for the
    //whole hand.
    quint32 hcpA;      //bit(0-3) C, bit(4-7) D, bit(8-11) H, bit(12-15) S, bit(16-21) hand.

    //The weighted count of aces and kings: A=2 & K=1. Range
    //is [0,3] for each suit and [0,12] for the hand}.
    quint16 controlsA; //bit(0-1) C, bit(2-3) D, bit(4-5) H, bit(6-7) S, bit(8-11) hand.

    //The count of aces, kings and queens. Range is [0,3] for a
    //single suit and [0,12] for the hand.
    quint16 hcA;        //bit(0-1) C, bit(2-3) D, bit(4-5) H, bit(6-7) S, bit(8-11) hand.

    //The count of cards higher than 9. Range is [0,5] for a single
    //suit and [0,13] for the hand.
    quint16 honorA;      //bit(0-2) C, bit(3-5) D, bit(6-8) H, bit(9-11) S, bit(12-15) hand.

    //The count of aces. Ranges are [0,1] and [0,4].
    quint8 aceA;           //bit(0) C, bit(1) D, bit(2) H, bit(3) S, bit(4-6) hand.

    //The count of kings. Ranges are [0,1] and [0,4].
    quint8 kingA;           //bit(0) C, bit(1) D, bit(2) H, bit(3) S, bit(4-6) hand.

    //The count of queens. Ranges are [0,1] and [0,4].
    quint8 queenA;           //bit(0) C, bit(1) D, bit(2) H, bit(3) S, bit(4-6) hand.

    //The count of jacks. Ranges are [0,1] and [0,4].
    quint8 jackA;           //bit(0) C, bit(1) D, bit(2) H, bit(3) S, bit(4-6) hand.

    //The count of tens. Ranges are [0,1] and [0,4].
    quint8 tenA;           //bit(0) C, bit(1) D, bit(2) H, bit(3) S, bit(4-6) hand.

    //The count of aces in the whole hand + the count of kings in
    //the suit. Range is [0,5].
    quint16 rkcbS;         //bit(0-2) C, bit(3-5) D, bit(6-8) H, bit(9-11) S.


    //-------Attributes for Evaluating the Distribution of the Hand--------

    //How balanced is the hand? (0=balanced-9=freak).
    quint8 bal;

    //Distribution points. 3 points for void, 2 points for singleton and 1 point for doubleton. Range is [0,9].
    quint32 dpA;            //bit(0-3) S, bit(4-7) H, bit(8-11) D, bit(12-15) C, bit(16-19) Hand.

    //The difference in length between 2 suits. Range is [−13,13].
    //Offset -13.
    quint32 dif2S;          //bit(0-4) D-C, bit(5-9) H-C, bit(10-14) H-D, bit(15-19) S-C, bit(20-24) S-D, bit(25-29) S-H.

    //The number of suits that are longer than 3. Range is [1,3].
    quint8 suits;

    //The length of a suit. [0,13].
    quint16 suitLenS;       //bit(0-3) C, bit(4-7) D, bit(8-11) H, bit(12-15) S.

    //The total length of two suits. [0,13].
    quint32 suitLen2S;      //bit(0-3) D+C, bit(4-7) H+C, bit(8-11) H+D, bit(12-15) S+C, bit(16-19) S+D, bit(20-24) S+H.

    //The length of the longer major suit. [0,13].
    quint8 lMaj;

    //The length of the longer minor suit. [0,13].
    quint8 lMin;

    //Is the suit the longest in the hand? [0,1].
    quint8 longestS;       //bit(0) C, bit(1) D, bit(2) H, bit(3) S.

    //Is the suit not shortest in the hand? [0,1].
    quint8 longS;         //bit(0) C, bit(1) D, bit(2) H, bit(3) S.


    //-------Attributes for Evaluating the Quality of the Suits--------

    //The weighted count of the intermediate cards (T = 4, 9 = 2,
    //8 = 1). Ranges are [0,7] for a suit and [0,28] for the hand.
    quint32 interA;         //bit(0-2) C, bit(3-5) D, bit(6-8) H, bit(9-11) S, bit(12-16) Hand.

    //Expected number of losers in s1 (a suit or the hand). Ranges: [0,3] and [0,12].
    quint16 losersA;       //bit(0-1) C, bit(2-3) D, bit(4-5) H, bit(6-7) S, bit(8-11) Hand.

    //Quick tricks (multiplied by 2). Ranges [0,4] and [0,16]
    quint32 quickTricksA;   //bit(0-2) C, bit(3-5) D, bit(6-8) H, bit(9-11), bit(12-16) Hand.

    //Playing tricks with s1 as trump (multiplied by 2). Range [0,26].
    quint32 playingTricksS;    //bit(0-4) C, bit(5-9) D, bit(10-14) H, bit(15-19) S, bit(20-24) Hand.

    //Stopper quality of the suit when playing in NT [0 = no stopper, 4 = double stopper].
    quint16 stopNTS;        //bit(0-2) C, bit(3-5) D, bit(6-8) H, bit(9-11) S.

    //A general quality assessment of the suit [0=bad, 6=excellent].
    quint16 qltyS;        //bit(0-3) C, bit(4-7) D, bit(8-11) H, bit(12-15) S.


    //-------Attributes for Evaluating the Quality of the Hand--------
    //High Card Points + Distribution Points. Range is [0,37].
    quint32 pointsS;       //bit(0-5) C, bit(6-11) D, bit(12-17) H, bit(18-23) S, bit(24-29) Hand.
};

#endif // CFEATURES_H
