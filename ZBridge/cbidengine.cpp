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
 * runs out for suggestion(s).
 *
 * Note the rule used for the calculated bid is also calculated and returned with the bid.
 * This rule is only used for debugging.
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

    //Get auction till now.
    CAuction auction;
    for (int i = 0; i < bidHistory.bidList.size(); i++)
        auction.auction.append(bidHistory.bidList[i].bid);

    QString txt;
    auctionToText(auction, &txt);
    qDebug() << QString(SEAT_NAMES[seat]) + ": " + txt;

    int size = bidHistory.bidList.size();

    //Is next bidder opener (player or defender)?
    int first = size % 2;
    bool nextBidIsOpen = (((size - first) % 4) == 0);

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
    else if ((highPartnerFeatures.getDp(ANY) <= 1) && (lowOwnFeatures.getDp(ANY) <= 1))
        suitAgree = NOTRUMP;
    else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) + lowOwnFeatures.getSuitLen(DIAMONDS)) >= 8)
        suitAgree = DIAMONDS;
    else if ((lowPartnerFeatures.getSuitLen(CLUBS) + lowOwnFeatures.getSuitLen(CLUBS)) >= 8)
        suitAgree = CLUBS;
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
    else if (lowPartnerFeatures.getDp(ANY) <= 1)
        newSuitAgree = NOTRUMP;
    else
        newSuitAgree = ANY;

    //Get bidded suits and highest level.
    bool oppSuit[5];
    int oppLevel = 0;
    bool ownSuit[5];
    bool ownProp[5];
    int ownLevel = 0;
    bool partnerSuit[5];
    bool partnerProp[5];
    int partnerLevel = 0;
    for (int i = 0; i < 5; i++)
        oppSuit[i] = ownSuit[i] = partnerSuit[i] = ownProp[i] = partnerProp[i] = false;

    //Opponent suits and level.
    for (int i = size - 1; i >= 0; i -= 2)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            oppSuit[suit] = true;
            if (oppLevel == 0)
                oppLevel = BID_LEVEL(bidHistory.bidList[i].bid);
        }
    }

    //Partner suit and level.
    for (int i = size - 2; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            partnerSuit[suit] = true;
            if (partnerLevel == 0)
                partnerLevel = BID_LEVEL(bidHistory.bidList[i].bid);
        }
    }

    //Own suit and level.
    for (int i = size - 4; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            ownSuit[suit] = true;
            if (ownLevel == 0)
                ownLevel = BID_LEVEL(bidHistory.bidList[i].bid);
        }
    }

    int level = 1;
    Suit suit =ANY;
    for (int i = 0; i < size; i++)
        if (BID_LEVEL(bidHistory.bidList[i].bid) != -1)
        {
            level = BID_LEVEL(bidHistory.bidList[i].bid);
            suit = BID_SUIT(bidHistory.bidList[i].bid);
        }


    //if (agreement on suit or nt)
    if (newSuitAgree != ANY)
    {
        Suit nextAgree = (newSuitAgree == NOTRUMP) ? (ANY) : (newSuitAgree);

        //Calculate point range.
        int lowTotPoints = lowPartnerFeatures.getPoints(nextAgree) + ownFeatures.getPoints(nextAgree);
        int highTotPoints = highPartnerFeatures.getPoints(nextAgree) + ownFeatures.getPoints(nextAgree);
        bool isMinPoints = isMin(lowOwnFeatures.getPoints(nextAgree), highOwnFeatures.getPoints(nextAgree),
                               ownFeatures.getPoints(nextAgree));
        bool isMaxPoints = isMax(lowOwnFeatures.getPoints(nextAgree), highOwnFeatures.getPoints(nextAgree),
                               ownFeatures.getPoints(nextAgree));

        //if (game is not possible)
        if (highTotPoints < 26)
        {
            //Bid pass and set points in rule.
            CFeatures lowFeatures;
            CFeatures highFeatures;
            pRule->getFeatures(&lowFeatures, &highFeatures);
            highFeatures.setPoints(nextAgree, 25 - highPartnerFeatures.getPoints(nextAgree));
            pRule->setFeatures(lowFeatures, highFeatures);
            bid.bid = BID_PASS;
            return bid;
        }

        //if (slam is possible - small or grand)
        if (highTotPoints >= 33)
        {
            //if (Blackwood or Gerber question)
            if (blackwoodOrGerberQuestion(bidHistory, suitAgree))
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
                pRule->setStatus(FORCING);
                return bid;
            }

            //if (Ask Blackwood or Gerber question)
            int noAces = CalculateNoCards(lowPartnerFeatures, ownFeatures, ACE);
            int noKings = CalculateNoCards(lowPartnerFeatures, ownFeatures, KING);
            Bids nextBid = blackwoodOrGerberAsk(bidHistory, noAces, noKings, lowTotPoints, highTotPoints,
                                                suitAgree, newSuitAgree);
            if (nextBid != BID_NONE)
            {
                if (newSuitAgree == NOTRUMP)
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setDp(ANY, 1);
                    lowFeatures.setPoints(ANY, 37 - lowPartnerFeatures.getPoints(ANY));
                    pRule->setFeatures(lowFeatures, highFeatures);
                }
                bid.bid = nextBid;
                pRule->setStatus(FORCING);
                return bid;
            }

            //if (grand slam)
            if ((noAces == 4) && (noKings >= 3 && ((lowTotPoints >= 37) || ((highTotPoints >= 37) && !isMinPoints))))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setCountCard(ANY, ACE, 4);
                lowFeatures.setCountCard(ANY, KING, 3);
                lowFeatures.setPoints(nextAgree, 37 - lowPartnerFeatures.getPoints(nextAgree));
                pRule->setFeatures(lowFeatures, highFeatures);
                if ((level < 7) || (suit < newSuitAgree))
                    bid.bid = (newSuitAgree == NOTRUMP) ? (BID_7NT) : (newSuitAgree == SPADES) ? (BID_7S) :
                              (newSuitAgree == HEARTS) ? (BID_7H) : (newSuitAgree == DIAMONDS) ? (BID_7D) : (BID_7C);
                else
                    bid.bid = BID_DOUBLE;

                pRule->setStatus(MUST_PASS);
                
                return bid;
            }
            //if (small slam)
            if (((noAces >= 3) && ((lowTotPoints >= 33) || ((highTotPoints >= 33) && !isMinPoints))))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setCountCard(ANY, ACE, 3);
                lowFeatures.setPoints(nextAgree, 33 - lowPartnerFeatures.getPoints(nextAgree));
                pRule->setFeatures(lowFeatures, highFeatures);
                if ((level < 6) || ((suit < newSuitAgree) && (level == 6)))
                    bid.bid = (newSuitAgree == NOTRUMP) ? (BID_6NT) :(newSuitAgree == SPADES) ? (BID_6S) :
                              (newSuitAgree == HEARTS) ? (BID_6H) : (newSuitAgree == DIAMONDS) ? (BID_6D) : (BID_6C);
                else
                    bid.bid = BID_DOUBLE;

                pRule->setStatus(MUST_PASS);

                return bid;
            }
        }

        //if (game is possible in major or in nt)
        if ((newSuitAgree == SPADES) || (newSuitAgree == HEARTS) || (newSuitAgree == NOTRUMP))
        {
            int maxLevel = (newSuitAgree == NOTRUMP) ? (3) : (4);

            //if (game in major or nt)
            if ((lowTotPoints >= 26) || ((highTotPoints >= 26 && !isMinPoints)))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setPoints(nextAgree, 25 - lowPartnerFeatures.getPoints(nextAgree));
                highFeatures.setPoints(nextAgree, 28 - lowPartnerFeatures.getPoints(nextAgree));
                pRule->setFeatures(lowFeatures, highFeatures);
                if ((level < maxLevel) || ((suit < suitAgree) && (level == maxLevel)))
                    bid.bid = (newSuitAgree == NOTRUMP) ? (BID_3NT) : (newSuitAgree == SPADES) ? (BID_4S) : (BID_4H);
                else
                    bid.bid = BID_DOUBLE;

                pRule->setStatus(MUST_PASS);
            }
            //if (game might be possible)
            if ((level < (maxLevel - 1)) || ((suit < newSuitAgree) && (level == (maxLevel - 1))))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                bid.bid = (newSuitAgree == NOTRUMP) ? (BID_2NT) : (newSuitAgree == SPADES) ? (BID_3S) : (BID_3H);
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setPoints(nextAgree, 23 - lowPartnerFeatures.getPoints(nextAgree));
                highFeatures.setPoints(nextAgree, 26 - lowPartnerFeatures.getPoints(nextAgree));
                pRule->setFeatures(lowFeatures, highFeatures);
            }
            else
                bid.bid = BID_DOUBLE;

            return bid;
        }

        //if (game is possible in minor - might be nt)
        if ((newSuitAgree == DIAMONDS) || (newSuitAgree == CLUBS))
        {
            //if (game is possible - in nt)
            if ((lowTotPoints >= 26) && !isMinPoints)
            {
                //Possibility for 3NT?
                if (level <= 3)
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
                        bid.bid = BID_3NT;
                        pRule->getFeatures(&lowFeatures, &highFeatures);
                        for (int i = 0; i < 4; i++)
                            if (((Suit)i != newSuitAgree) &&
                                    (lowOwnFeatures.getStopNT((Suit)i) < 3) && (lowPartnerFeatures.getStopNT((Suit)i) < 3))
                                lowFeatures.setStopNT((Suit)i, 3);
                        pRule->setFeatures(lowFeatures, highFeatures);
                        pRule->setStatus(MUST_PASS);

                        return bid;
                    }

                    //if (check stoppers)
                    int first = (level < 3) ? (0) : (suit + 1);
                    for (int i = first; i < 4; i++)
                    if ((Suit)i != newSuitAgree)
                    {
                        //if (stopper quality is ok)
                        if (ownFeatures.getStopNT((Suit)i) >= 3)
                        {
                            CFeatures lowFeatures;
                            CFeatures highFeatures;
                            bid.bid = MAKE_BID(i, 3);
                            pRule->getFeatures(&lowFeatures, &highFeatures);
                            lowFeatures.setStopNT((Suit)i, 3);
                            pRule->setFeatures(lowFeatures, highFeatures);
                            pRule->setStatus(FORCING);

                            return bid;
                        }
                    }
                }
            }
            //if (not game in minor)
            if (highTotPoints < 29)
            {
                //Bid pass and set points in rule.
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                highFeatures.setPoints(nextAgree, 29 - highPartnerFeatures.getPoints(nextAgree));
                pRule->setFeatures(lowFeatures, highFeatures);
                bid.bid = BID_PASS;
            }
            //if (game in minor)
            if ((lowTotPoints >= 29) || ((highTotPoints >= 29) && !isMinPoints))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setPoints(nextAgree, 28 - lowPartnerFeatures.getPoints(nextAgree));
                highFeatures.setPoints(nextAgree, 31 - lowPartnerFeatures.getPoints(nextAgree));
                pRule->setFeatures(lowFeatures, highFeatures);
                if ((level < 5) || ((suit < newSuitAgree) && (level == 5)))
                    bid.bid = (newSuitAgree == DIAMONDS) ? (BID_5D) : (BID_5C);
                else
                    bid.bid = BID_DOUBLE;

                pRule->setStatus(MUST_PASS);
            }
            //if (game is possible)
            else if ((level < 4)|| ((suit < newSuitAgree) && (level == 4)))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setPoints(nextAgree, 28 - lowPartnerFeatures.getPoints(nextAgree));
                highFeatures.setPoints(nextAgree, 31 - lowPartnerFeatures.getPoints(nextAgree));
                pRule->setFeatures(lowFeatures, highFeatures);
                bid.bid = (newSuitAgree == DIAMONDS) ? (BID_4D) : (BID_4C);
            }
            else
                bid.bid = BID_DOUBLE;

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
            int points = ownFeatures.getPoints(ANY);

            //Penalty double?
            Suit opp = BID_SUIT(bidHistory.bidList[size - 3].bid);
            if ((lowPartnerFeatures.getPoints(ANY) >= 12) || (level == 4) ||
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

            //Respond in nt?
            if (ownFeatures.getDp(ANY) <= 1)
            {
                int i;
                for (i = 0; i < 4; i++)
                    if (oppSuit[i] && !ownFeatures.getStopNT((Suit)i) >= 3)
                        break;
                //Are conditions for nt ok?
                if (i == 4)
                {
                    //1NT?
                    if ((points >= 6) && (points <= 9) && (level == 1) && (suit < NOTRUMP))
                    {
                        CFeatures lowFeatures;
                        CFeatures highFeatures;
                        pRule->getFeatures(&lowFeatures, &highFeatures);
                        highFeatures.setDp(ANY, 1);
                        lowFeatures.setPoints(ANY, 6);
                        highFeatures.setPoints(ANY, 9);
                        for (int i = 0; i < 4; i++)
                            if (oppSuit[i])
                                lowFeatures.setStopNT((Suit)i, 3);
                        pRule->setFeatures(lowFeatures, highFeatures);
                        bid.bid = BID_1NT;
                        return bid;
                    }
                    //2NT?
                    if ((points >= 10) && (points <= 12) && (level < 2) || (level == 2) && (suit < NOTRUMP))
                    {
                        CFeatures lowFeatures;
                        CFeatures highFeatures;
                        pRule->getFeatures(&lowFeatures, &highFeatures);
                        highFeatures.setDp(ANY, 1);
                        lowFeatures.setPoints(ANY, 10);
                        highFeatures.setPoints(ANY, 12);
                        for (int i = 0; i < 4; i++)
                            if (oppSuit[i])
                                lowFeatures.setStopNT((Suit)i, 3);
                        pRule->setFeatures(lowFeatures, highFeatures);
                        bid.bid = BID_2NT;
                        return bid;
                    }
                    //3NT?
                    if ((points >= 13) && (points <= 15) && (level < 3) || (level == 3) && (suit < NOTRUMP))
                    {
                        CFeatures lowFeatures;
                        CFeatures highFeatures;
                        pRule->getFeatures(&lowFeatures, &highFeatures);
                        highFeatures.setDp(ANY, 1);
                        lowFeatures.setPoints(ANY, 13);
                        highFeatures.setPoints(ANY, 15);
                        for (int i = 0; i < 4; i++)
                            if (oppSuit[i])
                                lowFeatures.setStopNT((Suit)i, 3);
                        pRule->setFeatures(lowFeatures, highFeatures);
                        bid.bid = BID_3NT;
                        return bid;
                    }
                }
            }
            //Find longest suit.
            int j = 0;
            for (int i = 1; i <= 4; i++)
                if (ownFeatures.getSuitLen((Suit)i) > ownFeatures.getSuitLen((Suit)j))
                    j = i;
            if (points <= 8)
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                highFeatures.setPoints(ANY, 8);
                pRule->setFeatures(lowFeatures, highFeatures);
                int bidLevel = (j <= suit) ? (level + 1) : (level);
                bid.bid = MAKE_BID(j, bidLevel);
                return bid;
            }
            if ((points > 8) && (points <= 12))
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                highFeatures.setPoints(ANY, 12);
                lowFeatures.setPoints(ANY, 9);
                pRule->setFeatures(lowFeatures, highFeatures);
                int bidLevel = (j <= suit) ? (level + 2) : (level + 1);
                bid.bid = MAKE_BID(j, bidLevel);
                return bid;
            }
            if (points > 12)
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setPoints(ANY, 13);
                pRule->setFeatures(lowFeatures, highFeatures);
                int bidLevel = (j <= suit) ? (level + 3) : (level + 2);
                bid.bid = MAKE_BID(j, bidLevel);
                return bid;
            }
        }

        //Find new suit or nt (if possible.
        int i;
        for (i = 0; i < 4; i++)
            if ((lowPartnerFeatures.getSuitLen((Suit)i) == 0) && (lowOwnFeatures.getSuitLen((Suit)i) == 0) &&
                    (!oppSuit[i]) && (ownFeatures.getSuitLen((Suit)i) >= 4))
                break;
        //New suit?
        if ((i < 4) && ((i > suit) || ((i <= suit) && (ownFeatures.getDp(ANY) > 1))))
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;
            pRule->getFeatures(&lowFeatures, &highFeatures);
            lowFeatures.setSuitLen((Suit)i, 4);
            pRule->setFeatures(lowFeatures, highFeatures);
            int newSuitLevel = (i > suit) ? (level) : (level + 1);
            bid.bid = MAKE_BID(i, newSuitLevel);
            return bid;
        }
        //NT?
        if (ownFeatures.getDp(ANY) <= 1)
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;
            pRule->getFeatures(&lowFeatures, &highFeatures);
            highFeatures.setDp(ANY, 1);
            //EAK How about limits???
            pRule->setFeatures(lowFeatures, highFeatures);
            int newSuitLevel = (suit != NOTRUMP) ? (level) : (level + 1);
            bid.bid = MAKE_BID(NOTRUMP, newSuitLevel);
            return bid;
        }

        //Rebid.
        {
            int j = 0;
            for (int i = 0; i < 4; i++)
                if (ownSuit[i] && ownFeatures.getPoints((Suit)i) > ownFeatures.getPoints((Suit)j))
                    j = i;
            //EAK How about limits.
            int newSuitLevel = (i > suit) ? (level) : (level + 1);
            bid.bid = MAKE_BID(j, newSuitLevel);
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
    QList<CRule *> rules;
    pRule->setdBRule(false);
    rules.append(pRule);

    //Get auction till now.
    CAuction auction;
    for (int i = 0; i < bidHistory.bidList.size(); i++)
        auction.auction.append(bidHistory.bidList[i].bid);

    QString txt;
    auctionToText(auction, &txt);
    qDebug() << QString(SEAT_NAMES[seat]) + ": " + txt;

    int size = bidHistory.bidList.size();

    //Is next bidder opener (player or defender)?
    int first = size % 2;
    bool nextBidIsOpen = (((size - first) % 4) == 0);

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

    CFeatures lowFeatures;
    CFeatures highFeatures;
    pRule->getFeatures(&lowFeatures, &highFeatures);

    //Already agrement on suit?
    Suit suitAgree;
    if ((lowPartnerFeatures.getSuitLen(SPADES) + lowOwnFeatures.getSuitLen(SPADES)) >= 8)
        suitAgree = SPADES;
    else if ((lowPartnerFeatures.getSuitLen(HEARTS) + lowOwnFeatures.getSuitLen(HEARTS)) >= 8)
        suitAgree = HEARTS;
    else if ((highPartnerFeatures.getDp(ANY) <= 1) && (lowOwnFeatures.getDp(ANY) <= 1))
        suitAgree = NOTRUMP;
    else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) + lowOwnFeatures.getSuitLen(DIAMONDS)) >= 8)
        suitAgree = DIAMONDS;
    else if ((lowPartnerFeatures.getSuitLen(CLUBS) + lowOwnFeatures.getSuitLen(CLUBS)) >= 8)
        suitAgree = CLUBS;
    else
        suitAgree = ANY;

    //Agrement on suit in next bid.
    Suit newSuitAgree = BID_SUIT(bid);
    Suit nextAgree = (newSuitAgree == NOTRUMP) ? (ANY) : (newSuitAgree);

    if (newSuitAgree == SPADES)
        lowFeatures.setSuitLen(SPADES, 8 - lowPartnerFeatures.getSuitLen(SPADES));
    else if (newSuitAgree == HEARTS)
        lowFeatures.setSuitLen(HEARTS, 8 - lowPartnerFeatures.getSuitLen(HEARTS));
    else if (newSuitAgree == DIAMONDS)
        lowFeatures.setSuitLen(DIAMONDS, 8 - lowPartnerFeatures.getSuitLen(DIAMONDS));
    else if (newSuitAgree == CLUBS)
        lowFeatures.setSuitLen(CLUBS, 8 - lowPartnerFeatures.getSuitLen(CLUBS));
    else if (newSuitAgree == NOTRUMP)
        lowFeatures.setDp(ANY, 1);

    //Get bidded suits and highest level.
    bool oppSuit[5];
    int oppLevel = 0;
    bool ownSuit[5];
    bool ownProp[5];
    int ownLevel = 0;
    bool partnerSuit[5];
    bool partnerProp[5];
    int partnerLevel = 0;
    for (int i = 0; i < 5; i++)
        oppSuit[i] = ownSuit[i] = partnerSuit[i] = ownProp[i] = partnerProp[i] = false;

    //Opponent suits and level.
    for (int i = size - 1; i >= 0; i -= 2)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            oppSuit[suit] = true;
            if (oppLevel == 0)
                oppLevel = BID_LEVEL(bidHistory.bidList[i].bid);
        }
    }

    //Partner suit and level.
    for (int i = size - 2; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            partnerSuit[suit] = true;
            if (partnerLevel == 0)
                partnerLevel = BID_LEVEL(bidHistory.bidList[i].bid);
        }
    }

    //Own suit and level.
    for (int i = size - 4; i >= 0; i -= 4)
    {
        if (IS_BID(bidHistory.bidList[i].bid))
        {
            Suit suit = BID_SUIT(bidHistory.bidList[i].bid);
            ownSuit[suit] = true;
            if (ownLevel == 0)
                ownLevel = BID_LEVEL(bidHistory.bidList[i].bid);
        }
    }

    int level = 1;
    Suit suit =ANY;
    for (int i = 0; i < size; i++)
        if (BID_LEVEL(bidHistory.bidList[i].bid) != -1)
        {
            level = BID_LEVEL(bidHistory.bidList[i].bid);
            suit = BID_SUIT(bidHistory.bidList[i].bid);
        }
}

bool CBidEngine::blackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit suitAgree)
{
    int size = bidHistory.bidList.size();
    if (size < 2)
        return false;

    Bids bid = bidHistory.bidList[size - 2].bid;
    if ((bid != BID_4NT) && (bid != BID_4C) && (bid != BID_5NT) && (bid != BID_5C))
            return false;

    bool agree = (suitAgree == SPADES) || (suitAgree == HEARTS) || (suitAgree == DIAMONDS) || (suitAgree == CLUBS);

    //Blackwood.
    if (agree && ((bid == BID_4NT) || (bid == BID_5NT)))
        return true;

    bool hasBidNT = false;
    for (int i = size - 4; i >= 0; i -= 4)
        if (BID_SUIT(bidHistory.bidList[size].bid) == NOTRUMP)
            hasBidNT = true;

    //Gerber.
    if (hasBidNT && ((bid == BID_4C) || (bid == BID_5C)))
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

Bids CBidEngine::blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings, int lowTotPoints,
                                      int highTotPoints, Suit suitAgree, Suit newSuitAgree)
{
    return BID_PASS;
}

bool CBidEngine::isAnyPass(Suit suitAgree, Bids bid)
{
    return ((suitAgree == ANY) && (bid == BID_PASS));
}

bool CBidEngine::isMinorPass(Suit suitAgree, Bids bid)
{
    return (((suitAgree == DIAMONDS) || (suitAgree == CLUBS)) && (bid == BID_PASS));
}

bool CBidEngine::isMajorOrNTPass(Suit suitAgree, Bids bid)
{
    return (((suitAgree == NOTRUMP) || (suitAgree == SPADES) || (suitAgree == HEARTS)) &&
            (bid == BID_PASS));
}

bool CBidEngine::isBlackwoodQuestion(Suit suitAgree, Bids bid)
{
    return (((suitAgree == SPADES) || (suitAgree == HEARTS) || (suitAgree == DIAMONDS) || (suitAgree == CLUBS)) &&
            (bid == BID_4NT));
}

bool CBidEngine::isGerberQuestion(Suit suitAgree, Bids bid)
{
    return ((suitAgree == NOTRUMP) &&
            (bid == BID_4C));
}

int CBidEngine::blackwoodAnswer(CBidHistory bidHistory, Suit suitAgree, Bids bid)
{
    int size = bidHistory.bidList.size();

    if (size < 2)
        return -1;

    if (((suitAgree == SPADES) || (suitAgree == HEARTS) || (suitAgree == DIAMONDS) || (suitAgree == CLUBS)) &&
             (bidHistory.bidList[size - 2].bid == BID_4NT) && ((bid == BID_5C) || (bid == BID_5D) ||
            (bid == BID_5H) || (bid == BID_5S)))
        return (bid - BID_5C);

    return -1;
}

int CBidEngine::gerberAnswer(CBidHistory bidHistory, Suit suitAgree, Bids bid)
{
    int size = bidHistory.bidList.size();

    if (size < 2)
        return -1;

    if ((suitAgree == NOTRUMP) &&
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

bool CBidEngine::isGameMajorOrNT(Suit suitAgree, Bids bid)
{
    return (((suitAgree == SPADES) && (bid == BID_4S)) || ((suitAgree == HEARTS) && (bid == BID_4H)) ||
            ((suitAgree == NOTRUMP) && (bid == BID_3NT)));
}

bool CBidEngine::isGameMinor(Suit suitAgree, Bids bid)
{
    return (((suitAgree == DIAMONDS) && (bid == BID_5D)) || ((suitAgree == CLUBS) && (bid == BID_5C)));
}

bool CBidEngine::isGameInvitationMajorOrNT(CBidHistory bidHistory, Suit suitAgree, Bids bid)
{
    int size = bidHistory.bidList.size();

    if ((size < 2) || (BID_SUIT(bidHistory.bidList[size - 2].bid) != BID_SUIT(bid)))
        return -1;

    return (((suitAgree == NOTRUMP) && (bid == BID_2NT)) ||
            ((suitAgree == SPADES) && (bid == BID_3S)) ||
            ((suitAgree == HEARTS) && (bid == BID_3H)));
}

bool CBidEngine::isGameInvitationMinor(CBidHistory bidHistory, Suit suitAgree, Bids bid)
{
    int size = bidHistory.bidList.size();

    if ((size < 2) || (BID_SUIT(bidHistory.bidList[size - 2].bid) != BID_SUIT(bid)))
        return -1;

    return (((suitAgree == DIAMONDS) && (bid == BID_4D)) ||
            ((suitAgree == CLUBS) && (bid == BID_4C)));
}

bool CBidEngine::isGameNTAfterStopper(Suit suitAgree, Bids bid)
{
    return (((suitAgree == DIAMONDS) || (suitAgree == CLUBS)) && (bid == BID_3NT));
}

bool CBidEngine::isStopperBid(Suit suitAgree, Bids bid)
{
    return (((suitAgree == DIAMONDS) || (suitAgree == CLUBS)) && (BID_LEVEL(bid) == 3) && (suitAgree != BID_SUIT(bid)));
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

