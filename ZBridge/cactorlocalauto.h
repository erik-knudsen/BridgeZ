/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CActorLocalAuto.h
  Developers: eak

  Revision History:
  12-oct-2015 eak: Original

  Abstract: Actor local class for auto play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the actor local class for auto play.
 */

#ifndef CACTORLOCALAUTO_H
#define CACTORLOCALAUTO_H

#include <QObject>
#include <QTextStream>

#include "../src-gen/ZBridgeClient.h"
#include "../src-gen/ZBridgeClientSync.h"

#include "Defines.h"
#include "cbiddb.h"
#include "cbiddesc.h"
#include "cbidandplay.h"
#include "cactorbase.h"

class CBidAndPlayEngines;
class CTblMngrBase;

/**
 * @brief This class implements the actor local class for auto play.
 *
 * This class performs a subset of the CActorLocal class. It is used in cases where the computer(s)
 * plays the current (user controlled) game automatically. This can happen when the MP or IMP score
 * method is being used for the play and there is a need for something to compare with. It performs
 * the same basic functionality as the CActorLocal class except the user interaction has been
 * stripped.
 */
class CActorLocalAuto : public CActorBase
{
    Q_OBJECT
public:
    CActorLocalAuto(Protocol protocol, QString teamName, Seat seat, CBidAndPlayEngines *bidAndPlayEngines,
                    CTblMngrBase *tableManager);

    void startNewSession();

    void clientRunCycle();
    void clientSyncRunCycle();

    void bidValue(Bids bid);
    void playValue(int card);
    void continueSync();

    void setUpdateGameInfo(bool updateGameInfo) { this->updateGameInfo = updateGameInfo; }

    CBidHistory &getBidHistory() { return bidAndPlay.getBidHistory(); }
    CPlayHistory &getPlayHistory() { return bidAndPlay.getPlayHistory(); }

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
    void reStart();

    void attemptSyncFromServerToClient();
    void confirmSyncFromServerToClient();
    void allSyncFromServerToClient();

    Actor getActorType() { return AUTO_ACTOR; }
    Seat getSeat() {return (Seat)zBridgeClientIface_get_client(&handle); }

    ZBridgeClient *getHandle() { return &handle; }

public slots:
    void bidValue();
    void playValue();

signals:
;
private:
    void clientActions();
    void clientSyncActions();

    bool updateGameInfo;/**< Control who updates game info? */
    bool updateCurrentGameInfo;

    Protocol protocol;  /**< Protocol. */

    QString teamName;   /**< Team name. */

    CBidAndPlay bidAndPlay; /**< Bid and play handling. */

    CTblMngrBase *tableManager; /**< Controlling table manager (Server or Client). */

    ZBridgeClient handle;   /**< Handle to client Yakindu state chart. */
    ZBridgeClientSync syncHandle; /**< Handle to client Yakindu state chart synchronization. */

    bool synchronizing;
};

#endif // CACTORLOCALAUTO_H
