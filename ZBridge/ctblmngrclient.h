/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrClient.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Table manager.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the for the table manager client class.
 */

#ifndef CTBLMNGRCLIENT_H
#define CTBLMNGRCLIENT_H

#include <QTimer>

#include "CTblMngr.h"
#include "../src-gen/ZBridgeClient.h"

#include "Defines.h"

class CZBridgeDoc;
class CGamesDoc;
class CActorLocal;
class CBidEngine;
class CPlayEngine;
class CPlayView;
class CRemoteActorClient;


/**
 * @brief This class implements the table manager client controller.
 *
 * The class controls one local actor (player), which on the corresponding server class is remote.
 */
class CTblMngrClient : public CTblMngr
{
    Q_OBJECT

public:
    CTblMngrClient(CZBridgeDoc *doc, CGamesDoc *games, CPlayView *playView, QObject *parent = 0);
    ~CTblMngrClient();


    /** @name Main menu activated methods.
     * These methods are activated via the applications main menu.
     */
    /*@{*/
    void newSession();
    void showAllCards();
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

    void sShowAuction();
    void sShowPlay();
    void sEnableContinueLeader();
    void sDisableContinueLeader();
    void sEnableContinueSync(int syncState);
    void sDisableContinueSync(int syncState);

    void sContinueLeader();
    void sContinueSync();
    /*@}*/

    /** @name TCP/IP communication slots.
     * These slot methods are signalled from the tcp/ip to server connection.
     */
    /*@{*/
    void sSocketError(QString err);
    void clientConnected();
    void clientDisConnected();
    void receiveLine(QString line);
    /*@}*/

private:
    //Reset table manager.
    void cleanTableManager();

    CActorLocal *actor;
    CRemoteActorClient *remoteActorClient;

    int protocol;

    int noHands;
    int currentCards[4][13];

    QTimer *leaderButton;

    CZBridgeDoc *doc;
    CGamesDoc *games;
    CPlayView *playView;

    ZBridgeClient *handle;
};

#endif // CTBLMNGRCLIENT_H
