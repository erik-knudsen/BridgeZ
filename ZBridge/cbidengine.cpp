﻿/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidEngine.cpp
  Developers: eak 

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Bid engine.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the bid engine.
 */

#include <cassert>
#include <QDebug>

#include "cbiddb.h"
#include "cbiddesc.h"
#include "cbiddbdefine.h"
#include "cbidhistory.h"
#include "cfeatures.h"
#include "cauction.h"
#include "cbiddbdefine.h"
#include "cbidengine.h"

//Bid limit levels.
const int BID_POINT_SIZE = 7;
const int BID_SUIT_POINT[BID_POINT_SIZE] = {17, 20, 22, 25, 28, 33, 37};
const int BID_NT_POINT[BID_POINT_SIZE] = {17, 23, 26, 28, 29, 33, 37};
const int BID_SUIT_MAJOR_GAME_INX = 3;
const int BID_SUIT_MINOR_GAME_INX = 4;
const int BID_NT_GAME_INX = 2;
const int BID_SMALL_SLAM_INX = 5;
const int BID_GRAND_SLAM_INX = 6;

//Newbid definitions. For new suit bidding.
const int OPEN_RESPONSE = 1;            //Next bid is response to opener by partner.
const int OPEN_REBID = 2;               //Next bid is rebid of opener.
const int OPEN_REBID_RESPONSE = 3;      //Next bid is second response to opener by partner.
const int OPEN_OTHER = -1;              //Next bid is other bids.
const int CATCHALL_NT_L = 6;            //Catch all NT low point level by partner.
const int CATCHALL_NT_H = 9;            //Catch all NT high point level by partner.
const int NEWSUIT_P1_1 = 6;             //Low point for response to opener on 1. level by partner.
const int NEWSUIT_P1_2 = 10;            //Low point for response to opener on 2. level by partner.
const int NEWSUIT_P1_J = 16;            //Low point for jump response to opener by partner.
const int NEWSUIT_O_J_L = 19;           //Low point for openers rebid jump response by opener.
const int NEWSUIT_O_J_H = 21;           //High point for openers rebid jump response by opener.
const int NEWSUIT_O_S_L = 12;           //Low point for openers simpel new suit response by opener.
const int NEWSUIT_O_S_H = 18;           //High point for openers simpel new suit response by opener.
const int NEWSUIT_O_3_L = 16;           //Low point for openers reverse suit response by opener.
const int NEWSUIT_O_3_H = 21;           //High point for openers reverse suit response by opener.
const int NEWSUIT_P2_3_L = 10;          //Low point for partners 2. bid on level 3.
const int NEWSUIT_P2_3_H = 12;          //High point for partners 2. bid on level 3.
const int NEWSUIT_P2_4 = 13;            //Low point for partners 2. bid on level 4.

//Rebid of suit definitions.
const int REBID_SL = 6;                 //Minimum suit length.
const int REBID_O = 12;                 //Base level for openers rebid.
const int REBID_P = 6;                  //Base level for partners rebid.

//Alert id definitions (from alert ids in the bidding database).
const int BLACKWOOD_ACE = 173;          //Blackwood ace question.
const int BLACKWOOD_KING = 174;         //Blackwood king question.
const int GERBER_ACE = 102;             //Gerber ace question.
const int GERBER_KING = 172;            //Gerber king question.
const int ZERO_OR_FOUR_ACES = 175;      //Zero or four aces.
const int ONE_ACE = 176;                //One ace.
const int TWO_ACES = 177;               //Two aces.
const int THREE_ACES = 178;             //Three aces.
const int ZERO_OR_FOUR_KINGS = 179;     //Zero or four kings.
const int ONE_KING = 180;               //One king.
const int TWO_KINGS = 181;              //Two kings.
const int THREE_KINGS = 182;            //Three kings.

/**
 * @brief Generate bid engine.
 * @param bidDB The bid database.
 * @param bidDesc Description of the bid database.
 * @param nsBbidOptionDoc Options for NS bidding.
 * @param ewBidOptionDoc Options for EW bidding.
 */
CBidEngine::CBidEngine(CBidDB *bidDB, CBidDesc *bidDesc,
           CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc)
{
    this->bidDB = bidDB;
    this->bidDesc = bidDesc;

    bidDBDefine = new CBidDBDefine(nsBidOptionDoc, ewBidOptionDoc);
}

CBidEngine::~CBidEngine()
{
    delete bidDBDefine;
}

/**
 * @brief Determine the next bid
 *
 * Determine the next bid by using the bidding database.\n
 * In cases where the bidding database is not enough then the search for the next bid
 * is extended by using a built in algoritmic approach.
 *
 *
 * @param[in] seat Bidders seat.
 * @param[in] bidHistory The bid history.
 * @param[in] scoringMethod The scoring method.
 * @param[in] teamVul Team vulnerability.
 * @return The determined next bid.
 */
CBid CBidEngine::getNextBid(Seat seat, CBidHistory &bidHistory, ScoringMethod scoringMethod,
                            Team teamVul)
{
    assert ((bidHistory.bidList.size() == 0) ? true : (((bidHistory.bidList.last().bidder + 1) % 4) == seat));

    CBid bid;
    bid.bidder = seat;

    CAuction auction;
    QList<CAuction> subAuction;
    QList<CRule *> pDefRules;
    QList<qint8> defBids;

    //Get features.
    CFeatures& features = bidHistory.getFeatures();

    //Get relevant pages and rules.
    QSet<qint16> &pages = bidDBDefine->getPages(seat);
    QSet<qint16> &rules = bidDBDefine->getRules(seat);

    //Get auction till now.
    for (int i = 0; i < bidHistory.bidList.size(); i++)
    {
        if (bidHistory.bidList[i].substitute)
            auction = findSubstituteAuction(auction, pages);
        auction.auction.append(bidHistory.bidList[i].bid);
    }

    QSetIterator<qint16> i(pages);
    bool cont = true;
    while (cont)
    {
        cont = false;
        i.toFront();
        while (i.hasNext())
        {
            qint16 page = i.next();
            if (bidDB->auctionExist(page, auction))
            {
                if (bidDB->isBids(page, auction))
                {
                    //Found auction. Get bids and rules for the auction.
                    QList<qint8> bids;
                    QList<CRule*> pRules;
                    bidDB->getBids(page, auction, &bids, &pRules);
                    for (int i = 0; i < bids.size(); i++)
                        if (rules.contains(pRules[i]->getId()))
                        {
                            //Found a defined rule. Check scoring method, vulnerability and features.
                            Vulnerability ruleVul = pRules[i]->getVulnerability();
                            if (((pRules[i]->getScoringMethod() == NOSCORE) ||
                                    (pRules[i]->getScoringMethod() == scoringMethod)) &&
                                    ((ruleVul == VUL_II) ||
                                     ((teamVul == NEITHER) && ((ruleVul == VUL_NI) || (ruleVul == VUL_NN))) ||
                                     ((teamVul == BOTH) && ((ruleVul == VUL_YI) || (ruleVul == VUL_YY))) ||
                                     ((((teamVul == NORTH_SOUTH) && ((seat == NORTH_SEAT) || (seat == SOUTH_SEAT))) ||
                                       ((teamVul == EAST_WEST) && ((seat == EAST_SEAT) || (seat == WEST_SEAT)))) &&
                                      ((ruleVul == VUL_YI) || (ruleVul == VUL_YN)))) &&
                                    (pRules[i]->RuleIsOk(features)))
                            {
                                defBids.append(bids[i]);
                                pDefRules.append(pRules[i]);
                            }
                        }
                }
                else
                {
                    //Found a substitute auction.
                    //Save the substitute auction (in case we need it later).
                    //Only one substitute auction is allowed (and only in one level).
                    assert (subAuction.size() == 0);
                    subAuction.append(bidDB->getSubstituteAuction(page, auction));
                }
            }
        }

        //Did we find anything?
        if (pDefRules.size() == 0)
        {
            //We did not find anything. Should we try a substitute auction?
            if (subAuction.size() != 0)
            {
                auction = subAuction[0];
                cont = true;
            }

            //As a last resort try to remove initial pass bids (if any) in the auction.
            else if ((auction.auction.size() > 0) &&
                    (auction.auction[0] == BID_PASS))
            {
                cont = true;
                CAuction oldAuction = auction;
                int first;
                for (first = 0; first < auction.auction.size(); first++)
                    if (auction.auction[first] != BID_PASS)
                        break;
                auction.auction.clear();
                for (int i = first; i < oldAuction.auction.size(); i++)
                    auction.auction.append(oldAuction.auction[i]);
            }
        }
    }

    //Found one or more rules with feature check ok?
    if (pDefRules.size() > 0)
    {
        //Find highest priority and bid.
        int inx = 0;
        int priority = pDefRules[inx]->getPriority();
        qint8 bidVal = defBids[inx];
        for (int i = 0; i < pDefRules.size(); i++)
            if (pDefRules[i]->getPriority() > priority)
            {
                inx = i;
                priority = pDefRules[inx]->getPriority();
                bidVal = defBids[inx];
            }

        //Find lowest bid.
        for (int i = 0; i < defBids.size(); i++)
            if ((pDefRules[i]->getPriority() == priority) &&
                    (bidVal > defBids[i]))
            {
                inx = i;
                bidVal = defBids[inx];
            }

        //Return found bid.
        bid.bid = (Bids)bidVal;
        bid.rules.append(pDefRules[inx]);
        bid.alert = getAlertIdDesc(bid.rules[0]->getAlertId());
        bid.substitute = (subAuction.size() > 0);

        return bid;
    }
    else
        //Built in algoritmic calculation of next bid.
        return calculateNextBid(seat, bidHistory, features, scoringMethod, teamVul);
}

/**
 * @brief Get possible rules for a given bid history and next bid as calculated by getNextBid.
 *
 * @param[in] seat Bidders seat.
 * @param[in] bidHistory The bid history.
 * @param[in]bid The bid calculated by getNext bid.
 * @param[in] scoringMethod The scoring method.
 * @param[in] teamVul Team vulnerability.
 * @param[out] substitute true if a substitute auction was used.
 * @return returns a list with possible rules.
 */
QList<CRule *> CBidEngine::getpRules(Seat seat, CBidHistory &bidHistory, Bids bid, ScoringMethod scoringMethod,
                                     Team teamVul, bool *substitute)
{
    assert ((bidHistory.bidList.size() == 0) ? true : (((bidHistory.bidList.last().bidder + 1) % 4) == seat));

    CAuction auction;
    QList<CAuction> subAuction;
    QList<CRule *> pDefRules;
    *substitute = false;

    //Get relevant pages and rules.
    QSet<qint16> &pages = bidDBDefine->getPages(seat);
    QSet<qint16> &rules = bidDBDefine->getRules(seat);

    //Get auction till now.
    for (int i = 0; i < bidHistory.bidList.size(); i++)
    {
        if (bidHistory.bidList[i].substitute)
            auction = findSubstituteAuction(auction, pages);
        auction.auction.append(bidHistory.bidList[i].bid);
    }

    QSetIterator<qint16> i(pages);
    bool cont = true;
    while (cont)
    {
        cont = false;
        i.toFront();
        while (i.hasNext())
        {
            qint16 page = i.next();
            if (bidDB->auctionExist(page, auction))
            {
                if (bidDB->isBids(page, auction))
                {
                    //Found auction. Get bids and rules for the auction.
                    QList<qint8> bids;
                    QList<CRule*> pRules;
                    bidDB->getBids(page, auction, &bids, &pRules);
                    for (int i = 0; i < pRules.size(); i++)
                        if (rules.contains(pRules[i]->getId()))
                        {
                            //Found a defined rule. Check scoring method and vulnerability.
                            Vulnerability ruleVul = pRules[i]->getVulnerability();
                            if ((bids[i] == bid) &&
                                    ((pRules[i]->getScoringMethod() == NOSCORE) ||
                                    (pRules[i]->getScoringMethod() == scoringMethod)) &&
                                    ((ruleVul == VUL_II) ||
                                     ((teamVul == NEITHER) && ((ruleVul == VUL_NI) || (ruleVul == VUL_NN))) ||
                                     ((teamVul == BOTH) && ((ruleVul == VUL_YI) || (ruleVul == VUL_YY))) ||
                                     ((((teamVul == NORTH_SOUTH) && ((seat == NORTH_SEAT) || (seat == SOUTH_SEAT))) ||
                                       ((teamVul == EAST_WEST) && ((seat == EAST_SEAT) || (seat == WEST_SEAT)))) &&
                                      ((ruleVul == VUL_YI) || (ruleVul == VUL_YN)))))
                                pDefRules.append(pRules[i]);
                        }
                }
                else
                {
                    //Found a substitute auction.
                    //Save the substitute auction (in case we need it later).
                    //Only one substitute auction is allowed (and only in one level).
                    assert (subAuction.size() == 0);
                    subAuction.append(bidDB->getSubstituteAuction(page, auction));
                }
            }
        }

        //Did we find anything?
        if (pDefRules.size() == 0)
        {
            //We did not find anything. Should we try a substitute auction?
            if (subAuction.size() != 0)
            {
                auction = subAuction[0];
                *substitute = true;
                cont = true;
            }

            //As a last resort try to remove initial pass bids (if any) in the auction.
            else if ((auction.auction.size() > 0) && (auction.auction[0] == BID_PASS))
            {
                cont = true;
                CAuction oldAuction = auction;
                int first;
                for (first = 0; first < auction.auction.size(); first++)
                    if (auction.auction[first] != BID_PASS)
                        break;
                auction.auction.clear();
                for (int i = first; i < oldAuction.auction.size(); i++)
                    auction.auction.append(oldAuction.auction[i]);
            }
        }
    }

    //Built in algorithmic calculation of relevant rules.
    if (pDefRules.size() == 0)
        calculatepRules(seat, bidHistory, bid, scoringMethod, teamVul, pDefRules);

    return pDefRules;
}

QString CBidEngine::getAlertIdDesc(quint8 alertId)
{
     return bidDesc->getAlertIdDesc(alertId);
}

CAuction CBidEngine::findSubstituteAuction(CAuction &auction, QSet<qint16> &pages)
{
    CAuction subAuction;
    QSetIterator<qint16> i(pages);

    while (i.hasNext())
    {
        qint16 page = i.next();
        if (bidDB->auctionExist(page, auction) && bidDB->isSubstituteAuction(page, auction))
        {
                subAuction = bidDB->getSubstituteAuction(page, auction);
                break;
        }
    }
    return subAuction;
}

/**
 * @brief Calculate the next bid
 *
 * Calculate the next bid by using an algoritmic approach. The method is used after the bidding database
 * runs out for suggestion(s). It extends the bidding database and it only covers bids that are not already
 * covered by the bidding database.\n\n
 *
 * NOTE the rule used for the calculated bid is also calculated and returned with the bid.
 * NOTE This calculated rule is only used for debugging and does not take part in anything else.\n\n
 *
 * The following approach is used:\n
 *   1. New suit bids are non forcing.
 *   2. Notrump bids, raises of partners suit and rebids are limit bids.
 *   3. After a minor suit fit has been found, a new suit bid on the three level shows\n
 *      stoppers in the bid suit and sufficient points for 3NT.
 *   4. Doubles are for penalties only.
 *   5. 4NT is Blackwood when a fit is certain.
 *   6. 4C is Gerber when notrump has been bid.
 *
 * @param[in] seat Bidders seat.
 * @param[in] bidHistory The bid history.
 * @param[in] ownFeatures The features of the cards for the next bidder.
 * @param[in] scoringMethod The scoring method (not used).
 * @param[in] teamVul Team vulnerability (not used).
 * @return The calculated next bid.
 */
CBid CBidEngine::calculateNextBid(Seat seat, CBidHistory &bidHistory, CFeatures &ownFeatures, ScoringMethod scoringMethod, Team teamVul)
{
    CRule *pRule = new CRule();
    QList<CRule *> rules;
    pRule->setdBRule(false);
    rules.append(pRule);
    CBid bid(seat, BID_PASS, "", rules, false);

    //*************************for debugging**************************
    //Get auction till now.
    CAuction auction;
    for (int i = 0; i < bidHistory.bidList.size(); i++)
        auction.auction.append(bidHistory.bidList[i].bid);
    QString txt;
    auctionToText(auction, &txt);
    qDebug() << QString(SEAT_NAMES[seat]) + ": " + txt;
    //***********************for debugging****************************

    int size = bidHistory.bidList.size();

    CFeatures& lowPartnerFeatures = bidHistory.getLowPartnerFeatures(seat);
    CFeatures& highPartnerFeatures = bidHistory.getHighPartnerFeatures(seat);
    CFeatures& lowOwnFeatures = bidHistory.getLowOwnFeatures(seat);
    CFeatures& highOwnFeatures = bidHistory.getHighOwnFeatures(seat);
    CFeatures& lowRHFeatures = bidHistory.getLowRHFeatures(seat);
    CFeatures& highRHFeatures = bidHistory.getHighRHFeatures(seat);
    CFeatures& lowLHFeatures = bidHistory.getLowLHFeatures(seat);
    CFeatures& highLHFeatures = bidHistory.getHighLHFeatures(seat);

    //Already agrement on suit?
    Suit suitAgree;
    if ((lowPartnerFeatures.getSuitLen(SPADES) + lowOwnFeatures.getSuitLen(SPADES)) >= 8)
        suitAgree = SPADES;
    else if ((lowPartnerFeatures.getSuitLen(HEARTS) + lowOwnFeatures.getSuitLen(HEARTS)) >= 8)
        suitAgree = HEARTS;
    else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) + lowOwnFeatures.getSuitLen(DIAMONDS)) >= 8)
        suitAgree = DIAMONDS;
    else if ((lowPartnerFeatures.getSuitLen(CLUBS) + lowOwnFeatures.getSuitLen(CLUBS)) >= 8)
        suitAgree = CLUBS;
    else if ((highPartnerFeatures.getDp(NOTRUMP) <= 1) || (highOwnFeatures.getDp(NOTRUMP) <= 1))
        suitAgree = NOTRUMP;
    else
        suitAgree = ANY;

    //Agrement on suit in next bid.
    Suit newSuitAgree;
    if (suitAgree != ANY)
        newSuitAgree = suitAgree;
    else if ((lowPartnerFeatures.getSuitLen(SPADES) + ownFeatures.getSuitLen(SPADES)) >= 8)
        newSuitAgree = SPADES;
    else if ((lowPartnerFeatures.getSuitLen(HEARTS) + ownFeatures.getSuitLen(HEARTS)) >= 8)
        newSuitAgree = HEARTS;
    else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) + ownFeatures.getSuitLen(DIAMONDS)) >= 8)
        newSuitAgree = DIAMONDS;
    else if ((lowPartnerFeatures.getSuitLen(CLUBS) + ownFeatures.getSuitLen(CLUBS)) >= 8)
        newSuitAgree = CLUBS;
    else if (nextBidCanBeNT(ownFeatures, lowPartnerFeatures, highPartnerFeatures,
                            lowRHFeatures, highRHFeatures, lowLHFeatures, highLHFeatures))
        newSuitAgree = NOTRUMP;
    else
        newSuitAgree = ANY;

    //Get bidded suits and highest level.
    bool oppSuit[5];
    bool ownSuit[5];
    bool partnerSuit[5];
    for (int i = 0; i < 5; i++)
        oppSuit[i] = ownSuit[i] = partnerSuit[i] = false;

    //Opponent suits and highest bid.
    Bids highOppBid = BID_NONE;
    for (int i = size - 1; i >= 0; i -= 2)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Bids bid = bidHistory.bidList[i].bid;
            Suit suit = BID_SUIT(bid);
            oppSuit[suit] = true;
            if (highOppBid == BID_NONE)
                highOppBid = bid;
        }
    }

    //Partner suits and highest bid.
    Bids highPartnerBid = BID_NONE;
    for (int i = size - 2; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Bids bid = bidHistory.bidList[i].bid;
            Suit suit = BID_SUIT(bid);
            partnerSuit[suit] = true;
            if (highPartnerBid == BID_NONE)
                highPartnerBid = bid;
        }
    }

    //Own suits and highest bid.
    Bids highOwnBid = BID_NONE;
    for (int i = size - 4; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Bids bid = bidHistory.bidList[i].bid;
            Suit suit = BID_SUIT(bid);
            ownSuit[suit] = true;
            if (highOwnBid == BID_NONE)
                highOwnBid = bid;
        }
    }
    //Highest of partner and own bid.
    Bids highOPBid = (highPartnerBid > highOwnBid) ? (highPartnerBid) : (highOwnBid);

    //Highest bid.
    Bids highBid = BID_NONE;
        for (int i = 0; i < size; i++)
        if (BID_LEVEL(bidHistory.bidList[i].bid) != -1)
            highBid = bidHistory.bidList[i].bid;


    //Must pass?
    if (mustPass(bidHistory))
    {
        pRule->setStatus(MUST_PASS);

        bid.bid = BID_PASS;
        return bid;
    }

    //Penalty double?
    if (isPenaltyDouble(bidHistory, lowPartnerFeatures))
    {
        pRule->setStatus(MUST_PASS);

        bid.bid = BID_PASS;
        return bid;
    }

    //Take out double (penalty doubles have been handled above)?
    if (isDouble(bidHistory))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;
        pRule->getFeatures(&lowFeatures, &highFeatures);

        Bids nextBid;
        int low, high;

        //Calculate takeout bid and point interval.
        nextBid = getTakeoutDouble(lowPartnerFeatures, ownFeatures, highBid, &low, &high);

        //If bid is a NT bid?
        Suit suit = BID_SUIT(nextBid);
        if (BID_SUIT(nextBid) == NOTRUMP)
        {
            pRule->getFeatures(&lowFeatures, &highFeatures);
            lowFeatures.setPoints(NOTRUMP, low);
            highFeatures.setPoints(NOTRUMP, high);
            highFeatures.setDp(NOTRUMP, 1);
            for (int i = 0; i < 4; i++)
                if (lowPartnerFeatures.getSuitLen((Suit)i) >= 4)
                    lowFeatures.setStopNT((Suit)i, 3);
        }
        //Otherwise a suit bid.
        else
        {
            pRule->getFeatures(&lowFeatures, &highFeatures);
            lowFeatures.setPoints((Suit)suit, low);
            highFeatures.setPoints((Suit)suit, high);
            lowFeatures.setSuitLen((Suit)suit, 4);
            pRule->setFeatures(lowFeatures, highFeatures);
        }

        pRule->setFeatures(lowFeatures, highFeatures);

        bid.bid = nextBid;

        return bid;
    }

    //if (agreement in suit or nt)
    if ((newSuitAgree == SPADES) ||(newSuitAgree == HEARTS) ||(newSuitAgree == DIAMONDS) ||(newSuitAgree == CLUBS) ||
            (suitAgree == NOTRUMP))
    {
        Suit agree = (suitAgree == NOTRUMP) ? (suitAgree) : (newSuitAgree);

        //Calculate point range and other parameters..
        if (lowPartnerFeatures.getPoints(agree) == 0)
            lowPartnerFeatures.setPoints(agree, lowPartnerFeatures.getPoints(NOTRUMP));
        if (highPartnerFeatures.getPoints(agree) == highPartnerFeatures.getMaxPoints())
            highPartnerFeatures.setPoints(agree, highPartnerFeatures.getPoints(NOTRUMP));
        int lowTotPoints = lowPartnerFeatures.getPoints(agree) + ownFeatures.getPoints(agree);
        int highTotPoints = highPartnerFeatures.getPoints(agree) + ownFeatures.getPoints(agree);
        bool isMinPoints = isMin(lowOwnFeatures.getPoints(agree), highOwnFeatures.getPoints(agree),
                               ownFeatures.getPoints(agree));
        bool isMaxPoints = isMax(lowOwnFeatures.getPoints(agree), highOwnFeatures.getPoints(agree),
                               ownFeatures.getPoints(agree));
        Bids game = (agree == SPADES) ? (BID_4S) : (agree == HEARTS) ? (BID_4H) :
                    (agree == DIAMONDS) ? (BID_5D) : (agree == CLUBS) ? (BID_5C) : (BID_3NT);

        //if (Blackwood or Gerber question)
        if (isBlackwoodOrGerberQuestion(bidHistory, agree))
        {
            //Count aces.
            int countAces = 0;
            for (int i = 0; i < 4; i++)
                if (ownFeatures.getCountCard((Suit)i, ACE) == 1)
                    countAces++;
            if (ownFeatures.getCountCard(ANY, ACE) > countAces)
                countAces = ownFeatures.getCountCard(ANY, ACE);

            //Count kings.
            int countKings = 0;
            for (int i = 0; i < 4; i++)
                if (ownFeatures.getCountCard((Suit)i, KING) == 1)
                    countKings++;
            if (ownFeatures.getCountCard(ANY, KING) > countAces)
                countKings = ownFeatures.getCountCard(ANY, KING);

            CFeatures lowFeatures;
            CFeatures highFeatures;
            pRule->getFeatures(&lowFeatures, &highFeatures);

            //if (Blackwod question about aces)
            if (bidHistory.bidList[size - 2].bid == BID_4NT)
            {
                lowFeatures.setCountCard(ANY, ACE, countAces);
                highFeatures.setCountCard(ANY, ACE, countAces);

                //Blackwood ace asking.
                if ((countAces == 0) || (countAces == 4))
                    pRule->setAlertId(ZERO_OR_FOUR_ACES);
                else if (countAces == 1)
                    pRule->setAlertId(ONE_ACE);
                else if (countAces == 2)
                    pRule->setAlertId(TWO_ACES);
                else if (countAces == 3)
                    pRule->setAlertId(THREE_ACES);

                bid.bid = ((countAces == 0) || (countAces == 4)) ? (BID_5C) :
                          (countAces == 1) ? (BID_5D) : (countAces == 2) ? (BID_5H) : (BID_5S);
            }
            //else if (Blackwood question about kings)
            else if (bidHistory.bidList[size - 2].bid == BID_5NT)
            {
                lowFeatures.setCountCard(ANY, KING, countKings);
                highFeatures.setCountCard(ANY, KING, countKings);

                //Blackwood king asking.
                if ((countKings == 0) || (countKings == 4))
                    pRule->setAlertId(ZERO_OR_FOUR_KINGS);
                else if (countKings == 1)
                    pRule->setAlertId(ONE_KING);
                else if (countKings == 2)
                    pRule->setAlertId(TWO_KINGS);
                else if (countKings == 3)
                    pRule->setAlertId(THREE_KINGS);

                bid.bid = ((countKings == 0) || (countKings == 4)) ? (BID_5C) :
                          (countKings == 1) ? (BID_5D) : (countKings == 2) ? (BID_5H) : (BID_5S);
            }
            //else if (Gerber question about aces)
            else if (bidHistory.bidList[size - 2].bid == BID_4C)
            {
                lowFeatures.setCountCard(ANY, ACE, countAces);
                highFeatures.setCountCard(ANY, ACE, countAces);

                //Gerber ace asking.
                if ((countAces == 0) || (countAces == 4))
                    pRule->setAlertId(ZERO_OR_FOUR_ACES);
                else if (countAces == 1)
                    pRule->setAlertId(ONE_ACE);
                else if (countAces == 2)
                    pRule->setAlertId(TWO_ACES);
                else if (countAces == 3)
                    pRule->setAlertId(THREE_ACES);

                bid.bid = ((countAces == 0) || (countAces == 4)) ? (BID_4D) :
                          (countAces == 1) ? (BID_4H) : (countAces == 2) ? (BID_4S) : (BID_4NT);
            }
            //else if (Gerber question about kings)
            else if (bidHistory.bidList[size - 2].bid == BID_5C)
            {
                lowFeatures.setCountCard(ANY, KING, countKings);
                highFeatures.setCountCard(ANY, KING, countAces);

                //Gerber king asking.
                if ((countKings == 0) || (countKings == 4))
                    pRule->setAlertId(ZERO_OR_FOUR_KINGS);
                else if (countKings == 1)
                    pRule->setAlertId(ONE_KING);
                else if (countKings == 2)
                    pRule->setAlertId(TWO_KINGS);
                else if (countKings == 3)
                    pRule->setAlertId(THREE_KINGS);

                bid.bid = ((countKings == 0) || (countKings == 4)) ? (BID_5D) :
                          (countKings == 1) ? (BID_5H) : (countKings == 2) ? (BID_5S) : (BID_5NT);
            }
            if (bid.bid > highBid)
            {
                pRule->setFeatures(lowFeatures, highFeatures);
                pRule->setStatus(FORCING);
            }
            else
                bid.bid = BID_DOUBLE;

            return bid;
        }

        //if (slam is possible - small or grand)
        if (lowTotPoints >= BID_SUIT_POINT[BID_SMALL_SLAM_INX])     //33
        {
            //if (Ask Blackwood or Gerber question)
            int noAces = CalculateNoCards(lowPartnerFeatures, ownFeatures, ACE);
            int noKings = CalculateNoCards(lowPartnerFeatures, ownFeatures, KING);
            Bids nextBid = blackwoodOrGerberAsk(bidHistory, noAces, noKings, lowTotPoints, agree);
            if (nextBid != BID_NONE)
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                if (nextBid == BID_4C)
                {                    
                    //Gerber ace asking.
                    pRule->setAlertId(GERBER_ACE);

                    int points = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(NOTRUMP);
                    if (points < 0) points = 0;
                    lowFeatures.setPoints(NOTRUMP, points);
                }
                else if (nextBid == BID_4NT)
                {                    
                    //Blackwood ace asking.
                    pRule->setAlertId(BLACKWOOD_ACE);

                    int points = BID_SUIT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(agree);
                    if (points < 0) points = 0;
                    if ((lowPartnerFeatures.getSuitLen(agree) + lowOwnFeatures.getSuitLen(agree)) < 8)
                        lowFeatures.setSuitLen(agree, 8 - lowPartnerFeatures.getSuitLen(agree));
                    lowFeatures.setPoints(agree, points);
                }
                else if (nextBid == BID_5C)
                {                    
                    //Gerber king asking.
                    pRule->setAlertId(GERBER_KING);

                    int points = BID_NT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(NOTRUMP);
                    if (points < 0) points = 0;
                    lowFeatures.setPoints(NOTRUMP, points);
                }
                else if (nextBid == BID_5NT)
                {                   
                    //Blackwood king asking.
                    pRule->setAlertId(BLACKWOOD_KING);

                    int points = BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(agree);
                    if (points < 0) points = 0;
                    if ((lowPartnerFeatures.getSuitLen(agree) + lowOwnFeatures.getSuitLen(agree)) < 8)
                        lowFeatures.setSuitLen(agree, 8 - lowPartnerFeatures.getSuitLen(agree));
                    lowFeatures.setPoints(agree, points);
                }

                //Check bid.
                if (nextBid > highBid)
                {
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(FORCING);
                }
                else if (canDouble(bidHistory) && (highOppBid >= nextBid))
                    nextBid = BID_DOUBLE;
                else
                    nextBid = BID_NONE;

                //Fall through if not ok.
                if (nextBid != BID_NONE)
                {
                    bid.bid = nextBid;
                    return bid;
                }
            }

            //if (grand slam)
            if ((noAces == 4) && (noKings >= 3 && ((lowTotPoints >= BID_SUIT_POINT[BID_GRAND_SLAM_INX]) ||
                  ((highTotPoints >= BID_SUIT_POINT[BID_GRAND_SLAM_INX]) && !isMinPoints))))  //37
            {
                //Find proper bid.
                nextBid = MAKE_BID(agree, 7);
                if (nextBid > highBid)
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setCountCard(ANY, ACE, 4);
                    lowFeatures.setCountCard(ANY, KING, 3);
                    highFeatures.setPoints(agree,
                            BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(agree));     //37
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(MUST_PASS);

                    bid.bid = nextBid;
                }
                else if (canDouble(bidHistory) && (highOppBid >= nextBid))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;
                
                return bid;
            }
            //if (small slam)
            if (((noAces >= 3) && ((lowTotPoints >= BID_SUIT_POINT[BID_SMALL_SLAM_INX]) ||
                                   ((highTotPoints >= BID_SUIT_POINT[BID_SMALL_SLAM_INX]) && !isMinPoints))))        //33
            {
                //Find proper bid.
                nextBid = MAKE_BID(agree, 6);
                if (nextBid > highBid)
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setCountCard(ANY, ACE, 3);
                    highFeatures.setPoints(agree,
                           BID_SUIT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(agree));  //33
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(MUST_PASS);

                    bid.bid = nextBid;
                }
                else if (canDouble(bidHistory) && (highOppBid >= nextBid))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;

                return bid;
            }

            //if too high  //escape from slam try.
            int maxLevel = (agree == NOTRUMP) ? (3) : ((agree == SPADES) || (agree == HEARTS)) ? (4) : (5);
            Bids escBid = MAKE_BID(agree, maxLevel);
            if (highOwnBid > escBid)
            {
                int level = BID_LEVEL(highOwnBid);
                nextBid = BID_PASS;
                if (BID_SUIT(highOwnBid) != agree)                      //Might happen with control bid after agreement.
                {
                    level++;
                    nextBid = MAKE_BID(agree, BID_LEVEL(highOwnBid) + 1);
                }

                Bids cmpBid = MAKE_BID(agree, level);
                if (highOppBid < cmpBid)
                    bid.bid = nextBid;
                else if (canDouble(bidHistory) && (highOppBid >= cmpBid))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;

                return bid;
            }
        }

        //if (game is possible in minor - check if we should play for 3NT instead)
        if ((agree == DIAMONDS) || (agree == CLUBS))
        {
            //if (game is possible - in nt) //Should really be tested on NOTRUMP points, but this is
            //of course not possible. We use minor points instead.
            if (lowTotPoints >= BID_NT_POINT[BID_NT_GAME_INX])
            {
                //Possibility for 3NT?
                if (BID_LEVEL(highBid) <= 3)
                {
                    //if (3NT)
                    int i;
                    for (i = 0; i < 4; i++)
                        if (((Suit)i != newSuitAgree) &&
                                (ownFeatures.getStopNT((Suit)i) < 3) && (lowPartnerFeatures.getStopNT((Suit)i) < 3))
                            break;
                    if ( i == 4)
                    {
                        CFeatures lowFeatures;
                        CFeatures highFeatures;
                        pRule->getFeatures(&lowFeatures, &highFeatures);
                        for (int i = 0; i < 4; i++)
                            if (((Suit)i != agree) &&
                                    (lowOwnFeatures.getStopNT((Suit)i) < 3) && (lowPartnerFeatures.getStopNT((Suit)i) < 3))
                                lowFeatures.setStopNT((Suit)i, 3);
                        pRule->setFeatures(lowFeatures, highFeatures);
                        pRule->setStatus(MUST_PASS);

                        bid.bid = BID_3NT;

                        return bid;
                    }

                    //if (check stoppers)
                    int first = (BID_LEVEL(highBid) < 3) ? (0) : (BID_SUIT(highBid) + 1);
                    for (int i = first; i < 4; i++)
                    if ((Suit)i != agree)
                    {
                        //if (stopper quality is ok)
                        if (ownFeatures.getStopNT((Suit)i) >= 3)
                        {
                            CFeatures lowFeatures;
                            CFeatures highFeatures;
                            pRule->getFeatures(&lowFeatures, &highFeatures);
                            lowFeatures.setStopNT((Suit)i, 3);
                            pRule->setFeatures(lowFeatures, highFeatures);
                            pRule->setStatus(FORCING);

                            bid.bid = MAKE_BID(i, 3);

                            return bid;
                        }
                    }
                }
            }
        }

        //if (game is possible in major or in nt or in minor)
        {
            Bids nextBid;
            int low, high;

            //Get point interval and bid.
            getLevel(agree, lowPartnerFeatures.getPoints(agree), ownFeatures.getPoints(agree), &nextBid,
                     &low, &high);

            //Assure agreed suit is bidded as the last bidded suit.
            if ((BID_SUIT(highOPBid) != agree) && (highOPBid > nextBid))
            {
                Suit highSuit = BID_SUIT(highOPBid);
                int highLevel = BID_LEVEL(highOPBid);
                int level = (highSuit > agree) ? (highLevel + 1) : (highLevel);
                nextBid = MAKE_BID(agree, level);
            }

            //Assure game is bidded if status is game forcing.
            int size = bidHistory.bidList.size();
            if ((bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING) &&
                    (nextBid < game))
                nextBid = game;

            //No reason to get higher than necessary.
            //If small slam is bidded, then this is based on points only (falls sometimes through to here).
            if ((nextBid > game) && (suitAgree == agree) && (nextBid < BID_6C))
                    nextBid = game;
            if ((nextBid <= highOPBid) || ((highOPBid > game) &&
                 !IS_BID(bidHistory.bidList[size - 1].bid) &&
                 (bidHistory.bidList[size - 2].bid == game)))
                nextBid = BID_PASS;

            //Find the proper bid to bid.
            if ((nextBid > highBid) || (nextBid == BID_PASS))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setPoints(agree, low);
                highFeatures.setPoints(agree, high);
                if ((agree != NOTRUMP) && (lowPartnerFeatures.getSuitLen(agree) + lowOwnFeatures.getSuitLen(agree)) < 8)
                    lowFeatures.setSuitLen(agree, 8 - lowPartnerFeatures.getSuitLen(agree));
                pRule->setFeatures(lowFeatures, highFeatures);

                int inx = ((agree == SPADES) || (agree == HEARTS) || (agree == NOTRUMP)) ?
                            (BID_SUIT_MAJOR_GAME_INX) :(BID_SUIT_MINOR_GAME_INX);                   //26, 29

                if ((((high + highPartnerFeatures.getPoints(agree)) < BID_SUIT_POINT[inx]) && (suitAgree == agree)) ||              //26
                        (nextBid == game) || (nextBid >= BID_6C))
                    pRule->setStatus(MUST_PASS);    //Game is not possible or game/small slam is bidded.

                bid.bid = nextBid;
            }
            else if (canDouble(bidHistory) && (highOppBid >= nextBid))
                bid.bid = BID_DOUBLE;
            else
                bid.bid = BID_PASS;

            return bid;
        }
    }

    //Not agreed on suit?
    else
    {
        //Find new suit (if possible).
        {
            int newSuit;
            for (newSuit = 0; newSuit < 4; newSuit++)
                if ((lowPartnerFeatures.getSuitLen((Suit)newSuit) == 0) && (lowOwnFeatures.getSuitLen((Suit)newSuit) == 0) &&
                        (!oppSuit[newSuit]) && (ownFeatures.getSuitLen((Suit)newSuit) >= 4))
                    break;
            //Found new suit?
            if ((newSuit < 4) && ((BID_SUIT(highBid) > newSuit) || (newSuitAgree != NOTRUMP)))
            {
                int low = -1;
                int  high = -1;
                int newLevel = (newSuit > BID_SUIT(highBid)) ? (BID_LEVEL(highBid)) : (BID_LEVEL(highBid) + 1);

                int points = ownFeatures.getPoints(NOTRUMP);                
                Bids newBid = MAKE_BID(newSuit, newLevel);

                int size = bidHistory.bidList.size();
                int bidder = nextBidder(bidHistory);

                //Response from openers partner.
                if (bidder == OPEN_RESPONSE)
                {
                    //Check if new bid is possible. Otherwise skip.
                    Bids partnerBid = bidHistory.bidList[size - 2].bid;
                    //Jump in new suit.
                    if ((((newBid - partnerBid) / 5) > 0) && (points < NEWSUIT_P1_J))                 //16
                        newBid = BID_NONE;
                    //New suit level 1.
                    else if ((((newBid - partnerBid) / 5) == 0) && (newLevel == 1) && (points < NEWSUIT_P1_1)) //6
                        newBid = BID_NONE;
                    //New suit level 2.
                    else if ((((newBid - partnerBid) / 5) == 0) && (newLevel == 2) && (points < NEWSUIT_P1_2))  //10
                        newBid = BID_NONE;

                    //Check for catch all 1NT.
                    if ((newBid == BID_NONE) &&
                        (BID_LEVEL(highOwnBid) == 1) && !(BID_LEVEL(highOppBid) > 1) && !oppSuit[NOTRUMP] &&
                            (points >= CATCHALL_NT_L) && (points <= CATCHALL_NT_H))            //6-9
                        newBid = BID_1NT;

                    //Check for jump (not really needed).
//                    if ((((newBid - partnerBid)/5) == 0) && (points >= NEWSUIT_P1_J))     //16
//                        newBid = Bids(newBid + 5);

                    //Points.
                    low = CATCHALL_NT_L;                //6
                    //Catch all 1NT (6-9)
                    if (newBid == BID_1NT)
                        high = CATCHALL_NT_H;           //9
                    //Jump in new suit.
                    else if (((newBid - partnerBid) / 5) > 0)
                        low = NEWSUIT_P1_J;                         //16
                    //New suit level 2.
                    else if ((((newBid - partnerBid) / 5) == 0) && (newLevel == 2))
                        low = NEWSUIT_P1_2;                 //10
                    //New suit level 1.
                    else if ((((newBid - partnerBid) / 5) == 0) && (newLevel == 1))
                        low = NEWSUIT_P1_1;                 //6
                }

                //Rebid of opener.
                else if (bidder == OPEN_REBID)
                {
                    Bids partnerBid = bidHistory.bidList[size - 2].bid;
                    Bids ownBid = bidHistory.bidList[size - 4].bid;
                    Bids cmpBid = IS_BID(partnerBid) ? (partnerBid) : (ownBid);

                    //Jump in new suit.
                    if ((((newBid - cmpBid) / 5) > 0) && (points < NEWSUIT_O_J_L))      //19
                        newBid = BID_PASS;
                    //Reverse suit.
                    else if ((((newBid - cmpBid) / 5) == 0) && (((newBid - ownBid) / 5) == 1) && (points < NEWSUIT_O_3_L))   //16
                        newBid = BID_PASS;
                    //Simpel new suit.
                    else if ((((newBid - cmpBid) / 5) == 0) && (((newBid - ownBid) / 5) == 0) && (points < NEWSUIT_O_S_L))  //12
                        newBid = BID_PASS;

                    //Check for jump.
                    if ((((newBid - cmpBid)/5) == 0) && (points >= NEWSUIT_O_J_L))          //19
                        newBid = Bids(newBid + 5);

                    //Points.
                    //Jump in new suit.
                    if (((newBid - cmpBid)/5) > 0)
                    {
                        low = NEWSUIT_O_J_L;        //19
                        high = NEWSUIT_O_J_H;       //21
                    }
                    //Reverse suit.
                    else if ((((newBid - cmpBid) / 5) == 0) && (((newBid - ownBid) / 5) == 1))
                    {
                        low = NEWSUIT_O_3_L;        //16
                        high = NEWSUIT_O_3_H;       //21
                    }
                    //Simpel new suit.
                    else if ((((newBid - cmpBid) / 5) == 0) && (((newBid - ownBid) / 5) == 0))
                    {
                        low = NEWSUIT_O_S_L;            //12
                        high = NEWSUIT_O_S_H;           //18
                    }
                }

                //Rebid of openers partner.
                else if (bidder == OPEN_REBID_RESPONSE)
                {
                    int level = BID_LEVEL(newBid);
                    if (points < NEWSUIT_P2_3_L)            //10
                        newBid = BID_NONE;
                    else if ((points >= NEWSUIT_P2_3_L) && (points <= NEWSUIT_P2_3_H) && (level > 3))       //10-12
                        newBid = BID_NONE;
                    else if ((points >= NEWSUIT_P2_3_L) && (points <= NEWSUIT_P2_3_H) && (level <= 3))      //10-12
                    {
                        low = NEWSUIT_P2_3_L;           //10
                        high = NEWSUIT_P2_3_H;          //12
                        newBid = (Bids)(newBid + (3 - level) * 5);
                    }
                    else if ((points >= 13) && (level > 4))
                        newBid = BID_NONE;
                    else if ((points >= NEWSUIT_P2_4) && (level <= 4))          //13
                    {
                        low = NEWSUIT_P2_4;                                     //13
                        newBid = (Bids)(newBid + (4 - level) * 5);
                    }
                }
                else
                    newBid = BID_NONE;

                //Found a bid (fall through otherwise)?
                if (newBid != BID_NONE)
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    if (low != -1)
                        lowFeatures.setPoints(NOTRUMP, low);
                    if (high != -1)
                        highFeatures.setPoints(NOTRUMP, high);
                    if (newBid != BID_1NT)
                        lowFeatures.setSuitLen((Suit)newSuit, 4);
                    pRule->setFeatures(lowFeatures, highFeatures);

                    bid.bid = newBid;

                    return bid;
                }
            }
        }

        //NT?
        if (newSuitAgree == NOTRUMP)
        {
            Bids nextBid;
            int low, high;

            //Get point interval and bid to bid.
            getLevel(NOTRUMP, lowPartnerFeatures.getPoints(newSuitAgree), ownFeatures.getPoints(newSuitAgree), &nextBid,
                     &low, &high);

            //Should we bid pass instead?
            if (((BID_LEVEL(nextBid) < BID_LEVEL(highPartnerBid)) || (BID_LEVEL(nextBid) < BID_LEVEL(highOwnBid))) ||
                    (((BID_LEVEL(nextBid) == BID_LEVEL(highPartnerBid)) || (BID_LEVEL(nextBid) == BID_LEVEL(highOwnBid))) &&
                     (ownSuit[NOTRUMP] || partnerSuit[NOTRUMP])))
                nextBid = BID_PASS;

            //Find useable bid.
            if ((nextBid > highBid) || (nextBid == BID_PASS))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                highFeatures.setDp(newSuitAgree, 1);
                lowFeatures.setPoints(newSuitAgree, low);
                highFeatures.setPoints(newSuitAgree, high);
                pRule->setFeatures(lowFeatures, highFeatures);
                if ((highPartnerFeatures.getPoints(newSuitAgree) + high) < BID_NT_POINT[BID_NT_GAME_INX]) //26
                    pRule->setStatus(MUST_PASS);                //Game is not possible.

                bid.bid = nextBid;
            }
            else if (canDouble(bidHistory) && (highOppBid >= nextBid))
                bid.bid = BID_DOUBLE;
            else
                bid.bid = BID_PASS;

            return bid;
        }

        //Rebid of own suit.
        {
            bool found = false;
            for (int i = 0; i < 4; i++)
                if ((ownSuit[i]) && (ownFeatures.getSuitLen((Suit)i) >= REBID_SL))
                    found = true;

            //Found a suit to rebid?
            if (found)
            {
                //Take the best if there are more than one possibility.
                int j = 0;
                for (int i = 0; i < 4; i++)
                    if (ownSuit[i] && (ownFeatures.getSuitLen((Suit)i) >= REBID_SL) &&              //6
                            (ownFeatures.getPoints((Suit)i) > ownFeatures.getPoints((Suit)j)))
                        j = i;
                int newSuitLevel = (j > BID_SUIT(highBid)) ? (BID_LEVEL(highBid)) : (BID_LEVEL(highBid) + 1);

                Bids nextBid;
                int low, high;

                //Get point interval and bid to bid.
                int level = isNextBidOpen(bidHistory) ? (REBID_P) : (REBID_O);              //6, 12
                getLevel((Suit)j, level, ownFeatures.getPoints((Suit)j), &nextBid,
                         &low, &high);

                //Is the found bid ok (otherwise fall through)?
                if ((BID_LEVEL(nextBid) >= newSuitLevel))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setSuitLen((Suit)j, REBID_SL);              //6
                    lowFeatures.setPoints((Suit)j, low);
                    highFeatures.setPoints((Suit)j, high);
                    pRule->setFeatures(lowFeatures, highFeatures);

                    bid.bid = nextBid;

                    return bid;
                }
            }
            bid.bid = BID_PASS;

            return bid;
        }
    }
    return bid;
}

/**
 * @brief Calculate possible rules for a given bid history and next bid as calculated by getNextBid.
 *
 * Calculate the possible rules by using an algoritmic approach. The method is used after the bidding
 * database runs out for suggestion(s).
 *
 * @param[in] seat Bidders seat.
 * @param[in] bidHistory The bid history.
 * @param[in]bid The bid calculated by getNext bid.
 * @param[in] scoringMethod The scoring method.
 * @param[in] teamVul Team vulnerability.
 * @param[out] pDefRules Calculated rules.
 */
void CBidEngine::calculatepRules(Seat seat, CBidHistory &bidHistory, Bids bid, ScoringMethod scoringMethod,
                                 Team teamVul, QList<CRule *> &pDefRules)
{
    CRule *pRule = new CRule();
    pRule->setdBRule(false);
    pDefRules.append(pRule);

    int size = bidHistory.bidList.size();

    CFeatures& lowPartnerFeatures = bidHistory.getLowPartnerFeatures(seat);
    CFeatures& highPartnerFeatures = bidHistory.getHighPartnerFeatures(seat);
    CFeatures& lowOwnFeatures = bidHistory.getLowOwnFeatures(seat);
    CFeatures& highOwnFeatures = bidHistory.getHighOwnFeatures(seat);

    //Already agrement on suit?
    Suit suitAgree;
    if ((lowPartnerFeatures.getSuitLen(SPADES) + lowOwnFeatures.getSuitLen(SPADES)) >= 8)
        suitAgree = SPADES;
    else if ((lowPartnerFeatures.getSuitLen(HEARTS) + lowOwnFeatures.getSuitLen(HEARTS)) >= 8)
        suitAgree = HEARTS;
    else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) + lowOwnFeatures.getSuitLen(DIAMONDS)) >= 8)
        suitAgree = DIAMONDS;
    else if ((lowPartnerFeatures.getSuitLen(CLUBS) + lowOwnFeatures.getSuitLen(CLUBS)) >= 8)
        suitAgree = CLUBS;
    else if ((highPartnerFeatures.getDp(NOTRUMP) <= 1) || (highOwnFeatures.getDp(NOTRUMP) <= 1))
        suitAgree = NOTRUMP;
    else
        suitAgree = ANY;

    //Agrement on suit in next bid.
    Suit newSuitAgree;
    if (suitAgree != ANY)
        newSuitAgree = suitAgree;
    else if ((lowPartnerFeatures.getSuitLen(SPADES) > 0) && (BID_SUIT(bid) == SPADES))
        newSuitAgree = SPADES;
    else if ((lowPartnerFeatures.getSuitLen(HEARTS) > 0) &&  (BID_SUIT(bid) == HEARTS))
        newSuitAgree = HEARTS;
    else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) > 0) && (BID_SUIT(bid) == DIAMONDS))
        newSuitAgree = DIAMONDS;
    else if ((lowPartnerFeatures.getSuitLen(CLUBS) > 0) && (BID_SUIT(bid) == CLUBS))
        newSuitAgree = CLUBS;
    else if ((BID_SUIT(bid) == NOTRUMP) && !((nextBidder(bidHistory) == OPEN_RESPONSE) && (bid == BID_1NT)))
        newSuitAgree = NOTRUMP;
    else
        newSuitAgree = ANY;

    //Highest bid.
    Bids highBid = BID_NONE;
    for (int i = 0; i < size; i++)
        if (BID_LEVEL(bidHistory.bidList[i].bid) != -1)
            highBid = bidHistory.bidList[i].bid;


    //Must pass is replicated to the next rule (always followed by pass).
    if (mustPass(bidHistory))
    {
        pRule->setStatus(MUST_PASS);

        return;
    }

    //Penalty double (always followed by pass).
    if (isPenaltyDouble(bidHistory, lowPartnerFeatures))
    {
        pRule->setStatus(MUST_PASS);

        return;
    }

    //Take out double (penalty doubles have been handled above)?
    if (isDouble(bidHistory))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;
        pRule->getFeatures(&lowFeatures, &highFeatures);

        int low, high;
        Suit suit = findTakeoutDouble(bid, highBid, &low, &high);
        if (suit == NOTRUMP)
        {
            pRule->getFeatures(&lowFeatures, &highFeatures);
            lowFeatures.setPoints(NOTRUMP, low);
            highFeatures.setPoints(NOTRUMP, high);
            highFeatures.setDp(NOTRUMP, 1);
            for (int i = 0; i < 4; i++)
                if (lowPartnerFeatures.getSuitLen((Suit)i) >= 4)
                    lowFeatures.setStopNT((Suit)i, 3);
        }
        else if (suit != ANY)
        {
            pRule->getFeatures(&lowFeatures, &highFeatures);
            lowFeatures.setPoints((Suit)suit, low);
            highFeatures.setPoints((Suit)suit, high);
            lowFeatures.setSuitLen((Suit)suit, 4);
            pRule->setFeatures(lowFeatures, highFeatures);
        }

        pRule->setFeatures(lowFeatures, highFeatures);

        return;
    }

    //Blackwood or Gerber questions are identified and count of aces/kings updated.
    if (isBlackwoodOrGerberQuestion(bidHistory, suitAgree))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);

        int noPAces = 0;
        for (int i = 0; i < 4; i++)
            if (lowPartnerFeatures.getCountCard((Suit)i, ACE) == 1)
                noPAces++;
        if (lowPartnerFeatures.getCountCard(ANY, ACE) > noPAces)
            noPAces = lowPartnerFeatures.getCountCard(ANY, ACE);

        int noPKings = 0;
        for (int i = 0; i < 4; i++)
            if (lowPartnerFeatures.getCountCard((Suit)i, ACE) == 1)
                noPKings++;
        if (lowPartnerFeatures.getCountCard(ANY, ACE) > noPKings)
            noPKings = lowPartnerFeatures.getCountCard(ANY, ACE);

        //if (Blackwod question about aces)
        if (bidHistory.bidList[size - 2].bid == BID_4NT)
        {
            int countAces = bid - BID_5C;

            //Blackwood ace asking.
            if (countAces == 0)
                pRule->setAlertId(ZERO_OR_FOUR_ACES);
            else if (countAces == 1)
                pRule->setAlertId(ONE_ACE);
            else if (countAces == 2)
                pRule->setAlertId(TWO_ACES);
            else if (countAces == 3)
                pRule->setAlertId(THREE_ACES);

            if ((countAces == 0) && (noPAces == 0))
                countAces = 4;
            if ((countAces >= 0) && (countAces <= 4))
            {
                lowFeatures.setCountCard(ANY, ACE, countAces);
                highFeatures.setCountCard(ANY, ACE, countAces);
            }
        }
        //else if (Blackwood question about kings)
        else if (bidHistory.bidList[size - 2].bid == BID_5NT)
        {
            int countKings = bid - BID_5C;

            //Blackwood king asking.
            if (countKings == 0)
                pRule->setAlertId(ZERO_OR_FOUR_KINGS);
            else if (countKings == 1)
                pRule->setAlertId(ONE_KING);
            else if (countKings == 2)
                pRule->setAlertId(TWO_KINGS);
            else if (countKings == 3)
                pRule->setAlertId(THREE_KINGS);

            if ((countKings == 0) && (noPKings == 0))
                countKings = 4;
            if ((countKings >= 0) && (countKings <= 4))
            {
                lowFeatures.setCountCard(ANY, KING, countKings);
                highFeatures.setCountCard(ANY, KING, countKings);
            }
        }
        //else if (Gerber question about aces)
        else if (bidHistory.bidList[size - 2].bid == BID_4C)
        {
            int countAces = bid - BID_4D;

            //Gerber ace asking.
            if (countAces == 0)
                pRule->setAlertId(ZERO_OR_FOUR_ACES);
            else if (countAces == 1)
                pRule->setAlertId(ONE_ACE);
            else if (countAces == 2)
                pRule->setAlertId(TWO_ACES);
            else if (countAces == 3)
                pRule->setAlertId(THREE_ACES);

            if ((countAces == 0) && (noPAces == 0))
                countAces = 4;
            if ((countAces >= 0) && (countAces <= 4))
            {
                lowFeatures.setCountCard(ANY, ACE, countAces);
                highFeatures.setCountCard(ANY, ACE, countAces);
            }
        }
        //else if (Gerber question about kings)
        else if (bidHistory.bidList[size - 2].bid == BID_5C)
        {
            int countKings = bid - BID_4D;

            //Gerber king asking.
            if (countKings == 0)
                pRule->setAlertId(ZERO_OR_FOUR_KINGS);
            else if (countKings == 1)
                pRule->setAlertId(ONE_KING);
            else if (countKings == 2)
                pRule->setAlertId(TWO_KINGS);
            else if (countKings == 3)
                pRule->setAlertId(THREE_KINGS);

            if ((countKings == 0) && (noPKings == 0))
                countKings = 4;
            if ((countKings >= 0) && (countKings <= 4))
            {
                lowFeatures.setCountCard(ANY, KING, countKings);
                highFeatures.setCountCard(ANY, KING, countKings);
            }
        }

        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(FORCING);

        return;
    }

    //Blackwood?
    if (((newSuitAgree == SPADES) || (newSuitAgree == HEARTS) || (newSuitAgree == DIAMONDS) || (newSuitAgree == CLUBS)) &&
            ((bid == BID_4NT) || (bid == BID_5NT)))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);

        if (bid == BID_4NT)
        {
            //Blackwood ace asking.
            pRule->setAlertId(BLACKWOOD_ACE);

            int points = BID_SUIT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(newSuitAgree);
            if (points < 0) points = 0;
            if ((lowPartnerFeatures.getSuitLen(newSuitAgree) + lowOwnFeatures.getSuitLen(newSuitAgree)) < 8)
                lowFeatures.setSuitLen(newSuitAgree, 8 - lowPartnerFeatures.getSuitLen(newSuitAgree));
            lowFeatures.setPoints(newSuitAgree, points);
        }
        else if (bid == BID_5NT)
        {
            //Blackwood king asking.
            pRule->setAlertId(BLACKWOOD_KING);

            int points = BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(newSuitAgree);
            if (points < 0) points = 0;
            if ((lowPartnerFeatures.getSuitLen(newSuitAgree) + lowOwnFeatures.getSuitLen(newSuitAgree)) < 8)
                lowFeatures.setSuitLen(newSuitAgree, 8 - lowPartnerFeatures.getSuitLen(newSuitAgree));
            lowFeatures.setPoints(newSuitAgree, points);
        }

        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(FORCING);

        return;
    }

    //Gerber?
    if ((suitAgree == NOTRUMP) && ((bid == BID_4C) || (bid == BID_5C)))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);

        if (bid == BID_4C)
        {
            //Gerber ace asking.
            pRule->setAlertId(GERBER_ACE);

            int points = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(suitAgree);
            if (points < 0) points = 0;
            lowFeatures.setPoints(suitAgree, points);
        }
        else if (bid == BID_5C)
        {
            //Gerber king asking.
            pRule->setAlertId(GERBER_KING);

            int points = BID_NT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(suitAgree);
            if (points < 0) points = 0;
            lowFeatures.setPoints(suitAgree, points);
        }

        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(FORCING);

        return;
    }

    //Grand slam?
    if (BID_LEVEL(bid) == 7)
    {
        if (newSuitAgree != ANY)
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;

            pRule->getFeatures(&lowFeatures, &highFeatures);
            highFeatures.setPoints(newSuitAgree,
                                  BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(newSuitAgree));     //37
            pRule->setFeatures(lowFeatures, highFeatures);
            pRule->setStatus(MUST_PASS);
        }
        return;
    }

    //Small slam.
    if (BID_LEVEL(bid) == 6)
    {
        if (newSuitAgree != ANY)
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;

            pRule->getFeatures(&lowFeatures, &highFeatures);
            highFeatures.setPoints(newSuitAgree,
                                  BID_SUIT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(newSuitAgree));  //33
            pRule->setFeatures(lowFeatures, highFeatures);
            pRule->setStatus(MUST_PASS);
        }
        return;
    }

    //Game in nt after minor agree.
    if (((suitAgree == DIAMONDS) || (suitAgree == CLUBS)) && (bid == BID_3NT))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);

        for (int i = 0; i < 4; i++)
            if (((Suit)i != suitAgree) &&
                    (lowOwnFeatures.getStopNT((Suit)i) < 3) && (lowPartnerFeatures.getStopNT((Suit)i) < 3))
                lowFeatures.setStopNT((Suit)i, 3);
        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(MUST_PASS);

        return;
    }

    //Stoppers in search for game in nt after minor agree.
    if (((suitAgree == DIAMONDS) || (suitAgree == CLUBS)) && (BID_LEVEL(bid) == 3) && (suitAgree != BID_SUIT(bid)))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);
        lowFeatures.setStopNT(BID_SUIT(bid), 3);
        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(FORCING);

        return;
    }

    //Limit bid in nt, major or minor.
    if (((suitAgree == NOTRUMP) && (BID_SUIT(bid) == NOTRUMP)) ||
            ((newSuitAgree == SPADES) || (newSuitAgree == HEARTS) || (newSuitAgree == DIAMONDS) || (newSuitAgree == CLUBS))
            && (BID_SUIT(bid) == newSuitAgree))
    {
        Suit agree = ((suitAgree == NOTRUMP) && (BID_SUIT(bid) == NOTRUMP)) ? (suitAgree) : (newSuitAgree);

        int low, high;
        findLevel(agree, lowPartnerFeatures.getPoints(agree), BID_LEVEL(bid), &low, &high);

        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);
        lowFeatures.setPoints(agree, low);
        highFeatures.setPoints(agree, high);
        if ((agree != NOTRUMP) && (lowPartnerFeatures.getSuitLen(agree) + lowOwnFeatures.getSuitLen(agree)) < 8)
            lowFeatures.setSuitLen(agree, 8 - lowPartnerFeatures.getSuitLen(agree));
        pRule->setFeatures(lowFeatures, highFeatures);

        int inx = ((agree == SPADES) || (agree == HEARTS) || (agree == NOTRUMP)) ?
                    (BID_SUIT_MAJOR_GAME_INX) :(BID_SUIT_MINOR_GAME_INX);                   //26, 29
        Bids game = ((agree == SPADES) || (agree == HEARTS) || (agree == NOTRUMP)) ? (BID_3NT) : (BID_5C);

        if ((((high + highPartnerFeatures.getPoints(agree)) < BID_SUIT_POINT[inx]) && (suitAgree == agree)) ||              //26
                (bid == game))
            pRule->setStatus(MUST_PASS);                    //Game is not possible or game is bidded.

        return;
    }

    //New suit or catch all NT.
    if (isNewSuit(newSuitAgree, bid) || (nextBidder(bidHistory) == OPEN_RESPONSE) && (bid == BID_1NT))
    {
        int low = -1;
        int high = -1;

        int bidder = nextBidder(bidHistory);

        //Response from openers partner.
        if (bidder == OPEN_RESPONSE)
        {
            Bids partnerBid = bidHistory.bidList[size - 2].bid;

            //Points.
            low = CATCHALL_NT_L;                //6
            //Catch all 1NT (6-9)
            if (bid == BID_1NT)
                high = CATCHALL_NT_H;           //9
            //Jump in new suit (16+).
            else if (((bid - partnerBid) / 5) > 0)
                low = NEWSUIT_P1_J;             //16
            //New suit level 2 (10+).
            else if ((((bid - partnerBid) / 5) == 0) && (BID_LEVEL(bid) == 2))
                low = NEWSUIT_P1_2;             //10
            //New suit level 1 (6+).
            else if ((((bid - partnerBid) / 5) == 0) && (BID_LEVEL(bid) == 1))
                low = NEWSUIT_P1_1;             //6
        }

        //Rebid of opener.
        else if (bidder == OPEN_REBID)
        {
            Bids partnerBid = bidHistory.bidList[size - 2].bid;
            Bids ownBid = bidHistory.bidList[size - 4].bid;
            Bids cmpBid = IS_BID(partnerBid) ? (partnerBid) : (ownBid);

            //Points.
            //Jump in new suit.
            if (((bid - cmpBid)/5) > 0)
            {
                low = NEWSUIT_O_J_L;                //19
                high = NEWSUIT_O_J_H;               //21
            }
            //Reverse suit.
            else if ((((bid - cmpBid) / 5) == 0) && (((bid - ownBid) / 5) == 1))
            {
                low = NEWSUIT_O_3_L;                //16
                high = NEWSUIT_O_3_H;               //21
            }
            //Simpel new suit.
            else if ((((bid - cmpBid) / 5) == 0) && (((bid - ownBid) / 5) == 0))
            {
                low = NEWSUIT_O_S_L;                //12
                high = NEWSUIT_O_S_H;               //18
            }
        }

        //Rebid of openers partner.
        else if (bidder == OPEN_REBID_RESPONSE)
        {
            int level = BID_LEVEL(bid);
            if (level == 3)
            {
                low = NEWSUIT_P2_3_L;               //10
                high = NEWSUIT_P2_3_H;              //12
            }
            else if (level == 4)
                low = NEWSUIT_P2_4;                 //13
        }

        CFeatures lowFeatures;
        CFeatures highFeatures;
        pRule->getFeatures(&lowFeatures, &highFeatures);
        if (low != -1)
            lowFeatures.setPoints(NOTRUMP, low);
        if (high != -1)
            highFeatures.setPoints(NOTRUMP, high);
        if (bid != BID_1NT)
            lowFeatures.setSuitLen(BID_SUIT(bid), 4);
        pRule->setFeatures(lowFeatures, highFeatures);

        return;
    }

    //New NT.
    if (newSuitAgree == NOTRUMP)
    {
        int low, high;
        findLevel(NOTRUMP, lowPartnerFeatures.getPoints(newSuitAgree), BID_LEVEL(bid), &low, &high);

        CFeatures lowFeatures;
        CFeatures highFeatures;
        pRule->getFeatures(&lowFeatures, &highFeatures);
        highFeatures.setDp(newSuitAgree, 1);
        lowFeatures.setPoints(newSuitAgree, low);
        highFeatures.setPoints(newSuitAgree, high);
        pRule->setFeatures(lowFeatures, highFeatures);
        if ((highPartnerFeatures.getPoints(newSuitAgree) + high) < BID_NT_POINT[BID_NT_GAME_INX]) //26
            pRule->setStatus(MUST_PASS);                //Game is not possible.
    }

    //Rebid own suit.
    if (isRebid(bidHistory, suitAgree, bid))
    {
        int low, high;

        int level = isNextBidOpen(bidHistory) ? (REBID_P) : (REBID_O);          //6, 12
        Suit suit = BID_SUIT(bid);
        int bidLevel = BID_LEVEL(bid);
        findLevel(suit, level, bidLevel, &low, &high);

        CFeatures lowFeatures;
        CFeatures highFeatures;
        pRule->getFeatures(&lowFeatures, &highFeatures);
        lowFeatures.setSuitLen(suit, REBID_SL);                 //6
        lowFeatures.setPoints(suit, low);
        highFeatures.setPoints(suit, high);
        pRule->setFeatures(lowFeatures, highFeatures);

        return;
    }
}

//Can the next bid to bid be a NT bid?
bool CBidEngine::nextBidCanBeNT(CFeatures ownFeatures,
                                CFeatures &lowPartnerFeatures, CFeatures &highPartnerFeatures,
                                CFeatures lowRHFeatures, CFeatures highRHFeatures,
                                CFeatures lowLHFeatures, CFeatures highLHFeatures)
{
    //Do not bid NT if opponent has bid NT.
    if ((highRHFeatures.getDp(NOTRUMP) <= 1) || (highLHFeatures.getDp(NOTRUMP) <= 1))
        return false;

    //Find which suits have been bidded.
    bool ownSuit[4];
    bool oppSuit[4];
    for (int i = 0; i < 4; i++)
        ownSuit[i] = oppSuit[i] = false;

    //Own suit.
    for (int i = 0; i < 4; i++)
    {
        if ((lowPartnerFeatures.getSuitLen((Suit)i) >= 4) || (ownFeatures.getSuitLen((Suit)i) >= 4))
            ownSuit[i] = true;
    }

    //Opp suit.
    for (int i = 0; i < 4; i++)
    {
        if ((lowRHFeatures.getSuitLen((Suit)i) >= 4) || (lowLHFeatures.getSuitLen((Suit)i) >= 4))
            oppSuit[i] = true;
    }

    //Check for distribution.
    bool foundDouble = false;
    int i;
    for (i = 0; i < 4; i++)
    if (!ownSuit[i] && !oppSuit[i])
    {
        int count = ownFeatures.getSuitLen((Suit)i);
        if ((count < 2) || (foundDouble && (count == 2)))
            break;
        if (count == 2)
            foundDouble = true;
    }
    if ( i < 4)
        return false;

    //Check for bids by opponent.
    for (i = 0; i < 4; i++)
    {
        if (!ownSuit[i] && oppSuit[i] && ownFeatures.getStopNT((Suit)i) < 3)
            break;
    }
    if (i < 4)
        return false;

    return true;
}

//Have we bidded must pass?
bool CBidEngine::mustPass(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    return ((size >= 2) && (bidHistory.bidList[size - 2].rules[0]->getStatus() == MUST_PASS));
}

//Have we bidded penalty double?
bool CBidEngine::isPenaltyDouble(CBidHistory &bidHistory, CFeatures lowPartnerFeatures)
{
    int size = bidHistory.bidList.size();

    if ((size < 2) || (bidHistory.bidList[size - 2].bid != BID_DOUBLE))
        return false;

    int i;
    for (i = size - 3; i >= size - 5; i -= 2)
        if (IS_BID(bidHistory.bidList[i].bid))
            break;

    return ((i >= (size - 5)) &&
            ((BID_LEVEL(bidHistory.bidList[i].bid) >= 4) || (lowPartnerFeatures.getPoints(NOTRUMP) >= 12)));
}

//Have we bidded double?
bool CBidEngine::isDouble(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    return ((size >= 2) && (bidHistory.bidList[size - 2].bid == BID_DOUBLE));
}

//Can the next bid be a double bid?
bool CBidEngine::canDouble(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    return (((size >= 1) && (BID_SUIT(bidHistory.bidList[size - 1].bid) != ANY)) ||
            ((size >= 3) && (bidHistory.bidList[size - 1].bid == BID_PASS) &&
                       (bidHistory.bidList[size - 2].bid == BID_PASS) &&
               (BID_SUIT(bidHistory.bidList[size - 3].bid) != ANY)));
}

//Get takeout double bid and point intervals.
Bids CBidEngine::getTakeoutDouble(CFeatures &lowPartnerFeatures, CFeatures &ownFeatures,
                               Bids highBid, int *low, int *high)
{
    //Bid NT?
    if (ownFeatures.getDp(NOTRUMP) <= 1)
    {
        //Hold in opponent suits?
        int i;
        for (i = 0; i < 4; i++)
            if ((lowPartnerFeatures.getSuitLen((Suit)i) >= 4) && (ownFeatures.getStopNT((Suit)i) < 3))
                break;

        //Is NT ok?
        if (i == 4)
        {
            Bids bid;
            getLevel(NOTRUMP, 12, ownFeatures.getPoints(NOTRUMP), &bid, low, high);
            if (bid <= BID_3NT)
                return bid;
        }
    }

    //Did not work with NT. Find the longest suit.
    int suit = 0;
    for (int i = 0; i < 4; i++)
        if ((ownFeatures.getPoints((Suit)i) > ownFeatures.getPoints((Suit)suit)))
            suit = i;

    int points = ownFeatures.getPoints((Suit)suit);
    int firstLevel = (suit > BID_SUIT(highBid)) ? (BID_LEVEL(highBid)) : (BID_LEVEL(highBid) + 1);
    *low = 0;
    *high = 8;
    int level = 0;
    if ((points > 8))
    {
        *low = 9;
        *high = 11;
        level++;
    }
    if (points > 11)
    {
        *low = 12;
        *high = ownFeatures.getMaxPoints();
        level++;
    }
    if ((firstLevel + level) > 4)
    {
        level = 4;
        if (level < firstLevel)
            level = firstLevel;
    }
    else
        level = firstLevel + level;

    return MAKE_BID(suit, level);
}

//Find takeout double point intervals.
Suit CBidEngine::findTakeoutDouble(Bids bid, Bids highBid, int *low, int *high)
{
    Suit suit = BID_SUIT(bid);
    int level = BID_LEVEL(bid);
    Suit highSuit = BID_SUIT(highBid);
    int highLevel = BID_LEVEL(highBid);

    if (suit == NOTRUMP)
        findLevel(suit, 12, level, low, high);

    else if (suit != ANY)
    {
        int firstLevel = (suit > highSuit) ? (highLevel) : (highLevel + 1);

        if ((level - firstLevel) == 0)
        {
            *low = 0;
            *high = 8;
        }
        else if ((level - firstLevel) == 1)
        {
            *low = 9;
            *high = 11;
        }
        else if (((level - firstLevel) == 2) || (level == 4))
        {
            *low = 12;
            *high = 37;
        }
    }
    return suit;
}

//Do we have a Blackwood or Gerber question?
bool CBidEngine::isBlackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit agree)
{
    int size = bidHistory.bidList.size();
    if (size < 2)
        return false;

    Bids bid = bidHistory.bidList[size - 2].bid;
    if ((bid != BID_4NT) && (bid != BID_4C) && (bid != BID_5NT) && (bid != BID_5C))
            return false;

    //Gerber.
    if ((agree == NOTRUMP) && ((bid == BID_4C) || (bid == BID_5C)))
        return true;

    //Blackwood.
    if (((bid == BID_4NT) || (bid == BID_5NT)) && (agree != NOTRUMP))
        return true;

    return false;
}

//Should we ask a Blackwood or Gerber question?
Bids CBidEngine::blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings,
                                      int lowTotPoints, Suit agree)
{
    int size = bidHistory.bidList.size();
    if (size < 2)
        return BID_NONE;

    if (agree == NOTRUMP)
    {
        if ((size < 4) || (bidHistory.bidList[size - 4].bid != BID_4C) &&
                ((noAces <= 2) || ((noAces == 3) && (lowTotPoints >= BID_NT_POINT[BID_GRAND_SLAM_INX]))))
            return BID_4C;
        else if ((size > 4) && ((bidHistory.bidList[size - 4].bid == BID_4C)) &&
                 (noKings <= 3) && (lowTotPoints >= BID_NT_POINT[BID_GRAND_SLAM_INX]))
            return BID_5C;
    }
    else
    {
        if ((size < 4) || (bidHistory.bidList[size - 4].bid != BID_4NT) &&
                ((noAces <= 2) || ((noAces == 3) && (lowTotPoints >= BID_SUIT_POINT[BID_GRAND_SLAM_INX]))))
            return BID_4NT;
        else if ((size > 4) && ((bidHistory.bidList[size - 4].bid == BID_4NT)) &&
                 (noKings <= 3) && (lowTotPoints >= BID_SUIT_POINT[BID_GRAND_SLAM_INX]))
            return BID_5NT;
    }

    return BID_NONE;
}

//Calculate sum number of a given card for own and partner.
int CBidEngine::CalculateNoCards(CFeatures partnerFeatures, CFeatures ownFeatures, int cardVal)
{
    int noOwn = 0;
    for (int i = 0; i < 4; i++)
        if (ownFeatures.getCountCard((Suit)i, cardVal) == 1)
            noOwn++;
    if (ownFeatures.getCountCard(ANY, cardVal) > noOwn)
        noOwn = ownFeatures.getCountCard(ANY, cardVal);

    int noPartner = 0;
    for (int i = 0; i < 4; i++)
        if (partnerFeatures.getCountCard((Suit)i, cardVal) == 1)
            noPartner++;
    if (partnerFeatures.getCountCard(ANY, cardVal) > noPartner)
        noPartner = partnerFeatures.getCountCard(ANY, cardVal);

    return noOwn + noPartner;

}

//Get bid for a given suit and partner and own points.
void CBidEngine::getLevel(Suit agree, int lowPartnerPoints, int ownPoints, Bids *bid, int *low, int *high)
{
    int points = lowPartnerPoints + ownPoints;
    const int *BID_POINT = (agree == NOTRUMP) ? (BID_NT_POINT) : (BID_SUIT_POINT);

    int i;
    for (i = 0; i < (BID_POINT_SIZE - 1); i++)
        if (points < BID_POINT[i])
            break;
    if (i == 0)
    {
        *low = 0;
        *bid = BID_PASS;
        *high = BID_POINT[0] - lowPartnerPoints - 1;
    }
    else
    {
        *bid = MAKE_BID(agree, i);
        *low = (BID_POINT[i - 1] >= lowPartnerPoints) ? (BID_POINT[i - 1] - lowPartnerPoints) : (0);
        *high = BID_POINT[i] - lowPartnerPoints - 1;
    }
}

//Find point interval for a given suit and level.
void CBidEngine::findLevel(Suit agree, int lowPartnerPoints, int bidLevel, int *low, int *high)
{
    const int *BID_POINT = (agree == NOTRUMP) ? (BID_NT_POINT) : (BID_SUIT_POINT);

    if (bidLevel < 1)                   //BID_PASS.
    {
        *low = 0;
        int i;
        for (i = 0; i < (BID_POINT_SIZE - 1); i++)
            if (BID_POINT[i] > lowPartnerPoints)
                break;
        *high = BID_POINT[i] - lowPartnerPoints;
    }
    else
    {

        *low = BID_POINT[bidLevel - 1] - lowPartnerPoints;
        *high = BID_POINT[bidLevel] - lowPartnerPoints;
    }
}

//Is the bid a new suit?
bool CBidEngine::isNewSuit(Suit suitNAgree, Bids bid)
{
    Suit suit = BID_SUIT(bid);

    return (suitNAgree == ANY) && ((suit == SPADES) || (suit == HEARTS) || (suit == DIAMONDS) || (suit == CLUBS));
}

//Do we have a rebid of own suit?
bool CBidEngine::isRebid(CBidHistory &bidHistory, Suit suitPAgree, Bids bid)
{
    Suit bidSuit = BID_SUIT(bid);

    if ((suitPAgree != ANY) || ((bidSuit != CLUBS) && (bidSuit!= DIAMONDS) && (bidSuit != HEARTS) && (bidSuit != SPADES)))
        return false;

    int size = bidHistory.bidList.size();

    int i;
    for (i = size - 4; i >= 0; i -=4)
        if (bidSuit == BID_SUIT(bidHistory.bidList[i].bid))
            break;

    return (i >= 0);
}

//Is the next bidder the opener?
bool CBidEngine::isNextBidOpen(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    int first = size % 2;
    int i;
    for (i = first; i < size; i += 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;

    return (((size - i) % 4) == 0);
}

//What type is the next bidder (used to discriminate between openers first an second bid
//and partners first and second response i.e the first 4 bids - only used in cases where
//a new suit is bidded)?
int CBidEngine::nextBidder(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    int first = size % 2;
    int i;
    for (i = first; i < size; i += 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;

    int retVal;

    //Opener.
    if ((((size - i) % 4) == 0))
        retVal = ((size - i) == 4) ? (OPEN_REBID) : (OPEN_OTHER);

    //Partner.
    else
        retVal = ((size - i) == 2) ? (OPEN_RESPONSE) : ((size - i) == 6) ? (OPEN_REBID_RESPONSE) : (OPEN_OTHER);

    return retVal;
}

//Is the value minimum?
bool CBidEngine::isMin(int lowValue, int highValue, int value)
{
    return (value < ((highValue - lowValue) / 4 + lowValue));
}

//Is the value maximum?
bool CBidEngine::isMax(int lowValue, int highValue, int value)
{
    return (value > (highValue - (highValue - lowValue) / 4));
}



//Bid names (indexed by enum Bids).
const QString BID_NAMES[] =
{
    "P",
    "1C", "1D", "1H", "1S", "1N",
    "2C", "2D", "2H", "2S", "2N",
    "3C", "3D", "3H", "3S", "3N",
    "4C", "4D", "4H", "4S", "4N",
    "5C", "5D", "5H", "5S", "5N",
    "6C", "6D", "6H", "6S", "6N",
    "7C", "7D", "7H", "7S", "7N",
    "X", "XX", "?"
};

//Convert auction to text (to display).
void CBidEngine::auctionToText(CAuction &auction, QString *text)
{
    *text = "";

    for (int i = 0; i < auction.auction.size(); i++)
    {
        qint8 bid = auction.auction[i];
        if (!(IS_BID(bid) || (bid == BID_PASS) || IS_DOUBLE_BID(bid) || IS_REDOUBLE_BID(bid)))
            bid = 38;

        if (i != 0)
            *text += "-";
        *text += BID_NAMES[bid];
    }
}

