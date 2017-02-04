/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBids.h
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bids part of the bid database.
 */

#ifndef CBIDS_H
#define CBIDS_H

#include <QDataStream>
#include <QMultiMap>

#include "cauction.h"
#include "defines.h"
#include "crule.h"

/**
 * @brief Substitute auction (bid sequence) or map of next bid to required features.
 *
 */
class CBids
{
public:
    CBids();
    ~CBids();

    /** @name Get entries/values.
     */
    /*@{*/
    void getBids(QList<qint8> *bids, QList<CRule*> *pRules);
    CAuction getSubstituteAuction() { return substituteAuction; }
    QList<CRule *> getpRules(qint8 bid);
    CRule *getpRule(qint8 bid, CRule *pRule);
    bool contains(qint8 bid, CRule *pRule);
    bool contains(qint8 bid);
    /*@}*/

    /** @name Create entries/values.
     */
    /*@{*/
    void createBid(qint8 bid, CRule *pRule);
    void createSubstituteAuction(CAuction &auction) { substituteAuction = auction; }
    /*@}*/

    /** @name Delete entries/values.
     */
    /*@{*/
    void deleteBid(qint8 bid, CRule *pRule);
    void deleteSubstituteAuction() { substituteAuction.auction.clear(); }
    /*@}*/

    /** @name Check for substitute action or next bid..
     */
    /*@{*/
    bool isSubstituteAuction() { return substituteAuction.auction.size() > 0; }
    bool isBids() { return bids.size() > 0; }
    /*@}*/

    //Serialization.
    friend QDataStream &operator<<(QDataStream &out, const CBids *bids);
    friend QDataStream &operator>>(QDataStream &in, CBids *&bids);

private:
    CAuction substituteAuction;
    QMultiMap<qint8, CRule*> bids;
};

#endif // CBIDS_H
