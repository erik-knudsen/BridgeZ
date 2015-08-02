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
 * strategy classes.
 */

#ifndef CACTOR_H
#define CACTOR_H

#include <QObject>
#include <QTextStream>

#include "Defines.h"

class CTblMngr;

/**
 * \brief This abstract class implements the declaration of the generic strategy
 * for actor classes (local and remote).
 *
 * Actor strategy classes have a common method interface. The interface is
 * defined by means of the virtual methods in this class. And by means of the
 * methods, which are common for all actor managers (Local and Remote). These
 * methods are implemented in this class. Actor strategy classes also share a
 * a number of common signals. These signals are defined here.
 */
class CActor : public QObject
{
    Q_OBJECT
public:
    CActor(CTblMngr *tableManager);
    virtual ~CActor();

    virtual void xmitPBNFiles(QTextStream &originalStream, QTextStream &playedStream) { }
    virtual void startNewSession() = 0;

    virtual void bidValue(Bids bid) {}
    virtual void playValue(int card) {}
    virtual void continueLeader() {}
    virtual void continueSync() {}

    virtual void seated(QString teamName) = 0;
    virtual void teamNames(QString nsTeamName, QString ewTeamName) = 0;
    virtual void startOfBoard() = 0;
    virtual void dealInfo(int boardNumber, Seat dealer, Team vulnerability) = 0;
    virtual void cards(int cards[4][13]) = 0;
    virtual void bidDone(Seat bidder, Bids bid) = 0;
    virtual void playerToLead(Seat player) = 0;
    virtual void dummyToLead() = 0;
    virtual void playerPlays(Seat player, int card) = 0;
    virtual void dummyCards(int cards[13]) = 0;
    virtual void undoBid(bool reBid) = 0;
    virtual void undoTrick(bool rePlay) = 0;
    virtual void endOfSession() = 0;
    virtual void attemptSyncFromServerToClient() = 0;
    virtual void confirmSyncFromServerToClient() = 0;
    virtual void allSyncFromServerToClient() = 0;

    virtual Actor getActorType() = 0;
    virtual Seat getSeat() = 0;
    virtual bool isWaiting() = 0;
    virtual void setShowUser(bool showUser) {}

signals:
    void sConnect(QString name, Seat seat, int protocol);
    void sRTNames(Seat seat);
    void sRSBoard(Seat seat);
    void sRDealInfo(Seat seat);
    void sRCards(Seat seat);
    void sRBid(Seat player, Seat bidder);
    void sBid(Seat bidder, Bids bid);
    void sPlayerPlays(Seat player, int card);
    void sReadyForPlayer(Seat seat, Seat player, int trick);
    void sReadyForDummy(Seat seat, int trick);
    void sReadyForDummyCards(Seat seat);
    void sAttemptSyncFromClientToServer(Seat syncher);
    void sConfirmSyncFromClientToServer(Seat syncher);

private:
    CTblMngr *tableManager;
};

#endif // CACTOR_H
