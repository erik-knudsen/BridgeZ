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
 */
class CBidEngine
{
public:
    CBidEngine(CBidDB *bidDB, CBidDesc *bidDesc,
               CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc);
    virtual ~CBidEngine();

    CBid getNextBid(Seat seat, CBidHistory &bidHistory, int cards[13], ScoringMethod scoringMethod,
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
    void calculateRange(Seat seat, CFeatures &lowFeatures, CFeatures &highFeatures, CBidHistory &bidHistory);
    bool isNT(CBidHistory &bidHistory, int inx);
    bool nextBidCanBeNT(CFeatures ownFeatures, CFeatures &lowPartnerFeatures, CFeatures &highPartnerFeatures, CFeatures lowRHFeatures, CFeatures highRHFeatures,
                        CFeatures lowLHFeatures, CFeatures highLHFeatures);
    bool mustPass(CBidHistory &bidHistory);
    bool isPenaltyDouble(CBidHistory &bidHistory, CFeatures lowPartnerFeatures);
    bool isDouble(CBidHistory &bidHistory);
    bool canDouble(CBidHistory &bidHistory);
    Bids getTakeoutDouble(CFeatures &lowPartnerFeatures, CFeatures &ownFeatures,
                       int highLevel, Suit highSuit, int *low, int *high);
    Suit findTakeoutDouble(Bids bid, int highLevel, Suit highSuit, int *low, int *high);
    bool isBlackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit agree);
    Bids blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings, int lowTotPoints, Suit agree);
    int CalculateNoCards(CFeatures partnerFeatures, CFeatures ownFeatures, int cardVal);
    void getLevel(Suit agree, int lowPartnerPoints, int ownPoints , Bids *bid, int *low, int *high);
    void findLevel(Suit agree, int lowPartnerPoints, int bidLevel, int *low, int *high);
    bool isNewSuit(Suit suitNAgree, Bids bid);
    bool isRebid(CBidHistory &bidHistory, Suit suitPAgree, Bids bid);
    bool isNextBidOpen(CBidHistory &bidHistory);
    int limitNT(CBidHistory &bidHistory, Bids bid, CFeatures &lowPartnerFeatures);

    bool isMin(int lowValue, int highValue, int value);
    bool isMax(int lowValue, int highValue, int value);

    CBidDB *bidDB;
    CBidDesc *bidDesc;
    CBidDBDefine *bidDBDefine;

    void auctionToText(CAuction &auction, QString *text);
};

#endif // CBIDENGINE_H
