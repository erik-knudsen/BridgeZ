/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidDB.cpp
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the structure of the bid database.
 */

#include <cassert>
#include <QtAlgorithms>

#include "ZBridgeException.h"
#include "cbiddb.h"

const qint32 MAGIC_NUMBER = 0x6D7C8B9A;
const qint32 BIDDB_VERSION = 01;


CBidDB::CBidDB()
{
}

CBidDB::~CBidDB()
{
    clearBidDB();
}

void CBidDB::clearBidDB()
{
    qDeleteAll(pages);
    pages.clear();
}

QList<qint16> CBidDB::getPages()
{
    return pages.keys();
}

QList<CAuction> CBidDB::getAuctions(qint16 page)
{
    QList<CAuction> auctions;
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
        auctions = pAuctions->getAuctions();

    return auctions;
}

void CBidDB::getBids(qint16 page, CAuction &auction, QList<qint8> *bids, QList<CRule *> *pRules)
{
    bids->clear();
    pRules->clear();
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            pBids->getBids(bids, pRules);
    }
}

QList<CRule *> CBidDB::getpRules(qint16 page, CAuction &auction, qint8 bid)
{
    QList<CRule *> pRules;
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            pRules = pBids->getpRules(bid);
    }
    return pRules;
}

CAuction CBidDB::getSubstituteAuction(qint16 page, CAuction &auction)
{
    CAuction substituteAuction;
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            substituteAuction = pBids->getSubstituteAuction();
    }
    return substituteAuction;
}

CRule *CBidDB::getpRule(qint16 page, CAuction &auction, qint8 bid, CRule *pRule)
{
    assert(bidExist(page, auction, bid, pRule));

    CAuctions* pAuctions = pages.value(page, 0);
    CBids *pBids = pAuctions->getpBids(auction);
    return pBids->getpRule(bid, pRule);
}

bool CBidDB::pageExist(qint16 page)
{
    return pages.contains(page);
}

bool CBidDB::auctionExist(qint16 page, CAuction &auction)
{
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
        return pAuctions->contains(auction);

    return false;
}

bool CBidDB::bidExist(qint16 page, CAuction &auction, qint8 bid, CRule *pRule)
{
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            return pBids->contains(bid, pRule);
    }

    return false;
}

bool CBidDB::bidExist(qint16 page, CAuction &auction, qint8 bid)
{
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            return pBids->contains(bid);
    }

    return false;
}

bool CBidDB::isSubstituteAuction(qint16 page, CAuction &auction)
{
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            return pBids->isSubstituteAuction();
    }

    return false;
}

bool CBidDB::isBids(qint16 page, CAuction &auction)
{
    CAuctions* pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            return pBids->isBids();
    }

    return false;
}

void CBidDB::createPage(qint16 page)
{
    assert(!pageExist(page));

    CAuctions *pAuctions = new CAuctions();
    pages.insert(page, pAuctions);
}

void CBidDB::createAuction(qint16 page, CAuction &auction)
{
    assert(pageExist(page));
    if (auctionExist(page, auction))
        return;

    CAuctions *pAuctions = pages.value(page, 0);
    pAuctions->createAuction(auction);
}

CRule *CBidDB::createBid(qint16 page, CAuction &auction, qint8 bid)
{
    assert(auctionExist(page, auction) && !isSubstituteAuction(page, auction));
    assert((bid >= BID_PASS) && (bid <= BID_REDOUBLE));

    CAuctions *pAuctions = pages.value(page, 0);
    CBids *pBids = pAuctions->getpBids(auction);
    CRule *pRule = new CRule();
    pBids->createBid(bid, pRule);

    return pRule;
}

void CBidDB::createSubstituteAuction(qint16 page, CAuction &auction, CAuction &substituteAuction)
{
    assert(auctionExist(page, auction) && !isBids(page, auction));

    CAuctions *pAuctions = pages.value(page, 0);
    CBids *pBids = pAuctions->getpBids(auction);
    pBids->createSubstituteAuction(substituteAuction);
}

void CBidDB::deletePage(qint16 page)
{
    CAuctions *pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
        delete pAuctions;

    pages.remove(page);
}

void CBidDB::deleteAuction(qint16 page, CAuction &auction)
{
    CAuctions *pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
        pAuctions->deleteAuction(auction);
}

void CBidDB::deleteBid(qint16 page, CAuction &auction, qint8 bid, CRule *pRule)
{
    CAuctions *pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            pBids->deleteBid(bid, pRule);
    }
}

void CBidDB::deleteSubstituteAuction(qint16 page, CAuction &auction)
{
    CAuctions *pAuctions = pages.value(page, 0);
    if (pAuctions != 0)
    {
        CBids *pBids = pAuctions->getpBids(auction);
        if (pBids != 0)
            pBids->deleteSubstituteAuction();
    }
}

/**
 * @brief operator << Serialization of bid database.
 */
QDataStream &operator<<(QDataStream &out, const CBidDB &bidDB)
{
    out<<MAGIC_NUMBER<<BIDDB_VERSION;
    out<<bidDB.pages;

    return out;
}

/**
 * @brief operator >> Serialization of bid database.
 */
QDataStream &operator>>(QDataStream &in, CBidDB &bidDB)
{
    quint32 magicNumber;
    quint32 version;

    in>>magicNumber;
    if (magicNumber != MAGIC_NUMBER)
    {
        in.setStatus(QDataStream::ReadCorruptData);
        return in;
    }
    in>>version;
    if (version != BIDDB_VERSION)
    {
        in.setStatus(QDataStream::ReadCorruptData);
        return in;
    }
    in>>bidDB.pages;

    return in;
}

