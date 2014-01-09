/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CActor.cpp
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Generic strategy for actor.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the generic strategy for actor
 * strategy classes.
 */

#include "CTblMngr.h"
#include "cactor.h"

/**
 * @brief Constructor for actor.
 * @param tableManager Pointer to the table manager for the actor..
 *
 * The constructor creates common connections from the actor to the controlling table manager.
 */
CActor::CActor(CTblMngr *tableManager) :
    QObject(tableManager)
{
    this->tableManager = tableManager;

    connect(this, &CActor::sConnect, tableManager, &CTblMngr::sConnect);
    connect(this, &CActor::sRTNames, tableManager, &CTblMngr::sRTNames);
    connect(this, &CActor::sRSBoard, tableManager, &CTblMngr::sRSBoard);
    connect(this, &CActor::sRDealInfo, tableManager, &CTblMngr::sRDealInfo);
    connect(this, &CActor::sRCards, tableManager, &CTblMngr::sRCards);
    connect(this, &CActor::sRBid, tableManager, &CTblMngr::sRBid);
    connect(this, &CActor::sBid, tableManager, &CTblMngr::sBid);
    connect(this, &CActor::sPlayerPlays, tableManager, &CTblMngr::sPlayerPlays);
    connect(this, &CActor::sReadyForPlayer, tableManager, &CTblMngr::sReadyForPlayer);
    connect(this, &CActor::sReadyForDummy, tableManager, &CTblMngr::sReadyForDummy);
    connect(this, &CActor::sReadyForDummyCards, tableManager, &CTblMngr::sReadyForDummyCards);
}

CActor::~CActor()
{
}
