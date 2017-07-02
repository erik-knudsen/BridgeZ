/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrClientAuto.h
  Developers: eak

  Revision History:
  15-oct-2015 eak: Original

  Abstract: Table manager client for auto play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the for the table manager client class for auto play.
 */

#ifndef CTBLMNGRCLIENTAUTO_H
#define CTBLMNGRCLIENTAUTO_H

#include <QTimer>
#include <QBuffer>
#include <QTextStream>
#include <QHostAddress>

#include "CTblMngr.h"
#include "../src-gen/ZBridgeClient.h"

#include "Defines.h"

class CZBridgeDoc;
class CGamesDoc;
class CActorLocalAuto;
class CPlayEngine;
class CRemoteActorClient;
class CBidAndPlayEngines;


/**
 * @brief This class implements the table manager client controller for auto play.
 *
 * The class controls one local actor (player), which on the corresponding server class is remote.
 * This class performs a subset of the CTblMngrClient class. It is used in cases where the computer(s)
 * plays the current (user controlled) game automatically. This can happen when the MP or IMP score
 * method is being used for the play and there is a need for something to compare with. It performs
 * the same basic functionality as the CTblMngrClient class except the user interaction has been
 * stripped.
 */
class CTblMngrClientAuto : public CTblMngrBase
{
    Q_OBJECT

public:
    CTblMngrClientAuto(CZBridgeDoc *doc, CGamesDoc *games, CBidAndPlayEngines *bidAndPlayEngines,
                       QHostAddress hostAddress, QObject *parent = 0);
    ~CTblMngrClientAuto();

public slots:
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
    void sUpdateGame();
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

    /** @name Synchronization etc. slots.
     */
    /*@{*/
    void sAutoQuit();       /**< Quit the auto thread and delete the auto object (and children). */
    void sNewSession();     /**< Start a new auto session. */
    void sltDisconnect();   /**< Disconnect and prepare for new session. */
    /*@}*/

private:
    //Reset table manager.
    void cleanTableManager();

    CActorLocalAuto *actor;
    CRemoteActorClient *remoteActorClient;

    CBidAndPlayEngines *bidAndPlayEngines;

    int noHands;
    int currentCards[4][13];

    CZBridgeDoc *doc;
    CGamesDoc *games;

    QHostAddress hostAddress;

    ZBridgeClient *handle;
};

#endif // CTBLMNGRCLIENT_H
