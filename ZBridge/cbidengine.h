/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidEngine.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original 

  Abstract: Bid engine.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bid engine.
 */

#ifndef CBIDENGINE_H
#define CBIDENGINE_H

#include <QList>

#include "cbid.h"
#include "Defines.h"

class CBidDB;
class CBidDesc;
class CBidOptionDoc;
class CBidDBDefine;
class CBidHistory;
class CFeatures;
class CRule;
class CAuction;

/**
 * @brief The bid engine class handles automatic bidding.
 *
 * Automatic bidding is used for players, who are played by a computer. Either locally or remote.
 * It uses a two step approach. First it tries to find the next bid in the bidding database and if
 * this is not possible then it uses a built in algoritmic approach.
 */
class CBidEngine
{
public:
    CBidEngine(CBidDB *bidDB, CBidDesc *bidDesc,
               CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc);
    virtual ~CBidEngine();

    CBid getNextBid(Seat seat, CFeatures &ownFeatures, CBidHistory &bidHistory, ScoringMethod scoringMethod,
            Team teamVul);
    QList<CRule *> getpRules(Seat seat, CBidHistory &bidHistory, Bids bid, ScoringMethod scoringMethod,
                             Team teamVul, bool *substitute);
    QString getAlertIdDesc(quint8 alertId);

private:
    CAuction findSubstituteAuction(CAuction &auction, QSet<qint16> &pages);
    CBid calculateNextBid(Seat seat, CBidHistory &bidHistory, CFeatures &features, ScoringMethod scoringMethod,
                                Team teamVul);
    void calculatepRules(Seat seat, CBidHistory &bidHistory, Bids bid, ScoringMethod scoringMethod, Team teamVul,
                         QList<CRule *> &pDefRules);
    bool nextBidCanBeNT(CFeatures ownFeatures, CFeatures &lowPartnerFeatures, CFeatures &highPartnerFeatures, CFeatures lowRHFeatures, CFeatures highRHFeatures,
                        CFeatures lowLHFeatures, CFeatures highLHFeatures);
    bool mustPass(CBidHistory &bidHistory);
    bool isPenaltyDouble(CBidHistory &bidHistory, CFeatures lowPartnerFeatures);
    bool isDouble(CBidHistory &bidHistory);
    Bids getDoubleBid(CBidHistory &bidHistory);
    bool canDouble(CBidHistory &bidHistory);
    Bids getTakeoutDouble(CFeatures &lowPartnerFeatures, CFeatures &ownFeatures,
                       Bids highBid, Bids doubleBid, int *low, int *high);
    Suit findTakeoutDouble(Bids bid, Bids highBid, int *low, int *high);
    bool isBlackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit agree);
    Bids blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings, int lowTotPoints, Suit agree);
    int acesAsked(CBidHistory &bidHistory);
    int kingsAsked(CBidHistory &bidHistory);
    int CalculateNoCards(CFeatures features, int cardVal);
    void getLevel(Suit agree, int lowPartnerPoints, int ownPoints , Bids *bid, int *low, int *high);
    void findLevel(Suit agree, int lowPartnerPoints, int bidLevel, int *low, int *high);
    bool isNewSuit(Suit suitNAgree, Bids bid);
    bool isRebid(CBidHistory &bidHistory, Suit suitPAgree, Bids bid);
    bool isNextBidOpen(CBidHistory &bidHistory);
    bool isNextBidOverCall(CBidHistory bidHistory);
    int nextBidder(CBidHistory &bidHistory);
    bool isNTBidded(CBidHistory &bidHistory);
    Bids getLastNotAlertedBid(CBidHistory bidHistory);

    bool isMin(int lowValue, int highValue, int value);
    bool isMax(int lowValue, int highValue, int value);

    CBidDB *bidDB;                  /**< The bid database. */
    CBidDesc *bidDesc;              /**< Text descriptions for the bid database (alerts etc.). */
    CBidDBDefine *bidDBDefine;      /**< Definitions for the bid database (which conventions to use etc.) */

    void auctionToText(CAuction &auction, QString *text);
};

#endif // CBIDENGINE_H
