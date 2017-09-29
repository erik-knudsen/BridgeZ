/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidDesc.h
  Developers: eak

  Revision History:
  2-aug-2016 eak: Original

  Abstract: Text description of pages and of rules in the bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of text description for pages, rules and alerts in the bid database.
 */

#ifndef CBIDDESC_H
#define CBIDDESC_H

#include <QtGlobal>
#include <QDataStream>
#include <QMap>

/**
 * @brief Text description of pages, rules and alerts in the bid database.
 */
class CBidDesc
{
public:
    CBidDesc();
    ~CBidDesc();
    void clearBidDesc();

    void setPageDesc(qint16 pageId, QString &text);
    QString getPageDesc(qint16 pageId) { return pages.value(pageId, ""); }
    void setRuleIdDesc(qint16 ruleId, QString &text);
    QString getRuleIdDesc(qint16 ruleId) { return ruleIds.value(ruleId, ""); }
    void setAlertIdDesc(quint8 alertId, QString &text);
    QString getAlertIdDesc(quint8 alertId) { return alertIds.value(alertId, ""); }

    friend QDataStream &operator<<(QDataStream &out, const CBidDesc &bidDesc);
    friend QDataStream &operator>>(QDataStream &in, CBidDesc &bidDesc);

private:
    QMap<qint16, QString> pages;
    QMap<qint16, QString> ruleIds;
    QMap<quint8, QString> alertIds;
};

#endif // CBIDDESC_H
