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

//Bid limit levels.
const int BID_POINT_SIZE = 8;
const int BID_SUIT_POINT[BID_POINT_SIZE] = {17, 20, 22, 25, 28, 33, 37, 40};
const int BID_NT_POINT[BID_POINT_SIZE] = {17, 22, 26, 28, 29, 33, 37, 40};
const int BID_SUIT_MAJOR_GAME_INX = 3;
const int BID_SUIT_MINOR_GAME_INX = 4;
const int BID_NT_GAME_INX = 2;
const int BID_SMALL_SLAM_INX = 5;
const int BID_GRAND_SLAM_INX = 6;
const int DOUBLE_LIMIT = 12;

//Newbid definitions. For new suit bidding.
const int OPEN_RESPONSE = 1;            //Next bid is response to opener by partner.
const int OPEN_REBID = 2;               //Next bid is rebid of opener.
const int OPEN_REBID_RESPONSE = 3;      //Next bid is second response to opener by partner.
const int OPEN_OTHER = -1;              //Next bid is other bids.
const int OPEN_ALERT = -2;              //Next bid is preceeded by an alert in bidding sequence.
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
 * @param[in] seat Bidders seat.
 * @param[in] ownFeatures The features of the bidders cards.
 * @param[in] bidHistory The bid history.
 * @param[in] scoringMethod The scoring method.
 * @param[in] teamVul Team vulnerability.
 * @return The determined next bid.
 */
CBid CBidEngine::getNextBid(Seat seat, CFeatures &ownFeatures, CBidHistory &bidHistory, ScoringMethod scoringMethod,
                            Team teamVul)
{
    assert ((bidHistory.bidList.size() == 0) ? true : (((bidHistory.bidList.last().bidder + 1) % 4) == seat));

    CBid bid(seat, BID_PASS, "");
    bid.bidder = seat;

    CAuction auction;
    QList<CAuction> subAuction;
    QList<CRule *> pDefRules;
    QList<qint8> defBids;

    //Get relevant pages and rules.
    QSet<qint16> &pages = bidDBDefine->getPages(seat);
    QSet<qint16> &rules = bidDBDefine->getRules(seat);

    //Get auction till now.
    for (int i = 0; i < bidHistory.bidList.size(); i++)
    {
        //Is it a substitute auction?
        if (bidHistory.bidList[i].substitute)
        {
            CAuction oldAuction = auction;
            auction = findSubstituteAuction(oldAuction, pages);

            //Did we find the substitute auction?
            if (auction.auction.size() == 0)
            {
                //No it was not there. Try removing initial passes.
                int first;
                for (first = 0; first < oldAuction.auction.size(); first++)
                    if (oldAuction.auction[first] != BID_PASS)
                        break;
                for (int i = first; i < oldAuction.auction.size(); i++)
                    auction.auction.append(oldAuction.auction[i]);
                auction = findSubstituteAuction(auction, pages);

                //It must be there.
                assert(auction.auction.size() > 0);
            }
        }
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
                                    (pRules[i]->RuleIsOk(ownFeatures)))
                            {
                                defBids.append(bids[i]);
                                pDefRules.append(pRules[i]);
                                qDebug() << QString("Bid, page: %1 Bid: %2").arg(page).arg(BID_NAMES[bids[i]]);
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

        //Remove initial pass bids (if any) in the auction and get also any seat opening.
        if ((auction.auction.size() > 0) &&
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
        //Did we find anything?
        else if (pDefRules.size() == 0)
        {
            //We did not find anything. Should we try a substitute auction?
            if ((subAuction.size() != 0) && !(auction.auction == subAuction[0].auction))
            {
                auction = subAuction[0];
                cont = true;
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
        return calculateNextBid(seat, bidHistory, ownFeatures, scoringMethod, teamVul);
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
        //Maybe a substitute auction?
        if (bidHistory.bidList[i].substitute)
        {
            CAuction oldAuction = auction;
            auction = findSubstituteAuction(oldAuction, pages);

            //Did we find it?
            if (auction.auction.size() == 0)
            {
                //No it was not there. Try removing initial passes.
                int first;
                for (first = 0; first < oldAuction.auction.size(); first++)
                    if (oldAuction.auction[first] != BID_PASS)
                        break;
                for (int i = first; i < oldAuction.auction.size(); i++)
                    auction.auction.append(oldAuction.auction[i]);
                auction = findSubstituteAuction(auction, pages);

                //It must be there.
                assert(auction.auction.size() > 0);
            }
        }
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
                            {
                                pDefRules.append(pRules[i]);
                                qDebug() << QString("Rule, page: %1 Bid: %2").arg(page).arg(BID_NAMES[bids[i]]);
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

        //Remove initial pass bids (if any) in the auction and get also any seat opening.
        if ((auction.auction.size() > 0) && (auction.auction[0] == BID_PASS))
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
        //Did we find anything?
        else if (pDefRules.size() == 0)
        {
            //We did not find anything. Should we try a substitute auction?
            if ((subAuction.size() != 0) && !(auction.auction == subAuction[0].auction))
            {
                auction = subAuction[0];
                *substitute = true;
                cont = true;
            }
        }
    }

    //Built in algorithmic calculation of relevant rules.
    if (pDefRules.size() == 0)
        calculatepRules(seat, bidHistory, bid, scoringMethod, teamVul, pDefRules);

    return pDefRules;
}

/**
 * @brief Get text description of alert.
 *
 * @param[in] alertId The id of the alert as used in th bidding database..
 * @return returns a text description of the alert.
 */
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
 * NOTE the rule used for the calculated bid is also calculated and returned with the bid.\n
 * NOTEThis calculated rule is only used for debugging and does not take part in anything else.\n\n
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
    qDebug() << QString(SEAT_NAMES[seat]) + "cB: " + txt;
    //***********************for debugging****************************

    int size = bidHistory.bidList.size();

    CFeatures& lowPartnerFeatures = bidHistory.getLowFeatures((Seat)((seat +2) % 4));
    CFeatures& highPartnerFeatures = bidHistory.getHighFeatures((Seat)((seat +2) % 4));
    CFeatures& lowOwnFeatures = bidHistory.getLowFeatures(seat);
    CFeatures& highOwnFeatures = bidHistory.getHighFeatures(seat);
    CFeatures& lowRHFeatures = bidHistory.getLowFeatures((Seat)((seat +3) % 4));
    CFeatures& highRHFeatures = bidHistory.getHighFeatures((Seat)((seat +3) % 4));
    CFeatures& lowLHFeatures = bidHistory.getLowFeatures((Seat)((seat +1) % 4));
    CFeatures& highLHFeatures = bidHistory.getHighFeatures((Seat)((seat +1) % 4));

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
    else if (isNTBidded(bidHistory))
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

        Bids doubleBid = getDoubleBid(bidHistory);

        //Calculate takeout bid and point interval.
        nextBid = getTakeoutDouble(lowPartnerFeatures, ownFeatures, highBid, doubleBid, &low, &high);

        pRule->getFeatures(&lowFeatures, &highFeatures);

        //If bid is a NT bid?
        Suit suit = BID_SUIT(nextBid);
        if (BID_SUIT(nextBid) == NOTRUMP)
        {
            lowFeatures.setPoints(NOTRUMP, low);
            highFeatures.setPoints(NOTRUMP, high);
            highFeatures.setDp(NOTRUMP, 1);
            for (int i = 0; i < 4; i++)
                if (lowPartnerFeatures.getSuitLen((Suit)i) >= 4)
                    lowFeatures.setStopNT((Suit)i, 2);
        }
        //Otherwise a suit bid.
        else if (nextBid != BID_PASS)
        {
            lowFeatures.setPoints((Suit)suit, low);
            highFeatures.setPoints((Suit)suit, high);
            lowFeatures.setSuitLen((Suit)suit, 4);
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
        int lowTotPoints = lowPartnerFeatures.getExtPoints(agree, true) + ownFeatures.getPoints(agree);
        int highTotPoints = highPartnerFeatures.getExtPoints(agree, false) + ownFeatures.getPoints(agree);
        bool isMinPoints = isMin(lowOwnFeatures.getExtPoints(agree, true), highOwnFeatures.getExtPoints(agree, false),
                               ownFeatures.getPoints(agree));
        bool isMaxPoints = isMax(lowOwnFeatures.getExtPoints(agree, true), highOwnFeatures.getExtPoints(agree, false),
                               ownFeatures.getPoints(agree));
        Bids game = (agree == SPADES) ? (BID_4S) : (agree == HEARTS) ? (BID_4H) :
                    (agree == DIAMONDS) ? (BID_5D) : (agree == CLUBS) ? (BID_5C) : (BID_3NT);
        Bids pBid = getLastNotAlertedBid(bidHistory);

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
                lowFeatures.setCountCard(ANY, ACE, countAces);      //Not always true.
                highFeatures.setCountCard(ANY, ACE, countAces);     //Not always true.

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
                lowFeatures.setCountCard(ANY, KING, countKings);        //Not always true.
                highFeatures.setCountCard(ANY, KING, countKings);       //Not always true.

                //Blackwood king asking.
                if ((countKings == 0) || (countKings == 4))
                    pRule->setAlertId(ZERO_OR_FOUR_KINGS);
                else if (countKings == 1)
                    pRule->setAlertId(ONE_KING);
                else if (countKings == 2)
                    pRule->setAlertId(TWO_KINGS);
                else if (countKings == 3)
                    pRule->setAlertId(THREE_KINGS);

                bid.bid = ((countKings == 0) || (countKings == 4)) ? (BID_6C) :
                          (countKings == 1) ? (BID_6D) : (countKings == 2) ? (BID_6H) : (BID_6S);
            }
            //else if (Gerber question about aces)
            else if (bidHistory.bidList[size - 2].bid == BID_4C)
            {
                lowFeatures.setCountCard(ANY, ACE, countAces);      //Not always true.
                highFeatures.setCountCard(ANY, ACE, countAces);     //Not always true.

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
                lowFeatures.setCountCard(ANY, KING, countKings);        //Not always true.
                highFeatures.setCountCard(ANY, KING, countAces);        //Not always true.

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

        //if (slam is possible - small or grand and game has not been bidded)
        if ((pBid < game) && (lowTotPoints >= BID_SUIT_POINT[BID_SMALL_SLAM_INX]))     //33
        {
            //if (Ask Blackwood or Gerber question)
            int noPartnerAces = CalculateNoCards(lowPartnerFeatures, ACE);
            int noOwnAces = CalculateNoCards(ownFeatures, ACE);
            if ((noPartnerAces == 0) && (noOwnAces == 0) && (acesAsked(bidHistory) == ZERO_OR_FOUR_ACES))
                noPartnerAces = 4;
            int noPartnerKings = CalculateNoCards(lowPartnerFeatures, KING);
            int noOwnKings = CalculateNoCards(ownFeatures, KING);
            if ((noPartnerKings == 0) && (noOwnKings == 0) && (kingsAsked(bidHistory) == ZERO_OR_FOUR_KINGS))
                noPartnerKings = 4;
            int noAces = noPartnerAces + noOwnAces;
            int noKings = noPartnerKings + noOwnKings;
            Bids nextBid = blackwoodOrGerberAsk(bidHistory, noAces, noKings, lowTotPoints, agree, suitAgree);
            if (nextBid != BID_NONE)
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                if (nextBid == BID_4C)
                {                    
                    //Gerber ace asking.
                    pRule->setAlertId(GERBER_ACE);

                    int points = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                    if (points < 0) points = 0;
                    lowFeatures.setPoints(NOTRUMP, points);
                    highFeatures.updPoints(NOTRUMP, false);
                }
                else if (nextBid == BID_4NT)
                {                    
                    //Blackwood ace asking.
                    pRule->setAlertId(BLACKWOOD_ACE);

                    int points = BID_SUIT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(agree, true);
                    if (points < 0) points = 0;
                    if ((lowPartnerFeatures.getSuitLen(agree) + lowOwnFeatures.getSuitLen(agree)) < 8)
                        lowFeatures.setSuitLen(agree, 8 - lowPartnerFeatures.getSuitLen(agree));
                    lowFeatures.setPoints(agree, points);
                    highFeatures.updPoints(agree, false);
                }
                else if (nextBid == BID_5C)
                {                    
                    //Gerber king asking.
                    pRule->setAlertId(GERBER_KING);

                    int points = BID_NT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                    if (points < 0) points = 0;
                    lowFeatures.setPoints(NOTRUMP, points);
                    highFeatures.updPoints(NOTRUMP, false);
                }
                else if (nextBid == BID_5NT)
                {                   
                    //Blackwood king asking.
                    pRule->setAlertId(BLACKWOOD_KING);

                    int points = BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getExtPoints(agree, true);
                    if (points < 0) points = 0;
                    if ((lowPartnerFeatures.getSuitLen(agree) + lowOwnFeatures.getSuitLen(agree)) < 8)
                        lowFeatures.setSuitLen(agree, 8 - lowPartnerFeatures.getSuitLen(agree));
                    lowFeatures.setPoints(agree, points);
                    highFeatures.updPoints(agree, false);
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
                    lowFeatures.setCountCard(ANY, ACE, 4 - noPartnerAces);
                    lowFeatures.setCountCard(ANY, KING, ((3 - noPartnerKings) > 0) ?  (3 - noPartnerKings) : (0));
                    lowFeatures.updPoints(agree, true);
                    highFeatures.setPoints(agree,
                            40 - lowPartnerFeatures.getExtPoints(agree, true));     //40
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
                    lowFeatures.setCountCard(ANY, ACE, ((3 - noPartnerAces) > 0) ? (3 - noPartnerAces) : (0));
                    lowFeatures.updPoints(agree, true);
                    highFeatures.setPoints(agree,
                           BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getExtPoints(agree, true));  //37
                    pRule->setFeatures(lowFeatures, highFeatures);
//                    pRule->setStatus(MUST_PASS);

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
            nextBid = MAKE_BID(agree, maxLevel);
            int level = BID_LEVEL(highOPBid);
            if (highOPBid >= nextBid)
            {
                nextBid = BID_PASS;
                if (BID_SUIT(highOPBid) != agree)
                {
                    if (BID_SUIT(highOPBid) > agree)
                        level++;
                    nextBid = MAKE_BID(agree, level);
                }
            }

            Bids cmpBid = MAKE_BID(agree, level);
            if (highOppBid < cmpBid)
                bid.bid = nextBid;
            else if ((cmpBid >= BID_3NT) && canDouble(bidHistory))
                bid.bid = BID_DOUBLE;
            else
                bid.bid = BID_PASS;

            return bid;
        }

        //if (game is possible in minor - check if we should play for 3NT instead)
        if ((suitAgree == DIAMONDS) || (suitAgree == CLUBS))
        {
            //if (game is possible - in nt) //Should really be tested on NOTRUMP points, but this is
            //of course not possible. We use minor points instead.
            if (lowTotPoints >= BID_NT_POINT[BID_NT_GAME_INX])
            {
                //Possibility for 3NT?
                if (BID_LEVEL(highBid) <= 3)
                {
                    if (nextBidCanBeNT(ownFeatures, lowPartnerFeatures, highPartnerFeatures,
                                       lowRHFeatures, highRHFeatures, lowLHFeatures, highLHFeatures))
                    {
                        CFeatures lowFeatures;
                        CFeatures highFeatures;
                        pRule->getFeatures(&lowFeatures, &highFeatures);
                        int lowPoints = BID_NT_POINT[BID_NT_GAME_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                        if (lowPoints < 0)
                            lowPoints = 0;
                        int highPoints = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                        if (highPoints < 0)
                            highPoints = 0;
                        lowFeatures.setPoints(NOTRUMP, lowPoints);
                        highFeatures.setPoints(NOTRUMP, highPoints);
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
                        if (ownFeatures.getStopNT((Suit)i) >= 2)
                        {
                            CFeatures lowFeatures;
                            CFeatures highFeatures;
                            pRule->getFeatures(&lowFeatures, &highFeatures);
                            int lowPoints = BID_NT_POINT[BID_NT_GAME_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                            if (lowPoints < 0)
                                lowPoints = 0;
                            int highPoints = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                            if (highPoints < 0)
                                highPoints = 0;
                            lowFeatures.setPoints(NOTRUMP, lowPoints);
                            highFeatures.setPoints(NOTRUMP, highPoints);
                            lowFeatures.setStopNT((Suit)i, 2);
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
            getLevel(agree, lowPartnerFeatures.getExtPoints(agree, true), ownFeatures.getPoints(agree), &nextBid,
                     &low, &high);

            //Assure agreed suit is bidded as the last bidded suit (escape for slam control bids).
            if ((BID_SUIT(highOPBid) != agree) && (highOPBid > nextBid) && (nextBid >= BID_3S))
            {
                Suit highSuit = BID_SUIT(highOPBid);
                int highLevel = BID_LEVEL(highOPBid);
                int level = (highSuit > agree) ? (highLevel + 1) : (highLevel);
                nextBid = MAKE_BID(agree, level);
            }

            //Assure game is bidded if status is game forcing.
            if ((bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING) &&
                    (nextBid < game))
                nextBid = game;

            //Choose 3NT over 4C, 5C, 4D, 5D if NT is possible.
            if (((nextBid == BID_4C) || (nextBid == BID_5C) ||
                (nextBid == BID_4D) || (nextBid == BID_5D)) &&
                 (highBid < BID_3NT) &&
                    nextBidCanBeNT(ownFeatures, lowPartnerFeatures, highPartnerFeatures,
                                   lowRHFeatures, highRHFeatures, lowLHFeatures, highLHFeatures))
                nextBid = game = BID_3NT;

            //No reason to get higher than necessary.
            //If small slam is bidded, then this is based on points only (falls sometimes through to here).
            if ((nextBid > game) && (nextBid < BID_6C) && (game > highBid))
                    nextBid = game;

            if ((nextBid <= highOPBid) || ((highOPBid >= game) &&
                 !IS_BID(bidHistory.bidList[size - 1].bid)))
                nextBid = BID_PASS;

            //Find the proper bid to bid.
            if (nextBid > highBid)
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);

                if (nextBid == game)
                {
                    int lowP = lowPartnerFeatures.getExtPoints(BID_SUIT(game), true);
                    int min = ((nextBid == BID_4H) || (nextBid == BID_4S)) ? (BID_SUIT_POINT[BID_SUIT_MAJOR_GAME_INX]) :
                              ((nextBid == BID_5D) || (nextBid == BID_5C)) ? (BID_SUIT_POINT[BID_SUIT_MINOR_GAME_INX]) :
                               (BID_NT_POINT[BID_NT_GAME_INX]);
                    int max = ((nextBid == BID_4H) || (nextBid == BID_4S)) ? (BID_SUIT_POINT[BID_SMALL_SLAM_INX]) :
                              ((nextBid == BID_5D) || (nextBid == BID_5C)) ? (BID_SUIT_POINT[BID_SMALL_SLAM_INX]) :
                               BID_NT_POINT[BID_SMALL_SLAM_INX];
                    int lowPoints = min - lowP;
                    if (lowPoints < 0)
                        lowPoints = 0;
                    int highPoints = max - lowP;
                    if (highPoints < 0)
                        highPoints = 0;
                    lowFeatures.setPoints(newSuitAgree, lowPoints);
                    highFeatures.setPoints(newSuitAgree, highPoints);
                }
                else if ((acesAsked(bidHistory) < 0) || (kingsAsked(bidHistory) < 0))
                {
                    lowFeatures.setPoints(newSuitAgree, low);
                    highFeatures.setPoints(newSuitAgree, high);
                }

                if ((agree != NOTRUMP) && (lowPartnerFeatures.getSuitLen(agree) + lowOwnFeatures.getSuitLen(agree)) < 8)
                    lowFeatures.setSuitLen(agree, 8 - lowPartnerFeatures.getSuitLen(agree));

                pRule->setFeatures(lowFeatures, highFeatures);

                int inx = ((agree == SPADES) || (agree == HEARTS) || (agree == NOTRUMP)) ?
                            (BID_SUIT_MAJOR_GAME_INX) :(BID_SUIT_MINOR_GAME_INX);                   //26, 29

                if ((((high + highPartnerFeatures.getExtPoints(agree, false)) < BID_SUIT_POINT[inx]) && (suitAgree == agree)) ||              //26
                        (nextBid >= BID_6C) ||
                        ((nextBid > game) && ((acesAsked(bidHistory) > 0) || (kingsAsked(bidHistory) > 0))))
                    pRule->setStatus(MUST_PASS);                    //Game is not possible or slam is bidded.

                bid.bid = nextBid;
            }
            else if ((nextBid >= BID_3NT) && canDouble(bidHistory) && (highOppBid >= nextBid))
                bid.bid = BID_DOUBLE;

            //Assure forcing is taken care of.
            else if (((bidHistory.bidList[size - 2].rules[0]->getStatus() == FORCING) ||
                      (bidHistory.bidList[size - 2].rules[0]->getAlertId() > 0)) &&
                     !IS_BID(bidHistory.bidList[size - 1].bid))
                {
                    Suit highSuit = BID_SUIT(highBid);
                    int level = BID_LEVEL(highBid);
                    if (highSuit >= agree)
                        level++;
                    nextBid = MAKE_BID(agree, level);

                    if ((highBid < BID_3NT) && ((agree == CLUBS) || (agree == DIAMONDS)) &&
                        (level == 4) &&
                        nextBidCanBeNT(ownFeatures, lowPartnerFeatures, highPartnerFeatures,
                                       lowRHFeatures, highRHFeatures, lowLHFeatures, highLHFeatures))
                        nextBid = BID_3NT;

                    bid.bid = nextBid;
                }
            else
                bid.bid = BID_PASS;

            return bid;
        }
    }

    //Not agreed on suit?
    else
    {
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
                int suit = 0;
                for (int i = 0; i < 4; i++)
                    if (ownSuit[i] && (ownFeatures.getSuitLen((Suit)i) >= REBID_SL) &&              //6
                            (ownFeatures.getPoints((Suit)i) > ownFeatures.getPoints((Suit)suit)))
                        suit = i;

                Bids nextBid;
                int low, high;

                //Get point interval and bid to bid.
                int lowP;
                /*                if (nextBidder(bidHistory) == OPEN_ALERT)
                    lowP = lowPartnerFeatures.getExtPoints(NOTRUMP, true);      //There was an alert.
                else
                    lowP = isNextBidOpen(bidHistory) ? (REBID_P) : (REBID_O);   //6, 12
                if (lowPartnerFeatures.getExtPoints(NOTRUMP, true) > lowP)
                    lowP = lowPartnerFeatures.getExtPoints(NOTRUMP, true);
*/
                lowP = lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                getLevel((Suit)suit, lowP, ownFeatures.getPoints((Suit)suit), &nextBid,
                         &low, &high);

                //Try with NT instead?
                int level = BID_LEVEL(nextBid);
                if ((level < 4) || (((suit != CLUBS)  && (suit != DIAMONDS)) ||
                                    (newSuitAgree != NOTRUMP)))
                {
                    //Assure game is not bidded.
                    if (((level >= 5) && ((suit == CLUBS) || (suit == DIAMONDS))) ||
                            ((level >= 4) && ((suit == HEARTS) || (suit == SPADES))))
                    {
                        level = ((suit == CLUBS) || (suit == DIAMONDS)) ? (4) : (3);
                        nextBid = MAKE_BID(suit, level);
                        findLevel((Suit)suit, lowP, level, &low, &high);
                    }

                    //Bid game if game forcing.
                    Bids game = (suit == SPADES) ? (BID_4S) : (suit == HEARTS) ? (BID_4H) :
                                (suit == DIAMONDS) ? (BID_5D) : (suit == CLUBS) ? (BID_5C) : (BID_3NT);
                    if ((size >= 2) &&
                            (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING) &&
                            (nextBid < game))
                        nextBid = game;

                    //Choose 3NT over 4C, 5C, 4D, 5D if NT is possible.
                    if (((nextBid == BID_4C) || (nextBid == BID_5C) ||
                         (nextBid == BID_4D) || (nextBid == BID_5D)) &&
                            (highBid < BID_3NT) &&
                            nextBidCanBeNT(ownFeatures, lowPartnerFeatures, highPartnerFeatures,
                                           lowRHFeatures, highRHFeatures, lowLHFeatures, highLHFeatures))
                        nextBid = game = BID_3NT;

                    if ((nextBid <= highOPBid) || ((highOPBid >= game) &&
                         !IS_BID(bidHistory.bidList[size - 1].bid)))
                        nextBid = BID_PASS;

                    //Is the found bid ok?
                    if ((nextBid) > highBid)
                    {
                        CFeatures lowFeatures;
                        CFeatures highFeatures;
                        pRule->getFeatures(&lowFeatures, &highFeatures);

                        lowFeatures.setSuitLen((Suit)suit, REBID_SL);              //6
                        lowFeatures.setPoints(NOTRUMP, low);
                        highFeatures.setPoints(NOTRUMP, high);

                        pRule->setFeatures(lowFeatures, highFeatures);

                        bid.bid = nextBid;
                    }
                    else if ((nextBid >= BID_3NT) && canDouble(bidHistory) && (highOppBid >= nextBid))
                        bid.bid = BID_DOUBLE;

                    //Assure forcing is taken care of.
                    else if ((size >= 2) &&
                             ((bidHistory.bidList[size - 2].rules[0]->getStatus() == FORCING) ||
                              (bidHistory.bidList[size - 2].rules[0]->getAlertId() > 0)) &&
                             !IS_BID(bidHistory.bidList[size - 1].bid))
                    {
                        Suit highSuit = BID_SUIT(highBid);
                        int level = BID_LEVEL(highBid);
                        if (highSuit >= suit)
                            level++;
                        nextBid = MAKE_BID(suit, level);

                        if ((highBid < BID_3NT) && ((suit == CLUBS) || (suit == DIAMONDS)) &&
                                (level == 4) && (newSuitAgree == NOTRUMP))
                            nextBid = BID_3NT;

                        bid.bid = nextBid;
                    }
                    else
                        bid.bid = BID_PASS;

                    return bid;
                }
            }
        }

        //Find new suit (if possible).
        {
            int newSuit;
            for (newSuit = 0; newSuit < 4; newSuit++)
                if ((lowPartnerFeatures.getSuitLen((Suit)newSuit) == 0) && (lowOwnFeatures.getSuitLen((Suit)newSuit) == 0) &&
                        (!oppSuit[newSuit]) && (ownFeatures.getSuitLen((Suit)newSuit) >= 4))
                    break;
            //Found new suit?
            if ((newSuit < 4) && ((BID_SUIT(highBid) < newSuit) || (newSuitAgree != NOTRUMP)))
            {
                int low = -1;
                int  high = -1;
                int newLevel = (newSuit > BID_SUIT(highBid)) ? (BID_LEVEL(highBid)) : (BID_LEVEL(highBid) + 1);

                int points = ownFeatures.getPoints(NOTRUMP);
                Bids newBid = MAKE_BID(newSuit, newLevel);

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

                //None of the above.
                else if (bidder == OPEN_OTHER)
                    newBid = BID_NONE;

                //An alert was bidded in the bid sequence.
                else
                {
                    int level = (newSuit > BID_SUIT(highOPBid)) ? (BID_LEVEL(highOPBid)) : (BID_LEVEL(highOPBid) + 1);
                    newBid = MAKE_BID(newSuit, level);
                }

                //Try with NT instead?
                int level = BID_LEVEL(newBid);
                if ((level >= 0) &&
                        ((level < 4) || (((newSuit != CLUBS)  && (newSuit != DIAMONDS)) ||
                                         (newSuitAgree != NOTRUMP))))
                {
                    //Assure game is not bidded.
                    if (((level >= 5) && ((newSuit == CLUBS) || (newSuit == DIAMONDS))) ||
                            ((level >= 4) && ((newSuit == HEARTS) || (newSuit == SPADES))))
                    {
                        level = ((newSuit == CLUBS) || (newSuit == DIAMONDS)) ? (4) : (3);
                        newBid = MAKE_BID(newSuit, level);
                    }

                    //Assure game is bidded if status is game forcing.
                    Bids game = (newSuit == SPADES) ? (BID_4S) : (newSuit == HEARTS) ? (BID_4H) :
                                (newSuit == DIAMONDS) ? (BID_5D) : (newSuit == CLUBS) ? (BID_5C) : (BID_3NT);
                    if ((size >= 2) &&
                            (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING) &&
                            (newBid < game))
                        newBid = game;

                    //Choose 3NT over 4C, 5C, 4D, 5D if NT is possible.
                    if (((newSuit == BID_4C) || (newSuit == BID_5C) ||
                         (newSuit == BID_4D) || (newSuit == BID_5D)) &&
                            (highBid < BID_3NT) && (newSuitAgree == NOTRUMP))
                        newBid = game = BID_3NT;
                }

                //Found a bid (fall through otherwise)?
                if ((newBid != BID_NONE) && (newBid > highBid))
                {
                    CFeatures lowFeatures;
                    CFeatures highFeatures;
                    pRule->getFeatures(&lowFeatures, &highFeatures);
                    if (low != -1)
                    {
                        lowFeatures.setPoints(NOTRUMP, low);
                        highFeatures.updPoints(NOTRUMP, false);
                    }
                    if (high != -1)
                    {
                        lowFeatures.updPoints(NOTRUMP, true);
                        highFeatures.setPoints(NOTRUMP, high);
                    }
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
            getLevel(NOTRUMP, lowPartnerFeatures.getExtPoints(newSuitAgree, true), ownFeatures.getPoints(newSuitAgree), &nextBid,
                     &low, &high);

            //Do not go higher than 3NT.
            if ((nextBid > BID_3NT) && (highBid < BID_3NT) && (nextBid < BID_6C))
                nextBid = BID_3NT;

            //Assure game is bidded if status is game forcing.
            Bids game = BID_3NT;
            if ((size >= 2) &&
                    (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING) &&
                    (nextBid < game))
                nextBid = game;

            //Find useable bid.
            if (nextBid > highBid)
            {
                CFeatures lowFeatures;
                CFeatures highFeatures;
                pRule->getFeatures(&lowFeatures, &highFeatures);
                if (nextBid == BID_3NT)
                {
                    int lowPoints = BID_NT_POINT[BID_NT_GAME_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                    if (lowPoints < 0)
                        lowPoints = 0;
                    int highPoints = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
                    if (highPoints < 0)
                        highPoints = 0;
                    lowFeatures.setPoints(NOTRUMP, lowPoints);
                    highFeatures.setPoints(NOTRUMP, highPoints);
                }
                else
                {
                    lowFeatures.setPoints(newSuitAgree, low);
                    highFeatures.setPoints(newSuitAgree, high);
                }
                pRule->setFeatures(lowFeatures, highFeatures);
                if (((highPartnerFeatures.getExtPoints(newSuitAgree, false) + high) < BID_NT_POINT[BID_NT_GAME_INX]) || //26
                        ((nextBid == BID_3NT) || (nextBid >= BID_6C)))
                    pRule->setStatus(MUST_PASS);                //Game is not possible or game is bidden.

                bid.bid = nextBid;
            }
            else if ((nextBid >= BID_3NT) && canDouble(bidHistory) && (highOppBid >= nextBid))
                bid.bid = BID_DOUBLE;
            else if ((size >= 2) &&
                     ((bidHistory.bidList[size - 2].rules[0]->getStatus() == FORCING) ||
                      (bidHistory.bidList[size - 2].rules[0]->getAlertId() > 0)) &&
                     !IS_BID(bidHistory.bidList[size - 1].bid))
            {
                Suit highSuit = BID_SUIT(highBid);
                int level = BID_LEVEL(highBid);
                nextBid = MAKE_BID(NOTRUMP, level);

                bid.bid = nextBid;
            }
            else
                bid.bid = BID_PASS;

            return bid;
        }

        {
            //Assure forcing and game forcing is taken care of (just bid the longest suit).
            if ((size >= 2) &&
                    ((bidHistory.bidList[size - 2].rules[0]->getStatus() == FORCING) ||
                     (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING) ||
                     (bidHistory.bidList[size - 2].rules[0]->getAlertId() > 0)) &&
                    !IS_BID(bidHistory.bidList[size - 1].bid))
            {
                int i, length, suit;
                length = 0;
                for (i = 0; i < 4; i++)
                    if ((ownFeatures.getSuitLen((Suit)i) + (lowPartnerFeatures.getSuitLen((Suit)i))) >= length)
                    {
                        length = ownFeatures.getSuitLen((Suit)i) + lowPartnerFeatures.getSuitLen((Suit)i);
                        suit = i;
                    }

                int level = (suit > BID_SUIT(highBid)) ? (BID_LEVEL(highBid)) : (BID_LEVEL(highBid) + 1);
                Bids nextBid = MAKE_BID(suit, level);

                //Bid game if game forcing.
                Bids game = (suit == SPADES) ? (BID_4S) : (suit == HEARTS) ? (BID_4H) :
                            (suit == DIAMONDS) ? (BID_5D) : (suit == CLUBS) ? (BID_5C) : (BID_3NT);

                if ((size >= 2) &&
                        (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING) &&
                        (nextBid < game))
                    nextBid = game;

                bid.bid = nextBid;
            }
            else
                bid.bid = BID_PASS;
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
 * @param[in] bid The bid calculated by getNext bid.
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

    qDebug() << QString(SEAT_NAMES[seat]) + "pB: " + BID_NAMES[bid];

    int size = bidHistory.bidList.size();

    CFeatures& lowPartnerFeatures = bidHistory.getLowFeatures((Seat)((seat +2) % 4));
    CFeatures& highPartnerFeatures = bidHistory.getHighFeatures((Seat)((seat +2) % 4));
    CFeatures& lowOwnFeatures = bidHistory.getLowFeatures(seat);
    CFeatures& highOwnFeatures = bidHistory.getHighFeatures(seat);

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
    else if (isNTBidded(bidHistory))
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
    else if (((bid == BID_4NT) || (bid == BID_5NT)) && (size >= 2) &&
           (BID_SUIT(bidHistory.bidList[size -2].bid) >= CLUBS) && (BID_SUIT(bidHistory.bidList[size -2].bid) <= SPADES))
        newSuitAgree = BID_SUIT(bidHistory.bidList[size -2].bid);
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
        Suit suit = findTakeoutDouble(lowPartnerFeatures.getExtPoints(NOTRUMP, true), bid, highBid, &low, &high);
        if (suit == NOTRUMP)
        {
            pRule->getFeatures(&lowFeatures, &highFeatures);
            lowFeatures.setPoints(NOTRUMP, low);
            highFeatures.setPoints(NOTRUMP, high);
            highFeatures.setDp(NOTRUMP, 1);
            for (int i = 0; i < 4; i++)
                if (lowPartnerFeatures.getSuitLen((Suit)i) >= 4)
                    lowFeatures.setStopNT((Suit)i, 2);
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
                countAces = -1;
            if ((countAces >= 0) && (countAces <= 4))
            {
                lowFeatures.setCountCard(ANY, ACE, countAces);
                highFeatures.setCountCard(ANY, ACE, countAces);
            }
        }
        //else if (Blackwood question about kings)
        else if (bidHistory.bidList[size - 2].bid == BID_5NT)
        {
            int countKings = bid - BID_6C;

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
                countKings = -1;
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
                countAces = -1;
            if ((countAces >= 0) && (countAces <= 4))
            {
                lowFeatures.setCountCard(ANY, ACE, countAces);
                highFeatures.setCountCard(ANY, ACE, countAces);
            }
        }
        //else if (Gerber question about kings)
        else if (bidHistory.bidList[size - 2].bid == BID_5C)
        {
            int countKings = bid - BID_5D;

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
                countKings = -1;
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

            int points = BID_SUIT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(newSuitAgree, true);
            if (points < 0) points = 0;
            if ((lowPartnerFeatures.getSuitLen(newSuitAgree) + lowOwnFeatures.getSuitLen(newSuitAgree)) < 8)
                lowFeatures.setSuitLen(newSuitAgree, 8 - lowPartnerFeatures.getSuitLen(newSuitAgree));
            lowFeatures.setPoints(newSuitAgree, points);
            highFeatures.updPoints(newSuitAgree, false);
        }
        else if (bid == BID_5NT)
        {
            //Blackwood king asking.
            pRule->setAlertId(BLACKWOOD_KING);

            int points = BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getExtPoints(newSuitAgree, true);
            if (points < 0) points = 0;
            if ((lowPartnerFeatures.getSuitLen(newSuitAgree) + lowOwnFeatures.getSuitLen(newSuitAgree)) < 8)
                lowFeatures.setSuitLen(newSuitAgree, 8 - lowPartnerFeatures.getSuitLen(newSuitAgree));
            lowFeatures.setPoints(newSuitAgree, points);
            highFeatures.updPoints(newSuitAgree, false);
        }

        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(FORCING);

        return;
    }

    //Gerber?
    Bids pBid = (size >= 2) ? bidHistory.bidList[size - 2].bid : BID_NONE;
    if (!isNextBidOpen(bidHistory) && (suitAgree == NOTRUMP) && ((bid == BID_4C) || (bid == BID_5C)) &&
            ((pBid == BID_1NT) || (pBid == BID_2NT)))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);

        if (bid == BID_4C)
        {
            //Gerber ace asking.
            pRule->setAlertId(GERBER_ACE);

            int points = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(suitAgree, true);
            if (points < 0) points = 0;
            lowFeatures.setPoints(suitAgree, points);
            highFeatures.updPoints(suitAgree, false);
        }
        else if (bid == BID_5C)
        {
            //Gerber king asking.
            pRule->setAlertId(GERBER_KING);

            int points = BID_NT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getExtPoints(suitAgree, true);
            if (points < 0) points = 0;
            lowFeatures.setPoints(suitAgree, points);
            highFeatures.updPoints(suitAgree, false);
        }

        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(FORCING);

        return;
    }

    //If the bid is Double then it is a penalty double (other doubles are handled
    //by the bidding tables and never comes here).
    if (bid == BID_DOUBLE)
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);

        lowFeatures.setPoints(NOTRUMP, DOUBLE_LIMIT);
        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(MUST_PASS);

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
            if (acesAsked(bidHistory) > 0)
            {
                int noPartnerAces = CalculateNoCards(highPartnerFeatures, ACE);
                lowFeatures.setCountCard(ANY, ACE, 4 - noPartnerAces);
            }
            if (kingsAsked(bidHistory) > 0)
            {
                int noPartnerKings = CalculateNoCards(highPartnerFeatures, KING);
                lowFeatures.setCountCard(ANY, KING, ((3 - noPartnerKings) > 0) ? (3 - noPartnerKings) : (0));
            }
            lowFeatures.updPoints(newSuitAgree, true);
            int points = 40 - lowPartnerFeatures.getPoints(newSuitAgree);
            if (points > 37)
                points = 37;
            highFeatures.setPoints(newSuitAgree, points);
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
            if (acesAsked(bidHistory) > 0)
            {
                int noPartnerAces = CalculateNoCards(highPartnerFeatures, ACE);
                lowFeatures.setCountCard(ANY, ACE, ((3 - noPartnerAces) > 0) ? (3 - noPartnerAces) : (0));
            }
            lowFeatures.updPoints(newSuitAgree, true);
            highFeatures.setPoints(newSuitAgree,
                                  BID_SUIT_POINT[BID_GRAND_SLAM_INX] - lowPartnerFeatures.getExtPoints(newSuitAgree, true));  //37
            pRule->setFeatures(lowFeatures, highFeatures);
//            pRule->setStatus(MUST_PASS);
        }
        return;
    }

    //Game in nt after minor agree.
    if (((suitAgree == DIAMONDS) || (suitAgree == CLUBS)) && (bid == BID_3NT))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);
        int lowPoints = BID_NT_POINT[BID_NT_GAME_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
        if (lowPoints < 0)
            lowPoints = 0;
        int highPoints = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
        if (highPoints < 0)
            highPoints = 0;
        lowFeatures.setPoints(NOTRUMP, lowPoints);
        highFeatures.setPoints(NOTRUMP, highPoints);
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
        int lowPoints = BID_NT_POINT[BID_NT_GAME_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
        if (lowPoints < 0)
            lowPoints = 0;
        int highPoints = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
        if (highPoints < 0)
            highPoints = 0;
        lowFeatures.setPoints(NOTRUMP, lowPoints);
        highFeatures.setPoints(NOTRUMP, highPoints);
        lowFeatures.setStopNT(BID_SUIT(bid), 2);
        pRule->setFeatures(lowFeatures, highFeatures);
        pRule->setStatus(FORCING);

        return;
    }

    //Limit bid in nt, major or minor.
    if (((suitAgree == NOTRUMP) && (BID_SUIT(bid) == NOTRUMP)) ||
            (((newSuitAgree == SPADES) || (newSuitAgree == HEARTS) || (newSuitAgree == DIAMONDS) || (newSuitAgree == CLUBS))
            && (BID_SUIT(bid) == newSuitAgree)))
    {
        int size = bidHistory.bidList.size();
        if ((size >= 2) && ((bidHistory.bidList[size - 2].rules[0]->getStatus() == FORCING) ||
                            (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING)))
            return;

        Suit agree = ((suitAgree == NOTRUMP) && (BID_SUIT(bid) == NOTRUMP)) ? (suitAgree) : (newSuitAgree);
        Bids game = (agree == SPADES) ? (BID_4S) : (agree == HEARTS) ? (BID_4H) :
                    (agree == DIAMONDS) ? (BID_5D) : (agree == CLUBS) ? (BID_5C) : (BID_3NT);

        int low, high;
        findLevel(agree, lowPartnerFeatures.getExtPoints(agree, true), BID_LEVEL(bid), &low, &high);

        CFeatures lowFeatures;
        CFeatures highFeatures;

        pRule->getFeatures(&lowFeatures, &highFeatures);

        if (bid == game)
        {
            int lowP = lowPartnerFeatures.getExtPoints(BID_SUIT(game), true);
            int min = ((bid == BID_4H) || (bid == BID_4S)) ? (BID_SUIT_POINT[BID_SUIT_MAJOR_GAME_INX]) :
                      ((bid == BID_5D) || (bid == BID_5C)) ? (BID_SUIT_POINT[BID_SUIT_MINOR_GAME_INX]) :
                       (BID_NT_POINT[BID_NT_GAME_INX]);
            int max = ((bid == BID_4H) || (bid == BID_4S)) ? (BID_SUIT_POINT[BID_SMALL_SLAM_INX]) :
                      ((bid == BID_5D) || (bid == BID_5C)) ? (BID_SUIT_POINT[BID_SMALL_SLAM_INX]) :
                       BID_NT_POINT[BID_SMALL_SLAM_INX];
            int lowPoints = min - lowP;
            if (lowPoints < 0)
                lowPoints = 0;
            int highPoints = max - lowP;
            if (highPoints < 0)
                highPoints = 0;
            lowFeatures.setPoints(agree, lowPoints);
            highFeatures.setPoints(agree, highPoints);
        }
        else if ((acesAsked(bidHistory) < 0) && (kingsAsked(bidHistory) < 0))
        {
            lowFeatures.setPoints(agree, low);
            highFeatures.setPoints(agree, high);
        }

        if ((agree != NOTRUMP) && (lowPartnerFeatures.getSuitLen(agree) + lowOwnFeatures.getSuitLen(agree)) < 8)
            lowFeatures.setSuitLen(agree, 8 - lowPartnerFeatures.getSuitLen(agree));
        pRule->setFeatures(lowFeatures, highFeatures);

        int inx = ((agree == SPADES) || (agree == HEARTS) || (agree == NOTRUMP)) ?
                    (BID_SUIT_MAJOR_GAME_INX) :(BID_SUIT_MINOR_GAME_INX);                   //26, 29

        if ((((high + highPartnerFeatures.getExtPoints(agree, false)) < BID_SUIT_POINT[inx]) && (suitAgree == agree)) ||              //26
                (bid >= BID_6C) ||
                ((bid > game) && ((acesAsked(bidHistory) > 0) || (kingsAsked(bidHistory) > 0))))
            pRule->setStatus(MUST_PASS);                    //Game is not possible or slam is bidded.

        return;
    }

    //Rebid own suit.
    if (isRebid(bidHistory, suitAgree, bid))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;
        pRule->getFeatures(&lowFeatures, &highFeatures);
        lowFeatures.setSuitLen(BID_SUIT(bid), REBID_SL);

        //Check for forcing or game forcing.
        int size = bidHistory.bidList.size();
        if ((size >= 2) && ((bidHistory.bidList[size - 2].rules[0]->getStatus() == FORCING) ||
                            (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING)))
        {
            pRule->setFeatures(lowFeatures, highFeatures);
            return;
        }

        int low, high;

        Suit suit = BID_SUIT(bid);
        int bidLevel = BID_LEVEL(bid);
        //Get point interval and bid to bid.
        int lowP;
        if (nextBidder(bidHistory) == OPEN_ALERT)
            lowP = lowPartnerFeatures.getExtPoints(NOTRUMP, true);      //There was an alert.
        else
            lowP = isNextBidOpen(bidHistory) ? (REBID_P) : (REBID_O);   //6, 12
        if (lowPartnerFeatures.getExtPoints(NOTRUMP, true) > lowP)
            lowP = lowPartnerFeatures.getExtPoints(NOTRUMP, true);
        findLevel(suit, lowP, bidLevel, &low, &high);

        Bids game = (suit == SPADES) ? (BID_4S) : (suit == HEARTS) ? (BID_4H) :
                    (suit == DIAMONDS) ? (BID_5D) : (BID_5C);

        if (bid == game)
        {
            int min = ((bid == BID_4H) || (bid == BID_4S)) ? (BID_SUIT_POINT[BID_SUIT_MAJOR_GAME_INX]) :
                      (BID_SUIT_POINT[BID_SUIT_MINOR_GAME_INX]);
            int max = ((bid == BID_4H) || (bid == BID_4S)) ? (BID_SUIT_POINT[BID_SMALL_SLAM_INX]) :
                      (BID_SUIT_POINT[BID_SMALL_SLAM_INX]);
            int lowPoints = min - lowP;
            if (lowPoints < 0)
                lowPoints = 0;
            int highPoints = max - lowP;
            if (highPoints < 0)
                highPoints = 0;
            lowFeatures.setPoints(NOTRUMP, lowPoints);
            highFeatures.setPoints(NOTRUMP, highPoints);
        }
        else
        {
            lowFeatures.setPoints(NOTRUMP, low);
            highFeatures.setPoints(NOTRUMP, high);
        }
        pRule->setFeatures(lowFeatures, highFeatures);

        if (bid >= game)
            pRule->setStatus(MUST_PASS);

        return;
    }

    //New suit or catch all NT.
    if ((isNewSuit(newSuitAgree, bid) && !isRebid(bidHistory, suitAgree, bid)) ||
            ((nextBidder(bidHistory) == OPEN_RESPONSE) && (bid == BID_1NT)))
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;
        pRule->getFeatures(&lowFeatures, &highFeatures);

        if (bid != BID_1NT)
            lowFeatures.setSuitLen(BID_SUIT(bid), 4);

        //Check for game forcing.
        int size = bidHistory.bidList.size();
        if ((size >= 2) && (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING))
        {
            pRule->setFeatures(lowFeatures, highFeatures);
            return;
        }

        bool forcing = ((size>= 2) &&(bidHistory.bidList[size - 2].rules[0]->getStatus() == FORCING));
        int low = -1;
        int high = -1;

        int bidder = nextBidder(bidHistory);

        //Response from openers partner.
        if (bidder == OPEN_RESPONSE)
        {
            forcing = false;
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
                forcing = false;
                low = NEWSUIT_O_J_L;                //19
                high = NEWSUIT_O_J_H;               //21
            }
            //Reverse suit.
            else if ((((bid - cmpBid) / 5) == 0) && (((bid - ownBid) / 5) == 1))
            {
                forcing = false;
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
            forcing = false;
            int level = BID_LEVEL(bid);
            if (level == 3)
            {
                low = NEWSUIT_P2_3_L;               //10
                high = NEWSUIT_P2_3_H;              //12
            }
            else if (level == 4)
                low = NEWSUIT_P2_4;                 //13
        }

        if (forcing)
            low = high = -1;

        if (low != -1)
        {
            lowFeatures.setPoints(NOTRUMP, low);
            highFeatures.updPoints(NOTRUMP, false);
        }
        if (high != -1)
        {
            lowFeatures.updPoints(NOTRUMP, true);
            highFeatures.setPoints(NOTRUMP, high);
        }
        pRule->setFeatures(lowFeatures, highFeatures);

        Suit suit = BID_SUIT(bid);
        Bids game = (suit == SPADES) ? (BID_4S) : (suit == HEARTS) ? (BID_4H) :
                    (suit == DIAMONDS) ? (BID_5D) : (BID_5C);
        if (bid >= game)
            pRule->setStatus(MUST_PASS);

        return;
    }

    //New NT.
    if ((newSuitAgree == NOTRUMP) && (suitAgree == ANY))
    {
        //Check for forcing or game forcing.
        int size = bidHistory.bidList.size();
        if ((size >= 2) && ((bidHistory.bidList[size - 2].rules[0]->getStatus() == FORCING) ||
                            (bidHistory.bidList[size - 2].rules[0]->getStatus() == GAME_FORCING)))
            return;

        int low, high;
        findLevel(NOTRUMP, lowPartnerFeatures.getExtPoints(newSuitAgree, true), BID_LEVEL(bid), &low, &high);

        CFeatures lowFeatures;
        CFeatures highFeatures;
        pRule->getFeatures(&lowFeatures, &highFeatures);

        if (bid == BID_3NT)
        {
            int lowPoints = BID_NT_POINT[BID_NT_GAME_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
            if (lowPoints < 0)
                lowPoints = 0;
            int highPoints = BID_NT_POINT[BID_SMALL_SLAM_INX] - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
            if (highPoints < 0)
                highPoints = 0;
            lowFeatures.setPoints(NOTRUMP, lowPoints);
            highFeatures.setPoints(NOTRUMP, highPoints);
        }
        else
        {
            lowFeatures.setPoints(newSuitAgree, low);
            highFeatures.setPoints(newSuitAgree, high);
        }

        pRule->setFeatures(lowFeatures, highFeatures);
        if (((highPartnerFeatures.getExtPoints(newSuitAgree, false) + high) < BID_NT_POINT[BID_NT_GAME_INX]) ||     //26
                (bid >= BID_3NT))
            pRule->setStatus(MUST_PASS);                //Game is not possible or game is bidden.
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

    //Find stoppers and which suits have been bidded.
    bool ownSuit[4];
    bool oppSuit[4];
    bool stpSuit[4];
    for (int i = 0; i < 4; i++)
        ownSuit[i] = oppSuit[i] = stpSuit[i] = false;

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

    //Stoppers.
    for (int i = 0; i < 4; i++)
    {
        if ((ownFeatures.getStopNT((Suit)i) >= 2) || (ownFeatures.getQlty((Suit)i) >= 2) ||
                (lowPartnerFeatures.getStopNT((Suit)i) >= 2) || (lowPartnerFeatures.getQlty((Suit)i) >= 2))
            stpSuit[i] = true;
    }

    //Check for distribution.
    bool foundDouble = false;
    int i;
    for (i = 0; i < 4; i++)
    if (!ownSuit[i] && !oppSuit[i] && !stpSuit[i])
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
        if (!ownSuit[i] && oppSuit[i] && !stpSuit[i])
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
            ((bidHistory.bidList[i].bid >= BID_3NT) && (lowPartnerFeatures.getExtPoints(NOTRUMP, true) >= DOUBLE_LIMIT)));
}

//Have we bidded double?
bool CBidEngine::isDouble(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    return (((size >= 3) && (bidHistory.bidList[size - 2].bid == BID_DOUBLE) && IS_BID(bidHistory.bidList[size - 3].bid)) ||
            ((size >= 5) && (bidHistory.bidList[size - 2].bid == BID_DOUBLE) &&
            (bidHistory.bidList[size - 3].bid == BID_PASS) && (bidHistory.bidList[size - 4].bid == BID_PASS) &&
            IS_BID(bidHistory.bidList[size - 5].bid)));
}

Bids CBidEngine::getDoubleBid(CBidHistory &bidHistory)
{
    assert (isDouble(bidHistory));

    int size = bidHistory.bidList.size();

    return (bidHistory.bidList[size - 3].bid == BID_PASS) ? (bidHistory.bidList[size - 5].bid) :
        (bidHistory.bidList[size - 3].bid);
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
                               Bids highBid, Bids doubleBid, int *low, int *high)
{
    //Bid NT?
    if (ownFeatures.getDp(NOTRUMP) <= 1)
    {
        //Hold in opponent suits?
        int i;
        for (i = 0; i < 4; i++)
            if ((lowPartnerFeatures.getSuitLen((Suit)i) >= 4) && (ownFeatures.getStopNT((Suit)i) < 2))
                break;

        //Is NT ok?
        if (i == 4)
        {
            Bids bid;
            getLevel(NOTRUMP, lowPartnerFeatures.getExtPoints(NOTRUMP, true), ownFeatures.getPoints(NOTRUMP), &bid, low, high);
            if ((bid <= BID_3NT) && (bid != BID_PASS) && (bid > highBid))
                return bid;
        }
    }

    //Did not work with NT. Find the longest suit (not the doubled suit).
    int suit = 0;
    int doubleSuit = (int)BID_SUIT(doubleBid);
    int len = 0;
    for (int i = 0; i < 4; i++)
        if ((i != doubleSuit) && (ownFeatures.getSuitLen((Suit)i) > len))
        {
            suit = i;
            len = ownFeatures.getSuitLen((Suit)suit);
        }

    int points = ownFeatures.getPoints((Suit)suit);
    int firstLevel = (suit > BID_SUIT(doubleBid)) ? (BID_LEVEL(doubleBid)) : (BID_LEVEL(doubleBid) + 1);
    int off = 12 - lowPartnerFeatures.getExtPoints(NOTRUMP, true);
    *low = 0 + off;
    *high = 8 + off;
    int level = 0;
    if ((points > (8 + off)))
    {
        *low = 9 + off;
        *high = 11 + off;
        level++;
    }
    if (points > (11  + off))
    {
        *low = 12 + off;
        *high = ownFeatures.getMaxPoints();
        level++;
    }

    if (*low < 0)
        *low = 0;
    if (*high > ownFeatures.getMaxPoints())
        *high = ownFeatures.getMaxPoints();

    if ((firstLevel + level) > 4)
    {
        level = 4;
        if (level < firstLevel)
            level = firstLevel;
    }
    else
        level = firstLevel + level;

    Bids bid = MAKE_BID(suit, level);

    return (bid > highBid) ? (bid) : (BID_PASS);
}

//Find takeout double point intervals.
Suit CBidEngine::findTakeoutDouble(int lowPartner, Bids bid, Bids highBid, int *low, int *high)
{
    Suit suit = BID_SUIT(bid);
    int level = BID_LEVEL(bid);
    Suit highSuit = BID_SUIT(highBid);
    int highLevel = BID_LEVEL(highBid);

    if (suit == NOTRUMP)
        findLevel(suit, lowPartner, level, low, high);

    else if (suit != ANY)
    {
        int firstLevel = (suit > highSuit) ? (highLevel) : (highLevel + 1);

        if ((level - firstLevel) == 0)
        {
            *low = 0 ;
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

        int off = 12 - lowPartner;
        *low += off;
        *high += off;

        if (*low < 0)
            *low = 0;
        if (*high > 37)
            *high = 37;
    }
    return suit;
}

//Do we have a Blackwood or Gerber question?
bool CBidEngine::isBlackwoodOrGerberQuestion(CBidHistory &bidHistory, Suit agree)
{
    int size = bidHistory.bidList.size();
    if (size < 4)
        return false;

    Bids bid = bidHistory.bidList[size - 2].bid;
    if ((bid != BID_4NT) && (bid != BID_4C) && (bid != BID_5NT) && (bid != BID_5C))
            return false;

    //Gerber.
    Bids pBid = bidHistory.bidList[size - 4].bid;
    if (isNextBidOpen(bidHistory) && (agree == NOTRUMP) && ((bid == BID_4C) || (bid == BID_5C)) &&
            ((pBid == BID_1NT) || (pBid == BID_2NT)))
        return true;

    //Blackwood.
    if (((bid == BID_4NT) || (bid == BID_5NT)) && (agree != NOTRUMP))
        return true;

    return false;
}

//Should we ask a Blackwood or Gerber question?
Bids CBidEngine::blackwoodOrGerberAsk(CBidHistory &bidHistory, int noAces, int noKings,
                                      int lowTotPoints, Suit agree, Suit suitAgree)
{
    int size = bidHistory.bidList.size();
    if (size < 2)
        return BID_NONE;

    Bids bid = bidHistory.bidList[size - 2].bid;
    if (!isNextBidOpen(bidHistory) && (agree == NOTRUMP) && ((bid == BID_1NT) || (bid == BID_2NT)))
    {
        if ((noAces <= 2) || ((noAces == 3) && (lowTotPoints >= BID_NT_POINT[BID_GRAND_SLAM_INX])))
            return BID_4C;
        else if ((noKings <= 3) && (lowTotPoints >= BID_NT_POINT[BID_GRAND_SLAM_INX]))
            return BID_5C;
    }
    else if (((suitAgree != ANY) && (agree != NOTRUMP)) || ((suitAgree == ANY) && (agree == BID_SUIT(bidHistory.bidList[size - 2].bid))))
    {
        if ((size < 4) || ((bidHistory.bidList[size - 4].bid != BID_4NT) &&
                ((noAces <= 2) || ((noAces == 3) && (lowTotPoints >= BID_SUIT_POINT[BID_GRAND_SLAM_INX])))))
            return BID_4NT;
        else if ((size > 4) && ((bidHistory.bidList[size - 4].bid == BID_4NT)) &&
                 (noKings <= 3) && (lowTotPoints >= BID_SUIT_POINT[BID_GRAND_SLAM_INX]))
            return BID_5NT;
    }

    return BID_NONE;
}

int CBidEngine::acesAsked(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    for (int i = size - 2; i >= 0; i -= 4)
    for (int j = 0; j < bidHistory.bidList[i].rules.size(); j++)
    {
        int alertId = bidHistory.bidList[i].rules[j]->getAlertId();
        if ((alertId == ZERO_OR_FOUR_ACES) || (alertId == ONE_ACE) || (alertId == TWO_ACES) || (alertId == THREE_ACES))
            return alertId;
    }
    return -1;
}

int CBidEngine::kingsAsked(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    for (int i = size - 2; i >= 0; i -= 4)
    for (int j = 0; j < bidHistory.bidList[i].rules.size(); j++)
    {
        int alertId = bidHistory.bidList[i].rules[j]->getAlertId();
        if ((alertId == ZERO_OR_FOUR_KINGS) || (alertId == ONE_KING) || (alertId == TWO_KINGS) || (alertId == THREE_KINGS))
            return alertId;
    }
    return -1;
}

//Calculate sum number of a given card for own and partner.
int CBidEngine::CalculateNoCards(CFeatures features, int cardVal)
{
    int noCard = 0;
    for (int i = 0; i < 4; i++)
        if (features.getCountCard((Suit)i, cardVal) == 1)
            noCard++;
    if (features.getCountCard(ANY, cardVal) > noCard)
        noCard = features.getCountCard(ANY, cardVal);

    return noCard;
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

//Is the next bidder an overcaller?
bool CBidEngine::isNextBidOverCall(CBidHistory bidHistory)
{
    int size = bidHistory.bidList.size();

    int first = size % 2;
    int i;
    for (i = first; i < size; i += 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;
    first = (first == 0) ? (1) : (0);
    int j;
    for (j = first; i < size; i += 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;

    return (i > j);
}

//What type is the next bidder (used to discriminate between openers first an second bid
//and partners first and second response i.e the first 4 bids - only used in cases where
//a new suit is bidded)?
int CBidEngine::nextBidder(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    for (int i = 0; i < size; i++)
        if (bidHistory.bidList[i].rules[0]->getAlertId() > 0)
            return OPEN_ALERT;

    int first = size % 2;
    int i;
    for (i = first; i < size; i += 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;

    if ((i < size) && (!IS_BID(bidHistory.bidList[i].bid)))
        return OPEN_OTHER;

    int retVal;

    //Opener.
    if ((((size - i) % 4) == 0))
        retVal = ((size - i) == 4) ? (OPEN_REBID) : (OPEN_OTHER);

    //Partner.
    else
        retVal = ((size - i) == 2) ? (OPEN_RESPONSE) : ((size - i) == 6) ? (OPEN_REBID_RESPONSE) : (OPEN_OTHER);

    return retVal;
}

//Check if a proper NT has been bidded by current bidder or partner.
//A NT bid is proper if it is not a catch all NT and not a conventional NT.
bool CBidEngine::isNTBidded(CBidHistory &bidHistory)
{
    int size = bidHistory.bidList.size();

    int first = size % 2;
    int i;
    for (i = first; i < size; i += 2)
        if (bidHistory.bidList[i].bid != BID_PASS)
            break;

    bool ntBid = false;
    for (int j = i; j < size; j += 2)
    {
        CFeatures lowFeatures;
        CFeatures highFeatures;

        bidHistory.bidList[j].rules[0]->getFeatures(&lowFeatures, &highFeatures);

        if ((BID_SUIT(bidHistory.bidList[j].bid) == NOTRUMP) &&
            (!((((j - i) == 2) && ((bidHistory.bidList[j].bid == BID_1NT) && (highFeatures.getDp(NOTRUMP) == highFeatures.getMaxDp()))) ||
             (bidHistory.bidList[j].alert > 0))))
            ntBid = true;
    }
    return ntBid;
}

Bids CBidEngine::getLastNotAlertedBid(CBidHistory bidHistory)
{
    int size = bidHistory.bidList.size();

    int i;
    for (i = size - 1; i >= 0; i--)
        if (bidHistory.bidList[i].alert == 0)
            break;

    return (i >= 0) ? bidHistory.bidList[i].bid : BID_NONE;
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

