/* Erik Aagaard Knudsen.
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

const int BID_POINT_SIZE = 7;
const int BID_SUIT_POINT[BID_POINT_SIZE] = {17, 20, 22, 25, 28, 33, 37};
const int BID_NT_POINT[BID_POINT_SIZE] = {17, 23, 26, 28, 29, 33, 37};
const int BID_SUIT_MAJOR_GAME_INX = 3;
const int BID_SUIT_MINOR_GAME_INX = 4;
const int BID_NT_GAME_INX = 2;
const int BID_SMALL_SLAM_INX = 5;
const int BID_GRAND_SLAM_INX = 6;

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
 * @param[in] cards The cards for the next bidder.
 * @param[in] scoringMethod The scoring method.
 * @param[in] teamVul Team vulnerability.
 * @return The determined next bid.
 */
CBid CBidEngine::getNextBid(Seat seat, CBidHistory &bidHistory, int cards[], ScoringMethod scoringMethod,
                            Team teamVul)
{
    assert ((bidHistory.bidList.size() == 0) ? true : (((bidHistory.bidList.last().bidder + 1) % 4) == seat));

    CBid bid;
    bid.bidder = seat;

    CAuction auction;
    QList<CAuction> subAuction;
    CFeatures features;
    QList<CRule *> pDefRules;
    QList<qint8> defBids;

    //Calculate features.
    features.setCardFeatures(cards);

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
 * runs out for suggestion(s) and it only covers bids that are not covered by the bidding database.\n\n
 *
 * Note the rule used for the calculated bid is also calculated and returned with the bid.
 * This rule is only used for debugging and does not take part in anything else.\n\n
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
 * @param[in] scoringMethod The scoring method.
 * @param[in] teamVul Team vulnerability.
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

    //Determine range of partner features.
    CFeatures lowPartnerFeatures;
    lowPartnerFeatures.setMinFeatures();
    CFeatures highPartnerFeatures;
    highPartnerFeatures.setMaxFeatures();
    for (int i = size - 2; i >= 0; i -= 4)
    {
        //For the rules of a bid get the widest range for all features.
        CFeatures lowRuleFeatures;
        lowRuleFeatures.setMaxFeatures();
        CFeatures highRuleFeatures;
        highRuleFeatures.setMinFeatures();
        for (int j = 0; j < bidHistory.bidList[i].rules.size(); j++)
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;
            bidHistory.bidList[i].rules[j]->getFeatures(&lowFeatures, &highFeatures);
            lowRuleFeatures.delimitFeatures(lowFeatures, false);
            highRuleFeatures.delimitFeatures(highFeatures, true);
        }
        //Get the most narrow range.
        lowPartnerFeatures.delimitFeatures(lowRuleFeatures, true);
        highPartnerFeatures.delimitFeatures(highRuleFeatures, false);
    }

    //Determine range of own features.
    CFeatures lowOwnFeatures;
    lowOwnFeatures.setMinFeatures();
    CFeatures highOwnFeatures;
    highOwnFeatures.setMaxFeatures();
    for (int i = size - 1; i >= 0; i -= 4)
    {
        //For the rules of a bid get the widest range for all features.
        CFeatures lowRuleFeatures;
        lowRuleFeatures.setMaxFeatures();
        CFeatures highRuleFeatures;
        highRuleFeatures.setMinFeatures();
        for (int j = 0; j < bidHistory.bidList[i].rules.size(); j++)
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;
            bidHistory.bidList[i].rules[j]->getFeatures(&lowFeatures, &highFeatures);
            lowRuleFeatures.delimitFeatures(lowFeatures, false);
            highRuleFeatures.delimitFeatures(highFeatures, true);
        }
        //Get the most narrow range.
        lowOwnFeatures.delimitFeatures(lowRuleFeatures, true);
        highOwnFeatures.delimitFeatures(highRuleFeatures, false);
    }

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
    else if ((highPartnerFeatures.getDp(ANY) <= 1) || (highOwnFeatures.getDp(ANY) <= 1))
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
    else if (ownFeatures.getDp(ANY) <= 1)
        newSuitAgree = NOTRUMP;
    else
        newSuitAgree = ANY;

    //Get bidded suits and highest level.
    bool oppSuit[5];
    Suit highOppSuit = ANY;
    int highOppLevel = 0;
    bool ownSuit[5];
    Suit highOwnSuit = ANY;
    int highOwnLevel = 0;
    bool partnerSuit[5];
    Suit highPartnerSuit = ANY;
    int highPartnerLevel = 0;
    for (int i = 0; i < 5; i++)
        oppSuit[i] = ownSuit[i] = partnerSuit[i] = false;

    //Opponent suits and highest level.
    for (int i = size - 1; i >= 0; i -= 2)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            oppSuit[suit] = true;
            if (highOppLevel == 0)
            {
                highOppSuit = suit;
                highOppLevel = BID_LEVEL(bidHistory.bidList[i].bid);
            }
        }
    }

    //Partner suit and highest level.
    for (int i = size - 2; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            partnerSuit[suit] = true;
            if (highPartnerLevel == 0)
            {
                highPartnerSuit = suit;
                highPartnerLevel = BID_LEVEL(bidHistory.bidList[i].bid);
            }
        }
    }

    //Own suit and highest level.
    for (int i = size - 4; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            ownSuit[suit] = true;
            if (highOwnLevel == 0)
            {
                highOwnSuit = suit;
                highOwnLevel = BID_LEVEL(bidHistory.bidList[i].bid);
            }
        }
    }

    //Highest bid (suit and level).
    int highLevel = 1;
    Suit highSuit =ANY;
    for (int i = 0; i < size; i++)
        if (BID_LEVEL(bidHistory.bidList[i].bid) != -1)
        {
            highLevel = BID_LEVEL(bidHistory.bidList[i].bid);
            highSuit = BID_SUIT(bidHistory.bidList[i].bid);
        }

    //All doubles are assumed for penalty.
    if (hasDoubled(bidHistory))
    {
        bid.bid = BID_PASS;
        return bid;
    }

    //Check for must pass.
    if (mustPass(bidHistory))
    {
        pRule->setStatus(MUST_PASS);
        bid.bid = BID_PASS;
        return bid;
    }

    //if (agreement in suit or nt)
    if ((newSuitAgree == SPADES) ||(newSuitAgree == HEARTS) ||(newSuitAgree == DIAMONDS) ||(newSuitAgree == CLUBS) ||
            (suitAgree == NOTRUMP))
    {
        Suit agree = (suitAgree == NOTRUMP) ? (suitAgree) : (newSuitAgree);
        Suit nextAgree = (agree == NOTRUMP) ? (ANY) : (agree);

        //Calculate point range.
        int lowTotPoints = lowPartnerFeatures.getPoints(nextAgree) + ownFeatures.getPoints(nextAgree);
        int highTotPoints = highPartnerFeatures.getPoints(nextAgree) + ownFeatures.getPoints(nextAgree);
        bool isMinPoints = isMin(lowOwnFeatures.getPoints(nextAgree), highOwnFeatures.getPoints(nextAgree),
                               ownFeatures.getPoints(nextAgree));
        bool isMaxPoints = isMax(lowOwnFeatures.getPoints(nextAgree), highOwnFeatures.getPoints(nextAgree),
                               ownFeatures.getPoints(nextAgree));

        //if (slam is possible - small or grand)
        if (highTotPoints >= BID_SUIT_POINT[BID_SMALL_SLAM_INX])     //33
        {
            //if (Blackwood or Gerber question)
            if (blackwoodOrGerberQuestion(bidHistory, agree))
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

                    bid.bid = ((countAces == 0) || (countAces == 4)) ? (BID_5C) :
                              (countAces == 1) ? (BID_5D) : (countAces == 2) ? (BID_5H) : (BID_5S);
                }
                //else if (Blackwood question about kings)
                else if (bidHistory.bidList[size - 2].bid == BID_5NT)
                {
                    lowFeatures.setCountCard(ANY, KING, countKings);
                    highFeatures.setCountCard(ANY, KING, countKings);

                    bid.bid = ((countKings == 0) || (countKings == 4)) ? (BID_5C) :
                              (countKings == 1) ? (BID_5D) : (countKings == 2) ? (BID_5H) : (BID_5S);
                }
                //else if (Gerber question about aces)
                else if (bidHistory.bidList[size - 2].bid == BID_4C)
                {
                    lowFeatures.setCountCard(ANY, ACE, countAces);
                    highFeatures.setCountCard(ANY, ACE, countAces);

                    bid.bid = ((countAces == 0) || (countAces == 4)) ? (BID_4D) :
                              (countAces == 1) ? (BID_4H) : (countAces == 2) ? (BID_4S) : (BID_4NT);
                }
                //else if (Gerber question about kings)
                else if (bidHistory.bidList[size - 2].bid == BID_5C)
                {
                    lowFeatures.setCountCard(ANY, KING, countKings);
                    highFeatures.setCountCard(ANY, KING, countAces);

                    bid.bid = ((countKings == 0) || (countKings == 4)) ? (BID_5D) :
                              (countKings == 1) ? (BID_5H) : (countKings == 2) ? (BID_5S) : (BID_5NT);
                }
                if ((highLevel < BID_LEVEL(bid.bid)) || ((highLevel == BID_LEVEL(bid.bid)) && (highSuit < agree)))
                {
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(FORCING);
                }
                else
                    bid.bid = BID_DOUBLE;

                return bid;
            }

            //if (Ask Blackwood or Gerber question)
            int noAces = CalculateNoCards(lowPartnerFeatures, ownFeatures, ACE);
            int noKings = CalculateNoCards(lowPartnerFeatures, ownFeatures, KING);
            Bids nextBid = blackwoodOrGerberAsk(bidHistory, noAces, noKings, highTotPoints, agree);
            if (nextBid != BID_NONE)
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                if (nextBid == BID_4C)
                {
                    int points = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(ANY);
                    if (points < 0) points = 0;
                    lowFeatures.setPoints(ANY, points);
                }
                else if (nextBid == BID_4NT)
                {
                    int points = BID_SUIT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(ANY);
                    if (points < 0) points = 0;
                    if ((lowPartnerFeatures.getSuitLen(nextAgree) + lowOwnFeatures.getSuitLen(nextAgree)) < 8)
                        lowFeatures.setSuitLen(nextAgree, 8 - lowPartnerFeatures.getSuitLen(nextAgree));
                    lowFeatures.setPoints(nextAgree, points);
                }
                else if (nextBid == BID_5C)
                {
                    int points = BID_NT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(ANY);
                    if (points < 0) points = 0;
                    lowFeatures.setPoints(ANY, points);
                }
                else if (nextBid == BID_5NT)
                {
                    int points = BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(ANY);
                    if (points < 0) points = 0;
                    if ((lowPartnerFeatures.getSuitLen(nextAgree) + lowOwnFeatures.getSuitLen(nextAgree)) < 8)
                        lowFeatures.setSuitLen(nextAgree, 8 - lowPartnerFeatures.getSuitLen(nextAgree));
                    lowFeatures.setPoints(nextAgree, points);
                }
                if ((highLevel < BID_LEVEL(nextBid)) || ((highLevel == BID_LEVEL(nextBid)) && agree > highSuit))
                {
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(FORCING);

                    bid.bid = nextBid;
                }
                else if (canDouble(bidHistory) &&
                    ((highOppLevel > BID_LEVEL(nextBid)) || ((highOppLevel == BID_LEVEL(nextBid)) && (highOppSuit >= agree))))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;

                return bid;
            }

            //if (grand slam)
            if ((noAces == 4) && (noKings >= 3 && ((lowTotPoints >= BID_SUIT_POINT[BID_GRAND_SLAM_INX]) ||
                  ((highTotPoints >= BID_SUIT_POINT[BID_GRAND_SLAM_INX]) && !isMinPoints))))  //37
            {
                if ((highLevel < 7) || (highSuit < agree))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setCountCard(ANY, ACE, 4);
                    lowFeatures.setCountCard(ANY, KING, 3);
                    lowFeatures.setPoints(nextAgree,
                            BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getPoints(nextAgree));     //37
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(MUST_PASS);

                    bid.bid = (agree == NOTRUMP) ? (BID_7NT) : (agree == SPADES) ? (BID_7S) :
                              (agree == HEARTS) ? (BID_7H) : (agree == DIAMONDS) ? (BID_7D) : (BID_7C);
                }
                else if (canDouble(bidHistory) &&
                    ((highOppLevel == 7) && (highOppSuit >= agree)))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;
                
                return bid;
            }
            //if (small slam)
            if (((noAces >= 3) && ((lowTotPoints >= BID_SUIT_POINT[BID_SMALL_SLAM_INX]) ||
                                   ((highTotPoints >= BID_SUIT_POINT[BID_SMALL_SLAM_INX]) && !isMinPoints))))        //33
            {
                if ((highLevel < 6) || ((highSuit < agree) && (highLevel == 6)))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setCountCard(ANY, ACE, 3);
                    lowFeatures.setPoints(nextAgree,
                           BID_SUIT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getPoints(nextAgree));  //33
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(MUST_PASS);

                    bid.bid = (agree == NOTRUMP) ? (BID_6NT) :(agree == SPADES) ? (BID_6S) :
                              (agree == HEARTS) ? (BID_6H) : (agree == DIAMONDS) ? (BID_6D) : (BID_6C);
                }
                else if (canDouble(bidHistory) &&
                    ((highOppLevel > 6) || ((highOppLevel == 6) && (highOppSuit >= agree))))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;

                return bid;
            }
        }

        //if (game is possible in major or in nt)
        if ((agree == SPADES) || (agree == HEARTS) || (agree == NOTRUMP))
        {
            int maxLevel = (agree == NOTRUMP) ? (3) : (4);

            //if (too high)  //escape from slam trying.
            if ((highOwnLevel > maxLevel) || ((highOwnLevel == maxLevel) && (highOwnSuit >= agree)))
            {
                pRule->setStatus(MUST_PASS);

                bid.bid = MAKE_BID(agree, highOwnLevel + 1);
            }

            //else if (game in major or nt)
            else if ((lowTotPoints >= BID_SUIT_POINT[BID_SUIT_MAJOR_GAME_INX]) ||
                     ((highTotPoints >= BID_SUIT_POINT[BID_SUIT_MAJOR_GAME_INX] && !isMinPoints)))   //26
            {
                Bids newBid = (agree == NOTRUMP) ? (BID_3NT) : (agree == SPADES) ? (BID_4S) : (BID_4H);
                if ((highLevel < maxLevel) || ((highSuit < agree) && (highLevel == maxLevel)))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setPoints(nextAgree, BID_SUIT_POINT[3] - lowPartnerFeatures.getPoints(nextAgree));
                    highFeatures.setPoints(nextAgree, BID_SUIT_POINT[4] - 1 - lowPartnerFeatures.getPoints(nextAgree));
                    if ((lowPartnerFeatures.getSuitLen(nextAgree) + lowOwnFeatures.getSuitLen(nextAgree)) < 8)
                        lowFeatures.setSuitLen(nextAgree, 8 - lowPartnerFeatures.getSuitLen(nextAgree));
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(MUST_PASS);

                    bid.bid = newBid;
                }
                else if (canDouble(bidHistory) &&
                    ((highOppLevel > BID_LEVEL(newBid)) || ((highOppLevel == BID_LEVEL(newBid)) && (highOppSuit >= agree))))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;
            }
            //else //if (game might be possible)
            else
            {
                Bids nextBid;
                int low, high;

                getLevel(agree, lowPartnerFeatures.getPoints(nextAgree), ownFeatures.getPoints(nextAgree), &nextBid,
                         &low, &high);

                if ((BID_LEVEL(nextBid) > highLevel) || ((highSuit < newSuitAgree) && (BID_LEVEL(nextBid) == highLevel)) ||
                        (nextBid == BID_PASS))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setPoints(nextAgree, low);
                    highFeatures.setPoints(nextAgree, high);
                    if ((lowPartnerFeatures.getSuitLen(nextAgree) + lowOwnFeatures.getSuitLen(nextAgree)) < 8)
                        lowFeatures.setSuitLen(nextAgree, 8 - lowPartnerFeatures.getSuitLen(nextAgree));
                    pRule->setFeatures(lowFeatures, highFeatures);
                    if (highTotPoints < BID_SUIT_POINT[BID_SUIT_MAJOR_GAME_INX])              //26
                        pRule->setStatus(MUST_PASS);                    //Game is not possible.

                    bid.bid = nextBid;
                }
                else if (canDouble(bidHistory) &&
                    ((highOppLevel > BID_LEVEL(nextBid)) || ((highOppLevel == BID_LEVEL(nextBid)) && (highOppSuit >= agree))))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;
            }

            return bid;
        }

        //if (game is possible in minor - might be nt)
        if ((agree == DIAMONDS) || (agree == CLUBS))
        {
            //if (game is possible - in nt) //Should really be tested on NOTRUMP points, but this is
            //of course not possible. We use minor points instead.
            if ((lowTotPoints >= BID_NT_POINT[BID_NT_GAME_INX]) && !isMinPoints)
            {
                //Possibility for 3NT?
                if (highLevel <= 3)
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
                    int first = (highLevel < 3) ? (0) : (highSuit + 1);
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
            //if (game in minor)
            if ((lowTotPoints >= BID_SUIT_POINT[BID_SUIT_MINOR_GAME_INX]) ||
                    ((highTotPoints >= BID_SUIT_POINT[BID_SUIT_MINOR_GAME_INX]) && !isMinPoints))    //29
            {
                if ((highLevel < 5) || ((highSuit < agree) && (highLevel == 5)))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setPoints(nextAgree,
                                BID_SUIT_POINT[BID_SUIT_MINOR_GAME_INX] - lowPartnerFeatures.getPoints(nextAgree));      //29
                    highFeatures.setPoints(nextAgree,
                                 BID_SUIT_POINT[BID_SUIT_MINOR_GAME_INX] + 3 - lowPartnerFeatures.getPoints(nextAgree)); //29
                    if ((lowPartnerFeatures.getSuitLen(nextAgree) + lowOwnFeatures.getSuitLen(nextAgree)) < 8)
                        lowFeatures.setSuitLen(nextAgree, 8 - lowPartnerFeatures.getSuitLen(nextAgree));
                    pRule->setFeatures(lowFeatures, highFeatures);
                    pRule->setStatus(MUST_PASS);

                    bid.bid = (agree == DIAMONDS) ? (BID_5D) : (BID_5C);
                }
                else if (canDouble(bidHistory) &&
                    ((highOppLevel > 5) || ((highOppLevel == 5) && (highOppSuit >= agree))))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;
            }
            //if (game is possible in minor)
            else if ((highLevel < 4)|| ((highSuit < newSuitAgree) && (highLevel == 4)))
            {
                Bids nextBid;
                int low , high;

                getLevel(agree, lowPartnerFeatures.getPoints(nextAgree), ownFeatures.getPoints(nextAgree), &nextBid,
                         &low, &high);

                if ((BID_LEVEL(nextBid) > highLevel) || ((highSuit < newSuitAgree) && (BID_LEVEL(nextBid) == highLevel)) ||
                        (nextBid == BID_PASS))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setPoints(nextAgree, low);
                    highFeatures.setPoints(nextAgree, high);
                    if ((lowPartnerFeatures.getSuitLen(nextAgree) + lowOwnFeatures.getSuitLen(nextAgree)) < 8)
                        lowFeatures.setSuitLen(nextAgree, 8 - lowPartnerFeatures.getSuitLen(nextAgree));
                    pRule->setFeatures(lowFeatures, highFeatures);
                    if (highTotPoints < BID_SUIT_POINT[BID_SUIT_MINOR_GAME_INX])              //29
                        pRule->setStatus(MUST_PASS);                                          //Game is not possible.

                    bid.bid = nextBid;
                }
                else if (canDouble(bidHistory) &&
                    ((highOppLevel > BID_LEVEL(nextBid)) || ((highOppLevel == BID_LEVEL(nextBid)) && (highOppSuit >= agree))))
                    bid.bid = BID_DOUBLE;
                else
                    bid.bid = BID_PASS;
            }
            else
                bid.bid = BID_PASS;

            return bid;
        }
    }

    //Not agreed on suit?
    else
    {
        //Check for double.
        int size = bidHistory.bidList.size();
        if ((size >= 3) && (bidHistory.bidList[size - 2].bid == BID_DOUBLE))
        {
            //Penalty double?
            Suit opp = BID_SUIT(bidHistory.bidList[size - 3].bid);
            if ((lowPartnerFeatures.getPoints(ANY) >= 12) || (highLevel == 4) ||
                    (bidHistory.bidList[size - 1].bid == BID_PASS) && (ownFeatures.getSuitLen(opp) >= 6) &&
                    (ownFeatures.getHonor(opp) >= 3))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setSuitLen(opp, 6);
                lowFeatures.setHonor(opp, 3);
                pRule->setFeatures(lowFeatures, highFeatures);

                bid.bid = BID_PASS;

                return bid;
            }
        }

        //Find new suit (if possible).
        {
            int i;
            for (i = 0; i < 4; i++)
                if ((lowPartnerFeatures.getSuitLen((Suit)i) == 0) && (lowOwnFeatures.getSuitLen((Suit)i) == 0) &&
                        (!oppSuit[i]) && (ownFeatures.getSuitLen((Suit)i) >= 4))
                    break;
            //New suit?
            if ((i < 4) && ((highSuit > i) || (newSuitAgree != NOTRUMP)))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setSuitLen((Suit)i, 4);
                pRule->setFeatures(lowFeatures, highFeatures);

                int newSuitLevel = (i > highSuit) ? (highLevel) : (highLevel + 1);
                bid.bid = MAKE_BID(i, newSuitLevel);

                return bid;
            }
        }

        //NT?
        if (newSuitAgree == NOTRUMP)
        {
            Bids nextBid;
            int low, high;

            getLevel(NOTRUMP, lowPartnerFeatures.getPoints(NOTRUMP), ownFeatures.getPoints(NOTRUMP), &nextBid,
                     &low, &high);

            if ((BID_LEVEL(nextBid) > highLevel) || ((highSuit < newSuitAgree) && (BID_LEVEL(nextBid) == highLevel)) ||
                    (nextBid == BID_PASS))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                highFeatures.setDp(ANY, 1);
                lowFeatures.setPoints(NOTRUMP, low);
                highFeatures.setPoints(NOTRUMP, high);
                pRule->setFeatures(lowFeatures, highFeatures);
                if ((highPartnerFeatures.getPoints(NOTRUMP) + ownFeatures.getPoints(NOTRUMP)) < BID_NT_POINT[BID_NT_GAME_INX]) //26
                    pRule->setStatus(MUST_PASS);                //Game is not possible.

                bid.bid = nextBid;
            }
            else if (canDouble(bidHistory) &&
                ((highOppLevel > BID_LEVEL(nextBid)) || ((highOppLevel == BID_LEVEL(nextBid)) && (highOppSuit >= newSuitAgree))))
                bid.bid = BID_DOUBLE;
            else
                bid.bid = BID_PASS;

            return bid;
        }

        //Rebid.
        {
            bool found = false;
            for (int i = 0; i < 4; i++)
                if ((ownSuit[i]) && (ownFeatures.getSuitLen((Suit)i) >= 6))
                    found = true;
            if (found)
            {
                int j = 0;
                for (int i = 0; i < 4; i++)
                    if (ownSuit[i] && (ownFeatures.getSuitLen((Suit)i) >= 6) &&
                            (ownFeatures.getPoints((Suit)i) > ownFeatures.getPoints((Suit)j)))
                        j = i;
                int newSuitLevel = (j > highSuit) ? (highLevel) : (highLevel + 1);

                Bids nextBid;
                int low, high;

                int level = isNextBidOpen(bidHistory) ? (12) : (6);
                getLevel((Suit)j, level, ownFeatures.getPoints((Suit)j), &nextBid,
                         &low, &high);

                if ((BID_LEVEL(nextBid) >= newSuitLevel))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setSuitLen((Suit)j, 6);
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
 * @return returns a list with possible rules.
 */
void CBidEngine::calculatepRules(Seat seat, CBidHistory &bidHistory, Bids bid, ScoringMethod scoringMethod,
                                 Team teamVul, QList<CRule *> &pDefRules)
{
    CRule *pRule = new CRule();
    pRule->setdBRule(false);
    pDefRules.append(pRule);

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

    //Determine range of partner features.
    CFeatures lowPartnerFeatures;
    lowPartnerFeatures.setMinFeatures();
    CFeatures highPartnerFeatures;
    highPartnerFeatures.setMaxFeatures();
    for (int i = size - 2; i >= 0; i -= 4)
    {
        //For the rules of a bid get the widest range for all features.
        CFeatures lowRuleFeatures;
        lowRuleFeatures.setMaxFeatures();
        CFeatures highRuleFeatures;
        highRuleFeatures.setMinFeatures();
        for (int j = 0; j < bidHistory.bidList[i].rules.size(); j++)
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;
            bidHistory.bidList[i].rules[j]->getFeatures(&lowFeatures, &highFeatures);
            lowRuleFeatures.delimitFeatures(lowFeatures, false);
            highRuleFeatures.delimitFeatures(highFeatures, true);
        }
        //Get the most narrow range.
        lowPartnerFeatures.delimitFeatures(lowRuleFeatures, true);
        highPartnerFeatures.delimitFeatures(highRuleFeatures, false);
    }

    //Determine range of own features.
    CFeatures lowOwnFeatures;
    lowOwnFeatures.setMinFeatures();
    CFeatures highOwnFeatures;
    highOwnFeatures.setMaxFeatures();
    for (int i = size - 1; i >= 0; i -= 4)
    {
        //For the rules of a bid get the widest range for all features.
        CFeatures lowRuleFeatures;
        lowRuleFeatures.setMaxFeatures();
        CFeatures highRuleFeatures;
        highRuleFeatures.setMinFeatures();
        for (int j = 0; j < bidHistory.bidList[i].rules.size(); j++)
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;
            bidHistory.bidList[i].rules[j]->getFeatures(&lowFeatures, &highFeatures);
            lowRuleFeatures.delimitFeatures(lowFeatures, false);
            highRuleFeatures.delimitFeatures(highFeatures, true);
        }
        //Get the most narrow range.
        lowOwnFeatures.delimitFeatures(lowRuleFeatures, true);
        highOwnFeatures.delimitFeatures(highRuleFeatures, false);
    }

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
    else if ((highPartnerFeatures.getDp(ANY) <= 1) || (highOwnFeatures.getDp(ANY) <= 1))
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
    else if (BID_SUIT(bid) == NOTRUMP)
        newSuitAgree = NOTRUMP;
    else
        newSuitAgree = ANY;

    //Get bidded suits and highest level.
    bool oppSuit[5];
    Suit highOppSuit = ANY;
    int highOppLevel = 0;
    bool ownSuit[5];
    Suit highOwnSuit = ANY;
    int highOwnLevel = 0;
    bool partnerSuit[5];
    Suit highPartnerSuit = ANY;
    int highPartnerLevel = 0;
    for (int i = 0; i < 5; i++)
        oppSuit[i] = ownSuit[i] = partnerSuit[i] = false;

    //Opponent suits and highest level.
    for (int i = size - 1; i >= 0; i -= 2)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            oppSuit[suit] = true;
            if (highOppLevel == 0)
            {
                highOppSuit = suit;
                highOppLevel = BID_LEVEL(bidHistory.bidList[i].bid);
            }
        }
    }

    //Partner suit and highest level.
    for (int i = size - 2; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            partnerSuit[suit] = true;
            if (highPartnerLevel == 0)
            {
                highPartnerSuit = suit;
                highPartnerLevel = BID_LEVEL(bidHistory.bidList[i].bid);
            }
        }
    }

    //Own suit and highest level.
    for (int i = size - 4; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            ownSuit[suit] = true;
            if (highOwnLevel == 0)
            {
                highOwnSuit = suit;
                highOwnLevel = BID_LEVEL(bidHistory.bidList[i].bid);
            }
        }
    }

    //Highest bid (suit and level).
    int highLevel = 1;
    Suit highSuit =ANY;
    for (int i = 0; i < size; i++)
        if (BID_LEVEL(bidHistory.bidList[i].bid) != -1)
        {
            highLevel = BID_LEVEL(bidHistory.bidList[i].bid);
            highSuit = BID_SUIT(bidHistory.bidList[i].bid);
        }

    int count;
    int level;

    //Doubles are all for penalty and are just passed.
    if (hasDoubled(bidHistory))
    {
        return;
    }

    //Must pass is replicated to the next rule.
    if (mustPass(bidHistory))
    {
        pRule->setStatus(MUST_PASS);

        return;
    }

    //
    if (blackwoodOrGerberQuestion(bidHistory, suitAgree))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);

        //if (Blackwod question about aces)
        if (bidHistory.bidList[size - 2].bid == BID_4NT)
        {
            int countAces = bid - BID_5C;
            if ((countAces == 0) && (lowPartnerFeatures.getCountCard(suitAgree, ACE) == 0))
                countAces = 4;
            lowFeatures.setCountCard(ANY, ACE, countAces);
            highFeatures.setCountCard(ANY, ACE, countAces);
        }
        //else if (Blackwood question about kings)
        else if (bidHistory.bidList[size - 2].bid == BID_5NT)
        {
            int countKings = bid - BID_5C;
            if ((countKings == 0) && (lowPartnerFeatures.getCountCard(suitAgree, KING) == 0))
                countKings = 4;
            lowFeatures.setCountCard(ANY, KING, countKings);
            highFeatures.setCountCard(ANY, KING, countKings);
        }
        //else if (Gerber question about aces)
        else if (bidHistory.bidList[size - 2].bid == BID_4C)
        {
            int countAces = bid - BID_4D;
            if ((countAces == 0) && (lowPartnerFeatures.getCountCard(ANY, ACE) == 0))
                countAces = 4;
            lowFeatures.setCountCard(ANY, ACE, countAces);
            highFeatures.setCountCard(ANY, ACE, countAces);
        }
        //else if (Gerber question about kings)
        else if (bidHistory.bidList[size - 2].bid == BID_5C)
        {
            int countKings = bid - BID_4D;
            if ((countKings == 0) && (lowPartnerFeatures.getCountCard(ANY, KING) == 0))
                countKings = 4;
            lowFeatures.setCountCard(ANY, KING, countKings);
            highFeatures.setCountCard(ANY, KING, countKings);
        }

        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(FORCING);

        return;
    }

    if (isAnyPass(suitAgree, bid))
    {
        return;
    }

    if (isMinorPass(suitAgree, bid))
    {
        return;
    }

    if (isMajorOrNTPass(suitAgree, bid))
    {
        return;
    }

    if (isBlackwoodQuestion(newSuitAgree, bid))
    {
    }

    if (isGerberQuestion(suitAgree, bid))
    {
        return;
    }

    if ((count = blackwoodAnswer(bidHistory, suitAgree, bid)) != -1)
    {
        return;
    }

    if ((count = gerberAnswer(bidHistory, suitAgree, bid)) != -1)
    {
        return;
    }

    if (isAnyPass(suitAgree, bid))
    {
        return;
    }

    if (isSmallSlam(bid))
    {
        return;
    }

    if (isGrandSlam(bid))
    {
        return;
    }

    if ((level = limitMajorOrMinor(bidHistory, newSuitAgree, bid, lowPartnerFeatures)) != -1)
    {
        return;
    }

//    if (isGameInvitationMinor(bidHistory, newSuitAgree, bid))
    {
        return;
    }

    if (isGameNTAfterStopper(suitAgree, bid))
    {
        return;
    }

    if (isStopperBid(suitAgree, bid))
    {
        return;
    }

    if (isDoublePass(bidHistory, bid))
    {
        return;
    }

    if ((level = limitNT(bidHistory, bid, lowPartnerFeatures)) != -1)
    {
        return;
    }

    if (isNewSuit(bidHistory, newSuitAgree, bid))
    {
        return;
    }

    if (isRebid(bidHistory, suitAgree, bid))
    {
        return;
    }
}


bool CBidEngine::blackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit agree)
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
    if ((bid == BID_4NT) || (bid == BID_5NT))
        return true;

    return false;
}

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
        if (ownFeatures.getCountCard((Suit)i, cardVal) == 1)
            noPartner++;
    if (ownFeatures.getCountCard(ANY, cardVal) > noPartner)
        noPartner = ownFeatures.getCountCard(ANY, cardVal);

    return noOwn + noPartner;

}

Bids CBidEngine::blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings,
                                      int highTotPoints, Suit agree)
{
    int size = bidHistory.bidList.size();
    if (size < 2)
        return BID_NONE;

    if (agree == NOTRUMP)
    {
        if ((size < 4) || (bidHistory.bidList[size - 4].bid != BID_4C) &&
                ((noAces <= 2) || ((noAces == 3) && (highTotPoints >= BID_NT_POINT[6]))))
            return BID_4C;
        else if ((size > 4) && ((bidHistory.bidList[size - 4].bid == BID_4C)) &&
                 (noKings <= 3) && (highTotPoints >= BID_NT_POINT[6]))
            return BID_5C;
    }
    else
    {
        if ((size < 4) || (bidHistory.bidList[size - 4].bid != BID_4NT) &&
                ((noAces <= 2) || ((noAces == 3) && (highTotPoints >= BID_SUIT_POINT[6]))))
            return BID_4NT;
        else if ((size > 4) && ((bidHistory.bidList[size - 4].bid == BID_4NT)) &&
                 (noKings <= 3) && (highTotPoints >= BID_SUIT_POINT[6]))
            return BID_5NT;
    }

    return BID_NONE;
}

void CBidEngine::getLevel(Suit agree, int lowPartnerPoints, int ownPoints, Bids *bid, int *low, int *high)
{
    int points = lowPartnerPoints + ownPoints;
    const int *BID_POINT = (agree == NOTRUMP) ? (BID_NT_POINT) : (BID_SUIT_POINT);

    int i;
    for (i = 0; i < BID_POINT_SIZE; i++)
        if (points < BID_POINT[i])
            break;
    if (i == 0)
    {
        *bid = BID_PASS;
        *high = BID_POINT[0] - lowPartnerPoints - 1;
        *low = 0;
    }
    else
    {
        *bid = MAKE_BID(agree, i);
        *high = BID_POINT[i] - lowPartnerPoints - 1;
        *low = (BID_POINT[i - 1] >= lowPartnerPoints) ? (BID_POINT[i - 1] - lowPartnerPoints) : (0);
    }
}

bool CBidEngine::canDouble(CBidHistory bidHistory)
{
    int size = bidHistory.bidList.size();

    return (((size >= 1) && (BID_SUIT(bidHistory.bidList[size - 1].bid) != ANY)) ||
            ((size >= 3) && (bidHistory.bidList[size - 1].bid == BID_PASS) &&
                       (bidHistory.bidList[size - 2].bid == BID_PASS) &&
               (BID_SUIT(bidHistory.bidList[size - 3].bid) != ANY)));
}

bool CBidEngine::isAnyPass(Suit suitPAgree, Bids bid)
{
    return ((suitPAgree == ANY) && (bid == BID_PASS));
}

bool CBidEngine::isMinorPass(Suit suitPAgree, Bids bid)
{
    return (((suitPAgree == DIAMONDS) || (suitPAgree == CLUBS)) && (bid == BID_PASS));
}

bool CBidEngine::isMajorOrNTPass(Suit suitPAgree, Bids bid)
{
    return (((suitPAgree == NOTRUMP) || (suitPAgree == SPADES) || (suitPAgree == HEARTS)) &&
            (bid == BID_PASS));
}

bool CBidEngine::isBlackwoodQuestion(Suit suitNAgree, Bids bid)
{
    return (((suitNAgree == SPADES) || (suitNAgree == HEARTS) || (suitNAgree == DIAMONDS) || (suitNAgree == CLUBS)) &&
            (bid == BID_4NT));
}

bool CBidEngine::isGerberQuestion(Suit suitPAgree, Bids bid)
{
    return ((suitPAgree == NOTRUMP) &&
            (bid == BID_4C));
}

int CBidEngine::blackwoodAnswer(CBidHistory bidHistory, Suit suitPAgree, Bids bid)
{
    int size = bidHistory.bidList.size();

    if (size < 2)
        return -1;

    if (((suitPAgree == SPADES) || (suitPAgree == HEARTS) || (suitPAgree == DIAMONDS) || (suitPAgree == CLUBS)) &&
             (bidHistory.bidList[size - 2].bid == BID_4NT) && ((bid == BID_5C) || (bid == BID_5D) ||
            (bid == BID_5H) || (bid == BID_5S)))
        return (bid - BID_5C);

    return -1;
}

int CBidEngine::gerberAnswer(CBidHistory bidHistory, Suit suitPAgree, Bids bid)
{
    int size = bidHistory.bidList.size();

    if (size < 2)
        return -1;

    if ((suitPAgree == NOTRUMP) &&
             (bidHistory.bidList[size - 2].bid == BID_4C) && ((bid == BID_4D) ||
             (bid == BID_4H) || (bid == BID_4S) || (bid == BID_4NT)))
        return (bid - BID_4D);

    return -1;
}

bool CBidEngine::isSmallSlam(Bids bid)
{
    return (BID_LEVEL(bid) == 6);
}

bool CBidEngine::isGrandSlam(Bids bid)
{
    return (BID_LEVEL(bid) == 7);
}

int CBidEngine::limitMajorOrMinor(CBidHistory bidHistory, Suit suitNAgree, Bids bid, CFeatures &lowPartnerFeatures)
{
    int level = BID_LEVEL(bid);
    Suit suit = BID_SUIT(bid);
    if ((suit != SPADES) && (suit != HEARTS) && (suit != DIAMONDS) && (suit != CLUBS) || (suit != suitNAgree))
        return -1;
    if ((level > 4) && ((suit == SPADES) || (suit == HEARTS)) ||
        (level > 5) && ((suit == DIAMONDS) && (suit == CLUBS)))
        return -1;

    int size = bidHistory.bidList.size();

    if ((size < 2) || (BID_SUIT(bidHistory.bidList[size - 2].bid) != suit))
        return -1;

    int points = lowPartnerFeatures.getPoints(suit);

    return (BID_SUIT_POINT[level] - points);
}

bool CBidEngine::isGameNTAfterStopper(Suit suitPAgree, Bids bid)
{
    return (((suitPAgree == DIAMONDS) || (suitPAgree == CLUBS)) && (bid == BID_3NT));
}

bool CBidEngine::isStopperBid(Suit suitPAgree, Bids bid)
{
    return (((suitPAgree == DIAMONDS) || (suitPAgree == CLUBS)) && (BID_LEVEL(bid) == 3) && (suitPAgree != BID_SUIT(bid)));
}

bool CBidEngine::isDoublePass(CBidHistory bidHistory, Bids bid)
{
    int size = bidHistory.bidList.size();

    if (size < 2)
        return -1;

    return ((bid == BID_PASS) && (bidHistory.bidList[size -2].bid == BID_DOUBLE));
}

int CBidEngine::limitNT(CBidHistory bidHistory, Bids bid, CFeatures &lowPartnerFeatures)
{
    int level = BID_LEVEL(bid);

    if ((level > 3) || (BID_SUIT(bid) != NOTRUMP))
        return -1;

    int size = bidHistory.bidList.size();
    int first = size % 2;

    //Find first non pass.
    int i;
    for (i = first; i < size; i += 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;

    bool nextBidIsOpen = (((size - i) % 4) == 0);

    //Catchall 1NT?
    if (!nextBidIsOpen && (bid == BID_1NT) && ((size = (i + 2))) &&
                       (BID_LEVEL(bidHistory.bidList[size - 2].bid) == 1))
        return -6;

    int points = lowPartnerFeatures.getHcp(ANY);

    return (BID_NT_POINT[level] - points);
}

bool CBidEngine::isRebid(CBidHistory bidHistory, Suit suitPAgree, Bids bid)
{
    if (suitPAgree != ANY)
        return -1;

    int size = bidHistory.bidList.size();

    int i;
    for (i = size - 4; i >= 0; i -=4)
        if (BID_SUIT(bid) == BID_SUIT(bidHistory.bidList[i].bid))
            break;

    return (i >= 0);
}

bool CBidEngine::isNextBidOpen(CBidHistory bidHistory)
{
    int size = bidHistory.bidList.size();

    int first = size % 2;
    int i;
    for (i = first; i < size; i += 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;

    return (((size - i) % 4) == 0);
}

bool CBidEngine::hasDoubled(CBidHistory bidHistory)
{
    int size = bidHistory.bidList.size();

    int i;
    for (i = size - 2; i >= 0; i -= 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;

    return ((i >= 0) && (bidHistory.bidList[i].bid == BID_DOUBLE));

}

bool CBidEngine::mustPass(CBidHistory bidHistory)
{
    int size = bidHistory.bidList.size();

    return ((size >= 2) && (bidHistory.bidList[size - 2].rules[0]->getStatus() == MUST_PASS));
}

bool CBidEngine::isNewSuit(CBidHistory bidHistory, Suit suitNAgree, Bids bid)
{
    Suit suit = BID_SUIT(bid);

    return (suitNAgree == ANY) && ((suit == SPADES) || (suit == HEARTS) || (suit == DIAMONDS) || (suit == CLUBS));
}

bool CBidEngine::isMin(int lowValue, int highValue, int value)
{
    return (value < ((highValue - lowValue) / 4 + lowValue));
}

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

