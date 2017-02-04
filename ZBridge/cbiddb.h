/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidDB.h
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the structure of the bid database.
 */

#ifndef CBIDDB_H
#define CBIDDB_H

#include <QtGlobal>
#include <QDataStream>
#include <QMap>

#include "cauction.h"
#include "cauctions.h"
#include "defines.h"


/**
 * @brief This class is the top level class of the bid database.
 *
 * The intention of the database is, based on definition of selected conventions, the given hand
 * (13 cards) and the auction (bidding sequence) till now, to identify the best next bid.
 *
 * The database has three levels:
 *   - The first level (page) selects a top level of rules, based on selected conventions. The first
 *     level is a one to one mapping (injective).
 *   - The second level (auction) delimits the rules in the top level set, to rules relevant for
 *     a given auction (bidding sequence). The second level is also a one to one mapping.
 *   - The third level (next bid) further delimits the rules to one rule based on the hand (13 card) features
 *     (High card points etc.) and in some cases also to selected conventions. The selected rule
 *     identifies the next bid. The third level is a one to many mapping (the next bid can be selected
 *     by more than one rule).
 *     In some cases the third level instead identifies a substitute auction (sequence of bids).
 *     It might also happen that the database cannot find a best next bid.
 */
class CBidDB
{
public:
    CBidDB();
    ~CBidDB();

    void clearBidDB();

    /** @name Get keys for the three mapping levels
     * Level 1 is page, level 2 is auction and level 3 is next bid or substitute auction.
     */
    /*@{*/
    QList<qint16> getPages();
    QList<CAuction> getAuctions(qint16 page);
    void getBids(qint16 page, CAuction &auction, QList<qint8> *bids, QList<CRule*> *pRules);
    QList<CRule *> getpRules(qint16 page, CAuction &auction, qint8 bid);
    CAuction getSubstituteAuction(qint16 page, CAuction &auction);
    CRule *getpRule(qint16 page, CAuction &auction, qint8 bid, CRule *pRule);
    /*@}*/

    /** @name Check existency of a given key in one of the three mapping levels
     * Level 1 is page, level 2 is auction and level 3 is next bid or substitute auction.
     */
    /*@{*/
    bool pageExist(qint16 page);
    bool auctionExist(qint16 page, CAuction &auction);
    bool bidExist(qint16 page, CAuction &auction, qint8 bid, CRule *pRule);
    bool bidExist(qint16 page, CAuction &auction, qint8 bid);
    bool isSubstituteAuction(qint16 page, CAuction &auction);
    bool isBids(qint16 page, CAuction &auction);
    /*@}*/

    /** @name Create an entry in one of the three mapping levels
     * Level 1 is page, level 2 is auction and level 3 is next bid or substitute auction.
     */
    /*@{*/
    void createPage(qint16 page);
    void createAuction(qint16 page, CAuction &auction);
    CRule *createBid(qint16 page, CAuction &auction, qint8 bid);
    void createSubstituteAuction(qint16 page, CAuction &auction, CAuction &substituteAuction);
    /*@}*/

    /** @name Delete an entry in one of the three mapping levels
     * Level 1 is page, level 2 is auction and level 3 is next bid or substitute auction.
     */
    /*@{*/
    void deletePage(qint16 page);
    void deleteAuction(qint16 page, CAuction &auction);
    void deleteBid(qint16 page, CAuction &auction, qint8 bid, CRule *pRule);
    void deleteSubstituteAuction(qint16 page, CAuction &auction);
    /*@}*/

    //Serialization.
    friend QDataStream &operator<<(QDataStream &out, const CBidDB &bidDB);
    friend QDataStream &operator>>(QDataStream &in, CBidDB &bidDB);

private:
    QMap<qint16, CAuctions*> pages;
};

#endif // CBIDDB_H
