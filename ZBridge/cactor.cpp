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
 * strategy classes (local and remote - not auto).
 */

#include "CTblMngrBase.h"
#include "cactor.h"

/**
 * @brief Constructor for actor.
 * @param tableManager Pointer to the table manager for the actor.
 *
 */
CActor::CActor(CTblMngrBase *tableManager) :
    CActorBase(tableManager)
{
}

CActor::~CActor()
{
}
