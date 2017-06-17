/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CRule.cpp
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid database.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the rule part of the bid database.
 */

#include <cassert>
#include <QtGlobal>

#include "crule.h"

CRule::CRule()
{
    initialize();
}

void CRule::initialize()
{
    dBRule = true;

    id = 0;
    priority = 0;           //Lowest priority.
    status = NON_FORCING;   //Not forcing rule.
    scoringMethod = NOSCORE;//Scoring method is not relevant.
    vulnerability = VUL_II; //Ignore vulnerability for both sides.
    alertId = 0;            //No alert.

    featureRange[0].setMinFeatures();
    featureRange[1].setMaxFeatures();
}

/**
 * @brief Check if the rule is ok for a given set of features.
 * @param feature The features to check.
 * @return true if ok otherwise false.
 */
bool CRule::RuleIsOk(CFeatures &feature)
{
    return (feature.featureIsOk(featureRange[1], featureRange[0]) == 0);

}

void CRule::setFeatures(CFeatures &lowFeatures, CFeatures &highFeatures)
{
    featureRange[0] = lowFeatures;
    featureRange[1] = highFeatures;
}

void CRule::getFeatures(CFeatures *lowFeatures, CFeatures *highFeatures)
{
    *lowFeatures = featureRange[0];
    *highFeatures = featureRange[1];
}

/**
 * @brief operator << Serialization of rule.
 */
QDataStream &operator<<(QDataStream &out, const CRule *rule)
{
    qint8 status8 = rule->status;
    qint8 scoringMethod8 = rule->scoringMethod;
    qint8 vulnerability8 = rule->vulnerability;

    out<<rule->id<<rule->priority<<status8;
    out<<scoringMethod8<<vulnerability8<<rule->alertId;
    out<<rule->featureRange[0]<<rule->featureRange[1];

    return out;
}

/**
 * @brief operator >> Serialization of rule.
 */
QDataStream &operator>>(QDataStream &in, CRule *&rule)
{
    rule = new CRule();

    qint8 status8;
    qint8 scoringMethod8;
    qint8 vulnerability8;
    in>>rule->id>>rule->priority>>status8;
    in>>scoringMethod8>>vulnerability8>>rule->alertId;
    in>>rule->featureRange[0]>>rule->featureRange[1];

    if (rule->priority > 7)
        rule->priority = 7;
    if (rule->priority < 0)
        rule->priority = 0;

    if (status8 > MUST_PASS)
        status8 = MUST_PASS;
    if (status8 < NON_FORCING)
        status8 = NON_FORCING;
    rule->status = (Forcing)status8;

    if ((scoringMethod8 != MP) && (scoringMethod8 != IMP))
        scoringMethod8 = NOSCORE;
    rule->scoringMethod = (ScoringMethod)scoringMethod8;

    if (vulnerability8 > VUL_YY)
        vulnerability8 = VUL_YY;
    if (vulnerability8 < VUL_II)
        vulnerability8 = VUL_II;
    rule->vulnerability = (Vulnerability)vulnerability8;

    return in;
}

