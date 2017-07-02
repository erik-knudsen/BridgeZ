/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CActorRemoteAuto.h
  Developers: eak

  Revision History:
  12-oct-2015 eak: Original

  Abstract: Actor remote class for auto play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the actor remote class for auto play.
 */

#ifndef CACTORREMOTEAUTO_H
#define CACTORREMOTEAUTO_H

#include <QObject>

#include "Defines.h"
#include "cactorbase.h"

class CRemoteActorFrontEnd;
class CTblMngrBase;

/**
 * @brief This class implements the actor remote class for auto play.
 *
 * This class performs a subset of the CActorRemote class. It is used in cases where the computer(s)
 * plays the current (user controlled) game automatically. This can happen when the MP or IMP score
 * method is being used for the play and there is a need for something to compare with. It performs
 * the same basic functionality as the CActorRemote class except the user interaction has been
 * stripped.
 */
class CActorRemoteAuto : public CActorBase
{
    Q_OBJECT
public:
    CActorRemoteAuto(Seat seat, CRemoteActorFrontEnd *frontEnd, CTblMngrBase *tableManager);

    void startNewSession();

    void seated(QString teamName);
    void teamNames(QString nsTeamName, QString ewTeamName);
    void startOfBoard();
    void dealInfo(int boardNumber, Seat dealer, Team vulnerability);
    void cards(int cards[4][13]);
    void bidDone(Seat bidder, Bids bid);
    void playerToLead(Seat player);
    void dummyToLead();
    void playerPlays(Seat player, int card);
    void dummyCards(int cards[13]);
    void undoBid(bool reBid);
    void undoTrick(bool rePlay);
    void endOfSession();
    void reStart() {}

    void attemptSyncFromServerToClient();
    void confirmSyncFromServerToClient();
    void allSyncFromServerToClient();

    Actor getActorType() { return REMOTE_ACTOR; }
    Seat getSeat() { return seat; }
    bool isWaiting() { return false; }

public slots:
    void receiveLine(QString line, bool connected);     /**< Receive a line from the client. */

signals:
    void start();
    void sendLine(QString line);            /**< Send a line  to the client. */

private:
    Seat seat;
    CRemoteActorFrontEnd *frontEnd;         /**< The front end. */
    CTblMngrBase * tableManager;            /**< The table manager. */
};

#endif // CACTORREMOTEAUTO_H
