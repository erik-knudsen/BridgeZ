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
    void calculatepRules(Seat seat, CBidHistory &bidHistory, Bids bid, ScoringMethod scoringMethod, Team teamVul, QList<CRule *> &pDefRules);
    bool blackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit bidAgree);
    int CalculateNoCards(CFeatures partnerFeatures, CFeatures ownFeatures, int cardVal);
    Bids blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings, int lowTotPoints, int highTotPoints);

    CBidDB *bidDB;
    CBidDesc *bidDesc;
    CBidDBDefine *bidDBDefine;

    void auctionToText(CAuction &auction, QString *text);
};

#endif // CBIDENGINE_H
