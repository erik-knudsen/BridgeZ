/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CActorBase.h
  Developers: eak

  Revision History:
  9-oct-2015 eak: Original

  Abstract: Generic base strategy for actor.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the generic base strategy for actor
 * strategy classes (local, remote and auto).
 */

#ifndef CACTORBASE_H
#define CACTORBASE_H

#include <QObject>
#include <QTextStream>

#include "Defines.h"

class CTblMngrBase;

/**
 * \brief This abstract class implements declaration of generic base strategy
 * for actor classes (local, remote and auto).
 *
 * Actor strategy classes have a common method interface. The interface is
 * partly defined by means of virtual methods in this class. And partly by means of
 * methods, which are common for all actor managers (Local, Remote and auto). These
 * methods are implemented in this class. Actor strategy classes also share a
 * a number of common signals. These signals are defined here.
 */
class CActorBase : public QObject
{
    Q_OBJECT
public:
    CActorBase(CTblMngrBase *tableManager);
    virtual ~CActorBase();

    virtual void setUpdateGameInfo(bool updateGameInfo) { }

    virtual void startNewSession() = 0;
    virtual Actor getActorType() = 0;

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
    void sUpdateGame();
    void sUpdateGameToNextDeal();
};

#endif // CACTORBASE_H
