/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrServerAuto.h
  Developers: eak

  Revision History:
  12-oct-2015 eak: Original

  Abstract: Table manager server for auto play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the table manager server class for auto play.
 */

#ifndef CTBLMNGRSERVERAUTO_H
#define CTBLMNGRSERVERAUTO_H

#include <QTimer>
#include <QHostAddress>

#include "CTblMngr.h"
#include "../src-gen/ZBridgeServer.h"
#include "../src-gen/ZBridgeServerSync.h"

#include "ZBridgeException.h"
#include "Defines.h"
#include "cbidhistory.h"
#include "cplayhistory.h"

class CZBridgeDoc;
class CGamesDoc;
class CActorBase;
class CRemoteActorServer;
class CBidAndPlayEngines;

/**
 * @brief This class implements the table manager server controller for auto play.
 *
 * This class performs a subset of the CTblMngrServer class. It is used in cases where the computer(s)
 * plays the current (user controlled) game automatically. This can happen when the MP or IMP score
 * method is being used for the play and there is a need for something to compare with. It performs
 * the same basic functionality as the CTblMngrServer class except the user interaction has been
 * stripped.
 */
class CTblMngrServerAuto : public CTblMngrBase
{
    Q_OBJECT

public:
    CTblMngrServerAuto(CZBridgeDoc *doc, CGamesDoc *games, CBidAndPlayEngines *bidAndPlayEngines,
                QHostAddress hostAddress, QObject *parent = 0) throw(NetProtocolException);
    ~CTblMngrServerAuto();

    //Run cycle for the server statechart.
    void serverRunCycle();
    void serverSyncRunCycle();

public slots:
    /** @name Actor slots.
     * These slot methods are signalled from the actors.
     */
    /*@{*/
    void sConnect(QString teamName, Seat seat, int protocol);
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
    /*@}*/

    /** @name Delay slots.
     * These slot methods are internal slots used for delays.
     */
    /*@{*/
    void startOfBoard();
    void playerToLead();
    void dummyToLead();
    /*@}*/

    /** @name Synchronization, auto quit and new session slots..
     */
    /*@{*/
    void sltPlayStart();    /**< Synchronize auto and ordinary play. */
    void sAutoQuit();       /**< Quit the auto thread and delete the auto object (and children). */
    void sNewSession();     /**< Start a new session. */
    /*@}*/

    //Reset table manager (used as slot when a remote client disconnects).
    void cleanTableManager();

    //Info, warning and error slots regarding server/client connection.
    void connectInfo(QString text);
    void connectWarning(QString text);
    void connectError(QString text);

private:
    void giveNewDeal();

    void serverActions();
    void serverSyncActions();
    void setUpdateGameInfo();

    ZBridgeServer handle;
    ZBridgeServerSync syncHandle;

    bool synchronizing;

    CActorBase *actors[4];
    CRemoteActorServer *remoteActorServer;
    CBidHistory bidHistory;
    CPlayHistory playHistory;
    CBidAndPlayEngines *bidAndPlayEngines;

    int currentBoardNo;
    Team currentVulnerable;

    int boardNo;
    Team vulnerable[4] = { NEITHER, NORTH_SOUTH, EAST_WEST, BOTH };
    Seat dealer[4] = { NORTH_SEAT, EAST_SEAT, SOUTH_SEAT, WEST_SEAT };

    int currentCards[4][13];
    bool updateGameInfo;
    bool waiting;

    bool firstAutoSync;

    CZBridgeDoc *doc;
    CGamesDoc *games;
};

#endif // CTBLMNGRSERVERAUTO_H
