/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrServer.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Table manager.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the table manager server class.
 */

#ifndef CTBLMNGRSERVER_H
#define CTBLMNGRSERVER_H

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
class CActor;
class CBidEngine;
class CPlayEngine;
class CPlayView;
class CRemoteActorServer;

/**
 * @brief This class implements the table manager server controller.
 *
 * The class controls all actors (players) local as well as remote. This is done by means of a
 * statechart generated by the Yakindu Statechart Tools. The statechart takes the table manager
 * server through the states needed by the manager. The statechart implements the @ref protocol
 * The protocol is used for both local and remote actors.
 * The protocol is best suited for automatic actors. I have added a few extensions to make it
 * better suited for manual actors. I have called this extended protocol the advanced protocol.
 */
class CTblMngrServer : public CTblMngr
{
    Q_OBJECT

public:
    CTblMngrServer(CZBridgeDoc *doc, CGamesDoc *games, QHostAddress hostAddress, CPlayView *playView,
                   QObject *parent = 0) throw(NetProtocolException);
    ~CTblMngrServer();

    //Run cycle for the server statechart.
    void serverRunCycle();
    void serverSyncRunCycle();

    /** @name Main menu activated methods.
     * These methods are activated via the applications main menu.
     */
    /*@{*/
    void newSession();
    void newDeal();
    void showAllCards();
    void reBid();
    void rePlay();
    void undo();
    /*@}*/

public slots:
    /** @name Play view slots.
     * These slot methods are signalled from the play view.
     */
    /*@{*/
    void buttonClicked(int button);
    void bidValue(Bids bid);
    void playValue(int card);
    void bidClose();
    void handClicked(Seat seat);
    /*@}*/

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

    void sShowAuction();
    void sShowPlay();
    void sEnableContinueLeader();
    void sDisableContinueLeader();
    void sEnableContinueSync(int syncState);
    void sDisableContinueSync(int syncState);

    void sContinueLeader();
    void sContinueSync();
    /*@}*/

    /** @name Delay slots.
     * These slot methods are internal slots used for delays.
     */
    /*@{*/
    void startOfBoard();
    void playerToLead();
    void dummyToLead();
    /*@}*/

    /** @name Synchronize slot.
     * This slot method is used for synchronizing auto play and ordinary play.
     */
    /*@{*/
    void sltPlayStart();
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
    void setShowUser(bool showAll);
    void setUpdateGameInfo();

    ZBridgeServer handle;
    ZBridgeServerSync syncHandle;

    bool synchronizing;

    CActor *actors[4];
    CRemoteActorServer *remoteActorServer;
    CBidHistory bidHistory;
    CPlayHistory playHistory;

    int protocol;

    int currentBoardNo;
    Team currentVulnerable;

    int boardNo;
    Team vulnerable[4] = { NEITHER, NORTH_SOUTH, EAST_WEST, BOTH };
    Seat dealer[4] = { NORTH_SEAT, EAST_SEAT, SOUTH_SEAT, WEST_SEAT };

    int currentCards[4][13];
    bool showUser;      /**< If true then the server handles the play view. */
    bool updateGameInfo;/**< If true then update of game info is handle by Table Manager. */
    bool waiting;

    QTimer *leaderButton;

    CZBridgeDoc *doc;
    CGamesDoc *games;
    CPlayView *playView;
};

#endif // CTBLMNGRSERVER_H
