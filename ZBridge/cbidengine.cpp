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

    //Already agreement on suit?
    Suit bidAgree;
    if ((lowPartnerFeatures.getSuitLen(SPADES) + lowOwnFeatures.getSuitLen(SPADES)) >= 8)
        bidAgree = SPADES;
    else if ((lowPartnerFeatures.getSuitLen(HEARTS) + lowOwnFeatures.getSuitLen(HEARTS)) >= 8)
        bidAgree = HEARTS;
    else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) + lowOwnFeatures.getSuitLen(DIAMONDS)) >= 8)
        bidAgree = DIAMONDS;
    else if ((lowPartnerFeatures.getSuitLen(CLUBS) + lowOwnFeatures.getSuitLen(CLUBS)) >= 8)
        bidAgree = CLUBS;
    else if ((highPartnerFeatures.getDp(ANY) <= 1) && highOwnFeatures.getDp(ANY) <= 1)
        bidAgree = NOTRUMP;
    else
        bidAgree = ANY;

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
    if (bidAgree != ANY)
    {
        //Calculate point range.
        int lowTotPoints = lowPartnerFeatures.getPoints(bidAgree) + ownFeatures.getPoints(bidAgree);
        int highTotPoints = highPartnerFeatures.getPoints(bidAgree) + ownFeatures.getPoints(bidAgree);

        //if (game is not possible)
        if (highTotPoints < 26)
        {
            //Bid pass and set points in rule.
            CFeatures lowFeatures;
            CFeatures highFeatures;
            pRule->getFeatures(&lowFeatures, &highFeatures);
            highFeatures.setPoints(bidAgree, 25 - highPartnerFeatures.getPoints(bidAgree));
            pRule->setFeatures(lowFeatures, highFeatures);
            bid.bid = BID_PASS;
            return bid;
        }
        //else if (slam is possible - small or grand)
        else if (highTotPoints >= 33)
        {
            //if (Blackwood or Gerber)
            if (blackwoodOrGerberQuestion(bidHistory, bidAgree))
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

            //if (Blackwood or Gerber question)
            int noAces = CalculateNoCards(lowPartnerFeatures, ownFeatures, ACE);
            int noKings = CalculateNoCards(lowPartnerFeatures, ownFeatures, KING);
            Bids nextBid = blackwoodOrGerberAsk(bidHistory, noAces, noKings, lowTotPoints, highTotPoints);
            if (nextBid != BID_NONE)
            {
                bid.bid = nextBid;
                pRule->setStatus(FORCING);
                return bid;
            }

            //if (possible grand slam)
            if ((noAces == 4) && (highTotPoints >= 35))
            {
                //if (grand slam)
                if ((lowTotPoints >= 35) && (noKings >= 3))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setCountCard(ANY, ACE, 4);
                    lowFeatures.setCountCard(ANY, KING, 3);
                    pRule->setFeatures(lowFeatures, highFeatures);
                    if ((level < 7) || (suit < bidAgree))
                        bid.bid = (bidAgree == NOTRUMP) ? (BID_7NT) : (bidAgree == SPADES) ? (BID_7S) :
                                                         (bidAgree == HEARTS) ? (BID_7H) : (bidAgree == DIAMONDS) ? (BID_7D) : (BID_7C);
                    else
                        bid.bid = BID_DOUBLE;

                    return bid;
                }
            }
            //Small slam?
            else if ((noAces >= 3) && (highTotPoints < 35))
            {
                if ((level < 6) || ((suit < bidAgree) && (level == 6)))
                    bid.bid = (bidAgree == NOTRUMP) ? (BID_6NT) :(bidAgree == SPADES) ? (BID_6S) :
                                                     (bidAgree == HEARTS) ? (BID_6H) : (bidAgree == DIAMONDS) ? (BID_6D) : (BID_6C);
                else
                    bid.bid = BID_DOUBLE;

                return bid;
            }
        }

        //if (game is possible in major)
        else if ((bidAgree == SPADES) || (bidAgree == HEARTS))
        {
            //if (game in major)
            if (lowTotPoints >= 26)
            {
                bid.bid = (bidAgree == SPADES) ? (BID_4S) : (BID_4H);
                pRule->setStatus(MUST_PASS);
            }
            //if (game is possible)
            else if (level < 3)
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                bid.bid = (bidAgree == SPADES) ? (BID_3S) : (BID_3H);
                pRule->getFeatures(&lowFeatures, &highFeatures);
                lowFeatures.setPoints(bidAgree, 26 - ownFeatures.getPoints(bidAgree));
                pRule->setFeatures(lowFeatures, highFeatures);
            }
            else
                bid.bid = BID_DOUBLE;

            return bid;
        }

        //if (game is possible in minor or nt)?
        else
        {
            //if (game is possible)
            if (lowTotPoints >= 26)
            {
                //if (minor)
                if ((bidAgree == DIAMONDS) || (bidAgree == CLUBS))
                {
                    for (int i = 0; i < 4; i++)
                    if ((Suit)i != bidAgree)
                    {
                        //if (stopper quality is ok)
                        if ((ownFeatures.getStopNT((Suit)i) >= 3) && ((level <= 3) && ((Suit)i > suit)))
                        {
                            CFeatures lowFeatures;
                            CFeatures highFeatures;
                            bid.bid = MAKE_BID(i, 3);
                            pRule->getFeatures(&lowFeatures, &highFeatures);
                            lowFeatures.setStopNT((Suit)i, 3);
                            pRule->setFeatures(lowFeatures, highFeatures);

                            return bid;
                        }
                    }
                }
            }
        }
    }

    //Not agreed on suit?
    else
    {
        Suit newBid;
        if ((lowPartnerFeatures.getSuitLen(SPADES) + ownFeatures.getSuitLen(SPADES)) >= 8)
            newBid = SPADES;
        else if ((lowPartnerFeatures.getSuitLen(HEARTS) + ownFeatures.getSuitLen(HEARTS)) >= 8)
            newBid = HEARTS;
        else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) + ownFeatures.getSuitLen(DIAMONDS)) >= 8)
            newBid = DIAMONDS;
        else if ((lowPartnerFeatures.getSuitLen(CLUBS) + ownFeatures.getSuitLen(CLUBS)) >= 8)
            newBid = CLUBS;
        else if ((highPartnerFeatures.getDp(ANY) <= 1) && ownFeatures.getDp(ANY) <= 1)
            newBid = NOTRUMP;
        else
            newBid = ANY;

        //New suit, nt, rebid?
        if (newBid == ANY)
        {
            //NT bid?
            if (ownFeatures.getDp(ANY) <= 1)
            {
                int level = ((ownLevel > partnerLevel) ? (ownLevel) : (partnerLevel)) + 1;
                bool ok = true;
            }
        }

        //NT bid?
        else if (newBid == NOTRUMP)
        {

        }

        if (lowPartnerFeatures.getPoints(ANY) + ownFeatures.getPoints(newBid))
            ;

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

    int size = bidHistory.bidList.size();
    int first = size % 2;

    bool nextBidderIsFirstBidder = (((size - first) % 4) == 0);

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
        //Get the most narrow range across bids.
        lowPartnerFeatures.delimitFeatures(lowRuleFeatures, true);
        highPartnerFeatures.delimitFeatures(highRuleFeatures, false);
    }

    //Determine range of own features.
    CFeatures lowOwnFeatures;
    lowOwnFeatures.setMinFeatures();
    CFeatures highOwnFeatures;
    highOwnFeatures.setMaxFeatures();
    for (int i = size - 4; i >= 0; i -= 4)
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
        //Get the most narrow range across bids.
        lowOwnFeatures.delimitFeatures(lowRuleFeatures, true);
        highOwnFeatures.delimitFeatures(highRuleFeatures, false);
    }

    //Already agreement on suit?
    Suit bidAgree;
    if ((lowPartnerFeatures.getSuitLen(SPADES) + lowOwnFeatures.getSuitLen(SPADES)) >= 8)
        bidAgree = SPADES;
    else if ((lowPartnerFeatures.getSuitLen(HEARTS) + lowOwnFeatures.getSuitLen(HEARTS)) >= 8)
        bidAgree = HEARTS;
    else if ((lowPartnerFeatures.getSuitLen(DIAMONDS) + lowOwnFeatures.getSuitLen(DIAMONDS)) >= 8)
        bidAgree = DIAMONDS;
    else if ((lowPartnerFeatures.getSuitLen(CLUBS) + lowOwnFeatures.getSuitLen(CLUBS)) >= 8)
        bidAgree = CLUBS;
    else if ((lowPartnerFeatures.getDp(ANY) <= 1) && lowOwnFeatures.getDp(ANY) <= 1)
        bidAgree = NOTRUMP;
    else
        bidAgree = ANY;

    //Get bidded colors and level.
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

    //Opponent suit and level.
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

    //New suit bids.
    Suit suit = BID_SUIT(bid);
    bool newSuit = (suit != NOTRUMP) && !ownSuit[suit] && !partnerSuit[suit] && !oppSuit[suit];

    //Rebids
    bool reBid = (suit != NOTRUMP) && ownSuit[suit] && !partnerSuit[suit];

    //nt bids.
    bool ntBid = (BID_SUIT(bid) == NOTRUMP);
/*
    //Agreement on Major suit.
    if ((bidAgree == SPADES) || (bidAgree == HEARTS))
    {
        int lowTotPoints = lowPartnerFeatures.getPoints(bidAgree) + ownFeatures.getPoints(bidAgree);
        int highTotPoints = highPartnerFeatures.getPoints(bidAgree) + ownFeatures.getPoints(bidAgree);
        //Not game?
        if (highTotPoints < 26)
        {
            CFeatures lowFeatures;
            CFeatures highFeatures;
            pRule->getFeatures(&lowFeatures, &highFeatures);
            highFeatures.setPoints(bidAgree, 26 - highPartnerFeatures.getPoints(bidAgree));
            pRule->setFeatures(lowFeatures, highFeatures);
            bid.bid =BID_PASS;
        }
        //Possible slam?
        else if (lowTotPoints >= 31)
        {
            bool ace[4];
            bool king[4];
            for (int i = 0; i < 4; i++)
            {
                ace[i] = (ownFeatures.getCountCard((Suit)i, ACE) == 1) || (ownFeatures.getSuitLen((Suit)i) == 0);
                king[i] = (ownFeatures.getCountCard((Suit)i, KING) == 1);
            }
            int level = 1;
            Suit suit =ANY;
            for (int i = 0; i < size; i++)
            if (BID_LEVEL(bidHistory.bidList[i].bid) != -1)
            {
                level = BID_LEVEL(bidHistory.bidList[i].bid);
                suit = BID_SUIT(bidHistory.bidList[i].bid);
            }
            int noAces = 0;
            int noKings = 0;
            for (int i = 0; i < 4; i++)
            {
                if (ace[i])
                    noAces++;
                if (king[i])
                    noKings++;
            }

            //Possible grand slam?
            if ((noAces == 4) && (lowTotPoints >= 35))
            {
                //Definately grand slam?
                if (noKings >= 3)
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    lowFeatures.setCountCard(ANY, ACE, 4);
                    lowFeatures.setCountCard(ANY, KING, 3);
                    pRule->setFeatures(lowFeatures, highFeatures);
                    if ((level < 7) || (suit < bidAgree))
                    {
                        bid.bid = (bidAgree == HEARTS) ? (BID_7H) : (BID_7S);
                    }
                    else
                        bid.bid = BID_DOUBLE;
                }
                //Ask partner about kings first.
                else
                {
                    if ((level < 5) || ((suit < NOTRUMP) && (level == 5)))
                    {
                        bid.bid = BID_5NT;
                        pRule->setId(SPC_Q_5NT);
                        pRule->setStatus(FORCING);
                    }
                    else
                        bid.bid = BID_DOUBLE;
                }
            }
            //Small slam?
            else if ((noAces >= 3) && (highTotPoints < 35))
            {
                if ((level < 6) || ((suit < bidAgree) && (level == 6)))
                {
                    bid.bid = (bidAgree == HEARTS) ? (BID_6H) : (BID_6S);
                }
                else
                    bid.bid = BID_DOUBLE;
            }
            //Check for hold first.
            else
            {

            }

        }
        //Game
        else
        {

        }
    }

    //Agreement on Minor suit?
    else if ((bidAgree == DIAMONDS) || (bidAgree == CLUBS))
    {

    }

    //Agreemet on NT?
    else if (bidAgree == NOTRUMP)
    {

    }

    //No agreement on suit.
    else
    {

    }
*/
}

bool CBidEngine::blackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit bidAgree)
{
    int size = bidHistory.bidList.size();
    if (size < 2)
        return false;

    Bids bid = bidHistory.bidList[size - 2].bid;
    if ((bid != BID_4NT) && (bid != BID_4C) && (bid != BID_5NT) && (bid != BID_5C))
            return false;

    bool agree = (bidAgree == SPADES) || (bidAgree == HEARTS) || (bidAgree == DIAMONDS) || (bidAgree == CLUBS);

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

Bids CBidEngine::blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings, int lowTotPoints, int highTotPoints)
{
    return BID_PASS;
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

