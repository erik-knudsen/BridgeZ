﻿/* Erik Aagaard Knudsen.
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
    bool blackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit agree);
    int CalculateNoCards(CFeatures partnerFeatures, CFeatures ownFeatures, int cardVal);
    Bids blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings, int highTotPoints, Suit agree);
    void getLevel(Suit agree, int lowPartnerPoints, int ownPoints , Bids *bid, int *level);
    bool canDouble(CBidHistory bidHistory);
    bool isMin(int lowValue, int highValue, int value);
    bool isMax(int lowValue, int highValue, int value);

    CBidDB *bidDB;
    CBidDesc *bidDesc;
    CBidDBDefine *bidDBDefine;

    void auctionToText(CAuction &auction, QString *text);

    bool isMinorPass(Suit suitPAgree, Bids bid);
    bool isMajorOrNTPass(Suit suitPAgree, Bids bid);
    bool isBlackwoodQuestion(Suit suitPAgree, Bids bid);
    bool isGerberQuestion(Suit suitPAgree, Bids bid);
    int blackwoodAnswer(CBidHistory bidHistory, Suit suitPAgree, Bids bid);
    int gerberAnswer(CBidHistory bidHistory, Suit suitPAgree, Bids bid);
    bool isAnyPass(Suit suitPAgree, Bids bid);
    bool isSmallSlam(Bids bid);
    bool isGrandSlam(Bids bid);
    int limitMajorOrMinor(CBidHistory bidHistory, Suit suitNAgree, Bids bid, CFeatures lowPartnerFeatures);
    bool isGameInvitationMinor(CBidHistory bidHistory, Suit suitNAgree, Bids bid);
    bool isGameNTAfterStopper(Suit suitPAgree, Bids bid);
    bool isStopperBid(Suit suitAgree, Bids bid);
    bool isDoublePass(CBidHistory bidHistory, Bids bid);
    int limitNT(CBidHistory bidHistory, Bids bid, CFeatures &lowPartnerFeatures);
    bool isNewSuit(CBidHistory bidHistory, Suit suitNAgree, Bids bid);
    bool isRebid(CBidHistory bidHistory, Suit suitPAgree, Bids bid);
    bool hasDoubled(CBidHistory bidHistory);
};

#endif // CBIDENGINE_H
