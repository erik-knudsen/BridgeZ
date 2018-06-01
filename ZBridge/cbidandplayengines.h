/* Erik Aagaard Knudsen.
  Copyright © 2017 - All Rights Reserved

  Project: ZBridge
  File: CBidAndPlayEngines.h
  Developers: eak

  Revision History:
  25-jan-2017 eak: Original

  Abstract: Bid and play engines. 

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bid and the play engines.
 */

#ifndef CBIDANDPLAYENGINES_H
#define CBIDANDPLAYENGINES_H

#include "cfeatures.h"
#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cbid.h"

class CBidDB;
class CBidDesc;
class CBidOptionDoc;
class CBidEngine;
class CPlayEngine;
class CRule;

/**
 * @brief This class handles computer generated bid and play of cards.
 */
class CBidAndPlayEngines
{
public:
    CBidAndPlayEngines();
    virtual ~CBidAndPlayEngines();

    void initialize(int computerLevel, CBidDB *bidDB, CBidDesc *bidDesc,
                    CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc,
                    ScoringMethod scoringMethod);
    void setScoringMethod(ScoringMethod scoringMethod) { this->scoringMethod = scoringMethod; }
    CBid getNextBid(Seat seat, CFeatures &ownFeatures, CBidHistory &bidHistory, Team teamVul);
    QList<CRule *> getpRules(Seat seat, CBidHistory &bidHistory, Bids bid, Team teamVul,
                             bool *substitute);
    QString getAlertIdDesc(quint8 alertId);
    int getNextPlay(Seat seat, Seat dummySeat, int ownCards[13], int dummyCards[13],
                    CBidHistory &bidHistory, CPlayHistory &playHistory);

private:
    CBidEngine *bidEngine;          /**< The bid engine. */
    CPlayEngine *playEngine;        /**< The play engine. */

    ScoringMethod scoringMethod;
};

#endif // CBIDANDPLAYENGINES_H
