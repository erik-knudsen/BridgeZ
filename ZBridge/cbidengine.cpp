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
 * @param[in] bidHistory The bid histyory.
 * @param[in] cards The cards for the next bidder.
 * @param[out] allFeatures Features for all players. Update current player.
 * @return The calculated next bid.
 */
Bids CBidEngine::getNextBid(CBidHistory &bidHistory, int cards[], CFeatures allFeatures[2][4])
{
    CFeatures features;
    CAuction auction;

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

    //First search for auction skipping substitute auctions.
    QSetIterator<qint16> i(pages);
    while (i.hasNext())
    {
        qint16 page = i.next();
        if (bidDB->auctionExist(page, auction) && bidDB->isBids(page, auction))
        {
            QList<qint8> bids;
            QList<CRule*> pRules;
            bidDB->getBids(page, auction, &bids, &pRules);
            bool found = false;
            for (int i = 0; i < bids.size(); i++)
                if (rules.contains(pRules[i]->getId()))
                {
                    found = true;
                    if (pRules[i]->RuleIsOk(features))
                        return (Bids)bids[i];
                }
            if (found)
                return BID_NONE;
        }
    }

    //Search for substitute auction skipping ordinary auctions.
}
