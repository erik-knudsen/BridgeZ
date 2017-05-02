/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CRule.h
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the rule part of the bid database.
 */

#ifndef CRULE_H
#define CRULE_H

#include <QDataStream>
#include <QtGlobal>

#include "defines.h"
#include "cfeatures.h"

/**
 * @brief Defines the features to be fulfilled for a given bid.
 */
class CRule
{
public:
    CRule();

    void initialize();
    bool RuleIsOk(CFeatures &feature);

    //Getters and setters.
    void setId(qint16 id) { this->id = id; }
    qint16 getId() { return id; }
    void setPriority(qint8 priority) { this->priority = priority; }
    qint8 getPriority() { return priority; }
    void setStatus(Forcing status) { this->status = status; }
    Forcing getStatus() { return status; }
    void setScoringMethod(ScoringMethod scoringMethod) { this->scoringMethod = scoringMethod; }
    ScoringMethod getScoringMethod() { return scoringMethod; }
    void setVulnerability(Vulnerability vulnerability) { this->vulnerability = vulnerability; }
    Vulnerability getVulnerability() { return vulnerability; }
    void setAlertId(quint8 alertId) { this->alertId = alertId; }
    quint8 getAlertId() { return alertId; }
    void setFeatures(CFeatures &lowFeatures, CFeatures &highFeatures);
    void getFeatures(CFeatures *lowFeatures, CFeatures *highFeatures);
    void setdBRule(bool dBRule) { this->dBRule = dBRule; }
    bool isdBRule() { return dBRule; }

    //Serialization.
    friend QDataStream &operator<<(QDataStream &out, const CRule *rule);
    friend QDataStream &operator>>(QDataStream &in, CRule *&rule);

private:
    bool dBRule;                    /**< true if the rule is from the bidding database. */
    qint16 id;                      /**< Identification of the rule. */
    qint8 priority;                 /**< Priority of the rule. */
    Forcing status;                 /**< Forcing etc. */
    ScoringMethod scoringMethod;    /**< Scoring method. */
    Vulnerability vulnerability;    /**< Vulnerability. */
    quint8 alertId;                 /**< Alert message. */

    CFeatures featureRange[2];      /**< Upper and lower values of features for the rule. */
};

#endif // CRULE_H
