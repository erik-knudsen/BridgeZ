/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CAuctions.cpp
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the auction bid part of the bid database.
 */

#include <QtAlgorithms>
#include <cassert>

#include "cauctions.h"

CAuctions::CAuctions()
{

}

CAuctions::~CAuctions()
{
    qDeleteAll(auctions);
    auctions.clear();
}

QList<CAuction> CAuctions::getAuctions()
{
    return auctions.keys();
}

CBids *CAuctions::getpBids(CAuction auction)
{
    return auctions.value(auction, 0);
}

bool CAuctions::contains(CAuction auction)
{
    return auctions.contains(auction);
}

void CAuctions::createAuction(CAuction auction)
{
    assert(!contains(auction));

    CBids *pBids = new CBids();
    auctions.insert(auction, pBids);
}

void CAuctions::deleteAuction(CAuction auction)
{
    CBids *pBids = auctions.value(auction, 0);
    if (pBids != 0)
        delete pBids;

    auctions.remove(auction);
}

/**
 * @brief operator << Serialization of auctions.
 */
QDataStream &operator<<(QDataStream &out, const CAuctions *auctions)
{
    out<<auctions->auctions;

    return out;
}

/**
 * @brief operator >> Serialization of auctions.
 */
QDataStream &operator>>(QDataStream &in, CAuctions *&auctions)
{
    auctions = new CAuctions();
    in>>auctions->auctions;

    return in;
}


