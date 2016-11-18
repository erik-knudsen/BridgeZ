/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CAuction.h
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of an auction (bid sequence).
 */

#include "cauction.h"

CAuction::CAuction()
{

}

/**
 * @brief operator << Serialization of auction.
 */
QDataStream &operator<<(QDataStream &out, const CAuction &auction)
{
    out<<auction.auction;
}

/**
 * @brief operator >> Serialization of auction.
 */
QDataStream &operator>>(QDataStream &in, CAuction &auction)
{
    in>>auction.auction;
}

/**
 * @brief operator < Sorting opeerator for auctions.
 */
bool operator<(const CAuction &auction_1, const CAuction &auction_2)
{
    int s1 = auction_1.auction.size();
    int s2 = auction_2.auction.size();
    int sz = (s1 > s2) ? s2 : s1;
    for (int i = 0; i < sz; i++)
    {
        if (auction_1.auction[i] > auction_2.auction[i])
            return false;
        if (auction_1.auction[i] < auction_2.auction[i])
            return true;
    }
    return (s1 < s2);
}

