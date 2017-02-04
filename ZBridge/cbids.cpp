/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBids.cpp
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the bids part of the bid database.
 */

#include <cassert>
#include <QtGlobal>
#include <QtAlgorithms>

#include "cbids.h"

CBids::CBids()
{

}

CBids::~CBids()
{
    qDeleteAll(bids);
    substituteAuction.auction.clear();
    bids.clear();
}

void CBids::getBids(QList<qint8> *bids, QList<CRule *> *pRules)
{
    *bids = this->bids.keys();
    *pRules = this->bids.values();
}

QList<CRule *> CBids::getpRules(qint8 bid)
{
    return (bids.values(bid));
}

CRule *CBids::getpRule(qint8 bid, CRule *pRule)
{
    return (bids.contains(bid, pRule)) ? pRule : 0;
}

bool CBids::contains(qint8 bid, CRule *pRule)
{
    return bids.contains(bid, pRule);
}

bool CBids::contains(qint8 bid)
{
    return bids.contains(bid);
}

void CBids::createBid(qint8 bid, CRule *pRule)
{
    bids.insertMulti(bid, pRule);
}

void CBids::deleteBid(qint8 bid, CRule *pRule)
{
    if (bids.contains(bid, pRule))
    {
        bids.remove(bid, pRule);
        delete pRule;
    }
}

/**
 * @brief operator << Seralization of bids.
 */
QDataStream &operator<<(QDataStream &out, const CBids *bids)
{
    out<<bids->substituteAuction;
    out<<bids->bids;

    return out;
}

/**
 * @brief operator >> Serialization of bids.
 */
QDataStream &operator>>(QDataStream &in, CBids *&bids)
{
    bids = new CBids();
    in>>bids->substituteAuction;
    in>>bids->bids;

    return in;
}

