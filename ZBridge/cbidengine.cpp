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

#include "cbiddb.h"
#include "cbiddesc.h"
#include "cbiddbdefine.h"
#include "cbidhistory.h"
#include "cfeatures.h"
#include "cauction.h"
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
 * @brief Calculate the next bid
 *
 * Calculate the next bid by using the bidding database.
 *
 * @param[in] bidHistory The bid history.
 * @param[in] cards The cards for the next bidder.
 * @param[in] scoringMethod The scoring method.
 * @param[in] vulnerability The vulnerability.
 * @param[out] forcing The forcing status.
 * @param[out] alertId The alert id.
 * @return The calculated next bid. If none was found then return BID_NONE.
 */
Bids CBidEngine::getNextBid(CBidHistory &bidHistory, int cards[], ScoringMethod scoringMethod,
                            Vulnerability vulnerability, Forcing *forcing, int *alertId)
{
    CAuction auction;
    CFeatures features;

    //Calculate features.
    features.setCardFeatures(cards);

    //Get auction till now.
    for (int i = 0; i < bidHistory.bidList.size(); i++)
        auction.auction.append(bidHistory.bidList[i].bid);

    //Next bidder.
    Seat seat = (Seat)((bidHistory.bidList.last().bidder + 1) % 4);

    //Get relevant pages and rules.
    QSet<qint16> &pages = bidDBDefine->getPages(seat);
    QSet<qint16> &rules = bidDBDefine->getRules(seat);

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
                    bool found = false;
                    QList<int> bidInx;
                    for (int i = 0; i < bids.size(); i++)
                        if (rules.contains(pRules[i]->getId()))
                        {
                            //Found a defined rule. Check features.
                            found = true;
                            if (((pRules[i]->getScoringMethod() == NOSCORE) ||
                                    (pRules[i]->getScoringMethod() == scoringMethod)) &&
                                    (pRules[i]->getVulnerability() == vulnerability) &&
                                    (pRules[i]->RuleIsOk(features)))
                                bidInx.append(i);
                        }
                    //Found one or more rules and feature check is ok?
                    if (bidInx.size() > 0)
                    {
                        //Find highest priority.
                        int j, i;
                        for (j = 1, i = bidInx[0]; j < bidInx.size(); j++)
                            if (pRules[i]->getPriority() < pRules[j]->getPriority())
                                i = j;

                        //Get forcing status.
                        *forcing = pRules[i]->getStatus();

                        //Get alert id.
                        *alertId = pRules[i]->getAlertId();

                        //Return found bid.
                        return (Bids)bids[i];
                    }
                    else if (found)
                        return (BID_NONE);
                }
                else
                {
                    //Found a substitute auction. Get the substitute auction.
                    //and try again.
                    cont = true;
                    auction = bidDB->getSubstituteAuction(page, auction);
                    break;
                }
            }
        }
        if (cont)
            continue;

        //We did not find an auction.
        //Try to remove initial pass bids (if any) in the auction.
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
    }

    return BID_NONE;
}

/**
 * @brief Get possible rules for a given bid history and next bid as calculated by getNextBid.
 * @param[in] bidHistory The bid history.
 * @param bid[in] The bid calculated by getNext bid.
 * @param scoringMethod The scoring method.
 * @param vulnerability The vulnerability.
 * @return returns a list with possible rules.
 */
QList<CRule *> CBidEngine::getpRules(CBidHistory &bidHistory, Bids bid, ScoringMethod scoringMethod,
                                     Vulnerability vulnerability)
{
    CAuction auction;
    QList<CRule *> pDefRules;

    //Get auction till now.
    for (int i = 0; i < bidHistory.bidList.size(); i++)
        auction.auction.append(bidHistory.bidList[i].bid);

    //Next bidder.
    Seat seat = (Seat)((bidHistory.bidList.last().bidder + 1) % 4);

    //Get relevant pages and rules.
    QSet<qint16> &pages = bidDBDefine->getPages(seat);
    QSet<qint16> &rules = bidDBDefine->getRules(seat);

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
                    //Found auction. Get rules for the auction and bid.
                    QList<CRule*> pRules;
                    pRules = bidDB->getpRules(page, auction, bid);
                    bool found = false;
                    for (int i = 0; i < pRules.size(); i++)
                        if (rules.contains(pRules[i]->getId()))
                        {
                            //Found a defined rule. Check features.
                            found = true;
                            if ((pRules[i]->getScoringMethod() == scoringMethod) &&
                                    (pRules[i]->getVulnerability() == vulnerability))
                                pDefRules.append(pRules[i]);
                        }
                    //Found one or more rules?
                    if (found)
                        return pDefRules;
                }
                else
                {
                    //Found a substitute auction. Get the substitute auction.
                    //and try again.
                    cont = true;
                    auction = bidDB->getSubstituteAuction(page, auction);
                    break;
                }
            }
        }
        if (cont)
            continue;

        //We did not find an auction.
        //Try to remove initial pass bids (if any) in the auction.
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
    }

    return pDefRules;           //Empty.
}
