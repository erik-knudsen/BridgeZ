/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrBase.cpp
  Developers: eak

  Revision History:
  9-oct-2015 eak: Original

  Abstract: Generic base strategy for table manager.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the generic base strategy for table manager
 * strategy classes (server, clients and auto).
 */

#include "CTblMngrBase.h"

CTblMngrBase::CTblMngrBase(QObject *parent) :
    QObject(parent)
{
}

CTblMngrBase::~CTblMngrBase()
{
}
