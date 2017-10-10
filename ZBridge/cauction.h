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
 * The file implements the declaration of an auction (bid sequence).
 */

#ifndef CAUCTION_H
#define CAUCTION_H

#include <QtGlobal>
#include <QDataStream>
#include <QList>


/**
 * @brief The CAuction class defines a bid sequence (an auction).
 */
class CAuction
{
public:
    CAuction();

    //Serialization.
    friend QDataStream &operator<<(QDataStream &out, const CAuction &auction);
    friend QDataStream &operator>>(QDataStream &in, CAuction &auction);

    //Sorting and equal operators.
    friend bool operator<(const CAuction &auction_1, const CAuction &auction_2);
    friend bool operator==(const CAuction &auction_1, const CAuction &auction_2);

    QList<qint8> auction;
};

#endif // CAUCTION_H
