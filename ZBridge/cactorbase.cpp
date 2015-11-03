/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CActorBase.cpp
  Developers: eak

  Revision History:
  9-oct-2015 eak: Original

  Abstract: Generic base strategy for actor.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the generic base strategy for actor
 * strategy classes (local, remote, auto).
 */

#include "CTblMngrBase.h"
#include "cactorbase.h"

/**
 * @brief Constructor for actor.
 * @param tableManager Pointer to the table manager for the actor..
 *
 * The constructor creates common connections from the actor to the controlling table manager.
 */
CActorBase::CActorBase(CTblMngrBase *tableManager) :
    QObject(tableManager)
{
    connect(this, &CActorBase::sConnect, tableManager, &CTblMngrBase::sConnect);
    connect(this, &CActorBase::sRTNames, tableManager, &CTblMngrBase::sRTNames);
    connect(this, &CActorBase::sRSBoard, tableManager, &CTblMngrBase::sRSBoard);
    connect(this, &CActorBase::sRDealInfo, tableManager, &CTblMngrBase::sRDealInfo);
    connect(this, &CActorBase::sRCards, tableManager, &CTblMngrBase::sRCards);
    connect(this, &CActorBase::sRBid, tableManager, &CTblMngrBase::sRBid);
    connect(this, &CActorBase::sBid, tableManager, &CTblMngrBase::sBid);
    connect(this, &CActorBase::sPlayerPlays, tableManager, &CTblMngrBase::sPlayerPlays);
    connect(this, &CActorBase::sReadyForPlayer, tableManager, &CTblMngrBase::sReadyForPlayer);
    connect(this, &CActorBase::sReadyForDummy, tableManager, &CTblMngrBase::sReadyForDummy);
    connect(this, &CActorBase::sReadyForDummyCards, tableManager, &CTblMngrBase::sReadyForDummyCards);
    connect(this, &CActorBase::sAttemptSyncFromClientToServer, tableManager, &CTblMngrBase::sAttemptSyncFromClientToServer);
    connect(this, &CActorBase::sConfirmSyncFromClientToServer, tableManager, &CTblMngrBase::sConfirmSyncFromClientToServer);
    connect(this, &CActorBase::sUpdateGame, tableManager, &CTblMngrBase::sUpdateGame);
    connect(this, &CActorBase::sUpdateGameToNextDeal, tableManager, &CTblMngrBase::sUpdateGameToNextDeal);
}

CActorBase::~CActorBase()
{
}
