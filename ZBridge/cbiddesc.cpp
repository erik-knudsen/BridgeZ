/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidDesc.cpp
  Developers: eak

  Revision History:
  2-aug-2016 eak: Original

  Abstract: Text description of pages and rules in the bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of text description for pages and rules in the bid database.
 */

#include "cbiddesc.h"

const qint32 MAGIC_NUMBER = 0x5E6D7C8B;
const qint32 BIDDB_VERSION = 01;

CBidDesc::CBidDesc()
{
}

CBidDesc::~CBidDesc()
{
    clearBidDesc();
}

void CBidDesc::clearBidDesc()
{
    pages.clear();
    ruleIds.clear();
    alertIds.clear();
}

void CBidDesc::setPageDesc(qint16 pageId, QString &text)
{
    pages.remove(pageId);
    if (text.size() > 0)
        pages.insert(pageId, text);
}

void CBidDesc::setRuleIdDesc(qint16 ruleId, QString &text)
{
    ruleIds.remove(ruleId);
    if (text.size() > 0)
        ruleIds.insert(ruleId, text);
}

void CBidDesc::setAlertIdDesc(quint8 alertId, QString &text)
{
    alertIds.remove(alertId);
    if (text.size() > 0)
        alertIds.insert(alertId, text);
}

QDataStream &operator<<(QDataStream &out, const CBidDesc &bidDesc)
{
    out<<MAGIC_NUMBER<<BIDDB_VERSION;
    out<<bidDesc.pages;
    out<<bidDesc.ruleIds;
    out<<bidDesc.alertIds;

    return out;
}
QDataStream &operator>>(QDataStream &in, CBidDesc &bidDesc)
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
    in>>bidDesc.pages;
    in>>bidDesc.ruleIds;
    in>>bidDesc.alertIds;

    return in;
}
