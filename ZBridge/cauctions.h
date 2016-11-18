/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CAuctions.h
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the auction bid part of the bid database.
 */

#ifndef CAUCTIONS_H
#define CAUCTIONS_H

#include <QtGlobal>
#include <QDataStream>
#include <QMap>
#include <QList>

#include "cbids.h"
#include "cauction.h"
#include "defines.h"

/**
 * @brief Maps bid sequence (auction).
 *
 * Maps the auction (bid sequence) to list of bids possible for this auction to get the next bid or
 * maps the auction to a substitute auction.
 *
 */
class CAuctions
{
public:
    CAuctions();
    ~CAuctions();

    QList<CAuction> getAuctions();          /**< Get list of auction keys from the auctions map. */
    CBids *getpBids(CAuction auction);      /**< Get value of a given entry in the auctions map. */
    bool contains(CAuction auction);        /**< Check if a given auction key exist in the auctions map. */
    void createAuction(CAuction auction);   /**< Create an entry in the auctions map. */
    void deleteAuction(CAuction auction);   /**< Delete a given entry in the auctions map. */

    //Serialization.
    friend QDataStream &operator<<(QDataStream &out, const CAuctions *auctions);
    friend QDataStream &operator>>(QDataStream &in, CAuctions *&auctions);

private:
    QMap<CAuction, CBids*> auctions;
};

#endif // CAUCTIONS_H
