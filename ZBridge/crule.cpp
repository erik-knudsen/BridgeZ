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
    const int NO_FEATURES = 115;
    int low[NO_FEATURES], high[NO_FEATURES], cur[NO_FEATURES];
    int i;

    i = 0;
    low[i++] = featureRange[0].getHcp(SPADES);
    low[i++] = featureRange[0].getHcp(HEARTS);
    low[i++] = featureRange[0].getHcp(DIAMONDS);
    low[i++] = featureRange[0].getHcp(CLUBS);
    low[i++] = featureRange[0].getHcp(ANY);

    low[i++] = featureRange[0].getControls(SPADES);
    low[i++] = featureRange[0].getControls(HEARTS);
    low[i++] = featureRange[0].getControls(DIAMONDS);
    low[i++] = featureRange[0].getControls(CLUBS);
    low[i++] = featureRange[0].getControls(ANY);

    low[i++] = featureRange[0].getHc(SPADES);
    low[i++] = featureRange[0].getHc(HEARTS);
    low[i++] = featureRange[0].getHc(DIAMONDS);
    low[i++] = featureRange[0].getHc(CLUBS);
    low[i++] = featureRange[0].getHc(ANY);

    low[i++] = featureRange[0].getHonor(SPADES);
    low[i++] = featureRange[0].getHonor(HEARTS);
    low[i++] = featureRange[0].getHonor(DIAMONDS);
    low[i++] = featureRange[0].getHonor(CLUBS);
    low[i++] = featureRange[0].getHonor(ANY);

    low[i++] = featureRange[0].getCountCard(SPADES, ACE);
    low[i++] = featureRange[0].getCountCard(HEARTS, ACE);
    low[i++] = featureRange[0].getCountCard(DIAMONDS, ACE);
    low[i++] = featureRange[0].getCountCard(CLUBS, ACE);
    low[i++] = featureRange[0].getCountCard(ANY, ACE);

    low[i++] = featureRange[0].getCountCard(SPADES, KING);
    low[i++] = featureRange[0].getCountCard(HEARTS, KING);
    low[i++] = featureRange[0].getCountCard(DIAMONDS, KING);
    low[i++] = featureRange[0].getCountCard(CLUBS, KING);
    low[i++] = featureRange[0].getCountCard(ANY, KING);

    low[i++] = featureRange[0].getCountCard(SPADES, QUEEN);
    low[i++] = featureRange[0].getCountCard(HEARTS, QUEEN);
    low[i++] = featureRange[0].getCountCard(DIAMONDS, QUEEN);
    low[i++] = featureRange[0].getCountCard(CLUBS, QUEEN);
    low[i++] = featureRange[0].getCountCard(ANY, QUEEN);

    low[i++] = featureRange[0].getCountCard(SPADES, JACK);
    low[i++] = featureRange[0].getCountCard(HEARTS, JACK);
    low[i++] = featureRange[0].getCountCard(DIAMONDS, JACK);
    low[i++] = featureRange[0].getCountCard(CLUBS, JACK);
    low[i++] = featureRange[0].getCountCard(ANY, JACK);

    low[i++] = featureRange[0].getCountCard(SPADES, TEN);
    low[i++] = featureRange[0].getCountCard(HEARTS, TEN);
    low[i++] = featureRange[0].getCountCard(DIAMONDS, TEN);
    low[i++] = featureRange[0].getCountCard(CLUBS, TEN);
    low[i++] = featureRange[0].getCountCard(ANY, TEN);

    low[i++] = featureRange[0].getRkcb(SPADES);
    low[i++] = featureRange[0].getRkcb(HEARTS);
    low[i++] = featureRange[0].getRkcb(DIAMONDS);
    low[i++] = featureRange[0].getRkcb(CLUBS);

    low[i++] = featureRange[0].getBal();

    low[i++] = featureRange[0].getDp(SPADES);
    low[i++] = featureRange[0].getDp(HEARTS);
    low[i++] = featureRange[0].getDp(DIAMONDS);
    low[i++] = featureRange[0].getDp(CLUBS);
    low[i++] = featureRange[0].getDp(ANY);

    low[i++] = featureRange[0].getDif2(DIAMONDS, CLUBS);
    low[i++] = featureRange[0].getDif2(HEARTS, CLUBS);
    low[i++] = featureRange[0].getDif2(HEARTS, DIAMONDS);
    low[i++] = featureRange[0].getDif2(SPADES, CLUBS);
    low[i++] = featureRange[0].getDif2(SPADES, DIAMONDS);
    low[i++] = featureRange[0].getDif2(SPADES, HEARTS);

    low[i++] = featureRange[0].getSuits();

    low[i++] = featureRange[0].getSuitLen(SPADES);
    low[i++] = featureRange[0].getSuitLen(HEARTS);
    low[i++] = featureRange[0].getSuitLen(DIAMONDS);
    low[i++] = featureRange[0].getSuitLen(CLUBS);

    low[i++] = featureRange[0].getSuitLen2(DIAMONDS, CLUBS);
    low[i++] = featureRange[0].getSuitLen2(HEARTS, CLUBS);
    low[i++] = featureRange[0].getSuitLen2(HEARTS, DIAMONDS);
    low[i++] = featureRange[0].getSuitLen2(SPADES, CLUBS);
    low[i++] = featureRange[0].getSuitLen2(SPADES, DIAMONDS);
    low[i++] = featureRange[0].getSuitLen2(SPADES, HEARTS);

    low[i++] = featureRange[0].getLMaj();

    low[i++] = featureRange[0].getLMin();

    low[i++] = featureRange[0].getLongest(SPADES);
    low[i++] = featureRange[0].getLongest(HEARTS);
    low[i++] = featureRange[0].getLongest(DIAMONDS);
    low[i++] = featureRange[0].getLongest(CLUBS);

    low[i++] = featureRange[0].getLong(SPADES);
    low[i++] = featureRange[0].getLong(HEARTS);
    low[i++] = featureRange[0].getLong(DIAMONDS);
    low[i++] = featureRange[0].getLong(CLUBS);

    low[i++] = featureRange[0].getInter(SPADES);
    low[i++] = featureRange[0].getInter(HEARTS);
    low[i++] = featureRange[0].getInter(DIAMONDS);
    low[i++] = featureRange[0].getInter(CLUBS);
    low[i++] = featureRange[0].getInter(ANY);

    low[i++] = featureRange[0].getLoosers(SPADES);
    low[i++] = featureRange[0].getLoosers(HEARTS);
    low[i++] = featureRange[0].getLoosers(DIAMONDS);
    low[i++] = featureRange[0].getLoosers(CLUBS);
    low[i++] = featureRange[0].getLoosers(ANY);

    low[i++] = featureRange[0].getQuickTricks(SPADES);
    low[i++] = featureRange[0].getQuickTricks(HEARTS);
    low[i++] = featureRange[0].getQuickTricks(DIAMONDS);
    low[i++] = featureRange[0].getQuickTricks(CLUBS);
    low[i++] = featureRange[0].getQuickTricks(ANY);

    low[i++] = featureRange[0].getPlayingTricks(SPADES);
    low[i++] = featureRange[0].getPlayingTricks(HEARTS);
    low[i++] = featureRange[0].getPlayingTricks(DIAMONDS);
    low[i++] = featureRange[0].getPlayingTricks(CLUBS);
    low[i++] = featureRange[0].getPlayingTricks(ANY);

    low[i++] = featureRange[0].getStopNT(SPADES);
    low[i++] = featureRange[0].getStopNT(HEARTS);
    low[i++] = featureRange[0].getStopNT(DIAMONDS);
    low[i++] = featureRange[0].getStopNT(CLUBS);

    low[i++] = featureRange[0].getQlty(SPADES);
    low[i++] = featureRange[0].getQlty(HEARTS);
    low[i++] = featureRange[0].getQlty(DIAMONDS);
    low[i++] = featureRange[0].getQlty(CLUBS);

    low[i++] = featureRange[0].getPoints(SPADES);
    low[i++] = featureRange[0].getPoints(HEARTS);
    low[i++] = featureRange[0].getPoints(DIAMONDS);
    low[i++] = featureRange[0].getPoints(CLUBS);
    low[i++] = featureRange[0].getPoints(ANY);

    assert (i == NO_FEATURES);

    i = 0;
    high[i++] = featureRange[1].getHcp(SPADES);
    high[i++] = featureRange[1].getHcp(HEARTS);
    high[i++] = featureRange[1].getHcp(DIAMONDS);
    high[i++] = featureRange[1].getHcp(CLUBS);
    high[i++] = featureRange[1].getHcp(ANY);

    high[i++] = featureRange[1].getControls(SPADES);
    high[i++] = featureRange[1].getControls(HEARTS);
    high[i++] = featureRange[1].getControls(DIAMONDS);
    high[i++] = featureRange[1].getControls(CLUBS);
    high[i++] = featureRange[1].getControls(ANY);

    high[i++] = featureRange[1].getHc(SPADES);
    high[i++] = featureRange[1].getHc(HEARTS);
    high[i++] = featureRange[1].getHc(DIAMONDS);
    high[i++] = featureRange[1].getHc(CLUBS);
    high[i++] = featureRange[1].getHc(ANY);

    high[i++] = featureRange[1].getHonor(SPADES);
    high[i++] = featureRange[1].getHonor(HEARTS);
    high[i++] = featureRange[1].getHonor(DIAMONDS);
    high[i++] = featureRange[1].getHonor(CLUBS);
    high[i++] = featureRange[1].getHonor(ANY);

    high[i++] = featureRange[1].getCountCard(SPADES, ACE);
    high[i++] = featureRange[1].getCountCard(HEARTS, ACE);
    high[i++] = featureRange[1].getCountCard(DIAMONDS, ACE);
    high[i++] = featureRange[1].getCountCard(CLUBS, ACE);
    high[i++] = featureRange[1].getCountCard(ANY, ACE);

    high[i++] = featureRange[1].getCountCard(SPADES, KING);
    high[i++] = featureRange[1].getCountCard(HEARTS, KING);
    high[i++] = featureRange[1].getCountCard(DIAMONDS, KING);
    high[i++] = featureRange[1].getCountCard(CLUBS, KING);
    high[i++] = featureRange[1].getCountCard(ANY, KING);

    high[i++] = featureRange[1].getCountCard(SPADES, QUEEN);
    high[i++] = featureRange[1].getCountCard(HEARTS, QUEEN);
    high[i++] = featureRange[1].getCountCard(DIAMONDS, QUEEN);
    high[i++] = featureRange[1].getCountCard(CLUBS, QUEEN);
    high[i++] = featureRange[1].getCountCard(ANY, QUEEN);

    high[i++] = featureRange[1].getCountCard(SPADES, JACK);
    high[i++] = featureRange[1].getCountCard(HEARTS, JACK);
    high[i++] = featureRange[1].getCountCard(DIAMONDS, JACK);
    high[i++] = featureRange[1].getCountCard(CLUBS, JACK);
    high[i++] = featureRange[1].getCountCard(ANY, JACK);

    high[i++] = featureRange[1].getCountCard(SPADES, TEN);
    high[i++] = featureRange[1].getCountCard(HEARTS, TEN);
    high[i++] = featureRange[1].getCountCard(DIAMONDS, TEN);
    high[i++] = featureRange[1].getCountCard(CLUBS, TEN);
    high[i++] = featureRange[1].getCountCard(ANY, TEN);

    high[i++] = featureRange[1].getRkcb(SPADES);
    high[i++] = featureRange[1].getRkcb(HEARTS);
    high[i++] = featureRange[1].getRkcb(DIAMONDS);
    high[i++] = featureRange[1].getRkcb(CLUBS);

    high[i++] = featureRange[1].getBal();

    high[i++] = featureRange[1].getDp(SPADES);
    high[i++] = featureRange[1].getDp(HEARTS);
    high[i++] = featureRange[1].getDp(DIAMONDS);
    high[i++] = featureRange[1].getDp(CLUBS);
    high[i++] = featureRange[1].getDp(ANY);

    high[i++] = featureRange[1].getDif2(DIAMONDS, CLUBS);
    high[i++] = featureRange[1].getDif2(HEARTS, CLUBS);
    high[i++] = featureRange[1].getDif2(HEARTS, DIAMONDS);
    high[i++] = featureRange[1].getDif2(SPADES, CLUBS);
    high[i++] = featureRange[1].getDif2(SPADES, DIAMONDS);
    high[i++] = featureRange[1].getDif2(SPADES, HEARTS);

    high[i++] = featureRange[1].getSuits();

    high[i++] = featureRange[1].getSuitLen(SPADES);
    high[i++] = featureRange[1].getSuitLen(HEARTS);
    high[i++] = featureRange[1].getSuitLen(DIAMONDS);
    high[i++] = featureRange[1].getSuitLen(CLUBS);

    high[i++] = featureRange[1].getSuitLen2(DIAMONDS, CLUBS);
    high[i++] = featureRange[1].getSuitLen2(HEARTS, CLUBS);
    high[i++] = featureRange[1].getSuitLen2(HEARTS, DIAMONDS);
    high[i++] = featureRange[1].getSuitLen2(SPADES, CLUBS);
    high[i++] = featureRange[1].getSuitLen2(SPADES, DIAMONDS);
    high[i++] = featureRange[1].getSuitLen2(SPADES, HEARTS);

    high[i++] = featureRange[1].getLMaj();

    high[i++] = featureRange[1].getLMin();

    high[i++] = featureRange[1].getLongest(SPADES);
    high[i++] = featureRange[1].getLongest(HEARTS);
    high[i++] = featureRange[1].getLongest(DIAMONDS);
    high[i++] = featureRange[1].getLongest(CLUBS);

    high[i++] = featureRange[1].getLong(SPADES);
    high[i++] = featureRange[1].getLong(HEARTS);
    high[i++] = featureRange[1].getLong(DIAMONDS);
    high[i++] = featureRange[1].getLong(CLUBS);

    high[i++] = featureRange[1].getInter(SPADES);
    high[i++] = featureRange[1].getInter(HEARTS);
    high[i++] = featureRange[1].getInter(DIAMONDS);
    high[i++] = featureRange[1].getInter(CLUBS);
    high[i++] = featureRange[1].getInter(ANY);

    high[i++] = featureRange[1].getLoosers(SPADES);
    high[i++] = featureRange[1].getLoosers(HEARTS);
    high[i++] = featureRange[1].getLoosers(DIAMONDS);
    high[i++] = featureRange[1].getLoosers(CLUBS);
    high[i++] = featureRange[1].getLoosers(ANY);

    high[i++] = featureRange[1].getQuickTricks(SPADES);
    high[i++] = featureRange[1].getQuickTricks(HEARTS);
    high[i++] = featureRange[1].getQuickTricks(DIAMONDS);
    high[i++] = featureRange[1].getQuickTricks(CLUBS);
    high[i++] = featureRange[1].getQuickTricks(ANY);

    high[i++] = featureRange[1].getPlayingTricks(SPADES);
    high[i++] = featureRange[1].getPlayingTricks(HEARTS);
    high[i++] = featureRange[1].getPlayingTricks(DIAMONDS);
    high[i++] = featureRange[1].getPlayingTricks(CLUBS);
    high[i++] = featureRange[1].getPlayingTricks(ANY);

    high[i++] = featureRange[1].getStopNT(SPADES);
    high[i++] = featureRange[1].getStopNT(HEARTS);
    high[i++] = featureRange[1].getStopNT(DIAMONDS);
    high[i++] = featureRange[1].getStopNT(CLUBS);

    high[i++] = featureRange[1].getQlty(SPADES);
    high[i++] = featureRange[1].getQlty(HEARTS);
    high[i++] = featureRange[1].getQlty(DIAMONDS);
    high[i++] = featureRange[1].getQlty(CLUBS);

    high[i++] = featureRange[1].getPoints(SPADES);
    high[i++] = featureRange[1].getPoints(HEARTS);
    high[i++] = featureRange[1].getPoints(DIAMONDS);
    high[i++] = featureRange[1].getPoints(CLUBS);
    high[i++] = featureRange[1].getPoints(ANY);

    assert (i == NO_FEATURES);

    i = 0;
    cur[i++] = feature.getHcp(SPADES);
    cur[i++] = feature.getHcp(HEARTS);
    cur[i++] = feature.getHcp(DIAMONDS);
    cur[i++] = feature.getHcp(CLUBS);
    cur[i++] = feature.getHcp(ANY);

    cur[i++] = feature.getControls(SPADES);
    cur[i++] = feature.getControls(HEARTS);
    cur[i++] = feature.getControls(DIAMONDS);
    cur[i++] = feature.getControls(CLUBS);
    cur[i++] = feature.getControls(ANY);

    cur[i++] = feature.getHc(SPADES);
    cur[i++] = feature.getHc(HEARTS);
    cur[i++] = feature.getHc(DIAMONDS);
    cur[i++] = feature.getHc(CLUBS);
    cur[i++] = feature.getHc(ANY);

    cur[i++] = feature.getHonor(SPADES);
    cur[i++] = feature.getHonor(HEARTS);
    cur[i++] = feature.getHonor(DIAMONDS);
    cur[i++] = feature.getHonor(CLUBS);
    cur[i++] = feature.getHonor(ANY);

    cur[i++] = feature.getCountCard(SPADES, ACE);
    cur[i++] = feature.getCountCard(HEARTS, ACE);
    cur[i++] = feature.getCountCard(DIAMONDS, ACE);
    cur[i++] = feature.getCountCard(CLUBS, ACE);
    cur[i++] = feature.getCountCard(ANY, ACE);

    cur[i++] = feature.getCountCard(SPADES, KING);
    cur[i++] = feature.getCountCard(HEARTS, KING);
    cur[i++] = feature.getCountCard(DIAMONDS, KING);
    cur[i++] = feature.getCountCard(CLUBS, KING);
    cur[i++] = feature.getCountCard(ANY, KING);

    cur[i++] = feature.getCountCard(SPADES, QUEEN);
    cur[i++] = feature.getCountCard(HEARTS, QUEEN);
    cur[i++] = feature.getCountCard(DIAMONDS, QUEEN);
    cur[i++] = feature.getCountCard(CLUBS, QUEEN);
    cur[i++] = feature.getCountCard(ANY, QUEEN);

    cur[i++] = feature.getCountCard(SPADES, JACK);
    cur[i++] = feature.getCountCard(HEARTS, JACK);
    cur[i++] = feature.getCountCard(DIAMONDS, JACK);
    cur[i++] = feature.getCountCard(CLUBS, JACK);
    cur[i++] = feature.getCountCard(ANY, JACK);

    cur[i++] = feature.getCountCard(SPADES, TEN);
    cur[i++] = feature.getCountCard(HEARTS, TEN);
    cur[i++] = feature.getCountCard(DIAMONDS, TEN);
    cur[i++] = feature.getCountCard(CLUBS, TEN);
    cur[i++] = feature.getCountCard(ANY, TEN);

    cur[i++] = feature.getRkcb(SPADES);
    cur[i++] = feature.getRkcb(HEARTS);
    cur[i++] = feature.getRkcb(DIAMONDS);
    cur[i++] = feature.getRkcb(CLUBS);

    cur[i++] = feature.getBal();

    cur[i++] = feature.getDp(SPADES);
    cur[i++] = feature.getDp(HEARTS);
    cur[i++] = feature.getDp(DIAMONDS);
    cur[i++] = feature.getDp(CLUBS);
    cur[i++] = feature.getDp(ANY);

    cur[i++] = feature.getDif2(DIAMONDS, CLUBS);
    cur[i++] = feature.getDif2(HEARTS, CLUBS);
    cur[i++] = feature.getDif2(HEARTS, DIAMONDS);
    cur[i++] = feature.getDif2(SPADES, CLUBS);
    cur[i++] = feature.getDif2(SPADES, DIAMONDS);
    cur[i++] = feature.getDif2(SPADES, HEARTS);

    cur[i++] = feature.getSuits();

    cur[i++] = feature.getSuitLen(SPADES);
    cur[i++] = feature.getSuitLen(HEARTS);
    cur[i++] = feature.getSuitLen(DIAMONDS);
    cur[i++] = feature.getSuitLen(CLUBS);

    cur[i++] = feature.getSuitLen2(DIAMONDS, CLUBS);
    cur[i++] = feature.getSuitLen2(HEARTS, CLUBS);
    cur[i++] = feature.getSuitLen2(HEARTS, DIAMONDS);
    cur[i++] = feature.getSuitLen2(SPADES, CLUBS);
    cur[i++] = feature.getSuitLen2(SPADES, DIAMONDS);
    cur[i++] = feature.getSuitLen2(SPADES, HEARTS);

    cur[i++] = feature.getLMaj();

    cur[i++] = feature.getLMin();

    cur[i++] = feature.getLongest(SPADES);
    cur[i++] = feature.getLongest(HEARTS);
    cur[i++] = feature.getLongest(DIAMONDS);
    cur[i++] = feature.getLongest(CLUBS);

    cur[i++] = feature.getLong(SPADES);
    cur[i++] = feature.getLong(HEARTS);
    cur[i++] = feature.getLong(DIAMONDS);
    cur[i++] = feature.getLong(CLUBS);

    cur[i++] = feature.getInter(SPADES);
    cur[i++] = feature.getInter(HEARTS);
    cur[i++] = feature.getInter(DIAMONDS);
    cur[i++] = feature.getInter(CLUBS);
    cur[i++] = feature.getInter(ANY);

    cur[i++] = feature.getLoosers(SPADES);
    cur[i++] = feature.getLoosers(HEARTS);
    cur[i++] = feature.getLoosers(DIAMONDS);
    cur[i++] = feature.getLoosers(CLUBS);
    cur[i++] = feature.getLoosers(ANY);

    cur[i++] = feature.getQuickTricks(SPADES);
    cur[i++] = feature.getQuickTricks(HEARTS);
    cur[i++] = feature.getQuickTricks(DIAMONDS);
    cur[i++] = feature.getQuickTricks(CLUBS);
    cur[i++] = feature.getQuickTricks(ANY);

    cur[i++] = feature.getPlayingTricks(SPADES);
    cur[i++] = feature.getPlayingTricks(HEARTS);
    cur[i++] = feature.getPlayingTricks(DIAMONDS);
    cur[i++] = feature.getPlayingTricks(CLUBS);
    cur[i++] = feature.getPlayingTricks(ANY);

    cur[i++] = feature.getStopNT(SPADES);
    cur[i++] = feature.getStopNT(HEARTS);
    cur[i++] = feature.getStopNT(DIAMONDS);
    cur[i++] = feature.getStopNT(CLUBS);

    cur[i++] = feature.getQlty(SPADES);
    cur[i++] = feature.getQlty(HEARTS);
    cur[i++] = feature.getQlty(DIAMONDS);
    cur[i++] = feature.getQlty(CLUBS);

    cur[i++] = feature.getPoints(SPADES);
    cur[i++] = feature.getPoints(HEARTS);
    cur[i++] = feature.getPoints(DIAMONDS);
    cur[i++] = feature.getPoints(CLUBS);
    cur[i++] = feature.getPoints(ANY);

    assert (i == NO_FEATURES);

    for (int i = 0; i < NO_FEATURES; i++)
        if ((low[i] > cur[i]) || (high[i] < cur[i]))
            return false;

    return true;
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

