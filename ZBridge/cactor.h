/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CActor.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Generic strategy for actor.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the generic strategy for actor
 * strategy classes (local and remote - not auto).
 */

#ifndef CACTOR_H
#define CACTOR_H

#include <QObject>
#include <QTextStream>

#include "Defines.h"
#include "cactorbase.h"

class CTblMngrBase;

/**
 * \brief This abstract class implements the declaration of the generic strategy
 * for actor classes (local and remote - not auto).
 *
 * Actor strategy classes have a common method interface. The interface is
 * defined partly by means of the virtual methods in this class. And partly by means of
 * methods, which are common for all actor managers (Local and Remote). These
 * methods are implemented in this class.
 */
class CActor : public CActorBase
{
    Q_OBJECT
public:
    CActor(CTblMngrBase *tableManager);
    virtual ~CActor();

    virtual void xmitPBNFiles(QTextStream &originalStream, QTextStream &playedStream,
                              ScoringMethod scoringMethod) { }
    virtual void bidValue(Bids bid) {}
    virtual void playValue(int card) {}
    virtual void continueSync() {}

    virtual Seat getSeat() = 0;
    virtual void setShowUser(bool showUser) {}
    virtual void setManual(bool manual) { }
    virtual void getHint() { }
};

#endif // CACTOR_H
